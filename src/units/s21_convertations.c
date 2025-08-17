#include "../s21_decimal.h"
// This project made by beedlesh haywoota woodsjul thymemas

// Функции для проверки состояния
// числа______________________________________________

s21_bool is_nan(float value)  // Проверка на NaN (не число)
{
  return (value == value) ? FALSE : TRUE;
}
s21_bool is_inf(float value)  // Проверка на бесконечность
{
  const float infinity = 1.0 / 0.0;
  s21_bool res = FALSE;
  if (value == infinity) res = TRUE;
  return res;
}
s21_bool is_valid(float value)  // Проверка на корректность числа
{
  s21_bool res = TRUE;
  if (is_nan(value) || is_inf(value) || is_inf(-value)) res = FALSE;
  return res;
}

s21_bool is_small_float(const float f_num) {
  // const double epsilon = 0.0000001;
  // return ((double)f_num < epsilon)?TRUE:FALSE;
  return (f_num == 0.0) ? TRUE : FALSE;
}
//___________________________________________________________________________________

// Преобразование из int в s21_decimal
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return CONVERTATION_ERR;
  make_zero_decimal(dst);
  if (src != 0) {
    s21_bool src_is_min_int = (src == INT_MIN);
    if (src < 0) {
      dst->is_negative = TRUE;
      if (src_is_min_int)  // INT_MIN == -2147483648
        src++;  // INT_MAX == 2147483647 поэтому сперва нужно уменьшить
                // абсолютное значение на 1
      src *= -1;
    }
    if (src_is_min_int)
      dst->low = (unsigned)src + 1;  // INT_MAX == |INT_MIN|+1
    else
      dst->low = (unsigned)src;
  }

  return OK;
}

// преобразование float в научную нотацию в виде строки, расчёт мантиссы в виде
// целого числа и экспоненты
unsigned significand_from_float(const float src, const unsigned precision,
                                int *exp) {
  const unsigned significand_len = 1 + precision;  // *.precisionE+3
  char str_scntfc_not[15] = {
      0};  // мантисса + точка + "E" + знак + двух или трёх значная экспонента
  sprintf(str_scntfc_not, "%.*E", precision, src);

  // достаём экспоненту:
  const unsigned ind_E = strcspn(str_scntfc_not, "E");
  unsigned str_ind = ind_E + 2;  //  индекс начала значения экспоненты
  *exp = 0;
  while (str_scntfc_not[str_ind]) {
    *exp *= 10;
    *exp += (str_scntfc_not[str_ind] - '0');
    ++str_ind;
  }
  if (str_scntfc_not[ind_E + 1] == '-') *exp *= -1;

  // считаем significand как целое число из значимых цифр (мантиссы) "5.123456"
  // -> 5123456
  unsigned significand = str_scntfc_not[0] - '0';
  str_ind = 2;  // индекс в строке str_scntfc_not первой цифры после точки
  for (; str_ind <= significand_len; str_ind++) {
    significand *= 10;
    significand += (str_scntfc_not[str_ind] - '0');
  }
  *exp -= precision;  // т.к. в exp учтена лишь первая цифра significand
  return significand;
}

// Проверка экспоненты в записи числа в научной нотации на предельно допустимый
// scale 28
s21_bool is_too_small(const int exp) {
  s21_bool too_small = FALSE;
  if (exp < 0)
    if ((unsigned)(-exp) > max_scale) too_small = TRUE;
  return too_small;
}

// Преобразование из float в s21_decimal
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  res_code res = OK;
  if (!dst || !is_valid(src)) res = CONVERTATION_ERR;
  if (dst) make_zero_decimal(dst);

  if (res == OK && !is_small_float(src)) {
    if (src < 0) {
      dst->is_negative = TRUE;
      src = -src;
    }
    int precision = 6;
    int exp = 0;
    unsigned significand = significand_from_float(src, precision, &exp);

    if (is_too_small(exp + precision))
      res = IS_TOO_SMALL;
    else {
      if (is_too_small(exp))  // придётся обрезать significand до части, которая
                              // влезет в скейл
      {
        precision +=
            (exp + max_scale);  // уменьшаем точность до той, что влезет в скейл
        significand = significand_from_float(src, precision, &exp);
      }
      dst->low = significand;  // 7-значное число, точно влезет в low
      if (exp < 0) dst->scale = (unsigned)(-exp);
    }

    while (exp > 0 && res == OK)
    // -exp == scale > 0 поэтому нужно уменьшить экспоненту до 0, умножая
    // мантиссу на 10
    {
      res = decimal_multiply_by_10(dst);
      exp--;
      dst->scale = 0;
    }
    if (res != OK) make_zero_decimal(dst);
  }
  return (res == OK) ? OK : CONVERTATION_ERR;
}

// Преобразование из s21_decimal в int
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!dst) return CONVERTATION_ERR;
  res_code res = OK;
  *dst = 0;
  if (!is_zero(src)) {
    s21_bool is_negative = (src.is_negative) ? TRUE : FALSE;
    src.is_negative = 0;

    s21_decimal int_dec = src;
    if (src.scale)
      // из README: если в числе decimal есть дробная часть, то её следует
      // отбросить (0.9 преобразуется в 0)
      res = s21_truncate(src, &int_dec);
    // int_dec.scale == 0
    if (int_dec.middle || int_dec.high) res = IS_TOO_LARGE;

    if (res == OK) {
      s21_bool is_min_int =
          (is_negative && (int_dec.low == (unsigned)INT_MAX + 1));
      if (is_min_int)
        *dst = INT_MIN;
      else if (int_dec.low <= (unsigned)INT_MAX) {
        *dst = int_dec.low;
        if (is_negative) *dst *= -1;
      } else
        res = IS_TOO_LARGE;
    }
  }
  return (res == OK) ? OK : CONVERTATION_ERR;
}

long double pow_of_2(const unsigned n) {
  long double res = 1;
  for (unsigned i = 1; i <= n; i++) res *= 2;
  return res;
}

// Преобразование из s21_decimal в float
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst) return CONVERTATION_ERR;
  res_code res = OK;
  *dst = 0.0;
  if (!is_zero(src)) {
    long double big_float = 0;
    for (unsigned i = 0; (i < bits_in_mantissa); i++) {
      s21_bool is_one = get_bit(src, i);
      if (is_one) big_float += pow_of_2(i);
    }
    unsigned scale = src.scale;
    while (scale--) big_float /= 10;
    *dst = (float)big_float;
  }
  if (src.is_negative) *dst *= -1;

  return res;
}
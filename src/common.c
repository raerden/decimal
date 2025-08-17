#include "s21_decimal.h"
// This project made by beedlesh haywoota woodsjul thymemas

// Проверка на ноль
s21_bool is_zero(const s21_decimal value) {
  return !(value.low || value.middle || value.high);
}

// Возвращает результат умножения указанного decimal на -1
int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result) return CALCULATION_ERR;
  for (unsigned i = 0; i <= ints_in_mantissa; i++)
    result->bits[i] = value.bits[i];

  result->is_negative = !(value.is_negative);

  return OK;
}

s21_decimal s21_mod(s21_decimal value) {
  s21_decimal res = value;
  res.is_negative = 0;
  return res;
}

// вспомогательная функция для get и set bit
void get_correct_offset(int bit_index, int *byte_index, int *bit_offset) {
  *byte_index = bit_index / bits_in_int;  // Индекс байта 0,1,2
  *bit_offset = bit_index % bits_in_int;  // Смещение бита в байте
}

// Получение значения бита по индексу
int get_bit(s21_decimal value, int bit_index) {
  int byte_index = 0;
  int bit_offset = 0;

  get_correct_offset(bit_index, &byte_index, &bit_offset);
  const int mask = 1 << bit_offset;

  return !!(value.bits[byte_index] & mask);
}

// Установка значения бита по индексу
void set_bit(s21_decimal *value, int bit_index, int bit_value) {
  int byte_index = 0;
  int bit_offset = 0;
  get_correct_offset(bit_index, &byte_index, &bit_offset);

  if (bit_value)
    value->bits[byte_index] |=
        (1 << bit_offset);  // Установка бита в 1 через побитовое ИЛИ
  else
    value->bits[byte_index] &=
        ~(1 << bit_offset);  // Установка бита в 0 через побитовое И
}

// зануляет переданный децимал
void make_zero_decimal(s21_decimal *value) {
  for (unsigned i = 0; i <= ints_in_mantissa; i++) value->bits[i] = 0;
}

// Смещение битов влево на shift
int decimal_shift_left(s21_decimal *value, unsigned shift) {
  int res = OK;

  if (shift > 0) {
    unsigned overflow = 0;
    for (unsigned i = 0; i < ints_in_mantissa; i++) {
      unsigned memo = value->bits[i];
      value->bits[i] =
          (memo << shift) |
          overflow;  // Сдвиг влево с учетом предыдущего переполнения
      overflow = memo >> (bits_in_int - shift);  // Сохраняем переполнение
    }
    // Если в overflow есть значение для bits[3], возвращаем ошибку переполнения
    if (overflow != 0) res = IS_TOO_LARGE;
  }
  return res;
}

// проверка возвращаемого кода ошибки
s21_bool check_res_code(s21_decimal *value_1, s21_decimal *value_2) {
  int res_code = OK;
  s21_decimal result = {};
  res_code = s21_add(*value_1, *value_2, &result);
  value_1->zero = '\0';
  value_2->zero = value_1->zero;
  // printf("%s %s", (char*)value_1, (char*)value_2);
  return res_code;
}

// Сложение мантисс двух чисел
int add_decimal_mantissa(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *result) {
  int res = OK;

  unsigned memo = 0;
  for (unsigned i = 0; i < bits_in_mantissa; i++) {
    unsigned sum = get_bit(value_1, i) + get_bit(value_2, i) + memo;
    if (sum > 1)
      memo = 1;  // Перенос в следующий бит
    else
      memo = 0;
    sum %= 2;  // 1 + 1 = 2 % 2 = 0
    set_bit(result, i, sum);
  }

  if (memo > 0)
    // Если остался перенос в bits[3], значит переполнение
    res = IS_TOO_LARGE;
  check_res_code(&value_1, &value_2);

  return res;
}

res_code decimal_multiply_by_10(s21_decimal *value) {
  res_code res = OK;

  if (value->scale >= max_scale) res = IS_TOO_LARGE;

  s21_decimal dec1 = *value;
  s21_decimal dec2 = *value;
  if (res == OK) res = decimal_shift_left(&dec1, 3);  // умножение на 8
  if (res == OK) res = decimal_shift_left(&dec2, 1);  // умножение на 2
  if (res == OK)
    res = add_decimal_mantissa(
        dec1, dec2, value);  // Сложение "сдвинутых" мантисс 8 + 2 = 10

  return res;
}

// деление на 10 в столбик с остатком
// value_1 - делимое, quotient - частное, remainder - остаток
// деление выполняется от старшего бита к младшему
// результат записывается в quotient, остаток в remainder
res_code div_by_10(const s21_decimal value_1, s21_decimal *quotient,
                   unsigned *remainder) {
  s21_decimal quot = {0};
  s21_decimal ten = {0};
  unsigned rem = 0;
  res_code res = OK;

  quot.bits[3] = value_1.bits[3];
  ten.bits[0] = 10;

  if (is_zero(value_1))
    res = OK;
  else if (s21_is_less(s21_mod(value_1), ten)) {
    // проверка что число 0 < x < 10
    if (value_1.scale == max_scale)
      res = IS_TOO_SMALL;  // деление на 10 невозможно. Число слишком маленькое
    else {
      rem = value_1.bits[0];  // остаток равен делимому
      quot.bits[0] = 0;       // частное равно 0.
      quot.scale =
          value_1.scale + 1;  // увеличиваем scale на 1, т.к. делим на 10
    }
  } else {
    for (int i = bits_in_mantissa - 1; i >= 0; --i) {
      rem = (rem << 1) | get_bit(value_1, i);
      // если rem >= 10, вычитаем 10 и устанавливаем 1 в результат
      if (rem >= 10) {
        rem -= 10;
        set_bit(&quot, i, 1);
      } else
        set_bit(&quot, i, 0);
    }
  }

  *quotient = quot;
  *remainder = rem;
  return res;
}
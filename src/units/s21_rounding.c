#include "../s21_decimal.h"
// This project made by beedlesh haywoota woodsjul thymemas

const s21_decimal zero_dec = {{0, 0, 0, 0}};  // 0
const s21_decimal one = {{1, 0, 0, 0}};       // 1

// Отбрасывание дробной части
int s21_truncate(s21_decimal value, s21_decimal *result) {
  res_code truncation_res = OK;
  if (result == NULL)
    truncation_res = CALCULATION_ERR;
  else {
    make_zero_decimal(result);
    result->is_negative = value.is_negative;  // scale оставляем 0
    for (unsigned i = 0; i < ints_in_mantissa; i++)
      result->bits[i] = value.bits[i];
    int scale = (int)value.scale;

    while (scale && truncation_res == OK) {
      unsigned remainder = 0;
      truncation_res =
          div_by_10(*result, result,
                    &remainder);  // деление на 10 нацело (остаток игнорируем)
      --scale;
    }
    result->scale = 0;  // на всякий случай
    if (truncation_res != OK) make_zero_decimal(result);
  }
  return truncation_res;
}

// общая функция округления - вниз и до ближайшего целого
int round_by_type(int (*round_type)(s21_decimal, s21_decimal *),
                  s21_decimal value, s21_decimal *result) {
  res_code res = OK;
  if (result == NULL) res = CALCULATION_ERR;
  s21_decimal int_dec = zero_dec;  // целая часть положительного числа
  s21_bool minus = value.is_negative;

  if (res == OK && !is_zero(value)) {
    value.is_negative = 0;

    // получение обрезанной целой части и дробной части
    res = s21_truncate(value, &int_dec);
    s21_decimal frac_part = zero_dec;
    if (res == OK) res = s21_sub(value, int_dec, &frac_part);

    if (round_type == s21_floor && s21_is_greater(frac_part, zero_dec)) {
      if (minus)
        // -123.234 -> 123 -> 124 -> -124
        res = s21_add(int_dec, one, &int_dec);
    }
    if (round_type == s21_round && s21_is_greater(frac_part, zero_dec)) {
      s21_decimal half = {{5, 0, 0, 0}};  // 0.5
      half.scale = 1;
      // frac_part < 0.5 => оставляем как есть
      // frac_part >= 0.5 => прибавляем 1 (здесь нет банковского округления!)
      if (s21_is_greater_or_equal(frac_part, half))
        res = s21_add(int_dec, one, &int_dec);
    }
  }
  if (res == OK) {
    if (minus) int_dec.is_negative = TRUE;
    *result = int_dec;
  }

  return res;
}

// Округление до ближайшего целого числа в сторону отрицательной бесконечности
int s21_floor(s21_decimal value, s21_decimal *result) {
  return round_by_type(s21_floor, value, result);
}

// Округление до ближайшего целого
int s21_round(s21_decimal value, s21_decimal *result) {
  return round_by_type(s21_round, value, result);
}
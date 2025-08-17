#include "../s21_decimal.h"
// This project made by beedlesh haywoota woodsjul thymemas

// -1 для v1 < v2, 0 для v1 == v2, 1 для v1 > v2
int align_and_compare(s21_decimal value_1, s21_decimal value_2) {
  // чтобы -0 стал 0:
  if (is_zero(value_1)) value_1.is_negative = FALSE;
  if (is_zero(value_2)) value_2.is_negative = FALSE;

  big_decimal bc_value_1 = {0};
  big_decimal bc_value_2 = {0};
  decimal_to_bigdec(value_1, &bc_value_1);
  decimal_to_bigdec(value_2, &bc_value_2);
  bigdec_alignment(&bc_value_1, &bc_value_2);
  return bigdec_comparison(bc_value_1, bc_value_2);
}

// Проверка на равенство
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  return (align_and_compare(value_1, value_2) == 0) ? TRUE : FALSE;
}

// Проверка на меньше
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  return (align_and_compare(value_1, value_2) < 0) ? TRUE : FALSE;
}

// Проверка на меньше или равно
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return (align_and_compare(value_1, value_2) <= 0);
}

// Проверка на больше
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less_or_equal(value_1, value_2);
}

// Проверка на больше или равно
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less(value_1, value_2);
}

// Проверка на неравенство
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
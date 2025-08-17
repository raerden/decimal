#include "../s21_decimal.h"
// This project made by beedlesh haywoota woodsjul thymemas

// Сложение двух чисел
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  big_decimal bd_res = {0};
  res_code res = OK;
  make_zero_decimal(result);

  if (value_1.is_negative && !value_2.is_negative) {
    value_1.is_negative = 0;
    res = s21_sub(value_2, value_1, result);
  } else if (!value_1.is_negative && value_2.is_negative) {
    value_2.is_negative = 0;
    res = s21_sub(value_1, value_2, result);
  } else {  // одного знака
    big_decimal bd_val1 = {0};
    big_decimal bd_val2 = {0};
    decimal_to_bigdec(value_1, &bd_val1);
    decimal_to_bigdec(value_2, &bd_val2);
    bigdec_alignment(&bd_val1, &bd_val2);
    bigdec_add_mantissa(bd_val1, bd_val2, &bd_res);
    bd_res.scale = bd_val1.scale;
    bd_res.is_negative = bd_val1.is_negative;
    res = bigdec_to_decimal(bd_res, result);
  }
  return res;
}

// Вычитание двух чисел
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  big_decimal bd_res = {0};
  res_code res = OK;
  make_zero_decimal(result);

  if (value_1.is_negative && !value_2.is_negative) {
    value_2.is_negative = 1;
    res = s21_add(value_1, value_2, result);
  } else if (!value_1.is_negative && value_2.is_negative) {
    value_2.is_negative = 0;
    res = s21_add(value_1, value_2, result);
  } else {  // одного знака
    big_decimal bd_val1 = {0};
    big_decimal bd_val2 = {0};
    decimal_to_bigdec(value_1, &bd_val1);
    bd_val1.is_negative = 0;
    decimal_to_bigdec(value_2, &bd_val2);
    bd_val2.is_negative = 0;
    bigdec_alignment(&bd_val1, &bd_val2);
    if (bigdec_comparison(bd_val1, bd_val2) < 0) {
      bigdec_sub_mantissa(bd_val2, bd_val1, &bd_res);
      bd_res.is_negative = !value_2.is_negative;
    } else {
      bigdec_sub_mantissa(bd_val1, bd_val2, &bd_res);
      bd_res.is_negative = value_1.is_negative;
    }
    bd_res.scale = bd_val1.scale;
    res = bigdec_to_decimal(bd_res, result);
  }
  return res;
}

// Умножение двух чисел
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  big_decimal bd_res = {0};
  res_code res = -1;
  make_zero_decimal(result);

  if (is_zero(value_1) || is_zero(value_2))
    res = OK;
  else {
    big_decimal bd_val1 = {0};
    big_decimal bd_val2 = {0};
    decimal_to_bigdec(value_1, &bd_val1);
    decimal_to_bigdec(value_2, &bd_val2);
    bigdec_mul_mantissa(bd_val1, bd_val2, &bd_res);
    bd_res.is_negative = value_1.is_negative ^ value_2.is_negative;
    bd_res.scale = value_1.scale + value_2.scale;
    res = bigdec_to_decimal(bd_res, result);
  }
  return res;
}

// Деление двух чисел
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  res_code res = -1;
  make_zero_decimal(result);

  if (is_zero(value_2))
    res = DIV_BY_ZERO;
  else if (is_zero(value_1)) {
    res = OK;
    result->is_negative = value_1.is_negative ^ value_2.is_negative;
  } else {
    big_decimal bd_val1 = {0};
    big_decimal bd_val2 = {0};
    big_decimal bd_res = {0};
    decimal_to_bigdec(value_1, &bd_val1);
    bd_val1.is_negative = 0;
    decimal_to_bigdec(value_2, &bd_val2);
    bd_val2.is_negative = 0;
    bigdec_alignment(&bd_val1, &bd_val2);
    bigdec_div_mantissa(bd_val1, bd_val2, &bd_res);
    bd_res.is_negative = value_1.is_negative ^ value_2.is_negative;
    res = bigdec_to_decimal(bd_res, result);
  }
  return res;
}
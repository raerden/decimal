#include "../s21_decimal.h"

// Арифметические операторы

// Сложение двух чисел
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    res_code res = OK;
    big_decimal bd_val1, bd_val2, bd_res = {0};

    make_zero_decimal(result);    
    decimal_to_bigdec(value_1, &bd_val1);
    decimal_to_bigdec(value_2, &bd_val2);
    res = bigdec_alignment(&bd_val1, &bd_val2);

    if (res == OK) {
        bigdec_add_mantissa(bd_val1, bd_val2, &bd_res);
        res = bigdec_to_decimal(bd_res, result);
    }
    
    return res;
}

// Вычитание двух чисел
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    result->bits[0] = value_1.bits[0] - value_2.bits[0];
    return OK;
}

// Умножение двух чисел
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  

// Деление двух чисел
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  
#include "../s21_decimal.h"

// Арифметические операторы

// Сложение двух чисел
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  

// Вычитание двух чисел
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    result->bits[0] = value_1.bits[0] - value_2.bits[0];
    return OK;
}

// Умножение двух чисел
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  

// Деление двух чисел
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  
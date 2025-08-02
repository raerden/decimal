#include "../s21_decimal.h"

// Операторы сравнения

/*

1. проверяем знак числа
if (value_1.is_negative != value_2.is_negative) {
    res = value_1.is_negative ? -1 : 1;
} else {
    // сравниваем мантиссы
    ...
}

*/

// Проверка на меньше
int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
    big_decimal bc_value_1, bc_value_2 = {0};
    int res;
    decimal_to_bigdec(value_1, &bc_value_1);
    decimal_to_bigdec(value_2, &bc_value_2);
    if(bigdec_alignment(&bc_value_1, &bc_value_2)==OK) res=bigdec_comparison_mantissa(bc_value_1, bc_value_2);

    return res==-1?1:0;
}             

// Проверка на меньше или равно
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
    big_decimal bc_value_1, bc_value_2 = {0};
    int res;
    decimal_to_bigdec(value_1, &bc_value_1);
    decimal_to_bigdec(value_2, &bc_value_2);
    if(bigdec_alignment(&bc_value_1, &bc_value_2)==OK) res=bigdec_comparison_mantissa(bc_value_1, bc_value_2);

    return res<1?1:0;
}   

// Проверка на больше
int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
    big_decimal bc_value_1, bc_value_2 = {0};
    int res;
    decimal_to_bigdec(value_1, &bc_value_1);
    decimal_to_bigdec(value_2, &bc_value_2);
    if(bigdec_alignment(&bc_value_1, &bc_value_2)==OK) res=bigdec_comparison_mantissa(bc_value_1, bc_value_2);

    return res==1?1:0;
}         

// Проверка на больше или равно
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    big_decimal bc_value_1, bc_value_2 = {0};
    int res;
    decimal_to_bigdec(value_1, &bc_value_1);
    decimal_to_bigdec(value_2, &bc_value_2);
    if(bigdec_alignment(&bc_value_1, &bc_value_2)==OK) res=bigdec_comparison_mantissa(bc_value_1, bc_value_2);

    return res>-1?1:0;
} 

// Проверка на равенство
int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
    big_decimal bc_value_1, bc_value_2 = {0};
    int res;
    decimal_to_bigdec(value_1, &bc_value_1);
    decimal_to_bigdec(value_2, &bc_value_2);
    if(bigdec_alignment(&bc_value_1, &bc_value_2)==OK) res=bigdec_comparison_mantissa(bc_value_1, bc_value_2);

    return res==0?1:0;
}             

// Проверка на неравенство 
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
    big_decimal bc_value_1, bc_value_2 = {0};
    int res;
    decimal_to_bigdec(value_1, &bc_value_1);
    decimal_to_bigdec(value_2, &bc_value_2);
    if(bigdec_alignment(&bc_value_1, &bc_value_2)==OK) res=bigdec_comparison_mantissa(bc_value_1, bc_value_2);

    return res!=0?1:0;
}     
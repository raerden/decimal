#include "../s21_decimal.h"

// Арифметические операторы

// Сложение двух чисел
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    make_zero_decimal(result);
    big_decimal bd_res = {0};
    
    if (value_1.is_negative && !value_2.is_negative) s21_sub(value_2, value_1, result);
    else if (!value_1.is_negative && value_2.is_negative) s21_sub(value_1, value_2, result);
    else { // одного знака
        
        big_decimal bd_val1 = {0};
        big_decimal bd_val2 = {0};
        decimal_to_bigdec(value_1, &bd_val1);
        decimal_to_bigdec(value_2, &bd_val2);
        bigdec_alignment(&bd_val1, &bd_val2);
        
        bigdec_add_mantissa(bd_val1, bd_val2, &bd_res);
        bd_res.scale = bd_val1.scale;
        bd_res.is_negative = bd_val1.is_negative;

    }
    return bigdec_to_decimal(bd_res, result);

} 

// Вычитание двух чисел
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    make_zero_decimal(result);
    big_decimal bd_val1 = {0};
    big_decimal bd_val2 = {0};
    big_decimal bd_res = {0};
    
    decimal_to_bigdec(value_1, &bd_val1);
    decimal_to_bigdec(value_2, &bd_val2);
    
    if (value_1.is_negative && !value_2.is_negative) {
        //первое отрицательное -7-5 = -12, -5-7 = -12
        bigdec_add_mantissa(bd_val1, bd_val2, &bd_res);
        bd_res.is_negative = 1;
    } else if (!value_1.is_negative && value_2.is_negative) {
        //второе отрицательное. Просто сложить
        bigdec_add_mantissa(bd_val1, bd_val2, &bd_res);
    } else if (!value_1.is_negative && !value_2.is_negative) {
        //оба положительные
        if (s21_is_less(value_1, value_2)) {
            //первое число меньше второго. 5 - 7 = -2
            bigdec_sub_mantissa(bd_val2, bd_val1, &bd_res);
            bd_res.is_negative = 1;
        } else { // 7 - 5 = 2
            bigdec_sub_mantissa(bd_val1, bd_val2, &bd_res);
            bd_res.is_negative = 0;
        }
    } else if (value_1.is_negative && value_2.is_negative) {
        //оба отрицательные
        if (s21_is_less(value_1, value_2)) { // -5 - -7 = 2
            //первое число меньше второго. Вычитаем первое из второго
            bigdec_sub_mantissa(bd_val2, bd_val1, &bd_res);
            bd_res.is_negative = 0;//знак положительный. 
        } else {    // -7 - -5 = -2
            bigdec_sub_mantissa(bd_val1, bd_val2, &bd_res);
            bd_res.is_negative = 1;
        }
    }

    return bigdec_to_decimal(bd_res, result);
}

// Умножение двух чисел
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  

// Деление двух чисел
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  
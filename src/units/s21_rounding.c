#include "../s21_decimal.h"

// Округление до ближайшего целого числа вниз
int s21_floor(s21_decimal value, s21_decimal *result);      // 5465466

// Округление до ближайшего целого
int s21_round(s21_decimal value, s21_decimal *result);      

// Отбрасывание дробной части
int s21_truncate(s21_decimal value, s21_decimal *result)
{
    res_code truncation_res = OK;
    if (result == NULL)
        truncation_res = CALCULATION_ERR;
    else
    {
        make_zero_decimal(result);
        result->is_negative = value.is_negative;        // scale оставляем 0
        for (unsigned i = 0; i <= ints_in_mantissa; i++)
            result->bits[i] = value.bits[i];
        // int scale = (int)value.scale;
        // while (scale) 
        // {
        //     bitwise_div_by_ten(result);                 // деление на 10 нацело (остаток игнорируем)
        //     --scale;
        // }
    }
    return truncation_res;
    
}  
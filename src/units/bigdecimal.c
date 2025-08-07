#include "../s21_decimal.h"

// Проверка на ноль
s21_bool bigdec_is_zero(big_decimal const value) {
    int res = TRUE;
    for (unsigned i = 0; (res == TRUE) && (i < INTS_IN_BIGDECIMAL-1); i++)
        if (value.bits[i] != 0)
            res = FALSE; // не ноль
    return res; 
}

// вспомогательная функция для get и set bit
void bigdec_get_correct_offset(unsigned bit_index, unsigned* byte_index, unsigned* bit_offset)
{
    *byte_index = bit_index / bits_in_int;           // Индекс байта 0,1,2
    *bit_offset = bit_index % bits_in_int;           // Смещение бита в байте
}

// Получение значения бита по индексу
int bigdec_get_bit(big_decimal value, unsigned bit_index) {
    unsigned byte_index = 0;
    unsigned bit_offset = 0;
    bigdec_get_correct_offset(bit_index, &byte_index, &bit_offset);
        
    const unsigned mask = (1 << bit_offset);
    int res = !!(value.bits[byte_index] & mask);    // двойное отрицание делает из числа 0 или 1 (!!30 => 1 !! 0 => 0)

    return res;
}

// Установка значения бита по индексу
void bigdec_set_bit(big_decimal *value, unsigned bit_index, unsigned bit_value) {
    unsigned byte_index = 0;
    unsigned bit_offset = 0;
    bigdec_get_correct_offset(bit_index, &byte_index, &bit_offset);
    
    if (bit_value)
        value->bits[byte_index] |= (1 << bit_offset);  // Установка бита в 1 через побитовое ИЛИ
    else
        value->bits[byte_index] &= ~(1 << bit_offset); // Установка бита в 0 через побитовое И
}

// Смещение битов влево на Shift
res_code bigdec_shift_left(big_decimal *value, unsigned shift) {
    res_code res = OK;
    int z = shift;
    while(z > 0) {
        shift = z > 31 ? 31 : z;  
            
        int overflow = 0;
        for (unsigned i = 0; i < ints_in_big_mantissa; i++) {
            unsigned memo = value->bits[i];
            value->bits[i] = (memo << shift) | overflow; // Сдвиг влево с учетом предыдущего переполнения
            overflow = memo >> (bits_in_int - shift);    // Сохраняем переполнение
        }
        // Если в overflow есть значение для bits[7], возвращаем ошибку переполнения
        if (overflow)
            res = OUT_OF_RANGE; 
        z = z - shift;    
    }
    return res;
}

void make_zero_bigdec(big_decimal* bd)
{
    for (unsigned i = 0; i < INTS_IN_BIGDECIMAL; i++)
        bd->bits[i] = 0;
}

// Сложение мантисс двух чисел
void bigdec_add_mantissa(big_decimal value_1, big_decimal value_2, big_decimal *result) {
    make_zero_bigdec(result);
    unsigned memo = 0;
    unsigned i = 0;

    for (i = 0; i < ints_in_big_mantissa * bits_in_int; i++) {        // 7 * 32 = 224
        int sum = bigdec_get_bit(value_1, i) + bigdec_get_bit(value_2, i) + memo;
        if (sum > 1)
            memo = 1;  // Перенос в следующий бит
        else
            memo = 0;
        sum %= 2;      // 1 + 1 = 2 % 2 = 0
        bigdec_set_bit(result, i, sum);
    }

    bigdec_set_bit(result, i, memo);
}

//вычитание мантисс
int bigdec_sub_mantissa(big_decimal value_1, big_decimal value_2, big_decimal *result) {
    unsigned znach=0;
    big_decimal one = { {1, 0, 0, 0, 0, 0, 0} };
    big_decimal vich_1 = {0};
    for(unsigned i=0; i<bits_in_big_mantissa; i++){
        if(bigdec_get_bit(value_1, i) ) znach = i;
    }
    for(unsigned i=0; i<znach+1; i++){
        bigdec_set_bit(&value_2, i, bigdec_get_bit(value_2, i)  ? 0: 1);
    }
    bigdec_add_mantissa(value_2, one, &vich_1);
    bigdec_add_mantissa(value_1, vich_1, result);
    bigdec_set_bit(result, znach+1, 0);
    return 0;
}

res_code bigdec_multiply_by_10(big_decimal* value) {
    big_decimal dec1 = *value; 
    big_decimal dec2 = *value; 
    s21_bool is_negative = value->is_negative;
    res_code res = bigdec_shift_left(&dec1, 3);   // умножение на 8
    if (res == OK)
    {
        bigdec_shift_left(&dec2, 1);             // умножение на 2
        bigdec_add_mantissa(dec1, dec2, value);  // cложение "cдвинутых" мантисс 8 + 2 = 10
        value->is_negative = is_negative;     
    }
    return res;
}

void bigdec_alignment(big_decimal* value_1, big_decimal* value_2) {
    unsigned scale_diff = 0;
    if (value_1->scale > value_2->scale) {
        scale_diff = value_1->scale - value_2->scale;
        unsigned scale = value_2->scale;
        for (unsigned i = 0; i < scale_diff; i++) {
            bigdec_multiply_by_10(value_2);         // Умножаем value_2 на 10 scale_diff раз
            scale++;
        }
        value_2->scale = scale;
    } else if (value_2->scale > value_1->scale) {
        scale_diff = value_2->scale - value_1->scale;
        unsigned scale = value_1->scale;
        for (unsigned i = 0; i < scale_diff; i++) {
            bigdec_multiply_by_10(value_1);         // Умножаем value_1 на 10 scale_diff раз
            scale++;
        }
        value_1->scale = scale;
    }
}

// Смещение битов вправо на shift
res_code bigdec_shift_right(big_decimal *value, const unsigned shift) {
    res_code  res = OK;

    if (shift > 0) {
        unsigned overflow = 0;
        for (int i = ints_in_big_mantissa - 1; i >= 0; i--) {
            unsigned memo = value->bits[i];
            value->bits[i] = overflow | (memo >> shift); // Сдвиг вправо с учетом предыдущего бита
            overflow = memo << (bits_in_int - shift);    // Сохраняем правую часть текущего бита
        }

        // Если в overflow есть значение для bits[7], возвращаем ошибку переполнения
        if (overflow != 0)
            res = OUT_OF_RANGE; 
    }

    return res;
}

//-1 - первое меньше, 0 - равны, 1 - первое больше
int bigdec_comparison(big_decimal value_1, big_decimal value_2) {
    const int equal = 0;
    const int v1_less = -1;
    const int v2_less = 1;
    int comp_res = equal;

    if (value_1.is_negative && !value_2.is_negative) comp_res = v1_less;
    else if (!value_1.is_negative && value_2.is_negative) comp_res = v2_less;
    else // одного знака
    {
        s21_bool both_negative = (value_1.is_negative && value_2.is_negative);
        s21_bool both_positive = !both_negative;
        for(int i = bits_in_big_mantissa - 1; (i >= 0) && (comp_res == equal); i--)
        {
            if (bigdec_get_bit(value_1, i) < bigdec_get_bit(value_2, i))
                comp_res = (both_positive)? v1_less : v2_less;
            else if (bigdec_get_bit(value_1, i) > bigdec_get_bit(value_2, i))
                comp_res = (both_positive)? v2_less : v1_less;
        }
    }
    return comp_res;
}

res_code decimal_to_bigdec(s21_decimal dec, big_decimal *big_dec) {
    big_dec->mantissa[0] = dec.low;
    big_dec->mantissa[1] = dec.middle;
    big_dec->mantissa[2] = dec.high;
    big_dec->mantissa[3] = 0;
    big_dec->mantissa[4] = 0;
    big_dec->mantissa[5] = 0;
    big_dec->mantissa[6] = 0;
    big_dec->lower_word = dec.lower_word;
    big_dec->scale = dec.scale;
    big_dec->zero = dec.zero;
    big_dec->is_negative = dec.is_negative;

    return OK;
} 

//деление мантисс
int bigdec_div_mantissa(big_decimal value_1, big_decimal value_2, big_decimal *result) {
    big_decimal ostatok;
    unsigned ost = value_1.bits[0] | value_1.bits[1] | value_1.bits[2] | value_1.bits[3] | value_1.bits[4] | value_1.bits[5] | value_1.bits[6] ;
    unsigned znach_1, znach_2, flag = 0;
    int shift;

    for(unsigned i=0; i<bits_in_big_mantissa; i++){
        if(bigdec_get_bit(value_1, i) ) znach_1 = i;
    }
    for(unsigned i=0; i<bits_in_big_mantissa; i++){
        if(bigdec_get_bit(value_2, i) ) znach_2 = i;
    }
    shift = znach_1-znach_2;
    bigdec_shift_left(&value_2, shift);

    while (ost!=0 && flag==0 )
    {   bigdec_shift_left(result, 1);

        if(bigdec_comparison(value_1, value_2)!=-1) {
            
            bigdec_sub_mantissa(value_1, value_2, &ostatok);
            ost = ostatok.bits[0] | ostatok.bits[1] | ostatok.bits[2] | ostatok.bits[3] | ostatok.bits[4] | ostatok.bits[5] | ostatok.bits[6] ;
            bigdec_set_bit(result, 0, 1);
        } else { 
            bigdec_set_bit(result, 0, 0);
        }

        if(shift>0 && ost==0)
            bigdec_shift_left(result, shift);

        

        if(shift>=0 && ost!=0){bigdec_shift_right(&value_2, 1);} 
        if(shift<0 && ost!=0) {bigdec_shift_left(&ostatok, 1); result->scale++;}

        shift--;

        flag = bigdec_get_bit(*result, bits_in_big_mantissa-1);
        value_1 = ostatok;
    }

    return 0;
}

res_code bigdec_div_by_10(big_decimal* value){
    big_decimal result = {0};
    big_decimal ostatok;
    big_decimal value_1 = *value;
    unsigned ost = value_1.bits[0] | value_1.bits[1] | value_1.bits[2] | value_1.bits[3] | value_1.bits[4] | value_1.bits[5] | value_1.bits[6] ;
    unsigned znach_1, flag = 0;
    int shift;
    big_decimal ten = { {10, 0, 0, 0, 0, 0, 0} };

    for(unsigned i=0; i<bits_in_big_mantissa; i++){
        if(bigdec_get_bit(value_1, i) ) znach_1 = i;
    }

    shift = znach_1-3;
    bigdec_shift_left(&ten, shift);

    while (ost!=0 && flag==0 )
    {   bigdec_shift_left(&result, 1);
        if(bigdec_comparison(value_1, ten)!=-1) {
            
            bigdec_sub_mantissa(value_1, ten, &ostatok);
            ost = ostatok.bits[0] | ostatok.bits[1] | ostatok.bits[2] | ostatok.bits[3] | ostatok.bits[4] | ostatok.bits[5] | ostatok.bits[6] ;
            bigdec_set_bit(&result, 0, 1);
        } else { 
            bigdec_set_bit(&result, 0, 0);
        }
        
        if(shift>0 && ost==0){bigdec_shift_left(&result, shift);} 
        shift=shift-1;
        if(shift>=0 && ost!=0){bigdec_shift_right(&ten, 1);} 

        value_1 = ostatok;
        
    }
    for(int i = 0; i>INTS_IN_BIGDECIMAL-1; i++) value->bits[i] = result.bits[i];

    return OK;
}

unsigned max_nonzero_bit(big_decimal value) {
    unsigned max_bit = 0;
    
    for(unsigned i = 0; i < bits_in_big_mantissa; i++)
        if (bigdec_get_bit(value, i)) max_bit = i;

    return max_bit;
}

res_code bigdec_to_decimal(big_decimal big_dec, s21_decimal* dec) {
    res_code res;
    const big_decimal ten = { {10, 0, 0, 0, 0, 0, 0} };

    if(max_nonzero_bit(big_dec) - big_dec.scale == 95) {
        bigdec_round(&big_dec);
    }

    if (big_dec.scale==0 && max_nonzero_bit(big_dec)>95) 
        res = IS_TOO_LARGE;
    
    if (big_dec.scale - max_nonzero_bit(big_dec) > max_scale)
        res = IS_TOO_SMALL;

    while(big_dec.scale>max_scale){ 
        if(max_nonzero_bit(big_dec)<95) bigdec_div_mantissa(big_dec, ten, &big_dec);
        else {bigdec_div_by_10(&big_dec); big_dec.scale --;}
        
    }

    if (big_dec.scale<29 && max_nonzero_bit(big_dec)<96) { //!!!!
        dec->low = big_dec.mantissa[0];
        dec->middle = big_dec.mantissa[1];
        dec->high = big_dec.mantissa[2];
        dec->lower_word = big_dec.lower_word;
        dec->scale = big_dec.scale;
        dec->zero = big_dec.zero;
        dec->is_negative = big_dec.is_negative;
        res = OK;
    }

    return res;
}

//умножение мантисс big_decimal
void bigdec_mul_mantissa(big_decimal value_1, big_decimal value_2, big_decimal *result) {
    big_decimal val = {0}, res = {0};
    for(unsigned i=0; i<bits_in_big_mantissa; i++){        
        if(bigdec_get_bit(value_2, i) ) {
            val= value_1;
            bigdec_shift_left(&val, i);
            bigdec_add_mantissa(res, val, result);
            res = *result;
        }
    }
}

void bigdec_round(big_decimal *value) {
    const big_decimal one_left = { {0, 0, 0, 0, 0, 0, 0x80000000} };
    big_decimal one = { {1, 0, 0, 0, 0, 0, 0} };

    big_decimal a = *value;
    bigdec_shift_left(&a, bits_in_big_mantissa-a.scale);
    bigdec_shift_right(value, a.scale);
    value->scale = 0;
    if(bigdec_comparison(a, one_left)==1) bigdec_add_mantissa(*value, one, value); 
    if(bigdec_comparison(a, one_left)==0 && bigdec_get_bit(*value, 0)) bigdec_add_mantissa(*value, one, value); 


}

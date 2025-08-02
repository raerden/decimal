#include "s21_decimal.h"

// Проверка на ноль
int is_zero(s21_decimal const *value) {
    return !(value->bits[0] || value->bits[1] || value->bits[2]);
}

// вспомогательная функция для get и set bit
int get_correct_offset(int bit_index, int* byte_index, int* bit_offset)
{
    res_code res = OK;
    if (bit_index < 0 || ((unsigned)bit_index >= bits_in_mantissa))
        res = OUT_OF_RANGE;
    *byte_index = bit_index / bits_in_int;           // Индекс байта 0,1,2
    *bit_offset = bit_index % bits_in_int;           // Смещение бита в байте
    return res;
}

// Получение значения бита по индексу
int get_bit(s21_decimal value, int bit_index) {
    int byte_index = 0;
    int bit_offset = 0;
    res_code res = get_correct_offset(bit_index, &byte_index, &bit_offset);
    
    if ( res != OUT_OF_RANGE)
    {
        const int mask = 1 << bit_offset;
        res = !!(value.bits[byte_index] & mask);
    }
    return res;
}

// Установка значения бита по индексу
int set_bit(s21_decimal *value, int bit_index, int bit_value) {
    int byte_index = 0;
    int bit_offset = 0;
    res_code res = get_correct_offset(bit_index, &byte_index, &bit_offset);
    
    if (res != OUT_OF_RANGE)
    {
        if (bit_value)
            value->bits[byte_index] |= (1 << bit_offset); // Установка бита в 1 через побитовое ИЛИ
        else
            value->bits[byte_index] &= ~(1 << bit_offset); // Установка бита в 0 через побитовое И
        res = OK;
    }
    return res;
}

// выводит на экран  число типа unsigned int в двоичной записи    -> *((unsigned*)&num)
void printBinary(unsigned num) {
    for (int i = bits_in_int - 1; i >= 0; i--) {
        if (((num >> i) & 1) == 0) {
             printf("0");
        } else {
            printf("\033[3m1\033[0m");
        }
        // printf("%d", (num >> i) & 1);
        if (i % bits_in_byte == 0) printf(" ");
    }
    printf("\n");
}

// выводит на экран decimal число в двоичной записи
void printDecimalBinary(s21_decimal num) {
    for(int i = ints_in_mantissa; i >= 0; i--) {
        printf("[%d] ", i);
        printBinary(num.bits[i]);
    }
}

// выводит на экран decimal  как инты
void printDecimalAsInts(s21_decimal num) {
    if (num.is_negative) putc('-', stdout);
    for(int i = ints_in_mantissa-1; i >= 0; i--)
        printf("%u ", num.bits[i]);
    printf("scale = %u\n", num.scale);
}

void printBigDecimalBinary(big_decimal num, const char *ints) {
    for(int i = INTS_IN_BIGDECIMAL - 1; i >= 0; i--) {
        if (strlen(ints) && strchr(ints, '0' + i) == NULL) continue; // Пропускаем индексы, не указанные в строке
        printf("[%d] ", i);
        printBinary(num.bits[i]);
    }
}

// зануляет переданный децимал
void make_zero_decimal(s21_decimal* dcm)
{
    for (unsigned i = 0; i <= ints_in_mantissa; i++ )
        dcm->bits[i] = 0;
}

// Возвращает результат умножения указанного decimal на -1
int s21_negate(s21_decimal value, s21_decimal *result)
{
    if (!result) return CALCULATION_ERR;
    for (unsigned i = 0; i <= ints_in_mantissa; i++)
        result->bits[i] = value.bits[i];

    result->is_negative = !(value.is_negative);
    
    return OK;
}

// Смещение битов влево на shift
int decimal_shift_left(s21_decimal *value, unsigned shift) {
    int res = OK;

    if (res == OK && shift > 0) {
        unsigned overflow = 0;
        for (unsigned i = 0; i < ints_in_mantissa; i++) {
            unsigned memo = value->bits[i];
            value->bits[i] = (memo << shift) | overflow; // Сдвиг влево с учетом предыдущего переполнения
            overflow = memo >> (bits_in_int - shift); // Сохраняем переполнение
        }
        // Если в overflow есть значение для bits[3], возвращаем ошибку переполнения
        if (overflow != 0) {
            res = OUT_OF_RANGE; 
        }
    }

    return res;
}

// Сложение мантисс двух чисел
int add_decimal_mantissa(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    int res = OK;

    unsigned memo = 0;
    for (unsigned i = 0; res == OK && i < bits_in_mantissa; i++) {
        unsigned sum = get_bit(value_1, i) + get_bit(value_2, i) + memo;
        if (sum > 1)
            memo = 1; // Перенос в следующий бит
        else
            memo = 0;
        sum %= 2; // 1 + 1 = 2 % 2 = 0
        res = set_bit(result, i, sum);
    }

    if (memo > 0) {
        // Если остался перенос в bits[3], значит переполнение
        res = OUT_OF_RANGE;
        printf("add_decimal_mantissa: overflow occurred, returning OUT_OF_RANGE\n");
    }

    return res;
}

int decimal_multiply_by10(s21_decimal *value) {
    int res = OK;
    
    if (value->scale >= max_scale) {
        res = OUT_OF_RANGE;
        printf("s21_decimal_multiply_by10: scale overflow, returned OUT_OF_RANGE\n");
    }
    else 
        value->scale++;
    
    s21_decimal dec1 = *value; 
    s21_decimal dec2 = *value; 
    if (res == OK)
        res = decimal_shift_left(&dec1, 3);  // умножение на 8
    if (res == OK) 
        res = decimal_shift_left(&dec2, 1);  // умножение на 2
    if (res == OK) 
        res = add_decimal_mantissa(dec1, dec2, value); // Сложение "двинутых" мантисс 8 + 2 = 10

    return res;
}

int decimalAlignment(s21_decimal *value_1, s21_decimal *value_2) {
    int res = OK;

    if (value_1->scale > value_2->scale) {
        unsigned scale_diff = value_1->scale - value_2->scale;
        for (unsigned i = 0; res == OK && i < scale_diff; i++)
            res = decimal_multiply_by10(value_2); // Умножаем value_2 на 10 scale_diff раз
    } else if (value_2->scale > value_1->scale) {
        unsigned scale_diff = value_2->scale - value_1->scale;
        for (unsigned i = 0; res == OK && i < scale_diff; i++)
            res = decimal_multiply_by10(value_1); // Умножаем value_1 на 10 scale_diff раз
    }

    return res;
}
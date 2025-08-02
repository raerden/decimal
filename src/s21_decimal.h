#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // для конвертации из float в decimal
#include <limits.h>   // для граничных значений типов (при преобразованиях)

// константы
#define INTS_IN_DECIMAL 4
#define INTS_IN_BIGDECIMAL 8
static const unsigned bits_in_byte = 8;
static const unsigned bits_in_int = 32;
static const unsigned ints_in_mantissa = 3;
static const unsigned bits_in_mantissa = ints_in_mantissa * bits_in_int;
static const unsigned ints_in_big_mantissa = INTS_IN_BIGDECIMAL - 1;
static const unsigned bits_in_big_mantissa = ints_in_big_mantissa * bits_in_int;
static const unsigned max_scale = 28;


typedef union {
    unsigned bits[INTS_IN_DECIMAL];
    struct {
        unsigned low;
        unsigned middle;
        unsigned high;
        unsigned lower_word:16;
        unsigned scale:8;
        unsigned zero:7;
        unsigned is_negative:1;
    };
} s21_decimal;

static const s21_decimal max_decimal = {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};

typedef union {
    unsigned bits[INTS_IN_BIGDECIMAL];
    struct {
        unsigned mantissa[INTS_IN_BIGDECIMAL - 1];
        unsigned lower_word:16;
        unsigned scale:8;
        unsigned zero:7;
        unsigned is_negative:1;
    };
} big_decimal;

typedef enum {
    FALSE,
    TRUE,
} s21_bool;

typedef enum {
    OUT_OF_RANGE = -1,
    OK = 0,
    IS_TOO_LARGE = 1,
    CONVERTATION_ERR = 1,
    CALCULATION_ERR = 1,
    IS_TOO_SMALL = 2,
    DIV_BY_ZERO = 3,
} res_code;

// Арифметические операторы
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  // Сложение двух чисел
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  // Вычитание двух чисел
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  // Умножение двух чисел
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);  // Деление двух чисел

// Операторы сравнения
int s21_is_less(s21_decimal, s21_decimal);              // Проверка на меньше
int s21_is_less_or_equal(s21_decimal, s21_decimal);     // Проверка на меньше или равно
int s21_is_greater(s21_decimal, s21_decimal);           // Проверка на больше
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  // Проверка на больше или равно
int s21_is_equal(s21_decimal, s21_decimal);             // Проверка на равенство
int s21_is_not_equal(s21_decimal, s21_decimal);         // Проверка на неравенство 

// Преобразователи
int s21_from_int_to_decimal(int src, s21_decimal *dst);     // Преобразование из int в s21_decimal
int s21_from_float_to_decimal(float src, s21_decimal *dst); // Преобразование из float в s21_decimal
int s21_from_decimal_to_int(s21_decimal src, int *dst);     // Преобразование из s21_decimal в int
int s21_from_decimal_to_float(s21_decimal src, float *dst); // Преобразование из s21_decimal в float

// Округление
int s21_floor(s21_decimal value, s21_decimal *result);      // Округление до ближайшего целого числа вниз
int s21_round(s21_decimal value, s21_decimal *result);      // Округление до ближайшего целого
int s21_truncate(s21_decimal value, s21_decimal *result);   // Отбрасывание дробной части

// Возвращает результат умножения указанного decimal на -1
int s21_negate(s21_decimal value, s21_decimal *result);

// Общие функции
void printBinary(unsigned int x);
void printDecimalBinary(s21_decimal num);
void printDecimalAsInts(s21_decimal num); 
void printBigDecimalBinary(big_decimal num, const char *ints);
void make_zero_decimal(s21_decimal* dcm);

// Функции для проверки состояния числа
// int is_nan(s21_decimal value);                  // Проверка на NaN (не число)
// int is_inf(s21_decimal value);                  // Проверка на бесконечность
// int is_valid(s21_decimal value);                // Проверка на корректность числа 
int is_zero(s21_decimal const *value);          // Проверка на ноль

// Получение и установка битов
int get_bit(s21_decimal value, int bit_index);                  // Получение значения бита по индексу
int set_bit(s21_decimal *value, int bit_index, int bit_value);  // Установка значения бита по индексу

int decimal_shift_left(s21_decimal *value, unsigned shift);                              // Смещение битов влево на Shift
int add_decimal_mantissa(s21_decimal value_1, s21_decimal value_2, s21_decimal *result); // Сложение мантисс двух чисел
int decimal_multiply_by10(s21_decimal *value);                                           // Умножение decimal на 10
int decimal_aligment(s21_decimal *value_1, s21_decimal *value_2);                       // Выравнивание scale двух decimal чисел

// Функции для работы с big_decimal
s21_bool bigdec_is_zero(big_decimal const value); // Проверка на ноль
int bigdec_get_correct_offset(unsigned bit_index, unsigned* byte_index, unsigned* bit_offset);
int bigdec_get_bit(big_decimal value, unsigned bit_index);
int bigdec_set_bit(big_decimal *value, unsigned bit_index, unsigned bit_value);
int bigdec_shift_left(big_decimal *value, unsigned shift);
int bigdec_add_mantissa(big_decimal value_1, big_decimal value_2, big_decimal *result);
int bigdec_multiply_by10(big_decimal *value);
int bigdec_alignment(big_decimal *value_1, big_decimal *value_2);

res_code bigdec_shift_right(big_decimal *value, const unsigned shift);
int bigdec_comparison_mantissa(big_decimal value_1, big_decimal value_2);
int decimal_to_bigdec(s21_decimal dec, big_decimal *big_dec);
int bigdec_sub_mantissa(big_decimal value_1, big_decimal value_2, big_decimal *result);
int bigdec_div_mantissa(big_decimal value_1, big_decimal value_2, big_decimal *result);
res_code bigdec_div_by_10(big_decimal* value);


// временные функции для печати s21_decimal
void print_s21_decimal(const s21_decimal value);
int string_to_decimal(const char* str, s21_decimal* result);
#endif

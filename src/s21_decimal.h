#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H
// This project made by beedlesh haywoota woodsjul thymemas

#include <limits.h>  // для граничных значений типов (при преобразованиях)
#include <stdio.h>
#include <string.h>  // для конвертации из float в decimal

// КОНСТАНТЫ
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
    unsigned lower_word : 16;
    unsigned scale : 8;
    unsigned zero : 7;
    unsigned is_negative : 1;
  };
} s21_decimal;

typedef union {
  unsigned bits[INTS_IN_BIGDECIMAL];
  struct {
    unsigned mantissa[INTS_IN_BIGDECIMAL - 1];
    unsigned lower_word : 16;
    unsigned scale : 8;
    unsigned zero : 7;
    unsigned is_negative : 1;
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

// ***** Арифметические операторы *****

// Сложение двух чисел
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Вычитание двух чисел
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Умножение двух чисел
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Деление двух чисел
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// ***** Операторы сравнения *****

// Проверка на меньше
int s21_is_less(s21_decimal, s21_decimal);
// Проверка на меньше или равно
int s21_is_less_or_equal(s21_decimal, s21_decimal);
// Проверка на больше
int s21_is_greater(s21_decimal, s21_decimal);
// Проверка на больше или равно
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
// Проверка на равенство
int s21_is_equal(s21_decimal, s21_decimal);
// Проверка на неравенство
int s21_is_not_equal(s21_decimal, s21_decimal);

// ***** Преобразователи *****

// Преобразование из int в s21_decimal
int s21_from_int_to_decimal(int src, s21_decimal *dst);
// Преобразование из float в s21_decimal
int s21_from_float_to_decimal(float src, s21_decimal *dst);
// Преобразование из s21_decimal в int
int s21_from_decimal_to_int(s21_decimal src, int *dst);
// Преобразование из s21_decimal в float
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// ***** Округление *****

// Округление до ближайшего целого числа вниз
int s21_floor(s21_decimal value, s21_decimal *result);
// Округление до ближайшего целого
int s21_round(s21_decimal value, s21_decimal *result);
// Отбрасывание дробной части
int s21_truncate(s21_decimal value, s21_decimal *result);

// Возвращает результат умножения указанного decimal на -1
int s21_negate(s21_decimal value, s21_decimal *result);

// ***** Функции для работы с s21_decimal *****

// обнуляет переданный децимал
void make_zero_decimal(s21_decimal *value);
// Проверка мантиссы на ноль
s21_bool is_zero(const s21_decimal value);
// Получение значения бита по индексу
int get_bit(s21_decimal value, int bit_index);
// Установка значения бита по индексу
void set_bit(s21_decimal *value, int bit_index, int bit_value);
// Смещение битов влево на shift
int decimal_shift_left(s21_decimal *value, unsigned shift);
// Сложение мантисс двух чисел
int add_decimal_mantissa(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *result);
// Умножение decimal на 10
res_code decimal_multiply_by_10(s21_decimal *value);
// Деление decimal на 10 по модулю с получением остатка
res_code div_by_10(const s21_decimal value_1, s21_decimal *quotient,
                   unsigned *remainder);

// ***** Функции для работы с big_decimal *****

// cложение мантисс
void bigdec_add_mantissa(const big_decimal value_1, const big_decimal value_2,
                         big_decimal *result);
// вычитание мантисс
int bigdec_sub_mantissa(big_decimal value_1, big_decimal value_2,
                        big_decimal *result);
// деление мантисс
int bigdec_div_mantissa(big_decimal value_1, big_decimal value_2,
                        big_decimal *result);
// умножение мантисс
void bigdec_mul_mantissa(const big_decimal value_1, const big_decimal value_2,
                         big_decimal *result);
// приведение к одному скейлу
void bigdec_alignment(big_decimal *value_1, big_decimal *value_2);
// сравнение -1: первое меньше, 0: равны, 1: первое больше
int bigdec_comparison(const big_decimal value_1, const big_decimal value_2);
// конвертация s21_decimal to big_decimal
void decimal_to_bigdec(const s21_decimal dec, big_decimal *big_dec);
// конвертация big_decimal to s21_decimal
res_code bigdec_to_decimal(big_decimal big_dec, s21_decimal *dec);

#endif
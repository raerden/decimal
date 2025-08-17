#include "s21_decimal.h"


int multiply_by_10(s21_decimal *value) {
    int res = OK;

    s21_decimal dec1 = *value; 
    s21_decimal dec2 = *value; 

    res = decimal_shift_left(&dec1, 3);  // умножение на 8
    if (res == OK) 
        res = decimal_shift_left(&dec2, 1);  // умножение на 2
    if (res == OK) 
        res = add_decimal_mantissa(dec1, dec2, value); // Сложение "двинутых" мантисс 8 + 2 = 10

    return res;
}

void print_s21_decimal(const s21_decimal value) {
    char result[30] = {0}; // 29 максимум для 96-битного числа + 1 для нуля
    int len = 1; // начинаем с 0

    for (int i = 2; i >= 0; --i) {
        for (int j = 31; j >= 0; --j) {
            int carry = (value.bits[i] >> j) & 1;

            for (int k = 0; k < len; ++k) {
                int val = result[k] * 2 + carry;
                result[k] = val % 10;
                carry = val / 10;
            }

            if (carry) {
                result[len++] = carry;
            }
        }
    }

    if (value.scale >= len)
        len = value.scale + 1;
    // Печатаем число в правильном порядке
    if (value.is_negative) printf("-");
    for (int i = len - 1; i >= 0; --i) {
        if (i == value.scale - 1)
            putchar('.');
        putchar(result[i] + '0');
    }
    putchar('\n');
}

void print_s21_bigdecimal(const big_decimal value) {
    char result[300] = {0}; 
    int len = 1; // начинаем с 0

    for (int i = 6; i >= 0; --i) {
        for (int j = 31; j >= 0; --j) {
            int carry = (value.bits[i] >> j) & 1;

            for (int k = 0; k < len; ++k) {
                int val = result[k] * 2 + carry;
                result[k] = val % 10;
                carry = val / 10;
            }

            if (carry) {
                result[len++] = carry;
            }
        }
    }

    if (value.scale >= len)
        len = value.scale + 1;
    // Печатаем число в правильном порядке
    if (value.is_negative) printf("-");
    for (int i = len - 1; i >= 0; --i) {
        if (i == value.scale - 1)
            putchar('.');
        putchar(result[i] + '0');
    }
    putchar('\n');
}

// Добавление одной цифры к decimal.  unsigned long long = 64 bit
int add_digit(s21_decimal* dec, unsigned int digit) {
  unsigned long long sum = (unsigned long long)dec->bits[0] + digit;
  dec->bits[0] = (unsigned int)sum;

  sum = (unsigned long long)dec->bits[1] + (sum >> 32);
  dec->bits[1] = (unsigned int)sum;

  sum = (unsigned long long)dec->bits[2] + (sum >> 32);
  dec->bits[2] = (unsigned int)sum;

  return (sum >> 32) == 0;  // 1 — успешно, 0 — переполнение
}

// Преобразование строки в s21_decimal
int string_to_decimal(const char* str, s21_decimal* result) {
  result->bits[0] = result->bits[1] = result->bits[2] = result->bits[3] = 0;

  if (strchr(str, '.') != NULL) {
    result->scale = strlen(strchr(str, '.')) - 1;
  }
  if (strchr(str, '-') != NULL) {
    result->is_negative = 1;
  }

  for (const char* p = str; *p != '\0'; p++) {
    char ch = *p;
    if (ch == '.' || ch == '-') {
        continue;
    } else {    
        if (ch < '0' || ch > '9') {
        return 0;  // ошибка: не цифра и не точка
        }

        multiply_by_10(result);
        if (!add_digit(result, ch - '0')) {
        return 0;  // переполнение
        }
    }
  }

  return 1;  // успешно
}

void print_decimal_to_hex(s21_decimal value_1) {
    printf("{{0x%08X, 0x%08X, 0x%08X, 0x%08X}};\n", 
        value_1.bits[0], value_1.bits[1], value_1.bits[2], value_1.bits[3]);
}
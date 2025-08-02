#include "s21_decimal.h"


int multiply_by_10(s21_decimal *value) {
    int res = OK;

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

    // Печатаем число в правильном порядке
    for (int i = len - 1; i >= 0; --i) {
        putchar(result[i] + '0');
    }
    putchar('\n');
}
/*
Как это работает:
    Алгоритм имитирует умножение числа на 2 и прибавление очередного бита (двойчно-десятичное представление).
    Число хранится в char result[] в десятичной форме, в обратном порядке (младшая цифра в result[0]).
    При обработке каждого бита: все цифры умножаются на 2, и добавляется текущий бит в младший разряд.
    Этот метод прост и работает быстро для 96-битных чисел.
*/

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

  for (const char* p = str; *p != '\0'; p++) {
    char ch = *p;
    if (ch < '0' || ch > '9') {
      return 0;  // ошибка: не цифра
    }

    multiply_by_10(result);
    if (!add_digit(result, ch - '0')) {
      return 0;  // переполнение
    }
  }

  return 1;  // успешно
}

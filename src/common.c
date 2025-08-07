#include "s21_decimal.h"

// Проверка на ноль
int is_zero(const s21_decimal value) {
    return !(value.bits[0] || value.bits[1] || value.bits[2]);
}

// вспомогательная функция для get и set bit
void get_correct_offset(int bit_index, int* byte_index, int* bit_offset)
{
    *byte_index = bit_index / bits_in_int;           // Индекс байта 0,1,2
    *bit_offset = bit_index % bits_in_int;           // Смещение бита в байте
}

// Получение значения бита по индексу
int get_bit(s21_decimal value, int bit_index) {
    int byte_index = 0;
    int bit_offset = 0;

    get_correct_offset(bit_index, &byte_index, &bit_offset);
    const int mask = 1 << bit_offset;

    return !!(value.bits[byte_index] & mask);
}

// Установка значения бита по индексу
void set_bit(s21_decimal *value, int bit_index, int bit_value) {
    int byte_index = 0;
    int bit_offset = 0;
    get_correct_offset(bit_index, &byte_index, &bit_offset);

    if (bit_value)
        value->bits[byte_index] |= (1 << bit_offset); // Установка бита в 1 через побитовое ИЛИ
    else
        value->bits[byte_index] &= ~(1 << bit_offset); // Установка бита в 0 через побитовое И
}

// зануляет переданный децимал
void make_zero_decimal(s21_decimal* value)
{
    for (unsigned i = 0; i <= ints_in_mantissa; i++ )
        value->bits[i] = 0;
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

    if (shift > 0) {
        unsigned overflow = 0;
        for (unsigned i = 0; i < ints_in_mantissa; i++) {
            unsigned memo = value->bits[i];
            value->bits[i] = (memo << shift) | overflow; // Сдвиг влево с учетом предыдущего переполнения
            overflow = memo >> (bits_in_int - shift);    // Сохраняем переполнение
        }
        // Если в overflow есть значение для bits[3], возвращаем ошибку переполнения
        if (overflow != 0)
            res = IS_TOO_LARGE;
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
        set_bit(result, i, sum);
    }

    if (memo > 0)
        // Если остался перенос в bits[3], значит переполнение
        res = IS_TOO_LARGE;

    return res;
}

res_code decimal_multiply_by_10(s21_decimal* value) {
    res_code res = OK;
    
    if (value->scale >= max_scale) {
        res = IS_TOO_LARGE;
    }
    
    s21_decimal dec1 = *value; 
    s21_decimal dec2 = *value; 
    if (res == OK)
        res = decimal_shift_left(&dec1, 3);  // умножение на 8
    if (res == OK) 
        res = decimal_shift_left(&dec2, 1);  // умножение на 2
    if (res == OK) 
        res = add_decimal_mantissa(dec1, dec2, value); // Сложение "сдвинутых" мантисс 8 + 2 = 10

    return res;
}

int decimal_alignment(s21_decimal *value_1, s21_decimal *value_2) {
    int res = OK;

    if (value_1->scale > value_2->scale) {
        unsigned scale_diff = value_1->scale - value_2->scale;
        for (unsigned i = 0; res == OK && i < scale_diff; i++) {
            res = decimal_multiply_by_10(value_2); // Умножаем value_2 на 10 scale_diff раз
            value_2->scale++;
        }
    } else if (value_2->scale > value_1->scale) {
        unsigned scale_diff = value_2->scale - value_1->scale;
        for (unsigned i = 0; res == OK && i < scale_diff; i++) {
            res = decimal_multiply_by_10(value_1); // Умножаем value_1 на 10 scale_diff раз
            value_1->scale++;
        }
    }

    return res;
}

s21_decimal s21_mod(s21_decimal value) {
    s21_decimal res = value;
    res.is_negative = 0;
    return res;
}

// деление на 10 в столбик с остатком
// value_1 - делимое, quotient - частное, remainder - остаток
// деление выполняется от старшего бита к младшему
// результат записывается в quotient, остаток в remainder
res_code div_by_10(const s21_decimal value_1, s21_decimal *quotient, unsigned *remainder) {
    s21_decimal quot = {0};
    s21_decimal ten = {0};
    unsigned rem = 0;
    res_code res = OK;

    quot.bits[3] = value_1.bits[3];
    ten.bits[0] = 10;
    
    if (is_zero(value_1)) {
        res = OK;
    } else if (s21_is_less(s21_mod(value_1), ten)) {
        //проверка что число 0 < x < 10
        if (value_1.scale == 28) {
            res = IS_TOO_SMALL;    // деление на 10 невозможно. Число слишком маленькое
        } else {
            rem = value_1.bits[0]; // остаток равен делимому
            quot.bits[0] = 0;      // частное равно 0.      Занулять ли частное?
            quot.scale = value_1.scale + 1; // увеличиваем scale на 1, т.к. делим на 10
        }
    } else {
        for (int i = bits_in_mantissa - 1; i >= 0; --i) {
            rem = (rem << 1) | get_bit(value_1, i);
            // если rem >= 10, вычитаем 10 и устанавливаем 1 в результат
            if (rem >= 10) {
                rem -= 10;
                set_bit(&quot, i, 1);
            } else {
                set_bit(&quot, i, 0);
            }
        }
        
    }

    *quotient = quot;
    *remainder = rem;
    return res;
}

int div_mantissa(const s21_decimal value_1, s21_decimal value_2, s21_decimal* quotient, s21_decimal* remainder) {
    s21_decimal quot = {0};
    s21_decimal rem = {0};

    //двигаемся от старшего бита к младшему
    for (int i = bits_in_mantissa - 1; i >= 0; --i) {
        // printf("i = %d, rem = %u\n", i, rem.bits[0]);
        // rem = rem * 2 + текущий бит
        decimal_shift_left(&rem, 1);
        set_bit(&rem, 0, get_bit(value_1, i));
        // printBinary(rem.bits[0]);
        // printf("rem after shift = %u\n", rem.bits[0]);

        // если rem >= 10, вычитаем 10 и устанавливаем 1 в результат
        if (s21_is_greater_or_equal(rem, value_2)) {
            // printf("rem >= 5, rem = %u\n", rem.bits[0]);
            s21_sub(rem, value_2, &rem);
            set_bit(&quot, i, 1);
            // printf("set_bit quot at %d to 1\n", i);
            // printBinary(quot.bits[2]);
        } else {
            // printf("set_bit quot at %d to 0\n", i);
            // printBinary(quot.bits[0]);
            set_bit(&quot, i, 0);
        }
        // printf("rem after division = %u\n\n", rem.bits[0]);
    }

    *quotient = quot;
    *remainder = rem;
    return OK;
}


// ФУНКЦИИ ПЕЧАТИ НА ЭКРАН: ____________________________________________________________________

void print_italic_1()
{
    printf("\033[3m1\033[0m");
}

// выводит на экран  число типа unsigned int в двоичной записи    -> *((unsigned*)&num)
void printBinary(unsigned num) {
    for (int i = bits_in_int - 1; i >= 0; i--) {
        if (((num >> i) & 1) == 0) {
             printf("0");
        } else {
            print_italic_1();
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
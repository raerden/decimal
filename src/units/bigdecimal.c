#include "../s21_decimal.h"
// This project made by beedlesh haywoota woodsjul thymemas

static const big_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};

// зануляет переданный бигдец
void make_zero_bigdec(big_decimal* bd) {
  for (unsigned i = 0; i <= ints_in_big_mantissa; i++) bd->bits[i] = 0;
}

// вспомогательная функция для get и set bit
void bigdec_get_correct_offset(unsigned bit_index, unsigned* byte_index,
                               unsigned* bit_offset) {
  *byte_index = bit_index / bits_in_int;  // Индекс байта 0,1,2
  *bit_offset = bit_index % bits_in_int;  // Смещение бита в байте
}

// Получение значения бита по индексу
int bigdec_get_bit(big_decimal value, unsigned bit_index) {
  unsigned byte_index = 0;
  unsigned bit_offset = 0;
  bigdec_get_correct_offset(bit_index, &byte_index, &bit_offset);

  const unsigned mask = (1 << bit_offset);
  int res = !!(
      value.bits[byte_index] &
      mask);  // двойное отрицание делает из числа 0 или 1 (!!30 => 1 !! 0 => 0)

  return res;
}

// Проверка числа на нечётность (проверяем нулевой бит, если 1 - нечётное)
s21_bool is_odd(big_decimal bd) { return bigdec_get_bit(bd, 0); }

// Установка значения бита по индексу
void bigdec_set_bit(big_decimal* value, unsigned bit_index,
                    unsigned bit_value) {
  unsigned byte_index = 0;
  unsigned bit_offset = 0;
  bigdec_get_correct_offset(bit_index, &byte_index, &bit_offset);

  if (bit_value)
    value->bits[byte_index] |=
        (1 << bit_offset);  // Установка бита в 1 через побитовое ИЛИ
  else
    value->bits[byte_index] &=
        ~(1 << bit_offset);  // Установка бита в 0 через побитовое И
}

unsigned max_nonzero_bit(const big_decimal value) {
  unsigned max_bit = 0;

  for (unsigned i = bits_in_big_mantissa - 1; (i > 0) && !max_bit; i--)
    if (bigdec_get_bit(value, i)) max_bit = i;

  return max_bit;
}

// Смещение битов влево на shift
res_code bigdec_shift_left(big_decimal* value, unsigned shift) {
  res_code res = OK;
  int shift_align = shift;
  int overflow = 0;
  const int max_bit_in_int = bits_in_int - 1;
  while (shift_align > 0) {
    shift = (shift_align > max_bit_in_int) ? max_bit_in_int : shift_align;
    for (unsigned i = 0; i < ints_in_big_mantissa; i++) {
      unsigned memo = value->bits[i];
      value->bits[i] =
          (memo << shift) |
          overflow;  // Сдвиг влево с учетом предыдущего переполнения
      overflow = memo >> (bits_in_int - shift);  // Сохраняем переполнение
    }
    shift_align -= shift;
  }

  if (overflow)  // Если в overflow есть значение для bits[7], возвращаем ошибку
                 // переполнения
    res = OUT_OF_RANGE;
  return res;
}

// Смещение битов вправо на shift
res_code bigdec_shift_right(big_decimal* value, unsigned shift) {
  res_code res = OK;
  int shift_align = shift;
  unsigned overflow = 0;
  const int max_bit_in_int = bits_in_int - 1;
  while (shift_align > 0) {
    shift = (shift_align > max_bit_in_int) ? max_bit_in_int : shift_align;
    for (int i = ints_in_big_mantissa - 1; i >= 0; i--) {
      unsigned memo = value->bits[i];
      value->bits[i] =
          overflow | (memo >> shift);  // Сдвиг вправо с учетом предыдущего бита
      overflow = memo << (bits_in_int -
                          shift);  // Сохраняем правую часть текущего бита
    }

    shift_align -= shift;
  }
  if (overflow != 0)  // Если в overflow есть значение для bits[7], возвращаем
                      // ошибку переполнения
    res = OUT_OF_RANGE;
  return res;
}

// Сложение мантисс двух чисел
void bigdec_add_mantissa(const big_decimal value_1, const big_decimal value_2,
                         big_decimal* result) {
  make_zero_bigdec(result);
  unsigned memo = 0;
  unsigned i = 0;

  for (i = 0; i < ints_in_big_mantissa * bits_in_int; i++) {  // 7 * 32 = 224
    int sum = bigdec_get_bit(value_1, i) + bigdec_get_bit(value_2, i) + memo;
    if (sum > 1)
      memo = 1;  // Перенос в следующий бит
    else
      memo = 0;
    sum %= 2;  // 1 + 1 = 2 % 2 = 0
    bigdec_set_bit(result, i, sum);
  }
  bigdec_set_bit(result, i, memo);
}

// умножение переданного бигдец-числа на 10 с помощью побитовых сдвигов
res_code bigdec_multiply_by_10(big_decimal* value) {
  big_decimal dec1 = *value;
  big_decimal dec2 = *value;
  s21_bool is_negative = value->is_negative;
  res_code res = bigdec_shift_left(&dec1, 3);  // умножение на 8
  if (res == OK) {
    bigdec_shift_left(&dec2, 1);  // умножение на 2
    bigdec_add_mantissa(dec1, dec2,
                        value);  // cложение "cдвинутых" мантисс 8 + 2 = 10
    value->is_negative = is_negative;
  }
  return res;
}

// умножение мантисс big_decimal
void bigdec_mul_mantissa(const big_decimal value_1, const big_decimal value_2,
                         big_decimal* result) {
  big_decimal copy_v1 = {0}, prvs_res = {0};
  for (unsigned i = 0; i < bits_in_big_mantissa; i++)
    if (bigdec_get_bit(value_2, i)) {
      copy_v1 = value_1;
      bigdec_shift_left(&copy_v1, i);
      bigdec_add_mantissa(prvs_res, copy_v1, result);
      prvs_res = *result;
    }
}

// приведение двух чисел бигдец к одному скейлу
void bigdec_alignment(big_decimal* value_1, big_decimal* value_2) {
  if (value_1->scale > value_2->scale) {
    unsigned scale_diff = value_1->scale - value_2->scale;
    unsigned scale = value_2->scale;
    for (unsigned i = 0; i < scale_diff; i++) {
      bigdec_multiply_by_10(value_2);  // Умножаем value_2 на 10 scale_diff раз
      scale++;
    }
    value_2->scale = scale;
  } else if (value_2->scale > value_1->scale) {
    unsigned scale_diff = value_2->scale - value_1->scale;
    unsigned scale = value_1->scale;
    for (unsigned i = 0; i < scale_diff; i++) {
      bigdec_multiply_by_10(value_1);  // Умножаем value_1 на 10 scale_diff раз
      scale++;
    }
    value_1->scale = scale;
  }
}

// -1 - первое меньше, 0 - равны, 1 - первое больше
int bigdec_comparison(const big_decimal value_1, const big_decimal value_2) {
  const int equal = 0;
  const int v1_less = -1;
  const int v2_less = 1;
  int comp_res = equal;

  if (value_1.is_negative && !value_2.is_negative)
    comp_res = v1_less;
  else if (!value_1.is_negative && value_2.is_negative)
    comp_res = v2_less;
  else  // одного знака
  {
    s21_bool both_negative = (value_1.is_negative && value_2.is_negative);
    s21_bool both_positive = !both_negative;
    for (int i = bits_in_big_mantissa - 1; (i >= 0) && (comp_res == equal);
         i--) {
      if (bigdec_get_bit(value_1, i) < bigdec_get_bit(value_2, i))
        comp_res = (both_positive) ? v1_less : v2_less;
      else if (bigdec_get_bit(value_1, i) > bigdec_get_bit(value_2, i))
        comp_res = (both_positive) ? v2_less : v1_less;
    }
  }
  return comp_res;
}

void decimal_to_bigdec(const s21_decimal dec, big_decimal* big_dec) {
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
}

// вычитание мантисс
int bigdec_sub_mantissa(big_decimal value_1, big_decimal value_2,
                        big_decimal* result) {
  unsigned max_1_bit = max_nonzero_bit(value_1);
  big_decimal val_to_sub = {0};
  for (unsigned i = 0; i <= max_1_bit; i++)
    bigdec_set_bit(&value_2, i, bigdec_get_bit(value_2, i) ? 0 : 1);
  bigdec_add_mantissa(value_2, one, &val_to_sub);
  bigdec_add_mantissa(value_1, val_to_sub, result);
  bigdec_set_bit(result, max_1_bit + 1, 0);
  return 0;
}

unsigned remainder_after_sub(const big_decimal bd) {
  unsigned res = bd.bits[0];
  for (unsigned i = 1; i < ints_in_big_mantissa; i++) res |= bd.bits[i];
  return res;
}

// деление мантисс
int bigdec_div_mantissa(big_decimal value_1, big_decimal value_2,
                        big_decimal* result) {
  make_zero_bigdec(result);
  value_1.is_negative = 0;
  value_2.is_negative = 0;
  big_decimal bd_rem = value_1;
  big_decimal result_ = {0}, res = {0};
  unsigned rem = remainder_after_sub(value_1);
  unsigned max_1_bit = 0;
  int scale = 0;

  while ((rem != 0) && (max_1_bit < 2 * bits_in_mantissa)) {
    int shift = max_nonzero_bit(value_1) - max_nonzero_bit(value_2);
    if (shift > 0) bigdec_shift_left(&value_2, shift);
    make_zero_bigdec(&res);

    while (shift >= 0 && rem != 0) {
      bigdec_shift_left(&res, 1);
      if (bigdec_comparison(value_1, value_2) >= 0) {
        bigdec_sub_mantissa(value_1, value_2, &bd_rem);
        rem = remainder_after_sub(bd_rem);
        bigdec_set_bit(&res, 0, 1);
      } else
        bigdec_set_bit(&res, 0, 0);

      if (shift > 0 && rem == 0) {
        bigdec_shift_left(&res, shift);
        shift = 0;
      }
      shift--;
      if (shift >= 0 && rem != 0) bigdec_shift_right(&value_2, 1);
      value_1 = bd_rem;
    }

    if (shift < 0 && rem != 0) {
      bigdec_multiply_by_10(&bd_rem);
      value_1 = bd_rem;
      scale++;
      bigdec_multiply_by_10(&result_);
      bigdec_add_mantissa(result_, res, result);
      result_ = *result;
    }

    if (shift < 0 && rem == 0) {
      bigdec_multiply_by_10(&result_);
      bigdec_add_mantissa(result_, res, result);
    }

    max_1_bit = max_nonzero_bit(*result);
    if (max_1_bit >= 2 * bits_in_mantissa) scale--;
  }
  result->scale = scale;

  return 0;
}

// деление на 10 нацело с остатком
void bigdec_div_by_10(big_decimal* value, unsigned* remainder) {
  big_decimal result = {0};
  big_decimal bd_rem = *value;
  big_decimal value_copy = *value;
  unsigned rem = remainder_after_sub(value_copy);
  int shift = max_nonzero_bit(value_copy) - ints_in_mantissa;
  big_decimal ten = {{10, 0, 0, 0, 0, 0, 0, 0}};
  bigdec_shift_left(&ten, shift);

  while (rem != 0 && shift >= 0) {
    bigdec_shift_left(&result, 1);
    if (bigdec_comparison(value_copy, ten) >= 0) {
      bigdec_sub_mantissa(value_copy, ten, &bd_rem);
      rem = remainder_after_sub(bd_rem);
      bigdec_set_bit(&result, 0, 1);
    } else
      bigdec_set_bit(&result, 0, 0);

    if (shift > 0 && rem == 0) {
      bigdec_shift_left(&result, shift);
      shift = 0;
    }
    shift--;
    if (shift >= 0 && rem != 0) {
      bigdec_shift_right(&ten, 1);
      value_copy = bd_rem;
    }
  }
  for (unsigned i = 0; i < ints_in_big_mantissa; i++)
    value->bits[i] = result.bits[i];
  value->scale--;
  *remainder = rem;
}

void set_dec_by_bigdec(const big_decimal big, s21_decimal* dec) {
  dec->low = big.mantissa[0];
  dec->middle = big.mantissa[1];
  dec->high = big.mantissa[2];
  dec->lower_word = big.lower_word;
  dec->scale = big.scale;
  dec->zero = big.zero;
}

res_code bigdec_to_decimal(big_decimal big_dec, s21_decimal* dec) {
  make_zero_decimal(dec);
  res_code res = -1;
  unsigned rem = 0;
  long double frac_rem = 0;
  unsigned minus = big_dec.is_negative;
  big_dec.is_negative = 0;

  unsigned max_1_bit = max_nonzero_bit(big_dec);
  while ((big_dec.scale > max_scale || max_1_bit > (bits_in_mantissa - 1)) &&
         res == -1) {
    if (max_1_bit < 4 && big_dec.scale > max_scale &&
        (big_dec.mantissa[0] < 5)) {
      res = IS_TOO_SMALL;
      dec->is_negative = minus;
    } else if (max_1_bit > (bits_in_mantissa - 1) && !big_dec.scale) {
      res = minus ? IS_TOO_SMALL : IS_TOO_LARGE;
      dec->is_negative = minus;
    } else {
      bigdec_div_by_10(&big_dec, &rem);
      if (frac_rem > 0) {
        const float small_rem = 0.1;
        const float large_rem = 0.9;
        const long double tmp_rem = frac_rem - (int)(frac_rem) / 10;
        if (tmp_rem < 0.5)
          frac_rem = small_rem;
        else if (tmp_rem > 0.5)
          frac_rem = large_rem;
      }
      frac_rem = frac_rem / 10 + rem;

      if (max_nonzero_bit(big_dec) < bits_in_mantissa &&
          (big_dec.scale <= max_scale)) {
        unsigned scale = big_dec.scale;
        if (frac_rem > 5 || (is_odd(big_dec) && (frac_rem == 5)))
          bigdec_add_mantissa(big_dec, one, &big_dec);
        big_dec.scale = scale;
      }
      max_1_bit = max_nonzero_bit(big_dec);
    }
    if ((max_1_bit == 0) && (big_dec.scale == max_scale) && (frac_rem > 0) &&
        !is_odd(big_dec)) {
      res = IS_TOO_SMALL;
      dec->is_negative = minus;
    }
  }
  if ((res == -1) && (big_dec.scale <= max_scale) &&
      (max_1_bit < bits_in_mantissa)) {
    set_dec_by_bigdec(big_dec, dec);
    dec->is_negative = minus;
    res = OK;
  }
  return res;
}
#include "../s21_decimal.h"

// Функции для проверки состояния числа______________________________________________
s21_bool is_nan(float value)                       // Проверка на NaN (не число)
{
    return (value == value)?FALSE:TRUE;
}
s21_bool is_inf(float value)                       // Проверка на бесконечность
{
    const float infinity = 1.0/0.0;
    s21_bool res = FALSE;
    if (value == infinity) res = TRUE;
    return res;
}
s21_bool is_valid(float value)                     // Проверка на корректность числа
{
    s21_bool res = TRUE;
    if (is_nan(value) || is_inf(value) || is_inf(-value))
        res = FALSE;
    return res;
}
//___________________________________________________________________________________

// Преобразование из int в s21_decimal
int s21_from_int_to_decimal(int src, s21_decimal *dst)
{
    if (!dst) return CONVERTATION_ERR;
    make_zero_decimal(dst);
    s21_bool src_is_min_int = (src==INT_MIN);
    if (src < 0)
    {
        dst->is_negative = TRUE;
        if (src_is_min_int)            // INT_MIN == -2147483648
            src++;                     // INT_MAX == 2147483647 поэтому сперва нужно уменьшить абсолютное значение на 1
        src *= -1;
    }
    if (src_is_min_int)
        dst->low = (unsigned)src + 1;  // INT_MAX == |INT_MIN|+1
    else
        dst->low = (unsigned)src;

    return OK;
}

// преобразование float в научную нотацию в виде строки, расчёт мантиссы в виде целого числа и экспоненты
unsigned significand_from_float(const float src, const unsigned precision, int* exp)
{
    const unsigned significand_len = 1 + precision;        // *.precisionE+3
    const unsigned scntfc_not_len = significand_len + 6;   // мантисса + точка + "E" + знак + двух или трёх значная экспонента
    char* str_scntfc_not = (char*)calloc(sizeof(char), scntfc_not_len + 1);
    sprintf(str_scntfc_not, "%.*E", precision, src);

    // достаём экспоненту:
    const unsigned ind_E = strcspn(str_scntfc_not, "E");
    unsigned str_ind = ind_E + 2;                          //  индекс начала значения экспоненты
    while (str_scntfc_not[str_ind])
    {
        *exp *= 10;
        *exp += (str_scntfc_not[str_ind] - '0');
        ++str_ind;
    }
    if (str_scntfc_not[ind_E + 1] == '-')
        *exp *= -1;
    
    // считаем significand как целое число из значимых цифр (мантиссы) "5.123456" -> 5123456
    unsigned significand = str_scntfc_not[0] - '0';
    str_ind = 2;  // индекс в строке str_scntfc_not первой цифры после точки
    for (; str_ind <= significand_len; str_ind++)
    {
        significand *= 10;
        significand += (str_scntfc_not[str_ind] - '0');
    }

    free(str_scntfc_not);
    return significand;
}

// Преобразование из float в s21_decimal
int s21_from_float_to_decimal(float src, s21_decimal *dst)
{
    res_code res = OK;
    // !!! здесь нужна проверка на NULL, nan, inf, 0, 
    make_zero_decimal(dst);

    if (1/src < 0)       // 1/src по опыту s21_string чтобы не прогадать с малыми значениями float
    {
        dst->is_negative = TRUE;
        src = -src;
    }

    const int precision = 6;
    int exp = 0;
    unsigned significand = significand_from_float(src, precision, &exp);
    
    exp -= precision;
    dst->low = significand;

    while (exp > 0 && res == OK)
    // -exp == scale > 0 поэтому нужно уменьшить экспоненту до 0, умножая мантиссу на 10 
    {
        res = decimal_multiply_by_10(dst);
        exp--;
        dst->scale = 0;
    }

    if (exp < 0 && res == OK)          // while выше не выполнялся
    {
        while ((-exp > (int)max_scale) && (res == OK))
        {
            res = decimal_multiply_by_10(dst);
            exp++;
            dst->scale = 0;
        }
        if (res == OK)
            dst->scale = (unsigned)(-exp);
    }

    return (res == OK)?OK:CONVERTATION_ERR;
}

// Преобразование из s21_decimal в int
int s21_from_decimal_to_int(s21_decimal src, int *dst)
{
    res_code res = OK;
    if (!dst) res = CONVERTATION_ERR;
    else
    {
        s21_bool is_negative = (src.is_negative)?TRUE:FALSE;
        src.is_negative = 0;

        s21_decimal int_dec = src;
        if (src.scale)  
        // из README: если в числе decimal есть дробная часть, то её следует отбросить (0.9 преобразуется в 0)
            res = s21_truncate(src, &int_dec);
        // int_dec.scale == 0
        if (int_dec.middle || int_dec.high)
            res = CONVERTATION_ERR;
        
        if (res == OK && (int_dec.low <= INT_MAX))
        {
            *dst = int_dec.low;
            if (is_negative) 
                *dst += -1;
        }
        else 
        {
            *dst = 0;
            res = CONVERTATION_ERR;
        }
    }
    return res;
}     

// Преобразование из s21_decimal в float
int s21_from_decimal_to_float(s21_decimal src, float *dst)
{
    if (!dst) return CONVERTATION_ERR;
    res_code res = OK;
    *dst = 0.0;
    if (!is_zero(src))
    {
        long double big_float = 0;
        for (unsigned i = 0; (i < bits_in_mantissa) && (res == OK); i++)
        {
            s21_bool is_one = get_bit(src, i);
            long double tmp = big_float;
            if (is_one)
                big_float += pow(2, i);
            if (big_float < tmp)        // сомнительная, но проверка на переполнение
                res = IS_TOO_LARGE;
        }
        if (res == OK)
        {
            unsigned scale = src.scale;
            while (scale--) big_float /= 10;
            *dst = (float)big_float;
        }
    }

    if (src.is_negative)
        *dst *= -1;

    return res;
}

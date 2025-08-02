#include "s21_decimal.h"

// деление на 10 в столбик с остатком
// value_1 - делимое, quotient - частное, remainder - остаток
// деление выполняется от старшего бита к младшему
// результат записывается в quotient, остаток в remainder
int div_by_10(const s21_decimal value_1, s21_decimal *quotient, unsigned *remainder) {
    s21_decimal quot = {0};
    unsigned rem = 0;

    //двигаемся от старшего бита к младшему
    for (int i = bits_in_mantissa - 1; i >= 0; --i) {
        //printf("i = %d, rem = %u\n", i, rem);
        // rem = rem * 2 + текущий бит
        rem = (rem << 1) | get_bit(value_1, i);
        // printBinary(rem);
        // printf("rem after shift = %u\n", rem);

        // если rem >= 10, вычитаем 10 и устанавливаем 1 в результат
        if (rem >= 10) {
            // printf("rem >= 10, rem = %u\n", rem);
            rem -= 10;
            set_bit(&quot, i, 1);
            // printf("set_bit quot at %d to 1\n", i);
            // printBinary(quot.bits[0]);
        } else {
            // printf("set_bit quot at %d to 0\n", i);
            // printBinary(quot.bits[0]);
            set_bit(&quot, i, 0);
        }
        // printf("rem after division = %u\n\n", rem);
    }

    *quotient = quot;
    *remainder = rem;
    return OK;
}


int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
    return value_1.bits[0] >= value_2.bits[0];
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
    result->bits[0] = value_1.bits[0] - value_2.bits[0];
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





int main() {


    big_decimal bdcm1 = {0};
    big_decimal bdcm2 = {0};
    big_decimal bdcm3 = {0};
    s21_decimal dcm1 = {0};



    // dcm1.bits[0] = 0b11111111111111111111111111111110; // 4294967294
    // dcm1.bits[1] = 1; // 8589934590
    // print_s21_decimal(dcm1);

    bdcm1.bits[0] = 0b11111111111111111111111111111110; // 4294967294
    bdcm1.bits[1] = 1; // 8589934590
    // printBigDecimalBinary(bdcm1,"");

    bdcm2.bits[0] = 10; //делитель

    bigdec_div_mantissa(bdcm1, bdcm2, &bdcm3);

    printBigDecimalBinary(bdcm3,"");
    printf("%u\n\n", bdcm3.bits[0]);



    // // печать decimal числа
    // s21_decimal dcm1 = {0};
    // dcm1.bits[0] = 0xFFFFFFFF;
    // dcm1.bits[1] = 1;
    // printDecimalBinary(dcm1);
    // print_s21_decimal(dcm1); // 8589934591
    

    // // перевод строки в decimal
    // s21_decimal dcm2 = {0};
    // string_to_decimal("8589934591", &dcm2);
    // printDecimalBinary(dcm2);
    // print_s21_decimal(dcm2);



    // // печать полного decimal числа
    // s21_decimal dcm1 = {0};
    // dcm1.bits[0] = 0xFFFFFFFF;
    // dcm1.bits[1] = 0xFFFFFFFF;
    // dcm1.bits[2] = 0xFFFFFFFF;
    // printDecimalBinary(dcm1);
    // print_s21_decimal(dcm1); // 79228162514264337593543950335
    

    // // перевод строки в decimal
    // s21_decimal dcm2 = {0};
    // string_to_decimal("79228162514264337593543950335", &dcm2);
    // printDecimalBinary(dcm2);
    // print_s21_decimal(dcm2);



    




    // // Пример использования bigdec_Allignment
    // bdcm1.bits[0] = 1;
    // bdcm1.scale = 5;

    // bdcm2.bits[0] = 1;
    // bdcm2.scale = 0;

    // printf("BigDecimal_1 before alignment:\n");
    // printBigDecimalBinary(bdcm1, "701");
    // printf("BigDecimal_1 scale: %u bits[0]: %u\n\n", bdcm1.scale, bdcm1.bits[0]);

    // printf("BigDecimal_2 before alignment:\n");
    // printBigDecimalBinary(bdcm2, "701");
    // printf("BigDecimal_2 scale: %u bits[0]: %u\n\n", bdcm2.scale, bdcm2.bits[0]);

    // //  выравнивание scale двух чисел
    // bigdec_Allignment(&bdcm1, &bdcm2);


    // printf("BigDecimal_1 after alignment:\n");
    // printBigDecimalBinary(bdcm1, "701");
    // printf("BigDecimal_1 scale: %u bits[0]: %u\n\n", bdcm1.scale, bdcm1.bits[0]);

    // printf("BigDecimal_2 after alignment:\n");
    // printBigDecimalBinary(bdcm2, "");
    // printf("BigDecimal_2 scale: %u bits[0]: %u\n", bdcm2.scale, bdcm2.bits[0]);




    // //  умножение на 10
    // bdcm1.bits[0] = 1;
    // for(int i=0; i < 50; i++)
    //     bigdec_multiply_by10(&bdcm1);
    // printBigDecimalBinary(bdcm1, "");
    // printf("BigDecimal_1 scale: %u bits[0]: %u\n\n", bdcm1.scale, bdcm1.bits[0]);


    // // Пример использования addDecimalMantissa
    // bdcm1.bits[0] = 0b11111111111111111111111111111111;
    // bdcm1.bits[1] = 0b11111111111111111111111111111111;
    // bdcm1.bits[2] = 0b11111111111111111111111111111111;
    // bdcm1.bits[3] = 0b11111111111111111111111111111111;
    // bdcm1.bits[4] = 0b11111111111111111111111111111111;
    // bdcm1.bits[5] = 0b11111111111111111111111111111111;
    // bdcm1.bits[6] = 0b01111111111111111111111111111111;

    // bdcm2.bits[0] = 0b11111111111111111111111111111111;
    // bdcm2.bits[1] = 0b11111111111111111111111111111111;
    // bdcm2.bits[2] = 0b11111111111111111111111111111111;
    // bdcm2.bits[3] = 0b11111111111111111111111111111111;
    // bdcm2.bits[4] = 0b11111111111111111111111111111111;
    // bdcm2.bits[5] = 0b11111111111111111111111111111111;
    // bdcm2.bits[6] = 0b11111111111111111111111111111111;

    // printf("bdcm1 defore add:\n");
    // printBigDecimalBinary(bdcm1, "");
    // printf("Result bits[0]: %u\n", bdcm1.bits[0]);

    // printf("\nbdcm2 before add:\n");
    // printBigDecimalBinary(bdcm2, "");
    // printf("Result bits[0]: %u\n", bdcm2.bits[0]);

    // bigdec_add_mantissa(bdcm1, bdcm2, &bdcm3);
    // printf("\nAfter add bdcm1 + bdcm2:\n");
    // printBigDecimalBinary(bdcm3, "");
    // printf("dcm3 result bits[0]: %u\n", bdcm3.bits[0]);


 



    // Пример использования bigdec_set_bit
    // bdcm1.bits[0] = 255;
    // bdcm1.bits[1] = 255;
    // bdcm1.bits[2] = 255;
    // bdcm1.bits[3] = 255;
    // bdcm1.bits[4] = 255;
    // bdcm1.bits[5] = 255;
    // bdcm1.bits[6] = 255;
    // printBigDecimalBinary(bdcm1);
    // printf("\n");    
    // bigdec_ShiftLeft(&bdcm1, 25);
    // printBigDecimalBinary(bdcm1);



// ##################################################################### //
// ----------------------------- decimal funcs -------------------- //
// ##################################################################### //


    
    // s21_decimal dcm1 = {0};
    // s21_decimal dcm2 = {0};
    // s21_decimal dcm3 = {0};



// пример использования decimal_div
// dcm1.bits[0] = 4294967295; // 4294967295 * 2 = 8589934590
// dcm1.bits[1] = 1; // 4294967295 * 2 = 8589934590
// dcm2.bits[0] = 2;
// // 4294967295
// // 4294967295

// printf("dcm1: \n");
// printDecimalBinary(dcm1);
// printf("bits[0] %u\n", dcm1.bits[0]);

// printf("dcm2: \n");
// printDecimalBinary(dcm2);
// printf("bits[0] %u\n", dcm2.bits[0]);


// s21_decimal remainder = {0};
// div_mantissa(dcm1, dcm2, &dcm3, &remainder);

// printf("result: \n");
// printDecimalBinary(dcm3);
// printf("bits[0] %u\n", dcm3.bits[0]);
// printf("remainder: %u\n", remainder.bits[0]);


// // Пример использования s21_div_by_10
// dcm1.bits[0] = 4294967295;
// // dcm1.bits[1] = 1;
// unsigned remainder = 0;
// div_by_10(dcm1, &dcm2, &remainder);

// printf("dcm1: \n");
// printDecimalBinary(dcm1);
// printf("bits[0] %u\n", dcm1.bits[0]);

// printf("dcm2: \n");
// printDecimalBinary(dcm2);
// printf("result %u\n", dcm2.bits[0]);
// printf("remainder: %u\n", remainder);   



   

    // // пример использования decimalAlligment
    // dcm1.bits[0] = 1;
    // dcm1.scale = 2;
    // dcm2.bits[0] = 1;
    // dcm2.scale = 3;
    // printf("Decimal_1 before alignment:\n");
    // printDecimalBinary(dcm1);
    // printf("Decimal_1 scale: %u bits[0]: %u\n\n", dcm1.scale, dcm1.bits[0]);

    // printf("Decimal_2 before alignment:\n");
    // printDecimalBinary(dcm2);  
    // printf("Decimal_2 scale: %u bits[0]: %u\n\n", dcm2.scale, dcm2.bits[0]);

    // decimalAlligment(&dcm1, &dcm2);

    // printf("Decimal_1 after alignment:\n");
    // printDecimalBinary(dcm1);
    // printf("Decimal_1 scale: %u bits[0]: %u\n\n", dcm1.scale, dcm1.bits[0]);

    // printf("Decimal_2 after alignment:\n");
    // printDecimalBinary(dcm1);
    // printf("Decimal_2 scale: %u bits[0]: %u\n\n", dcm2.scale, dcm2.bits[0]);




    // // Пример использования decimal_multiply_by10
    // dcm1.bits[0] = 5; // 5
    // dcm1.scale = 0; // scale = 0
    // printf("Before multiply by 10:\n");
    // printDecimalBinary(dcm1);
    // printf("Result bits[0]: %u scale: %u\n\n", dcm1.bits[0], dcm1.scale);

    // int res = decimal_multiply_by10(&dcm1);
    // if (res == OK) {
    //     printf("After multiply by 10:\n");
    //     printDecimalBinary(dcm1);
    //     printf("Result bits[0]: %u scale: %u\n", dcm1.bits[0], dcm1.scale);
    // } else {
    //     printf("Error: %d\n", res);
    // }




    // Пример использования addDecimalMantissa
    // dcm1.bits[0] = 0b11111111111111111111111111111111;
    // dcm2.bits[0] = 0b11111111111111111111111111111111;

    // dcm1.bits[0] = 15;
    // dcm2.bits[0] = 15;

    // printf("dcm1 defore add:\n");
    // printDecimalBinary(dcm1);
    // printf("Result bits[0]: %u\n", dcm1.bits[0]);

    // printf("\ndcm2 before add:\n");
    // printDecimalBinary(dcm2);
    // printf("Result bits[0]: %u\n", dcm2.bits[0]);

    // addDecimalMantissa(dcm1, dcm2, &dcm3);
    // printf("\nAfter add dcm1 + dcm2:\n");
    // printDecimalBinary(dcm3);
    // printf("dcm3 result bits[0]: %u\n", dcm3.bits[0]);




    // // Пример использования decimalShiftLeft
    // dcm1.bits[0] = 255;
    // dcm1.bits[1] = 255;
    // dcm1.bits[2] = 123;
    // printf("Before shift:\n");
    // printDecimalBinary(dcm1);
    // decimalShiftLeft(&dcm1, 24);
    // printf("After shift:\n");
    // printDecimalBinary(dcm1);



    // printf("%d", get_bit(dcm, 34));
    // printf("\nsign of dcm: %s", dcm.is_negative?"negative":"positive");

    // s21_negate(dcm, &dcm);
    // printf("\nsign of dcm after reverse: %s\n", dcm.is_negative?"negative":"positive");
    // printDecimalBinary(dcm);

    return 0;
}

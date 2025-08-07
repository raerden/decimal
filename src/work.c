#include "s21_decimal.h"

int main() {
    s21_decimal dcm1 = {0};
    s21_decimal dcm2 = {0};
    s21_decimal dcm3 = {0};
    s21_decimal dcm4 = {0};
    big_decimal bdcm1 = {0};
    big_decimal bdcm2 = {0};
    big_decimal bdcm3 = {0};

s21_decimal value_1 = {{7, 0, 0, 0}};
s21_decimal value_2 = {{5, 0, 0, 0}};

s21_sub(value_1, value_2, &dcm3);

printDecimalBinary(dcm3);
printf("%u\n", dcm3.bits[0]);


//    div_mantissa(dcm1,dcm2, &dcm3, &dcm4);

//    printf("%d\n", s21_is_less(dcm1,dcm2));
//    printDecimalBinary(dcm3);
//    printf("%u\n", dcm3.bits[0]);
//    printDecimalBinary(dcm4);
//    printf("%u\n", dcm4.bits[0]);





    // //пример работы сложения
    // dcm1.bits[0] = 0xFFFFFFFF; // 4294967295
    // dcm1.bits[1] = 0xFFFFFFFF; 
    // dcm1.bits[2] = 0; 

    // dcm2.bits[0] = 0xFFFFFFFF; // 4294967295
    // dcm2.bits[1] = 0; 
    // dcm2.bits[2] = 0; 

    // s21_add(dcm1, dcm2, &dcm3);
    // printDecimalBinary(dcm3);
    // printf("%u\n", dcm3.bits[0]);
    // print_s21_decimal(dcm1);
    // print_s21_decimal(dcm2);
    // print_s21_decimal(dcm3);
/*
18446744073709551615
          4294967295
18446744078004518910

*/

    // //Пример работы округления
    // dcm1.bits[0] = 12346;
    // dcm1.scale = 1;
    // s21_round(dcm1, &dcm2);
    // printDecimalBinary(dcm2);
    // printf("%u\n", dcm2.bits[0]);
   

    // Целочисленное деление на 10 с остатком div_by_10
    // // dcm1.bits[0] = 11; // 4294967295
    // dcm1.bits[0] = 0xFFFFFFFF; // 4294967295
    // dcm1.bits[1] = 0xFFFFFFFF; 
    // dcm1.bits[2] = 0xFFFFFFFF; 
    
    // unsigned remainder = 0;

    // printf("Decimal 1:\n");
    // printDecimalBinary(dcm1);
    // print_s21_decimal(dcm1);

    // div_by_10(dcm1, &dcm2, &remainder); 

    // printf("quotient: \n");
    // printDecimalBinary(dcm2);
    // print_s21_decimal(dcm1);
    // print_s21_decimal(dcm2);
    // printf("remainder: %u\n", remainder);
    




// тестирование bigdec_div_mantissa

    // dcm1.bits[0] = 0b11111111111111111111111111111110; // 4294967294
    // dcm1.bits[1] = 1; // 8589934590
    // print_s21_decimal(dcm1);

    // bdcm1.bits[0] = 0b11111111111111111111111111111110; // 4294967294
    // bdcm1.bits[1] = 1; // 8589934590
    // // printBigDecimalBinary(bdcm1,"");

    // bdcm2.bits[0] = 10; //делитель

    // bigdec_div_mantissa(bdcm1, bdcm2, &bdcm3);

    // printBigDecimalBinary(bdcm3,"");
    // printf("%u\n\n", bdcm3.bits[0]);



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




    // // Пример использования decimal_multiply_by_10
    // dcm1.bits[0] = 5; // 5
    // dcm1.scale = 0; // scale = 0
    // printf("Before multiply by 10:\n");
    // printDecimalBinary(dcm1);
    // printf("Result bits[0]: %u scale: %u\n\n", dcm1.bits[0], dcm1.scale);

    // int res = decimal_multiply_by_10(&dcm1);
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

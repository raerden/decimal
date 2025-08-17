#include "s21_decimal.h"


void test_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    if (s21_is_equal(result, check)) {
        printf("Test пройден. Результат: \n");
        printDecimalBinary(result);
        print_s21_decimal(result);
        printf("\n");
    } else {
        printf("Test failed: ожидается \n");
        printDecimalBinary(check);
        print_s21_decimal(check);
        printf(", получено \n");
        printDecimalBinary(result);
        print_s21_decimal(result);
        printf("\n");

    }

    // ck_assert_int_eq(s21_is_equal(result, check), 1);
    // ck_assert_int_eq(code, OK);
}

void test_add_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    if (code == check) {
        printf("Test пройден: код ошибки %d\n", code);
    } else {
        printf("Test failed: ожидается код ошибки %d, получено %d\n", check, code);
        printDecimalBinary(result);
        printf("Res=%d, Scale is: %u\n", code, result.scale);
        print_s21_decimal(result);
    
    }
}


int main() {

    // printf("%.*E\n", 6, 1.1234567890123465f);

    // float flt = -1.234568e-24;
    // printf("\n\n   %.6E\n\n", flt);
    // s21_decimal result;
    // res_code res = s21_from_float_to_decimal(flt, &result);
    // printDecimalBinary(result);
    // printf("Result is %d\n", res);
    // printf("res: ");
    // print_s21_decimal(result);
    // printf("exp: -0.0000000000000000000000012346\n");


    // // 79228162514264337593543950335
    // s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
    // // 0.00000000000001
    // s21_decimal value_2 = {{0x00000001, 0x00000000, 0x00000000, 0x000E0000}};
    // // -79228162508115422902307433130
    // s21_decimal result = {{0xAAAAAAAA, 0xAAAAAAAA, 0xFFFFFFFF, 0x80000000}};

    // int res = s21_div(value_1, value_2, &result);
    // printDecimalBinary(result);
    // printf("Result is %d\n", res);
    // print_s21_decimal(result);

    // test_add(decimal1, decimal2, check);

//     int error = 2;
//     test_add_fail(decimal1, decimal2, error);


// перевод строки в десимал
// s21_decimal value_1;
// // string_to_decimal("7.922816251426433759354395032", &value_1);
// string_to_decimal("-1.0000000000001", &value_1);
// printDecimalBinary(value_1);
// printf("Scale: %d\n", value_1.scale);
// print_decimal_to_hex(value_1);
// print_s21_decimal(value_1);


// перевод чисел для тестов
char *str_value_1 = "-7.922816251426433759354395032";
char *str_value_2 = "-9.96446871326548";
char *str_value_exp = "98891133434344232389";

s21_decimal value_1 = {0};
s21_decimal value_2 = {0};
s21_decimal value_exp = {0};

string_to_decimal(str_value_1, &value_1);
string_to_decimal(str_value_2, &value_2);
string_to_decimal(str_value_exp, &value_exp);

big_decimal bdec;

decimal_to_bigdec(value_1, &bdec);
printDecimalBinary(value_1);
printf("\n");
printBigDecimalBinary(bdec, "");

print_s21_bigdecimal(bdec);


// printf("    // ");
// print_s21_decimal(value_1);
// printf("    s21_decimal value_1 = ");
// print_decimal_to_hex(value_1);

// printf("    // ");
// print_s21_decimal(value_2);
// printf("    s21_decimal value_2 = ");
// print_decimal_to_hex(value_2);

// printf("    // ");
// print_s21_decimal(value_exp);
// printf("    s21_decimal value_exp = ");
// print_decimal_to_hex(value_exp);

// printf("    // %s\n", value_1);


// s21_decimal value_1    = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}};
// s21_decimal value_2    = {{0x99999998, 0x99999999, 0x19999999, 0x1B0000}};
// s21_decimal result_exp = {{0x33333332, 0x33333333, 0x33333333, 0x1C0000}};
// printDecimalBinary(value_1);
// printf("Scale: %d\n", value_1.scale);
// print_s21_decimal(value_1);
// printf("\n");

// printDecimalBinary(value_2);
// printf("Scale: %d\n", value_2.scale);
// print_s21_decimal(value_2);
// printf("\n");

// printDecimalBinary(result_exp);
// print_s21_decimal(result_exp);
// printf("Scale: %d\n", result_exp.scale);
// printf("\n");




//     s21_decimal dcm1 = {0};
//     s21_decimal dcm2 = {0};
//     s21_decimal dcm3 = {0};
//     s21_decimal dcm4 = {0};
//     big_decimal bdcm1 = {0};
//     big_decimal bdcm2 = {0};
//     big_decimal bdcm3 = {0};

// s21_decimal value_1 = {{1, 0, 0, 0}};
// s21_decimal value_2 = {{3, 0, 0, 0}};

// // s21_decimal value_1 = {{1, 0, 0, 0}};
// // s21_decimal value_2 = {{1, 0, 0, (28 << 16)}}; // деление на очень маленькое число

// printDecimalBinary(value_1);
// printf("value_1: %d, Scale: %d\n", value_1.bits[0], value_1.scale);
// printf("\n");
// printDecimalBinary(value_2);
// printf("value_2: %d, Scale: %d\n", value_2.bits[0], value_2.scale);
// printf("\n");

// int res = s21_div(value_1, value_2, &dcm3);

// printDecimalBinary(dcm3);
// printf("Result: %d, Scale: %d\n", res, dcm3.scale);
// print_s21_decimal(dcm3);



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

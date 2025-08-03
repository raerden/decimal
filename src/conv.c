
/*
int main()
{
    float flt = 0.0003429853520f;         // 3.429854E-04
    printf("\nfloat numb = %+.6E\n", flt);
    s21_decimal dec = {{0, 0, 0, 0}};
    if (s21_from_float_to_decimal(flt, &dec) == OK)
    {
        printf("float to dec result: ");
        printDecimalAsInts(dec);
    }
    else printf("float to dec result: CONVERTATION_ERR\n");
    
    flt = 123456780000.0;
    printf("\nfloat numb = %+.6E\n", flt);
    if (s21_from_float_to_decimal(flt, &dec) == OK)
    {
        printf("float to dec result: ");
        printDecimalAsInts(dec);
    }
    else printf("float to dec result: CONVERTATION_ERR\n");

    flt = -1234560780.0;
    printf("\nfloat numb = %+.6E\n", flt);
    if (s21_from_float_to_decimal(flt, &dec) == OK)
    {
        printf("float to dec result: ");
        printDecimalAsInts(dec);
    }
    else printf("float to dec result: CONVERTATION_ERR\n");

    int int_v = INT_MIN;
    printf("\nint numb = %d\n", int_v);
    if (s21_from_int_to_decimal(int_v, &dec) == OK)
    {
        printf("int to dec result: ");
        printDecimalAsInts(dec);
    }
    else printf("int to dec result: CONVERTATION_ERR\n");

    int_v = 345689012;
    printf("\nint numb = %d\n", int_v);
    if (s21_from_int_to_decimal(int_v, &dec) == OK)
    {
        printf("int to dec result: ");
        printDecimalAsInts(dec);
    }
    else printf("int to dec result: CONVERTATION_ERR\n");

    int_v = 0;
    printf("\nint numb = %d\n", int_v);
    if (s21_from_int_to_decimal(int_v, &dec) == OK)
    {
        printf("int to dec result: ");
        printDecimalAsInts(dec);
    }
    else printf("int to dec result: CONVERTATION_ERR\n");

    make_zero_decimal(&dec);
    dec.low = 43269000;
    dec.scale = 0;
    dec.is_negative = FALSE;
    putc('\n', stdout);
    printDecimalAsInts(dec);
    if (s21_from_decimal_to_int(dec, &int_v) == OK)
        printf("dec to int result: %d\n", int_v);
    else printf("dec to int result: CONVERTATION_ERR\n");

    make_zero_decimal(&dec);
    dec.low = 43269000;
    dec.scale = 3;
    dec.is_negative = TRUE;
    putc('\n', stdout);
    printDecimalAsInts(dec);
    if (s21_from_decimal_to_int(dec, &int_v) == OK)
        printf("dec to int result: %d\n", int_v);
    else printf("dec to int result: CONVERTATION_ERR\n");

    make_zero_decimal(&dec);
    dec.low = 43269000;
    dec.middle = 127;
    dec.scale = 6;
    dec.is_negative = FALSE;
    putc('\n', stdout);
    printDecimalAsInts(dec);
    if (s21_from_decimal_to_int(dec, &int_v) == OK)
        printf("dec to int result: %d\n", int_v);
    else printf("dec to int result: CONVERTATION_ERR\n");

    make_zero_decimal(&dec);
    dec.low = INT_MAX;
    dec.middle = 0;
    dec.scale = 0;
    dec.is_negative = FALSE;
    putc('\n', stdout);
    printDecimalAsInts(dec);
    if (s21_from_decimal_to_int(dec, &int_v) == OK)
        printf("dec to int result: %d\n", int_v);
    else printf("dec to int result: CONVERTATION_ERR\n");

    make_zero_decimal(&dec);
    dec.low = (unsigned)INT_MAX + 1;
    dec.middle = 0;
    dec.scale = 0;
    dec.is_negative = FALSE;
    putc('\n', stdout);
    printDecimalAsInts(dec);
    if (s21_from_decimal_to_int(dec, &int_v) == OK)
        printf("dec to int result: %d\n", int_v);
    else printf("dec to int result: CONVERTATION_ERR\n");

    s21_decimal round_result = {{0,0,0,0}};
    s21_decimal dec1 = {{0,0,0,545}};
    dec1.scale = 1;
    putc('\n', stdout);
    printDecimalAsInts(dec1);
    if (s21_floor(dec1, &round_result) == OK)
    {
        printf("rounding result: ");
        printDecimalAsInts(round_result);
    }
    else printf("floor rounding result: CONVERTATION_ERR\n");

    dec1.is_negative = TRUE;
    putc('\n', stdout);
    printDecimalAsInts(dec1);
    if (s21_floor(dec1, &round_result) == OK)
    {
        printf("rounding result: ");
        printDecimalAsInts(round_result);
    }
    else printf("floor rounding result: CONVERTATION_ERR\n");

    putc('\n', stdout);
    printDecimalAsInts(dec1);
    if (s21_round(dec1, &round_result) == OK)
    {
        printf("rounding result: ");
        printDecimalAsInts(round_result);
    }
    else printf("rounding result: CONVERTATION_ERR\n");

    dec1.is_negative = FALSE;
    dec1.low = 34012;
    dec1.scale = 2;
    putc('\n', stdout);
    printDecimalAsInts(dec1);
    if (s21_round(dec1, &round_result) == OK)
    {
        printf("rounding result: ");
        printDecimalAsInts(round_result);
    }
    else printf("rounding result: CONVERTATION_ERR\n");

    dec1.low = 34050;
    putc('\n', stdout);
    printDecimalAsInts(dec1);
    if (s21_round(dec1, &round_result) == OK)
    {
        printf("rounding result: ");
        printDecimalAsInts(round_result);
    }
    else printf("rounding result: CONVERTATION_ERR\n");

    dec1.low = 34350;
    putc('\n', stdout);
    printDecimalAsInts(dec1);
    if (s21_round(dec1, &round_result) == OK)
    {
        printf("rounding result: ");
        printDecimalAsInts(round_result);
    }
    else printf("rounding result: CONVERTATION_ERR\n");

    dec1.low = 34657;
    putc('\n', stdout);
    printDecimalAsInts(dec1);
    if (s21_round(dec1, &round_result) == OK)
    {
        printf("rounding result: ");
        printDecimalAsInts(round_result);
    }
    else printf("rounding result: CONVERTATION_ERR\n");

    return 0;
}
*/
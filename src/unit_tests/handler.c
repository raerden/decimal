void test_add(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, OK);
}

void test_add_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
    s21_decimal result;
    int code = s21_add(decimal1, decimal2, &result);

    ck_assert_int_eq(code, check);
}

void test_sub(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result;
    int code = s21_sub(decimal1, decimal2, &result);

    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, OK);
}

void test_sub_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
    s21_decimal result;
    int code = s21_sub(decimal1, decimal2, &result);

    ck_assert_int_eq(code, check);
}

void test_mul(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, OK);
}

void test_mul_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(code, check);
}

void test_mul_fail2(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check, int code_check) {
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, code_check);
}

void test_div(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check) {
    s21_decimal result;
    int code = s21_div(decimal1, decimal2, &result);

    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, OK);
}

void test_div_fail(s21_decimal decimal1, s21_decimal decimal2, int check) {
    s21_decimal result;
    int code = s21_div(decimal1, decimal2, &result);

    ck_assert_int_eq(code, check);
}

void test_div_fail2(s21_decimal decimal1, s21_decimal decimal2, s21_decimal check, int code_check) {
    s21_decimal result;
    int code = s21_div(decimal1, decimal2, &result);

    ck_assert_int_eq(s21_is_equal(result, check), 1);
    ck_assert_int_eq(code, code_check);
}


typedef union float_cast_test {
    float f;
    struct {
        uint32_t mantisa : 23;
        uint32_t exponent : 8;
        uint32_t sign : 1;
    } parts;
    uint32_t bytes;
    int int32_bytes;
} float_cast_test;

void test_from_decimal_to_float(s21_decimal decimal, int check) {
    float result;
    int code = s21_from_decimal_to_float(decimal, &result);

    float_cast_test cast_result;
    cast_result.f = result;


    ck_assert_int_eq(cast_result.int32_bytes, check);
    ck_assert_int_eq(code, OK);
}

void test_from_float_to_decimal(int f, s21_decimal decimal_check) {
    s21_decimal result;

    float_cast_test cast_float;
    cast_float.int32_bytes = f;

    int code = s21_from_float_to_decimal(cast_float.f, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
}

void test_from_decimal_to_int(s21_decimal decimal, int check) {
    int result;
    int code = s21_from_decimal_to_int(decimal, &result);

    ck_assert_int_eq(result, check);
    ck_assert_int_eq(code, OK);
}

void test_from_decimal_to_int_fail(s21_decimal decimal) {
    int result;
    int code = s21_from_decimal_to_int(decimal, &result);

    ck_assert_int_eq(code, CONVERTATION_ERR);
}

void test_is_equal(s21_decimal decimal1, s21_decimal decimal2, int check) {
    int code = s21_is_equal(decimal1, decimal2);
    ck_assert_int_eq(code, check);
}

void test_is_greater(s21_decimal decimal1, s21_decimal decimal2, int check) {
    int code = s21_is_greater(decimal1, decimal2);

    ck_assert_int_eq(code, check);
}

void test_is_greater_or_equal(s21_decimal decimal1, s21_decimal decimal2, int check) {
    int code = s21_is_greater_or_equal(decimal1, decimal2);

    ck_assert_int_eq(code, check);
}

void test_is_less(s21_decimal decimal1, s21_decimal decimal2, int check) {
    int code = s21_is_less(decimal1, decimal2);

    ck_assert_int_eq(code, check);
}

void test_is_less_or_equal(s21_decimal decimal1, s21_decimal decimal2, int check) {
    int code = s21_is_less_or_equal(decimal1, decimal2);

    ck_assert_int_eq(code, check);
}

void test_is_not_equal(s21_decimal decimal1, s21_decimal decimal2, int check) {
    int code = s21_is_not_equal(decimal1, decimal2);

    ck_assert_int_eq(code, check);
}

void test_round(s21_decimal decimal, s21_decimal decimal_check) {
    s21_decimal result;

    int code = s21_round(decimal, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
    ck_assert_int_eq(decimal_check.is_negative, result.is_negative);
}

void test_floor(s21_decimal decimal, s21_decimal decimal_check) {
    s21_decimal result;

    int code = s21_floor(decimal, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
    ck_assert_int_eq(decimal_check.is_negative, result.is_negative);
}


void test_negate(s21_decimal decimal, s21_decimal decimal_check) {
    s21_decimal result;

    int code = s21_negate(decimal, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
    ck_assert_int_eq(decimal_check.is_negative, result.is_negative);
}

void test_truncate(s21_decimal decimal, s21_decimal decimal_check) {
    s21_decimal result;

    int code = s21_truncate(decimal, &result);

    ck_assert_int_eq(code, OK);
    ck_assert_int_eq(s21_is_equal(result, decimal_check), 1);
    ck_assert_int_eq(decimal_check.is_negative, result.is_negative);
}

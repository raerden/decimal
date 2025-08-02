CC = gcc
CFLAGS = -Wall -Wextra -Werror
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
UNAME := $(shell uname -s)
LDFLAGS = -lcheck -lm -lpthread
MEMTEST_EXEC = leaks -atExit --
ifeq ($(UNAME), Linux)
    LDFLAGS += -lsubunit
	MEMTEST_EXEC = valgrind
endif

SRC = common.c $(wildcard units/*.c)

OBJ_DIR = obj
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
TEST_SRC = test_s21_decimal.c
TEST_EXEC = test_s21_decimal
GCOV_DIR = gcov_report

all: clean s21_decimal.a

s21_decimal.a: $(OBJ)
	ar rcs $@ $^
	ranlib $@

$(TEST_SRC):
	checkmk tests.check > $@

$(TEST_EXEC): $(TEST_SRC) s21_decimal.a
	$(CC) $(CFLAGS) $< -o $@ s21_decimal.a $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

test: all $(TEST_EXEC)
	./$(TEST_EXEC)

gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report: clean $(TEST_SRC)
	mkdir -p $(GCOV_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(TEST_SRC) $(SRC) -o $(GCOV_DIR)/$(TEST_EXEC) $(LDFLAGS)
	./$(GCOV_DIR)/$(TEST_EXEC)
	lcov -t "$(TEST_EXEC)" -o $(GCOV_DIR)/coverage.info -c -d . --exclude "*/tests.check"
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/coverage.info
	@echo "=============================================="
	@echo "Отчёт покрытия успешно сгенерирован!"
	@echo "Откройте в браузере:"
	@echo "file://$(CURDIR)/$(GCOV_DIR)/report/index.html"
	@echo "=============================================="

clean:
	rm -rf $(OBJ_DIR) $(TEST_EXEC) $(GCOV_DIR) s21_decimal.a test_s21_decimal.c

cpptest:
	cppcheck --enable=all --suppress=missingIncludeSystem *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c c_sharp/*.c sprintf/*.c string_func/*.c

clang:
	@cp ../materials/linters/.clang-format .
	clang-format -n *.h *.c c_sharp/*.c sprintf/*.c string_func/*.c
	clang-format -i *.h *.c c_sharp/*.c sprintf/*.c string_func/*.c

memtest: all
	$(CC) $(CFLAGS) memtest.c s21_decimal.a -o memtest
	$(MEMTEST_EXEC) ./memtest
	rm -rf $(OBJ_DIR) s21_decimal.a memtest

run:
	gcc -Werror -Wextra units/*.c decimal2str.c common.c work.c
	./a.out
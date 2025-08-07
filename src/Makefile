CC = gcc
CFLAGS = -Wall -Wextra -Werror
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
UNAME := $(shell uname -s)
LDFLAGS = -lcheck -lm -lpthread
MEMTEST_EXEC = leaks -atExit --
#CHECK_FLAG := $(shell pkg-config --cflags --libs check)
ifeq ($(UNAME), Linux)
    LDFLAGS += -lsubunit
	MEMTEST_EXEC = valgrind
endif

SRC = common.c $(wildcard units/*.c)

UNIT_TEST_DIR = unit_tests
TESTS_CHECK = $(wildcard $(UNIT_TEST_DIR)/*.check)
TESTS_C = tests.c
TEST_EXEC = tests.run

OBJ_DIR = obj
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
GCOV_DIR = gcov_report

all: clean s21_decimal.a

s21_decimal.a: $(OBJ)
	@echo "*\c";
	@ar rcs $@ $^
	@echo "*\c";
	@ranlib $@

$(TEST_EXEC): 
	@echo "*\c";
	@checkmk $(TESTS_CHECK) > $(UNIT_TEST_DIR)/$(TESTS_C)
	@echo "*\c";
	@$(CC) $(CFLAGS) $(UNIT_TEST_DIR)/$(TESTS_C) -o $@ s21_decimal.a $(LDFLAGS)

gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report: clean
	mkdir -p $(GCOV_DIR)
	checkmk $(TESTS_CHECK) > $(UNIT_TEST_DIR)/$(TESTS_C)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(UNIT_TEST_DIR)/$(TESTS_C) $(SRC) -o $(GCOV_DIR)/$(TEST_EXEC) $(LDFLAGS)
	./$(GCOV_DIR)/$(TEST_EXEC)
	lcov -t "$(TEST_EXEC)" -o $(GCOV_DIR)/coverage.info -c -d . --exclude "*.check"
	genhtml -o $(GCOV_DIR)/report $(GCOV_DIR)/coverage.info
	@echo "=============================================="
	@echo "Отчёт покрытия успешно сгенерирован!"
	@echo "Откройте в браузере:"
	@echo "file://$(CURDIR)/$(GCOV_DIR)/report/index.html"
	@echo "=============================================="

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "*\c";
	@$(CC) $(CFLAGS) -c $< -o $@

test: all $(TEST_EXEC)
	@echo "*";
	@./$(TEST_EXEC)

clean:
	@rm -rf $(UNIT_TEST_DIR)/$(TESTS_C) $(TEST_EXEC) $(GCOV_DIR)
	@rm -rf $(OBJ_DIR) s21_decimal.a

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
	gcc -Werror -Wextra units/*.c decimal2str.c common.c work.c -lm
	./a.out
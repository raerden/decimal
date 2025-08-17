#This project made by beedlesh haywoota woodsjul thymemas

CC = gcc
CFLAGS = -Wall -Wextra -Werror
GCOV_FLAGS = -Wno-coverage-invalid-line-number -fprofile-arcs -ftest-coverage
UNAME := $(shell uname -s)
LDFLAGS = -lcheck -lm -lpthread
ifeq ($(UNAME), Linux)
    LDFLAGS += -lsubunit
endif

SRC = common.c $(wildcard units/*.c)

UNIT_TEST_DIR = unit_tests
TESTS_CHECK = $(wildcard $(UNIT_TEST_DIR)/*.check)
TESTS_CHECK_HUGE = $(UNIT_TEST_DIR)/test_1_header.check $(wildcard $(UNIT_TEST_DIR)/*.1check)
TESTS_C = tests.c
TEST_EXEC = tests.run

OBJ_DIR = obj
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
GCOV_DIR = gcov_report
REPORT_PATH = $(GCOV_DIR)/report/

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
	@$(CC) $(CFLAGS) $(UNIT_TEST_DIR)/$(TESTS_C) -o $(UNIT_TEST_DIR)/$@ s21_decimal.a $(LDFLAGS)

gcov_report: CFLAGS += $(GCOV_FLAGS)
gcov_report: clean
	@mkdir -p $(GCOV_DIR)
	@checkmk $(TESTS_CHECK) > $(UNIT_TEST_DIR)/$(TESTS_C)
	@$(CC) $(CFLAGS) $(GCOV_FLAGS) $(UNIT_TEST_DIR)/$(TESTS_C) $(SRC) -o $(GCOV_DIR)/$(TEST_EXEC) $(LDFLAGS)
	@./$(GCOV_DIR)/$(TEST_EXEC)
	@lcov -t "$(UNIT_TEST_DIR)/$(TEST_EXEC)" -o $(GCOV_DIR)/coverage.info -c -d . --exclude "*.check"
	@lcov --remove $(GCOV_DIR)/coverage.info "*/unit_tests/*" -o $(GCOV_DIR)/coverage.info --ignore-errors unused
	@genhtml -o $(REPORT_PATH) $(GCOV_DIR)/coverage.info
	@echo "<table width="100%"><td class=\"title\" style=\"color: lightgreen; -webkit-text-stroke: 1px black;\">#Project was made by<br>beedlesh<br>haywoota<br>woodsjul<br>thymemas</td></table>" >> $(REPORT_PATH)index.html
	@echo "=============================================="
	@echo "Отчёт покрытия успешно сгенерирован!"
	@echo "Откройте в браузере:"
	@echo "file://$(CURDIR)/$(REPORT_PATH)index.html"
	@echo "=============================================="
	@find "$(REPORT_PATH)src" -name "*.html" -type f -exec sh -c 'echo "<table width="100%"><td class=\"title\" style=\"color: lightgreen; -webkit-text-stroke: 1px black;\">#Project was made by<br>beedlesh<br>haywoota<br>woodsjul<br>thymemas</td></table>" >> "$$1"' _ {} \;

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@echo "*\c";
	@$(CC) $(CFLAGS) -c $< -o $@

test: all $(TEST_EXEC)
	@echo "*";
	@echo "Start of all the tests...\n"
	@./$(UNIT_TEST_DIR)/$(TEST_EXEC)

hugetest: all
	@echo "*\c";
	@checkmk $(TESTS_CHECK_HUGE) > $(UNIT_TEST_DIR)/$(TESTS_C)
	@echo "*\c";
	@$(CC) $(CFLAGS) $(UNIT_TEST_DIR)/$(TESTS_C) -o $(UNIT_TEST_DIR)/$(TEST_EXEC) s21_decimal.a $(LDFLAGS)
	@echo "*";
	@echo "Start of tests\n"
	@./$(UNIT_TEST_DIR)/$(TEST_EXEC)

clean:
	@rm -rf $(GCOV_DIR) $(UNIT_TEST_DIR)/$(TESTS_C) $(UNIT_TEST_DIR)/$(TEST_EXEC) 
	@rm -rf $(OBJ_DIR) s21_decimal.a

cpptest:
	cppcheck --enable=all --suppress=missingIncludeSystem *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c units/*.c

clang:
	cp ../materials/linters/.clang-format .
	clang-format -n *.h *.c units/*.c
	clang-format -i *.h *.c units/*.c
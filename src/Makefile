.PHONY: all clear rebuild

CC = g++
CFLAGS = -Wall -Werror -Wextra -Wno-sign-compare -Wno-array-bounds -std=c++17 -fsanitize=address

all: test

test:
	@$(CC) $(CFLAGS) s21_containers_test.cc -lgtest -pthread -o s21_containers_test
	@./s21_containers_test

gcov_report:
	$(CC) $(CCFLAGS) -fprofile-arcs -ftest-coverage s21_containers_test.cc -lgtest -pthread -o gcovreport \
	`pkg-config --cflags --libs check`
	./gcovreport
	lcov -t "gcovreport" --no-external -o gcovreport.info -c -d .
	genhtml -o report gcovreport.info

clean:
	@rm -rf *.o *.a *.gcda *.gcno *.info s21_containers_test report gcovreport

style:
	@clang-format -style=google -n -verbose *.cc *.h

rebuild: clean all

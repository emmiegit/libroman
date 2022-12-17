/*
 * main.c
 *
 * Copyright 2017 Emmie Maeda
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "roman.h"

static long str2long(const char *str)
{
	int neg;
	long sum;

	sum = 0;
	if (str[0] == '-') {
		neg = 1;
		str++;
	} else {
		neg = 0;
	}
	while (*str) {
		if (!isdigit(*str)) {
			return -1;
		}
		sum *= 10;
		sum += *str - '0';
		str++;
	}
	return (!neg) ? sum : -sum;
}

static void usage_and_exit(FILE *out, const char *argv0, int ret)
{
	fprintf(out, "Usage: %s [NUMBER]... [ROMAN NUMERAL]...\n", argv0);
	exit(ret);
}

static void print_roman(long num, int *ret)
{
	char buf[256];

	longtoroman(num, buf, sizeof(buf));
	if (errno) {
		puts("Result is too long.");
		*ret = EXIT_FAILURE;
	} else {
		puts(buf);
	}
}

static void print_numeral(const char *str, int *ret)
{
	long num;

	num = romantolong(str);
	if (errno) {
		puts("Invalid input.");
		*ret = EXIT_FAILURE;
	} else {
		printf("%ld\n", num);
	}
}

int main(int argc, const char **argv)
{
	long num;
	int i;
	int ret;

	if (argc == 1) {
		usage_and_exit(stderr, argv[0], EXIT_FAILURE);
	}

	ret = 0;
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "--help")) {
			usage_and_exit(stdout, argv[0], EXIT_SUCCESS);
		}

		num = str2long(argv[i]);
		if (num != -1) {
			print_roman(num, &ret);
		} else {
			print_numeral(argv[i], &ret);
		}
	}

	return ret;
}

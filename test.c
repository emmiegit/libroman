/*
 * test.c
 *
 * Copyright 2022 Ammon Smith
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

#include <string.h>
#include <stdio.h>

#include "roman.h"

static int exit_code;
static char buffer[256];

#define UNUSED(x)			((void)(x))

#define TEST(test_fn, input, expected)		\
	do {					\
		if (test_fn(input, expected)) {	\
			exit_code = 1;		\
		}				\
	} while (0)

#define TEST_R2L(input, expected)	TEST(test_r2l, input, expected)
#define TEST_L2R(input, expected)	TEST(test_l2r, input, expected)

#define RESULT_STRING(value)		(value) ? "\x1b[32mPASS\x1b[0m" : "\x1b[31mFAIL\x1b[0m"

static int test_r2l(const char *input, long expected)
{
	long actual;
	int result;

	actual = romantolong(input);
	result = actual == expected;

	printf("* [%s] %s - %ld\n", RESULT_STRING(result), input, actual);
	return result;
}

static int test_l2r(long input, const char *expected)
{
	int result;

	longtoroman(input, buffer, sizeof(buffer));
	result = !strcmp(buffer, expected);

	printf("* [%s] %ld - %s\n", RESULT_STRING(result), input, buffer);
	return result;
}

int main(int argc, const char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	puts("Parsing tests:");
	TEST_R2L("I", 1);

	puts("Conversion tests:");
	TEST_L2R(1, "I");

	return exit_code;
}


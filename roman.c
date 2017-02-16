/*
 * roman.c
 *
 * Copyright 2017 Ammon Smith
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
#include <string.h>

#include "roman.h"

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof((x)[0]))

static const struct numeral_value {
	char ch;
	short value;
} numerals[] = {
	{ 'I', 1 },
	{ 'V', 5 },
	{ 'X', 10 },
	{ 'L', 50 },
	{ 'C', 100 },
	{ 'D', 500 },
	{ 'M', 1000 }
};

static short get_digit(char ch)
{
	size_t i;

	ch = toupper(ch);
	for (i = 0; i < ARRAY_SIZE(numerals); i++) {
		if (numerals[i].ch == ch) {
			return numerals[i].value;
		}
	}
	return -1;
}

static int is_nulla(const char *str)
{
	size_t i;
	int equals;

	/*
	 * This function does a case-insensitive match for "nulla" or "nihil".
	 *
	 * Normally I'd just use strcasecmp(), but requiring POSIX 2001
	 * compatibility for just one function is absurd.
	 */

	/* The 'n' has already been checked by romantolong(). */
	equals = 1;
	for (i = 0; i < 5; i++) {
		if (tolower(str[i]) != "ulla"[i]) {
			equals = 0;
			break;
		}
	}
	if (equals) {
		return 1;
	}
	equals = 1;
	for (i = 0; i < 5; i++) {
		if (tolower(str[i]) != "ihil"[i]) {
			equals = 0;
			break;
		}
	}
	return equals;
}

long romantolong(const char *str)
{
	return romantolongn(str, strlen(str));
}

long romantolongn(const char *str, size_t len)
{
	int neg;
	long sum, part;
	short last, cur;
	size_t i;

	/* Ignore leading whitespace */
	for (i = 0; isspace(str[i]); i++) {
		if (i == len) {
			errno = 0;
			return 0;
		}
	}

	/* Set initial values and flags */
	sum = 0;
	if (str[i] == '-') {
		neg = 1;
		i++;
	} else if (tolower(str[i]) == 'n') {
		neg = 1;
		i++;
		/* Check for zero */
		if (i == len || is_nulla(str + i)) {
			errno = 0;
			return 0;
		}
	} else {
		neg = 0;
	}

	/* Ignore middle whitespace */
	for (; isspace(str[i]); i++) {
		if (i == len) {
			errno = EINVAL;
			return -1;
		}
	}

	last = get_digit(str[i++]);
	part = last;
	if (last < 0) {
		errno = EINVAL;
		return -1;
	}

	/* Read each character */
	for (; i < len; i++) {
		if (isspace(str[i])) {
			break;
		}

		cur = get_digit(str[i]);
		if (cur < 0) {
			errno = EINVAL;
			return -1;
		}

		if (last == cur) {
			part += cur;
		} else if (last < cur) {
			part = cur - part;
		} else { /* last > cur */
			sum += part;
			part = cur;
		}
		last = cur;
	}
	sum += part;

	/* Ignore trailing whitespace */
	for (; i < len; i++) {
		if (!isspace(str[i])) {
			errno = EINVAL;
			return -1;
		}
	}

	return (!neg) ? sum : -sum;
}

int longtoroman(long num, char *buf, size_t len)
{
	size_t i, bytes;

	/* Special cases */
	if (len == 0) {
		errno = ENOBUFS;
		return -1;
	}
	if (num == 0) {
		buf[0] = 'N';
		buf[1] = '\0';
		errno = 0;
		return 1;
	}

	/* Handling for the sign */
	if (num < 0) {
		num = -num;
		buf[0] = '-';
		bytes = 1;
	} else {
		bytes = 0;
	}

	/* Only iterate over the 10* series, not the 5* numerals */
	for (i = 0; i < ARRAY_SIZE(numerals); i += 2) {
		const struct numeral_value *nv;
		long digit;

		nv = &numerals[ARRAY_SIZE(numerals) - i - 1];
		digit = num / nv->value;
		num = num % nv->value;

		/* Optimize by skipping zero */
		if (digit == 0) {
			continue;
		}

		/*
		 * Traditionally, we use subtractive forms for 4 and 9, such as
		 * XL and XC for 40 and 90.
		 *
		 * For some digit n, the form is (n)(n + 1) for 4* and
		 * (n)(n + 2) for 9*. This can be reduced to an algebraic expression
		 * to calculate the offset from n, which is what "off" is. It can only
		 * be 1 or 2.
		 *
		 * For digits 5 or greater, the appropriate 5* value is printed,
		 * and then the remaining (n)s are added.
		 *
		 * For digits less than 5, it's simply that number of (n)s.
		 */

		if ((i != 0) && (digit == 4 || digit == 9)) {
			int off = (digit + 1) / 5;
			if (bytes + 2 >= len) {
				errno = ENOBUFS;
				return -1;
			}
			buf[bytes]     = nv->ch;
			buf[bytes + 1] = (nv+off)->ch;
			bytes += 2;
		} else if ((i != 0) && (digit >= 5)) {
			if (bytes + (digit - 4) >= len) {
				errno = ENOBUFS;
				return -1;
			}
			buf[bytes] = (nv+1)->ch;
			memset(buf + bytes + 1, nv->ch, digit - 1);
			bytes += (digit - 4);
		} else {
			if (bytes + digit >= len) {
				errno = ENOBUFS;
				return -1;
			}
			memset(buf + bytes, nv->ch, digit);
			bytes += digit;
		}

	}
	errno = 0;
	buf[bytes] = '\0';
	return bytes;
}


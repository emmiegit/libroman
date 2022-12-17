/*
 * test.c
 *
 * Copyright 2022 Emmie Maeda
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

#define TEST(test_fn, input, expected)			\
	do {						\
		if (!test_fn(input, expected)) {	\
			exit_code = 1;			\
		}					\
	} while (0)

#define TEST_R2L(input, expected)	TEST(test_r2l, input, expected)
#define TEST_L2R(input, expected)	TEST(test_l2r, input, expected)

#define RESULT_STRING(value)		(value) ? "\x1b[32mPASS\x1b[0m" : "\x1b[31mFAIL\x1b[0m"

static int test_r2l(const char *input, long expected)
{
	long actual;
	int passed;

	actual = romantolong(input);
	passed = actual == expected;

	printf("* [%s] %8s -> %4ld\n", RESULT_STRING(passed), input, actual);
	return passed;
}

static int test_l2r(long input, const char *expected)
{
	int passed;

	longtoroman(input, buffer, sizeof(buffer));
	passed = !strcmp(buffer, expected);

	printf("* [%s] %4ld -> %8s\n", RESULT_STRING(passed), input, buffer);
	return passed;
}

int main(int argc, const char **argv)
{
	UNUSED(argc);
	UNUSED(argv);

	puts("Parsing tests:");
	TEST_R2L("I", 1);
	TEST_R2L("II", 2);
	TEST_R2L("III", 3);
	TEST_R2L("IIII", 4);
	TEST_R2L("IV", 4);
	TEST_R2L("V", 5);
	TEST_R2L("VI", 6);
	TEST_R2L("VII", 7);
	TEST_R2L("VIII", 8);
	TEST_R2L("IIX", 8);
	TEST_R2L("IX", 9);
	TEST_R2L("X", 10);
	TEST_R2L("XI", 11);
	TEST_R2L("XII", 12);
	TEST_R2L("XIII", 13);
	TEST_R2L("XIIII", 14);
	TEST_R2L("XIV", 14);
	TEST_R2L("XV", 15);
	TEST_R2L("XVI", 16);
	TEST_R2L("XVII", 17);
	TEST_R2L("XVIII", 18);
	TEST_R2L("XVIIII", 19);
	TEST_R2L("XVIV", 19);
	TEST_R2L("XIX", 19);
	TEST_R2L("IXX", 19);
	TEST_R2L("XX", 20);
	TEST_R2L("XVV", 20);
	TEST_R2L("XXI", 21);
	TEST_R2L("XXII", 22);
	TEST_R2L("XXIII", 23);
	TEST_R2L("XXIIII", 24);
	TEST_R2L("XXIV", 24);
	TEST_R2L("XXIIIII", 25);
	TEST_R2L("XXV", 25);
	TEST_R2L("XXVI", 26);
	TEST_R2L("XXVII", 27);
	TEST_R2L("XXVIII", 28);
	TEST_R2L("XXIIX", 28);
	TEST_R2L("XXVIIII", 29);
	TEST_R2L("XXIX", 29);
	TEST_R2L("XXX", 30);

	putchar('\n');
	puts("Conversion tests:");
	TEST_L2R(1, "I");
	TEST_L2R(2, "II");
	TEST_L2R(3, "III");
	TEST_L2R(4, "IV");
	TEST_L2R(5, "V");
	TEST_L2R(6, "VI");
	TEST_L2R(7, "VII");
	TEST_L2R(8, "VIII");
	TEST_L2R(9, "IX");
	TEST_L2R(10, "X");
	TEST_L2R(11, "XI");
	TEST_L2R(12, "XII");
	TEST_L2R(13, "XIII");
	TEST_L2R(14, "XIV");
	TEST_L2R(15, "XV");
	TEST_L2R(16, "XVI");
	TEST_L2R(17, "XVII");
	TEST_L2R(18, "XVIII");
	TEST_L2R(19, "XIX");
	TEST_L2R(20, "XX");
	TEST_L2R(21, "XXI");
	TEST_L2R(22, "XXII");
	TEST_L2R(23, "XXIII");
	TEST_L2R(24, "XXIV");
	TEST_L2R(25, "XXV");
	TEST_L2R(26, "XXVI");
	TEST_L2R(27, "XXVII");
	TEST_L2R(28, "XXVIII");
	TEST_L2R(29, "XXIX");
	TEST_L2R(30, "XXX");
	TEST_L2R(31, "XXXI");
	TEST_L2R(32, "XXXII");
	TEST_L2R(33, "XXXIII");
	TEST_L2R(34, "XXXIV");
	TEST_L2R(35, "XXXV");
	TEST_L2R(36, "XXXVI");
	TEST_L2R(37, "XXXVII");
	TEST_L2R(38, "XXXVIII");
	TEST_L2R(39, "XXXIX");
	TEST_L2R(40, "XL");
	TEST_L2R(41, "XLI");
	TEST_L2R(42, "XLII");
	TEST_L2R(43, "XLIII");
	TEST_L2R(44, "XLIV");
	TEST_L2R(45, "XLV");
	TEST_L2R(46, "XLVI");
	TEST_L2R(47, "XLVII");
	TEST_L2R(48, "XLVIII");
	TEST_L2R(49, "XLIX");
	TEST_L2R(50, "L");
	TEST_L2R(51, "LI");
	TEST_L2R(52, "LII");
	TEST_L2R(53, "LIII");
	TEST_L2R(54, "LIV");
	TEST_L2R(55, "LV");
	TEST_L2R(56, "LVI");
	TEST_L2R(57, "LVII");
	TEST_L2R(58, "LVIII");
	TEST_L2R(59, "LIX");
	TEST_L2R(60, "LX");
	TEST_L2R(61, "LXI");
	TEST_L2R(62, "LXII");
	TEST_L2R(63, "LXIII");
	TEST_L2R(64, "LXIV");
	TEST_L2R(65, "LXV");
	TEST_L2R(66, "LXVI");
	TEST_L2R(67, "LXVII");
	TEST_L2R(68, "LXVIII");
	TEST_L2R(69, "LXIX");
	TEST_L2R(70, "LXX");
	TEST_L2R(71, "LXXI");
	TEST_L2R(72, "LXXII");
	TEST_L2R(73, "LXXIII");
	TEST_L2R(74, "LXXIV");
	TEST_L2R(75, "LXXV");
	TEST_L2R(76, "LXXVI");
	TEST_L2R(77, "LXXVII");
	TEST_L2R(78, "LXXVIII");
	TEST_L2R(79, "LXXIX");
	TEST_L2R(80, "LXXX");
	TEST_L2R(81, "LXXXI");
	TEST_L2R(82, "LXXXII");
	TEST_L2R(83, "LXXXIII");
	TEST_L2R(84, "LXXXIV");
	TEST_L2R(85, "LXXXV");
	TEST_L2R(86, "LXXXVI");
	TEST_L2R(87, "LXXXVII");
	TEST_L2R(88, "LXXXVIII");
	TEST_L2R(89, "LXXXIX");
	TEST_L2R(90, "XC");
	TEST_L2R(91, "XCI");
	TEST_L2R(92, "XCII");
	TEST_L2R(93, "XCIII");
	TEST_L2R(94, "XCIV");
	TEST_L2R(95, "XCV");
	TEST_L2R(96, "XCVI");
	TEST_L2R(97, "XCVII");
	TEST_L2R(98, "XCVIII");
	TEST_L2R(99, "XCIX");
	TEST_L2R(100, "C");
	TEST_L2R(101, "CI");
	TEST_L2R(102, "CII");
	TEST_L2R(103, "CIII");
	TEST_L2R(104, "CIV");
	TEST_L2R(105, "CV");
	TEST_L2R(106, "CVI");
	TEST_L2R(107, "CVII");
	TEST_L2R(108, "CVIII");
	TEST_L2R(109, "CIX");
	TEST_L2R(110, "CX");
	TEST_L2R(111, "CXI");
	TEST_L2R(112, "CXII");
	TEST_L2R(113, "CXIII");
	TEST_L2R(114, "CXIV");
	TEST_L2R(115, "CXV");
	TEST_L2R(116, "CXVI");
	TEST_L2R(117, "CXVII");
	TEST_L2R(118, "CXVIII");
	TEST_L2R(119, "CXIX");
	TEST_L2R(120, "CXX");
	TEST_L2R(121, "CXXI");
	TEST_L2R(122, "CXXII");
	TEST_L2R(123, "CXXIII");
	TEST_L2R(124, "CXXIV");
	TEST_L2R(125, "CXXV");
	TEST_L2R(126, "CXXVI");
	TEST_L2R(127, "CXXVII");
	TEST_L2R(128, "CXXVIII");
	TEST_L2R(129, "CXXIX");
	TEST_L2R(130, "CXXX");
	TEST_L2R(131, "CXXXI");
	TEST_L2R(132, "CXXXII");
	TEST_L2R(133, "CXXXIII");
	TEST_L2R(134, "CXXXIV");
	TEST_L2R(135, "CXXXV");
	TEST_L2R(136, "CXXXVI");
	TEST_L2R(137, "CXXXVII");
	TEST_L2R(138, "CXXXVIII");
	TEST_L2R(139, "CXXXIX");
	TEST_L2R(140, "CXL");
	TEST_L2R(141, "CXLI");
	TEST_L2R(142, "CXLII");
	TEST_L2R(143, "CXLIII");
	TEST_L2R(144, "CXLIV");
	TEST_L2R(145, "CXLV");
	TEST_L2R(146, "CXLVI");
	TEST_L2R(147, "CXLVII");
	TEST_L2R(148, "CXLVIII");
	TEST_L2R(149, "CXLIX");
	TEST_L2R(150, "CL");
	TEST_L2R(151, "CLI");
	TEST_L2R(152, "CLII");
	TEST_L2R(153, "CLIII");
	TEST_L2R(154, "CLIV");
	TEST_L2R(155, "CLV");
	TEST_L2R(156, "CLVI");
	TEST_L2R(157, "CLVII");
	TEST_L2R(158, "CLVIII");
	TEST_L2R(159, "CLIX");
	TEST_L2R(160, "CLX");
	TEST_L2R(161, "CLXI");
	TEST_L2R(162, "CLXII");
	TEST_L2R(163, "CLXIII");
	TEST_L2R(164, "CLXIV");
	TEST_L2R(165, "CLXV");
	TEST_L2R(166, "CLXVI");
	TEST_L2R(167, "CLXVII");
	TEST_L2R(168, "CLXVIII");
	TEST_L2R(169, "CLXIX");
	TEST_L2R(170, "CLXX");
	TEST_L2R(171, "CLXXI");
	TEST_L2R(172, "CLXXII");
	TEST_L2R(173, "CLXXIII");
	TEST_L2R(174, "CLXXIV");
	TEST_L2R(175, "CLXXV");
	TEST_L2R(176, "CLXXVI");
	TEST_L2R(177, "CLXXVII");
	TEST_L2R(178, "CLXXVIII");
	TEST_L2R(179, "CLXXIX");
	TEST_L2R(180, "CLXXX");
	TEST_L2R(181, "CLXXXI");
	TEST_L2R(182, "CLXXXII");
	TEST_L2R(183, "CLXXXIII");
	TEST_L2R(184, "CLXXXIV");
	TEST_L2R(185, "CLXXXV");
	TEST_L2R(186, "CLXXXVI");
	TEST_L2R(187, "CLXXXVII");
	TEST_L2R(188, "CLXXXVIII");
	TEST_L2R(189, "CLXXXIX");
	TEST_L2R(190, "CXC");
	TEST_L2R(191, "CXCI");
	TEST_L2R(192, "CXCII");
	TEST_L2R(193, "CXCIII");
	TEST_L2R(194, "CXCIV");
	TEST_L2R(195, "CXCV");
	TEST_L2R(196, "CXCVI");
	TEST_L2R(197, "CXCVII");
	TEST_L2R(198, "CXCVIII");
	TEST_L2R(199, "CXCIX");
	TEST_L2R(200, "CC");
	TEST_L2R(300, "CCC");
	TEST_L2R(400, "CD");
	TEST_L2R(500, "D");
	TEST_L2R(600, "DC");
	TEST_L2R(700, "DCC");
	TEST_L2R(800, "DCCC");
	TEST_L2R(900, "CM");
	TEST_L2R(1000, "M");
	TEST_L2R(1100, "MC");
	TEST_L2R(2000, "MM");

	return exit_code;
}


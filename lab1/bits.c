/* 
 * CS:APP Data Lab 
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#include "btest.h"
#include <limits.h>

/*
 * Instructions to Students:
 *
 * STEP 1: Fill in the following struct with your identifying info.
 */
team_struct team =
{
   /* Team name: Replace with either:
      Your login ID if working as a one person team
      or, ID1+ID2 where ID1 is the login ID of the first team member
      and ID2 is the login ID of the second team member */
    "518021910789", 
   /* Student name 1: Replace with the full name of first team member */
   "Liu Shuchang",
   /* Login ID 1: Replace with the login ID of first team member */
   "518021910789",
   

   /* The following should only be changed if there are two team members */
   /* Student name 2: Full name of the second team member */
   "",
   /* Login ID 2: Login ID of the second team member */
   ""
};

#if 0
/*
 * STEP 2: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.
#endif

/*
 * STEP 3: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the btest test harness to check that your solutions produce 
 *      the correct answers. Watch out for corner cases around Tmin and Tmax.
 */
/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 9.0.0.  Version 9.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, fourth edition, plus
   Amd. 1  and Amd. 2 and 273 characters from forthcoming  10646, fifth edition.
   (Amd. 2 was published 2016-05-01,
   see https://www.iso.org/obp/ui/#iso:std:iso-iec:10646:ed-4:v1:amd:2:v1:en) */
/* We do not support C11 <threads.h>.  */
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
	/*
	 * after write tc2sm (p66 in chinese book)
	 * we know that x | -x == 0 iff x == 0 
	 * if x != 0 BSM of x | -x = 1
	 */	
      	return (~((x | (~x + 1)) >> 31)) & 0x1 ;
}
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
	/*
	 * use mask1 to count  4 bits together
	 * mask2 mask3 to combine
	 */ 
	int mask1 = 0x11;
	int ans, mask2 ,mask3;
	mask1 = mask1 | (mask1 << 8);
	mask1 = mask1 | (mask1 << 16);

	ans = x & mask1;
	ans += (x >> 1) & mask1;
	ans += (x >> 2) & mask1;
	ans += (x >> 3) & mask1;

	mask2 = 0xFF;
	mask2 = mask2 | ( mask2<<8);
	ans = (ans & mask2) + (ans>>16);

	mask3 = 0xF;
	mask3 = mask3 | (mask3 << 8);
	ans = (ans & mask3) + ((ans >> 4) & mask3);
	ans = ans + (ans >> 8);
	ans = ans & 0xFF;

	return ans;

 }
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
	/*
	 * first get the bigest significant bit of x
	 * then use right shift arithmetically
	 */
	int ans = ((x<<31)>>31);
	return ans;

  
}
/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int divpwr2(int x, int n) {
	/*
	 * we can find that if x is negative, there is a bias
	 * the bias 1
	 */ 
    int bias = (~0) ^ (0x1 << 31);
    bias = bias >> (32 + ~n);
    bias = (x >> 31) & bias ;
    return (x + bias) >> n;
     
}
/* 
 * evenBits - return word with all even-numbered bits set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 2
 */
int evenBits(void) {
	/*
	 * use left shift to set 1 
	 */
	int ans = 0x55;
	ans = ans | (ans << 8);
	ans = ans | (ans << 16);	
 	 return ans;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
	/*
	 * detect if x change after x << (32 - n) then >> (32 -n)
	 */	
	int bias = 32 + (~n + 1);
	return !(x ^ ((x << bias) >> bias));
}
/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
	/*
	 * use shift and mask to get the ans
	 */
	return ( (x >> ( n << 3)) & 0xFF);
	
}
/* 
 * isGreater - if x > y  then return 1, else return 0 
 *   Example: isGreater(4,5) = 0, isGreater(5,4) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isGreater(int x, int y) {
	/*
	 * note : 1. overflow
	 *        2. tmin = -tmin
	 * first: detect overflow
	 * then: use the MSB of (x + (~(y + 1)) +1) = (x + -(y +1)to reflect the comparison
	 *       note:why use x + -(y + 1) instead of  y + -x
	 *            beacuse if x==tmin , -x = x , y + -x == y + x  cause an error
	 *            however if y==tmax , y + 1 = tmin , -(y + 1)= tmin ,is right 
	 * then: use | and overflow to take place of "if"
	 *       if no overflow return !zf
	 *       if overflow return zf
	 */
	int w = ~(y + 1) + 1;
	int wf = (w  >> 31) & 0x1;
	int xf = (x >> 31) & 0x1;
	int zf = ((x + w) >> 31 ) & 0x1;

	int overflow = ((!(wf ^ xf)) & (wf ^ zf));

	return  ((!overflow) & !zf) | ( overflow & zf);

  }
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
	/*
	 * first get the bigest significant bit 
	 * then !
	 */
  return !(x >> 31); 
}
/* 
 * isNotEqual - return 0 if x == y, and 1 otherwise 
 *   Examples: isNotEqual(5,5) = 0, isNotEqual(4,5) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNotEqual(int x, int y) {
	/*
	 * use x + -y
	 */
  return !!(x + ~y + 1 );
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 60
 *   Rating: 4
 */
int isPower2(int x) {
	/*
	 * first : if x is negative we should return 0
	 *         we can form a mask= x >> 31 
	 *         if x>=0 mask == 0 else mask = ~0
	 *         then use mask | x to turn a negative num to ~0 without influencing the positive num 
	 * then : we count the num of 1 in x
	 *         x isPower2 iff  there is only one 1 
	 * All in all we use bitCount and isNotEqual
	 */
	int mask = x >> 31;
	
	int mask1 = 0x11;
	int cnt , mask2 ;
	x = x | mask;

	mask1 =mask1 | (mask1 << 8);
	mask1 = mask1 | (mask1 << 16);

	cnt = x & mask1;
	cnt += (x >> 1) & mask1;
	cnt += (x >> 2) & mask1;
	cnt += (x >> 3) & mask1;

	cnt = cnt + (cnt >> 16);

	mask2 = 0xF;
	mask2 = mask2 | (mask2 << 8);

	cnt = (cnt & mask2) + ((cnt & mask2) >> 4);
	cnt = (cnt & 0xFF) + (cnt >> 8);
       	
	return !(cnt + ~0 );

}
/* 
 * leastBitPos - return a mask that marks the position of the
 *               least significant 1 bit. If x == 0, return 0
 *   Example: leastBitPos(96) = 0x20
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 4 
 */
int leastBitPos(int x) {
	/* 
	 *  P66 in chinese book 
	 */
	 return x & (~x + 1);
}
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 1 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
	/* 
	 * make a mask = 000011111(32-n 1);
	 */
	int mask = ~( (1 << 31) >> (n + ~0) );
  return (x >> n) & mask;
}
/*
 * satAdd - adds two numbers but when positive overflow occurs, returns
 *          maximum possible value, and when negative overflow occurs,
 *          it returns minimum positive value.
 *   Examples: satAdd(0x40000000,0x40000000) = 0x7fffffff
 *             satAdd(0x80000000,0xffffffff) = 0x80000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 30
 *   Rating: 4
 */
int satAdd(int x, int y) {
	/*
	 * first: use 
	 *        int overflow = (!(xf ^ yf)) & (xf ^ zf)
       	 * 	  to detect the overflow
	 * then: make a mask and use | to take place of "if"
	 */ 

	int xf = (x >> 31) & 0x1;
	int yf = (y >> 31) & 0x1;
        int zf = ((x + y) >> 31) & 0x1;

	int overflow = (!(xf ^ yf)) & (xf ^ zf);
	int mask = (overflow << 31) >> 31; //if overflow mask = 0b1111111... else 0b00000...
	int max = ~(1 << 31) + (!zf);// if nagative overflow max = 0b 100000... , else 0b 01111...

	return ( (~mask) & (x + y) ) | ( mask & max);
	
}
/* 
 * tc2sm - Convert from two's complement to sign-magnitude 
 *   where the MSB is the sign bit
 *   You can assume that x > TMin
 *   Example: tc2sm(-5) = 0x80000005.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int tc2sm(int x) {
	/*
	 * first : take out the sign bit using flag = x & (1 << 31)
	 * then : make a mask 
	 *        if x < 0 , mask =0b 1111... ,else mask =0000...
	 * then : if x < 0 , x = -x, else x = x
	 *        use | and mask to take place of if
	 * at last : merge the sign bit and the unsigned value
	 */

	int flag = x & (1 << 31);
	int mask = x >> 31;
	int ans = (mask & (~x + 1)) | ((~mask) & x); //if x<0, x= ~x +1;
	
	return flag | ans;
}

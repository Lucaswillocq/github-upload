/* 
 * CS:APP Data Lab 
 * 
 * <Lucas Willocq lbw19>
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

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:

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
7. Use any data type other than int.  This implies that you
cannot use arrays, structs, or unions.


You may assume that your machine:
1. Uses 2s complement, 32-bit representations of integers.
2. Performs right shifts arithmetically.
3. Has unpredictable behavior when shifting if the shift amount
is less than 0 or greater than 31.


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

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
	the coding rules are less strict.  You are allowed to use looping and
	conditional control.  You are allowed to use both ints and unsigneds.
	You can use arbitrary integer and unsigned constants. You can use any arithmetic,
	logical, or comparison operations on int or unsigned data.

	You are expressly forbidden to:
	1. Define or use any macros.
	2. Define any additional functions in this file.
	3. Call any functions.
	4. Use any form of casting.
	5. Use any data type other than int or unsigned.  This means that you
	cannot use arrays, structs, or unions.
	6. Use any floating point data types, operations, or constants.


	NOTES:
	1. Use the dlc (data lab checker) compiler (described in the handout) to 
	check the legality of your solutions.
	2. Each function has a maximum number of operations (integer, logical,
			or comparison) that you are allowed to use for your implementation
	of the function.  The max operator count is checked by dlc.
	Note that assignment ('=') is not counted; you may use as many of
	these as you want without penalty.
	3. Use the btest test harness to check your functions for correctness.
	4. Use the BDD checker to formally verify your functions
	5. The maximum number of ops for each function is given in the
	header comment for each function. If there are any inconsistencies 
	between the maximum ops in the writeup and in this file, consider
	this file the authoritative source.

	/*
	 * STEP 2: Modify the following functions according the coding rules.
	 * 
	 *   IMPORTANT. TO AVOID GRADING SURPRISES:
	 *   1. Use the dlc compiler to check that your solutions conform
	 *      to the coding rules.
	 *   2. Use the BDD checker to formally verify that your solutions produce 
	 *      the correct answers.
	 */


#endif
	//1
	/* 
	
	 * bitXor - x^y using only ~ and & 
	 *   Example: bitXor(4, 5) = 1
	 *   Legal ops: ~ &
	 *   Max ops: 14
	 *   Rating: 1
	 */
int bitXor(int x, int y) {

// 1.) in XOR operation, x != y != True
	int z = ~(x&y);
// 2.) X or Y has to be True
	int w = ~(~x & ~y);
//3.) Both of these have to be true
	return z&w;

}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {

/* To find the minimum two's complemement integer, the most significant bit of the integer
	must be 1, while the remainder of the 32-bit word is filled out with zeros.  To accomplish this,
	you shift 1 (which is represented as 31 0's with the LSB a 1) 31 spots to the left.

*/

return 1 << 31;

}
//2
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {

// In a 32-bit representation, OxAAAAAAAA is the only integer in which all odd-numbered bits are 1.
// We aren't allowed to use this constant, so we must compromise.
	int y = 0xAA + (0xAA<<8) + (0xAA<<16) + (0xAA<<24);
/* If x and y have the same bits, then (x&y) will be considered true. If it is true, using the XOR operator on 
	this result with y will result in the whole expression being false.  The "!" then reverses this outcome, 
	resulting in returning 1 (true).
*/
	return !((x&y) ^ y);
}
//3
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
	// Don't have to do this anymore
	return 2;
}
//4
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {

/* 
 For this function, if x is any integer besides 0, it should return 0, and the oppposite if x is 0.
 To accomplish this we need to check the MSB of x, as if it's 1 then it can't be zero.
 Zero also has no complement, so we'll need to check that x has a complement to confirm it's not 0.


Right shifts are assumed to be arithmetic, so shifting 31 to the right results in all 1's
(0xFFFFFFFF) IF x is anything except 0. */

	int y = x >> 31;

/* Negating x and adding 1 is essentially the same thing as finding the inverse of x.
Shift this MSB over to the end and you'll either have all 0's or 1's.
*/
	int z = (~x+1) >> 31;
/* At this stage, wedon't know which one is the negative or the positive complement, so
we implement an OR operator.  If both y and z are all 1's (both non-zero), then adding 1 will result in
returning 0, while if one of y or z isn't nonzero, it will return 1.
*/ 

	return (y|z)+1;
}
//float
/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {

	//8 exponent bits//
   int exp = (uf >> 23) & 0xFF; 

   //23 fraction bits//
   int frac = uf & 0x7FFFFF;

   //amount to shift for bias
   int real_exp = exp - 127; 

   //returns if NaN or infinity//
   if(exp == 0x7F800000 || real_exp > 30)
      return 0x80000000u;

   //rounds down to zero if exponent is zero or if no left shifts//
   if(!exp || real_exp < 0)
      return 0;

  	//normalize fraction
    frac = frac | 0x800000;

    //check size of actual exponent, make sure no extra shifting needed
    if(real_exp >= 23)
      frac = frac << (real_exp-23);
    else if(real_exp < 23)
      frac = frac >> (23-real_exp); 

    //check sign bit for negative value//
   if((uf >> 31)& 1) 
      return ~frac + 1; 

   return frac;

}

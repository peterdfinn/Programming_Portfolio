// Challenge description: https://codefights.com/challenge/FAtkuto4nPpD9JPQk/main

public class FactorialTrailingZeroes {
    /*
     * Function: Factorial
     *
     * Description: This function finds the lowest positive integer N such that
     *  N! contains exactly P trailing zeroes.
     *
     * Inputs:
     *  P - The number of trailing zeroes requested.
     *
     * Return value:
     *  -1 if no such N exists.
     *  Otherwise, returns N.
     */
    public static int Factorial(int P) {
	/*
	 * When considering the value N! as N increases, we may consider only
	 * the values of N divisible by 5: in order for a new trailing zero to
	 * be added to the factorial, a new 5 must be added to the prime
	 * factorization, so that it may be multiplied with an extant 2 to
	 * produce 10 and create a new trailing zero.
	 */
	int N, counter;
	for (N = 5, counter = 0; counter < P; N += 5) {
	    if (N % 25 != 0) {
		++counter;
		continue;
	    }
	    for (int i = N; i % 5 == 0; i /= 5) {
		++counter;
	    }
	}
	if (counter == P) return N - 5;
	else return -1;
    }
}

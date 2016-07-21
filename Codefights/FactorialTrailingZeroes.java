public class FactorialTrailingZeroes {
    public static int Factorial(int P) {
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

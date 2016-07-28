// Challenge description: https://codefights.com/challenge/Q6Ai4nxpRjFx5sYWj/main

/*
 * Function: binaryPalindrome
 *
 * Description: This function finds the nth palindromic b-bit binary number.
 *
 * Inputs:
 *  b - The length, in bits, of the bit string.
 *  n - The index of the palindrome to find.
 *
 * Return value: The nth number that is a palindrome when expressed as a
 *  b-bit binary number.
 */
int binaryPalindrome(int b, int n) {
  int leftHalf = (n - 1) << (b / 2);
  int rightHalf = 0;
  for (int i = b - 1; i >= b - (b / 2); --i) {
    int ithBit = (leftHalf >> i) & 1;
    rightHalf += ithBit << ((b - 1) - i);
  }
  return leftHalf | rightHalf;
}


// Challenge description: https://codefights.com/challenge/ifSxNwEhkPER6hExe/main

using namespace std;

#include <string>

string stringDivision(string s) {

  /*
   * The challenge in this task is that the input string may be up to 35
   * characters long, which might mean as many as 70 digits, well beyond
   * the limits of even an 8-byte number.  I decided to get around this
   * problem by storing the number to calculate as an array of bytes
   * rather than as a single variable.
   */
  char number[70];
  for (int i = 0; i < 70; ++i) number[i] = -1;

  /*
   * First, I store each character in s as a single- or double-digit
   * number in my array of bytes, with the least significant bytes
   * coming first.
   */ 
  for (int i = s.size() - 1, j = 0; i >= 0; --i) {
    char thisChar = s[i] - 96;
    if (thisChar < 10) {
      number[j++] = thisChar;
    }
    else if (thisChar < (char) 20) {
      number[j++] = thisChar - 10;
      number[j++] = 1;
    }
    else {
      number[j++] = thisChar - 20;
      number[j++] = 2;
    }
  }

  /*
   * Then, I divide this number by 2.
   */
    
  if (number[0] % 2 != 0) {
    /* For the least significant digit, I make it even if it isn't
       already.  This has the effect of rounding the final product down
       to the nearest integer. */
    number[0]--;
  }
  for (int j = 0; (j < 70) && (number[j] != -1); ++j) {
    if (number[j] % 2 != 0) number[j - 1] += 5;
    number[j] /= 2;
  }
    
  /*
   * Then, I count the number of digits this new number has.
   */
  int digits = 1;
  for (int j = 69; j > 0; --j) {
    if (number[j] == 0) number[j] = -1;
    if (number[j] > 0) {
      digits = j + 1;
      break;
    }
  }

  /*
   * Finally, I convert each digit to a character and return the requested
   * string.
   */
  char answer[digits + 1];
  answer[digits] = 0;
  for (int j = 0; j < digits; ++j) {
    answer[digits - 1 - j] = number[j] + 97;
  }
    
  string retVal(answer);
  return retVal;
}


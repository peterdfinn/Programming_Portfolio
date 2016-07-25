public class StringDivision {

    String stringDivision(String s) {

	byte[] number = new byte[70];
	Arrays.fill(number, (byte) -1);
    
	for (int i = s.length() - 1, j = 0; i >= 0; --i) {
        
	    byte thisChar = (byte) (s.charAt(i) - 96);
	    if (thisChar  < 10) {
		number[j++] = thisChar;
	    }
	    else if (thisChar < 20) {
		number[j++] = (byte) (thisChar - 10);
		number[j++] = 1;
	    }
	    else {
		number[j++] = (byte) (thisChar - 20);
		number[j++] = 2;
	    }
	}
    
	if (number[0] % 2 != 0) {
	    number[0]--;
	}
	for (int j = 0; (j < 70) && (number[j] != -1); ++j) {
	    if (number[j] % 2 != 0) number[j - 1] += 5;
	    number[j] /= 2;
	}
    
	int digits = 1;
	for (int j = 69; j > 0; --j) {
	    if (number[j] == 0) number[j] = -1;
	    if (number[j] > 0) {
		digits = j + 1;
		break;
	    }
	}
    
	byte[] answer = new byte[digits];
	for (int j = 0; j < digits; ++j) {
	    answer[digits - 1 - j] = (byte) (number[j] + 97);
	}
    
	return new String(answer);
    
    }

}

public class ReduceDirections {

    /*
     * Function: reduceDirections
     *
     * Description: This function, when given a list of directions that are
     *  either "NORTH", "SOUTH", "EAST", or "WEST", will reduce the list to
     *  remove adjacent pairs that cancel each other out (north/south and
     *  east/west).  Note that this is NOT the same as finding the net
     *  direction of the desination: it only removes a pair of directions
     *  when they are adjacent to one another in the list.
     *
     * Inputs:
     *  directions - An array of strings containing the cardinal directions to
     *   follow.
     *
     * Return value: An array of strings, simplified to remove all superfluous
     *  pairs from the input.
     */
    public static String[] reduceDirections(String[] directions) {

	// Variable initialization
	boolean[] bools = new boolean[directions.length];
	Arrays.fill(bools, true);
	int sizeOfAnswer = directions.length;
	boolean simplifiedThisRound = true;

	/*
	 * Repeatedly scan "directions" until a scan eliminates no pairs.
	 */
	while (simplifiedThisRound) {
	    
	    simplifiedThisRound = false;
	    int thisIndex = getNextIndex(bools, -1);

	    /*
	     * Scan "directions", skipping over already deleted entries, looking
	     * for a now-adjacent pair of directions to delete.
	     */
	    while (true) {
		int next = getNextIndex(bools, thisIndex);
		if (next == -1) break;
		if (directions[thisIndex].equals(opposite(directions[next]))) {
		    bools[thisIndex] = false;
		    bools[next] = false;
		    simplifiedThisRound = true;
		    sizeOfAnswer -= 2;
		}
		thisIndex = getNextIndex(bools, thisIndex);
		if (thisIndex == -1) break;
	    }
	}

	/*
	 * Scan "bools", placing directions[i] into "answer" only if bools[i].
	 */
	String[] answer = new String[sizeOfAnswer];
	for (int boolIdx = 0, answerIdx = 0;
	     answerIdx < sizeOfAnswer; ++boolIdx) {
	    if (bools[boolIdx]) {
		answer[answerIdx++] = directions[boolIdx];
	    }
	}
    
	return answer;
    }

    /*
     * Function: getNextIndex
     *
     * Description: This function attempts to find the next entry in an array of
     *  booleans that is true.
     *
     * Inputs:
     *  bools - The array in which to search.
     *  index - The index into the array from which to start searching.
     *
     * Return value:
     *  -1 if there is no element in the array after "index" that is true.
     *  Otherwise, returns the index of the next true element of the array.
     */
    private static int getNextIndex(boolean[] bools, int index) {
    
	for (int i = index + 1; i < bools.length; ++i) {
	    if (bools[i]) return i;
	}
    
	return -1;
    }

    /*
     * Function: opposite
     *
     * Description: This private helper function returns a cardinal direction's
     *  opposite.
     */
    private static String opposite(String direction) {
	switch(direction) {
        case "NORTH":
            return "SOUTH";
        case "SOUTH":
            return "NORTH";
        case "EAST":
            return "WEST";
        case "WEST":
            return "EAST";
        default:
            return null;
	}
    }


}

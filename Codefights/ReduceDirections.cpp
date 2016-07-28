// Challenge description: https://codefights.com/challenge/zALN8MBi5vKKBBb6x/main

#include <string>
#include <vector>

using namespace std;

vector<string> ReduceDirections(vector<string> directions);
int getNextIndex(vector<int> bools, int index);
string opposite(string direction);



/*
 * Function: ReduceDirections
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
vector<string> ReduceDirections(vector<string> directions) {

  // Variable initialization
  vector<int> bools(directions.size());
  for (int i = 0; i < directions.size(); ++i) bools[i] = 1;
  int sizeOfAnswer = directions.size();
  int simplifiedThisRound = 1;

  /*
   * Repeatedly scan "directions" until a scan eliminates no pairs.
   */    
  while (simplifiedThisRound) {
        
    simplifiedThisRound = 0;
    int thisIndex = getNextIndex(bools, -1);

    /*
     * Scan "directions", skipping over already deleted entries, looking
     * for a now-adjacent pair of directions to delete.
     */
    while (1) {
      int next = getNextIndex(bools, thisIndex);
      if (next == -1) break;
      if (directions[thisIndex].compare(opposite(directions[next])) == 0) {
	bools[thisIndex] = 0;
	bools[next] = 0;
	simplifiedThisRound = 1;
	sizeOfAnswer -= 2;
      }
      thisIndex = getNextIndex(bools, thisIndex);
      if (thisIndex == -1) break;
    }
  }

  /*
   * Scan "bools", placing directions[i] into "answer" only if bools[i].
   */
  vector<string> answer(sizeOfAnswer);
  for (int boolIdx = 0, answerIdx = 0; answerIdx < sizeOfAnswer; ++boolIdx) {
    if (bools[boolIdx]) {
      answer[answerIdx++] = directions[boolIdx];
    }
  }
    
  return answer;
}


/*
 * Function: getNextIndex
 *
 * Description: This function attempts to find the next entry in a vector of
 *  booleans that is true.
 *
 * Inputs:
 *  bools - The vector in which to search.
 *  index - The index into the vector from which to start searching.
 *
 * Return value:
 *  -1 if there is no element in the vector after "index" that is true.
 *  Otherwise, returns the index of the next true element of the vector.
 */
int getNextIndex(vector<int> bools, int index) {
  for (int i = index + 1; i < bools.size(); ++i) {
    if (bools[i]) return i;
  }
    
  return -1;
}


/*
 * Function: opposite
 *
 * Description: This helper function returns a cardinal direction's
 *  opposite.
 */
string opposite(string direction) {
        
  if (!direction.compare("NORTH")) {
    string ret("SOUTH");
    return ret;
  }
        
  if (!direction.compare("SOUTH")) {
    string ret("NORTH");
    return ret;
  }
        
  if (!direction.compare("WEST")) {
    string ret("EAST");
    return ret;
  }
        
  if (!direction.compare("EAST")) {
    string ret("WEST");
    return ret;
  }
        
  return NULL;
}

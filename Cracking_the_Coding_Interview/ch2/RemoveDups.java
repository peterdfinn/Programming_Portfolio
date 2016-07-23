import java.util.*;

public class RemoveDups {

    /* NOTE: This main function contains my test for removeDups.  More formal
       testing can be seen in all Cracking the Coding Interview exercises after
       this one: I am well aware this does not constitute rigorous testing! */
    public static void main(String[] args) {
	LinkedList<Integer> testList1 = new LinkedList<Integer>();
	testList1.add(4);
	testList1.add(4);
	testList1.add(14);
	testList1.add(43);
	testList1.add(14);
	testList1.add(4);

	ListIterator<Integer> it = testList1.listIterator(0);
	while (it.hasNext()) System.out.print(it.next() + "\t");
	System.out.println("\n");

	removeDups(testList1);

	it = testList1.listIterator(0);
	while (it.hasNext()) System.out.print(it.next() + "\t");
	System.out.println("\n");
    }

    /*
     * Function: removeDups
     *
     * Description: This function removes duplicate entries from the input
     *  linked list.
     *
     * Inputs:
     *  input - The linked list from which to remove duplicates.
     */
    public static <T> void removeDups(LinkedList<T> input) {
	
	TreeSet<T> foundInInput = new TreeSet<T>(); // create temporary buffer

	/* Iterate through input, attempting to add each element into the
	   temporary buffer.  If that element is already in the buffer, it is
	   removed from input. */
	ListIterator<T> inputIterator = input.listIterator(0);
	while (inputIterator.hasNext()) {
	    if (!foundInInput.add(inputIterator.next())) {
		inputIterator.remove();
	    }
	}
    }

    /*
     * Function: removeDupsNoBuffer
     *
     * Description: This function removes duplicate entries from the input
     *  linked list.  It is functionally identical to removeDups except for the
     *  added constraint that no temporary buffers may be used.
     *
     * Inputs:
     *  input: The linked list from which to remove duplicates.
     */
    public static <T> void removeDupsNoBuffer(LinkedList<T> input) {

	int size = input.size();
	if (size < 2) return;

	T original;
	ListIterator<T> compare;

	for (int i = 0; i < size - 1; ++i) { // For each element of input...
	    original = input.get(i);
	    compare = input.listIterator(i + 1);

	    // ...compare it to all elements after it, deleting duplicates as
	    // they are found.
	    while (compare.hasNext()) {
		if (original.equals(compare.next())) {
		    compare.remove();
		    --size;
		}
	    }
	}
    }

}

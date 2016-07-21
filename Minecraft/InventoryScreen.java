/*
 * I love Minecraft, and when I discovered that Minecraft had been written in
 * Java, I decided to see if I could write my own skeleton code for it, for fun
 * and programming practice.  This sample contains the class InventoryScreen,
 * which obviously is meant to replicate a player's inventory screen.  It's not
 * complete, but I did write most of the important functions that would be
 * needed.
 */


public class InventoryScreen {
	
	private class InventoryCursor {
		/*
		 * This inner class represents the cursor used by a player to navigate
		 * his/her inventory screen.
		 */
		
		private char currentlySelected;
		/*
		 * If 1-9, refers to slots 1-9 in the Quick Select.
		 * If 10-36, refers to slots 1-27 in the Inventory Proper.
		 * If 37, refers to the helmet slot.
		 * If 38, refers to the chestplate slot.
		 * If 39, refers to the leggings slot.
		 * If 40, refers to the boots slot.
		 * Otherwise, undefined.
		 */
		
		private Item heldItem;
		/*
		 * If null, signifies that no item is currently held.
		 */

		private InventoryCursor() {
			currentlySelected = (char) 1;
			heldItem = null;
		}

		public char getCurrentlySelected() {return currentlySelected;}
		public void setCurrentlySelected(char newSlot) {
			if ((newSlot >= 1) && (newSlot <= 40)) {
				currentlySelected = newSlot;
			}
		}
		
		/*
		 * Function: currentlySelectedItem
		 * 
		 * Description: This function returns a pointer to the item in the slot
		 *  pointed to by the Inventory Cursor, or null if the slot is empty.
		 */
		public Item currentlySelectedItem() {
			int current = (int) currentlySelected;
			if ((current >= 1) && (current <= 9)) {
				return qs[current - 1];
			}
			if ((current >= 10) && (current <= 36)) {
				return ip[current - 10];
			}
			if (current == 37)
				return hs;
			if (current == 38)
				return cs;
			if (current == 39)
				return ls;
			if (current == 40)
				return bs;
			return null;
		}

		/*
		 * Function: grabPlaceItem
		 * 
		 * Description: This function swaps two Item objects, one of which is
		 *  held by the Inventory Cursor, and the other of which is in the slot
		 *  pointed to by the Inventory Cursor.x
		 */
		public void grabPlaceItem() {
			Item temp = heldItem;
			if ((currentlySelected >= (char) 1) && (currentlySelected <= (char) 9)) {
				heldItem = qs[(int) currentlySelected - 1];
				qs[(int) currentlySelected - 1] = temp;
			}
			if ((currentlySelected >= (char) 10) && (currentlySelected <= (char) 36)) {
				heldItem = ip[(int) currentlySelected - 10];
				ip[(int) currentlySelected - 10] = temp;
			}
			if (currentlySelected == (char) 37) {
				heldItem = hs;
				hs = (Armor) temp;
			}
			if (currentlySelected == (char) 38) {
				heldItem = cs;
				cs = (Armor) temp;
			}
			if (currentlySelected == (char) 39) {
				heldItem = ls;
				ls = (Armor) temp;
			}
			if (currentlySelected == (char) 40) {
				heldItem = bs;
				bs = (Armor) temp;
			}
		}
		
		public Item getHeldItem() {return heldItem;}
		
	} // end of class InventoryCursor
	
	public final static int ivSlots = 27; // the number of slots in the 
										  // Inventory Proper
	public final static int qsSlots = 9; // the number of slots in the Quick
										 // Select
	private Item[] ip; // Inventory Proper
	private Item[] qs; // Quick Select
	private Armor hs, cs, ls, bs; // Armor Slots (helmet, chestplate, leggings, boots)
	private InventoryCursor cursor;
	
	public InventoryScreen() {
		ip = new Item[ivSlots];
		qs = new Item[qsSlots];
		cursor = new InventoryCursor();
	}
	
	/*
	 * Function: quickMove
	 * 
	 * Description: This function provides the Quick Move functionality.
	 * 
	 *  If the currently selected item is in the Armor Slots, it is moved to the
	 *  Quick Select, or the Inventory Proper if there was no room in the Quick
	 *  Select, or not moved at all if there was no room in either.
	 *  
	 *  If the currently selected item is in the Inventory Proper, it is moved
	 *  to the appropriate Armor Slot if there is room there and the item is
	 *  armor; otherwise, it is moved to the Quick Select if there is room, or
	 *  not moved at all if there is no room.
	 *  
	 *  If the currently selected item is in the Quick Select, it is moved to
	 *  the appropriate Armor Slot if there is room there and it is armor;
	 *  otherwise, it is moved to the Inventory Proper if there is room there,
	 *  or not moved at all if there is no room.
	 */
	public void quickMove() {
		quickMove(cursor.currentlySelectedItem());
	}
	
	private void quickMove(Item item) {
		if (item == null) return;
		
		int location = (int) item.getLocation();
		if ((location >= 1) && (location <= 9)) { // item is in Quick Select
			if (item instanceof Armor) {
				if (item instanceof Helmet) {
					if (hs == null) {
						hs = (Armor) item;
						qs[location - 1] = null;
						return;
					}
				}
				else if (item instanceof Leggings) {
					if (ls == null) {
						ls = (Armor) item;
						qs[location - 1] = null;
						return;
					}
				}
				else if (item instanceof Chestplate) {
					if (cs == null) {
						cs = (Armor) item;
						qs[location - 1] = null;
						return;
					}
				}
				else if (item instanceof Boots) {
					if (bs == null) {
						bs = (Armor) item;
						qs[location - 1] = null;
						return;
					}
				}
				else return;
			}
			
			int newIndex = 0;
			while ((newIndex >= 0) && (!Item.isEmpty(item))) {
				newIndex = moveItemTo(ip, item);
			}
			return;
		}
		else if ((location >= 37) && (location <= 40)) { // item is in Armor Slots
			int newIndex = 0;
			while ((newIndex >= 0) && (!Item.isEmpty(item))) {
				newIndex = moveItemTo(qs, item);
			}
			if (!Item.isEmpty(item)) {
				while ((newIndex >= 0) && (!Item.isEmpty(item))) {
					newIndex = moveItemTo(ip, item);
				}
			}
			return;
		}
		else if ((location >= 10) && (location <= 36)) { // item is in Inventory Proper
			if (item instanceof Armor) {
				if (item instanceof Helmet) {
					if (hs == null) {
						hs = (Armor) item;
						ip[location - 10] = null;
						return;
					}
				}
				else if (item instanceof Leggings) {
					if (ls == null) {
						ls = (Armor) item;
						ip[location - 10] = null;
						return;
					}
				}
				else if (item instanceof Chestplate) {
					if (cs == null) {
						cs = (Armor) item;
						ip[location - 10] = null;
						return;
					}
				}
				else if (item instanceof Boots) {
					if (bs == null) {
						bs = (Armor) item;
						ip[location - 10] = null;
						return;
					}
				}
				else return;
			}
			
			int newIndex = 0;
			while ((newIndex >= 0) && (!Item.isEmpty(item))) {
				newIndex = moveItemTo(qs, item);
			}
			return;
		}
	}
	
	/*
	 * Function: moveItemTo
	 * 
	 * Description: This is a private helper function written to be called from
	 *  within quickMove.  It attempts to find the best index into an array at
	 *  which to store an item.  The best index is defined as the first slot in
	 *  which there is a non-full stack of the same item, or the first empty
	 *  slot if no such slot exists.
	 *  
	 * Inputs:
	 *  array - The array into which to search for the best index.
	 *  item - The Item to be placed into the array.
	 *  
	 * Return value:
	 *  The index into which the item is placed, or -1 if no viable slot was
	 *  found.
	 */
	private int moveItemTo(Item array[], Item item) {
		int retVal = -1;
		for (int i = 0; i < array.length; ++i) {
			if (Item.isEmpty(array[i])) {
				if (retVal < 0) retVal = i;
				else continue;
			}
			else if (array[i].getName().compareTo(item.getName()) == 0) {
				if (array[i].getStackNumber() < array[i].getStackLimit()) {
					int spareRoomInSlot = array[i].getStackLimit() - array[i].getStackNumber();
					int amountToMove = Math.min(spareRoomInSlot, item.getStackNumber());
					item.addToStack(amountToMove * -1);
					array[i].addToStack(amountToMove);
					return i;
				}
			}
		}
		if (retVal < 0) return retVal;
		array[retVal] = item;
		int itemLocation = (int) item.getLocation();
		if ((itemLocation >= 1) && (itemLocation <= 9)) { // item is in Quick Select
			qs[itemLocation - 1] = null;
		}
		else if ((itemLocation >= 10) && (itemLocation <= 36)) { // item is in Inventory Proper
			ip[itemLocation - 10] = null;
		}
		else if (itemLocation == 37) {
			hs = null;
		}
		else if (itemLocation == 38) {
			cs = null;
		}
		else if (itemLocation == 39) {
			ls = null;
		}
		else if (itemLocation == 40) {
			bs = null;
		}
		else return -1;
		
		return retVal;
	}
	
	/*
	 * Function: up
	 * 
	 * Description: This function changes the position of the cursor, as if the
	 *  player pressed 'up' on the controller.
	 */
	public void up() {
		int newSlot;
		int current = (int) cursor.getCurrentlySelected();
		if ((current >= 1) && (current <= 9)) {
			newSlot = current + 27;
		}
		else if ((current >= 10) && (current <= 18)) {
			newSlot = 40;
		}
		else if ((current >= 19) && (current <= 36)) {
			newSlot = current - 9;
		}
		else if (current == 37) {
			newSlot = 1;
		}
		else if ((current >= 38) && (current <= 40)) {
			newSlot = current - 1;
		}
		else return;
		
		cursor.setCurrentlySelected((char) newSlot);
	}
	
	/*
	 * Function: down
	 * 
	 * Description: This function changes the position of the cursor, as if the
	 *  player pressed 'down' on the controller.
	 */
	public void down() {
		int newSlot;
		int current = (int) cursor.getCurrentlySelected();
		if ((current >= 1) && (current <= 9)) {
			newSlot = 37;
		}
		else if ((current >= 10) && (current <= 27)) {
			newSlot = current + 9;
		}
		else if ((current >= 28) && (current <= 36)) {
			newSlot = current - 27;
		}
		else if ((current >= 37) && (current <= 39)) {
			newSlot = current + 1;
		}
		else if (current == 40) {
			newSlot = 10;
		}
		else return;
		
		cursor.setCurrentlySelected((char) newSlot);
	}
	
	/*
	 * Function: left
	 * 
	 * Description: This function changes the position of the cursor, as if the
	 *  player pressed 'left' on the controller.
	 */
	public void left() {
		int newSlot;
		int current = (int) cursor.getCurrentlySelected();
		if ((current >= 37) && (current <= 40)) {
			newSlot = current;
		}
		else if ((current >= 1) && (current <= 36)) {
			if (((current - 1) % 9) == 0) {
				newSlot = current + 8;
			}
			else {
				newSlot = current - 1;
			}
		}
		else return;
		
		cursor.setCurrentlySelected((char) newSlot);
	}
	
	/*
	 * Function: right
	 * 
	 * Description: This function changes the position of the cursor, as if the
	 *  player pressed 'right' on the controller.
	 */
	public void right() {
		int newSlot;
		int current = (int) cursor.getCurrentlySelected();
		if ((current >= 37) && (current <= 40)) {
			newSlot = current;
		}
		else if ((current >= 1) && (current <= 36)) {
			if ((current % 9) == 0) {
				newSlot = current - 8;
			}
			else {
				newSlot = current + 1;
			}
		}
		else return;
		
		cursor.setCurrentlySelected((char) newSlot);
	}
	
	/* 
	 * Function: whatsThis
	 * 
	 * Description: This function displays the info for whatever item the
	 *  cursor is currently pointed to, if it's pointed to any item.
	 */
	public void whatsThis() {
		String info = null;
		Item item = cursor.getHeldItem();
		if (item != null) {
			info = item.getInfo();
		}
		else {
			item = cursor.currentlySelectedItem();
			if (item != null)
				info = item.getInfo();
		}
		if (info != null) {
			// Display String info if non-null.
		}
	}
	
}

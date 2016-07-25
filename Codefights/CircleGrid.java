// Challenge description: https://codefights.com/challenge/g7QePyA3sLwwgyzRM/main

ublic class CircleGrid {

    /*
     * Function: circleGrid
     * 
     * Description: This function counts the number of points with integer
     *  coordinates that lie strictly within a circle.
     * 
     * Inputs:
     *  X - The x coordinate of the circle's center.
     *  Y - The y coordinate of the circle's center.
     *  R - The radius of the circle.
     * 
     * Return value: The number of points that lie within the circle.
     */
    public static int circleGrid(double X, double Y, double R) {
    
	/* This can be done by imagining a square that contains the circle,
	 * centered at the circle's center, with side length 2R.  Then, we
	 * can test each point within that square to see if the distance from
	 * the circle's center is less than R.  If so, then the point is within
	 * the circle.  We can return the number of such points.
	 */
    
	double xStart = Math.ceil(X - R);
	double xFinish = Math.floor(X + R);
	double yStart = Math.ceil(Y - R);
	double yFinish = Math.floor(Y + R);
    
	int returnValue = 0;
    
	for (double x = xStart; x <= xFinish; x += 1.0) {
	    for (double y = yStart; y <= yFinish; y += 1.0) {
		if (euclideanDist(x, y, X, Y) < R) ++returnValue;
	    }
	}
    
	return returnValue;
    }

    /*
     * Function: euclideanDist
     * 
     * Description: This function calculates the Euclidean distance between two
     * points on a 2-dimensional plane.
     * 
     * Inputs:
     *  x1 - The x coordinate of the first point.
     *  y1 - The y coordinate of the first point.
     *  x2 - The x coordinate of the second point.
     *  y2 - The y coordinate of the secondp point.
     * 
     * Return value: The distance between (x1, y1) and (x2, y2).
     */
    public static double euclideanDist(double x1, double y1, double x2, double y2) {
	double xDist = x1 - x2;
	double yDist = y1 - y2;
	return Math.sqrt(xDist * xDist + yDist * yDist);
    }
}

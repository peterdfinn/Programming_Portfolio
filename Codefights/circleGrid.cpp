#include <cmath>

int circleGrid(double X, double Y, double R);
double euclideanDist(double x1, double y1, double x2, double y2);

int circleGrid(double X, double Y, double R) {
  double xStart = ceil(X - R);
  double xFinish = floor(X + R);
  double yStart = ceil(Y - R);
  double yFinish = floor(Y + R);
    
  int returnValue = 0;
    
  for (double x = xStart; x <= xFinish; x += 1.0) {
    for (double y = yStart; y <= yFinish; y += 1.0) {
      if (euclideanDist(x, y, X, Y) < R) ++returnValue;
    }
  }
    
  return returnValue;
}

double euclideanDist(double x1, double y1, double x2, double y2) {
  double xDist = x1 - x2;
  double yDist = y1 - y2;
  return sqrt(xDist * xDist + yDist * yDist);
}


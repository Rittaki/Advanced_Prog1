#include "minCircle.h"
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

/**
 * @brief calculate euclidian distance between two points
 * @param a first point
 * @param b second point
 * @return distance
 */
float dist(Point &a, Point &b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
/**
 * @brief twoPointCircle calculates the minimal circle that contains 2 points
 * @param a first point
 * @param b second point
 * @return Circle minimal circle
 */
Circle twoPointCircle(Point &a, Point &b) {
	Point c = Point((a.x + b.x)/2.0, (a.y + b.y)/2.0);
	float radius = dist(a, b) / 2.0;
	return Circle(c, radius);
}
/**
 * @brief threePointCircle calculates the minimal circle that contains 3 points
 * @param a first point
 * @param b second point
 * @param c third point
 * @return Circle minimal circle
 */
Circle threePointCircle(Point &a, Point &b, Point &c) {
	float bx = b.x - a.x;
	float by = b.y - a.y;
	float cx = c.x - a.x;
	float cy = c.y - a.y;
	float be = bx * bx + by * by;
	float ce = cx * cx + cy * cy;
	float de = bx * cy - by * cx;
	Point i = Point((cy * be - by * ce)/(2 * de),
					(bx * ce - cx * be)/(2 * de));
	i.x = i.x + a.x;
	i.y = i.y + a.y;
	return Circle(i, dist(i, a));
}
/**
 * @brief pointIncircle checks if the given point is in the circle
 * @param p point
 * @param c circle
 * @return bool true/false
 */
bool pointInCircle(Point &p, Circle &c) {
	return dist(c.center, p) <= c.radius;
}
/**
 * @brief Welzl function uses Welzl's algorithm to find MEC (minimal
 * enclosing circle) of points, taking one random point and removing it, 
 * and recursively reaches minimal circle
 * four base cases of 0, 1, 2 or 3 points
 * @param vecPoints vector of given points
 * @param boundaries vector of points on boundaries
 * @param size number of points
 * @return Circle minimal enclosing circle 
 */
Circle Welzl(vector<Point> &vecPoints, vector<Point> boundaries, size_t size) {
	if (size == 0 || boundaries.size() == 3)
	{
		switch (boundaries.size())
		{
		case 0:
			return Circle(Point(0,0),0);
			break;
		case 1:
			return Circle(Point(boundaries[0].x, boundaries[0].y), 0);
			break;
		case 2:
			return twoPointCircle(boundaries[0], boundaries[1]);
			break;
		case 3:
			return threePointCircle(boundaries[0], boundaries[1], boundaries[2]);
			break;
		default:
			break;
		}
	}
	int index = rand() % size;
	Point randomPoint = vecPoints[index];
	swap(vecPoints[index], vecPoints[size - 1]);

	Circle mec = Welzl(vecPoints, boundaries, size - 1);
	if (pointInCircle(randomPoint, mec))
	{
		return mec;
	}
	
	boundaries.push_back(randomPoint);
	return Welzl(vecPoints, boundaries, size - 1);
}

Circle findMinCircle(Point** points,size_t size){
	vector<Point> vecPoints;
	for (int i = 0; i < size; i++)
	{
		vecPoints.push_back(*points[i]);
	}
	random_shuffle(vecPoints.begin(), vecPoints.end());
	return Welzl(vecPoints, {}, vecPoints.size());
}
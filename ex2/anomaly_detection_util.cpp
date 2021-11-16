/*
 * anomaly_detection_util.cpp
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */
#include <math.h>
#include <cstdlib>
#include "anomaly_detection_util.h"

float avg(float* x, int size){
	float sum=0;
	for(int i=0;i<size;sum+=x[i],i++);
	return sum/size;
}

// returns the variance of X and Y
float var(float* x, int size){
	float av=avg(x,size);
	float sum=0;
	for(int i=0;i<size;i++){
		sum+=x[i]*x[i];
	}
	return sum/size - av*av;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
	float sum=0;
	for(int i=0;i<size;i++){
		sum+=x[i]*y[i];
	}
	sum/=size;

	return sum - avg(x,size)*avg(y,size);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	return cov(x,y,size)/(sqrt(var(x,size))*sqrt(var(y,size)));
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
	// float x[size],y[size];

	// CHANGED
	float* x = new float[size];
	float* y = new float[size];
	for(int i=0;i<size;i++){
		x[i]=points[i]->x;
		y[i]=points[i]->y;
	}
	float a=cov(x,y,size)/var(x,size);
	float b=avg(y,size) - a*(avg(x,size));

	// CHANGED
	delete x;
	delete y;

	return Line(a,b);
}

Line linear_reg(float* x, float* y, int size){
    float* pointsX = new float[size];
	float* pointsY = new float[size];
    for (int i = 0; i < size; i++) {
        pointsX[i] = x[i];
        pointsY[i] = y[i];
    }
    float a = cov(pointsX, pointsY, size) / var(pointsX, size);
    float b = avg(pointsY, size) - a * (avg(pointsX, size));
	
	delete pointsX;
	delete pointsY;

    return Line(a,b);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	Line l=linear_reg(points,size);
	return dev(p,l);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	// CHANGED
	// should be fabs() windows implementaion of abs only accepts int, double
	return fabs(p.y-l.f(p.x));
}

float max_dev(float* x, float* y, int size, Line l) {
	float max = 0;
	for (int i = 0; i < size; i++)
	{
		Point p(x[i], y[i]);
		float devi = dev(p, l);
		if (max < devi)
		{
			max = devi;
		}	
	}
	return max;
}


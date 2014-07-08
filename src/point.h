#ifndef GUARD_POINTS_H
#define GUARD_POINTS_H

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Point{
public:
	Point 				*mLast;
	vector <double> 	mPos;
	vector <double> 	mVel; 	// current velocity
	vector <double> 	mVel1;	// stores velocity last time step
	vector <double>  	mVel2;  // stores velocity 2 time steps ago
	vector <double>		mVelNL;
	vector <double>  	mSPrime;
	vector <double>  	mS2Prime;
	vector <double> 	mSegLength;
	Point(){};
	~Point(){};
};

#endif
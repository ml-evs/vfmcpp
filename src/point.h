#ifndef GUARD_POINTS_H
#define GUARD_POINTS_H

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Point{
public:
	/*member data*/
	Point 			*mPrev;			// pointer to previous point in filament
	Point 			*mNext;			// pointer to next point in filament
	vector <double>	mPos;			// position
	vector <double>	mVel; 			// current velocity
	vector <double> mVel1;			// velocity last time step
	vector <double> mVel2;  		// velocity 2 time steps ago
	vector <double>	mVelNL; 		// non-local contributions to velocity
	vector <double> mSPrime;		// tangent at point
	vector <double> mS2Prime;		// binormal at point
	double			mSegLength; 	// distance to last point
	double 			mCharge; 		// charge at point 
	/*member functions*/
	Point(){
		/*default constructor just reserves memory*/
		mVel.resize(3); mPos.resize(3);
		mVel1.resize(3); mVel2.resize(3);  mVelNL.resize(3);
		mSPrime.resize(3); mS2Prime.resize(3);
		mCharge = 0; mSegLength = 0;
	}
	~Point(){};
};

#endif
#ifndef GUARD_POINTS_H
#define GUARD_POINTS_H

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Point{
public:
	/*member data*/
	Point 			*mLast;			// pointer to last point in filament
	vector <double>	mPos;			// position
	vector <double>	mVel; 			// current velocity
	vector <double> mVel1;			// velocity last time step
	vector <double> mVel2;  		// velocity 2 time steps ago
	vector <double>	mVelNL; 		// non-local contributions to velocity
	vector <double> mSPrime;		// tangent at point
	vector <double> mS2Prime;		// binormal at point
	double			mSegLength; 	// distance to last point
	/*member functions*/
	Point(){
		/*default constructor just reserves memory*/
		mVel.reserve(3); mPos.reserve(3);
		mVel1.reserve(3); mVel2.reserve(3);  mVelNL.reserve(3);
		mSPrime.reserve(3); mS2Prime.reserve(3);
	}
	~Point(){};
};

#endif
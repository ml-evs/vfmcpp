#ifndef GUARD_POINTS_H
#define GUARD_POINTS_H

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Point{
public:
	/* member data */
	Point 			*mPrev;				// pointer to previous point in filament
	Point 			*mNext;				// pointer to next point in filament
	vector <double>	mPos;				// position
	vector <double>	mVel; 				// current velocity
	vector <double> mVel1;				// velocity last time step
	vector <double> mVel2;  			// velocity 2 time steps ago
	vector <double> mVel3;  			// velocity 3 time steps ago
	vector <double>	mVelNL; 			// non-local contributions to velocity
	vector <double> mSPrime;			// tangent at point
	vector <double> mS2Prime;			// binormal at point
	vector <double> mSegLast; 			// vector to last point
	double			mSegLength; 		// distance to last point
	double 			mCharge; 			// charge at point 
	int 			mFlagFilled;		// flag showing how many velocity steps back are present
	bool 			mMarkedForDeletion; // flag telling reconnect whether this point needs to be removed
	/* member functions */
	Point(){
		/* default constructor just reserves memory */
		mVel.resize(3); mPos.resize(3);
		mVel1.resize(3); mVel2.resize(3); mVel3.resize(3); mVelNL.resize(3);
		mSPrime.resize(3); mS2Prime.resize(3); mSegLast.resize(3);
		mCharge = 0; mSegLength = 0; mFlagFilled = 0; mMarkedForDeletion = false;
	}
	Point(Point* occ){
		/* parameterised constructor copies a point, used in reconnection */
		mVel = occ->mVel; mVel1 = occ->mVel1; mVel2 = occ->mVel2; mVel3 = occ->mVel3; mVelNL.resize(3);
		mPos = occ->mPos; mSPrime.resize(3); mS2Prime.resize(3); mSegLast.resize(3);
		mCharge = 0; mSegLength = 0; mFlagFilled = occ->mFlagFilled; mMarkedForDeletion = false;
	}
	Point(vector <double> CurrentPos){
		/* parameterised constructor copies a point, used in reconnection */
		mPos.resize(3);
		mPos[0] = CurrentPos[0]; mPos[1] = CurrentPos[1]; mPos[2] = CurrentPos[2];
		mVel.resize(3); mVel1.resize(3); mVel2.resize(3); mVel3.resize(3); mVelNL.resize(3);
		mSPrime.resize(3); mS2Prime.resize(3); mSegLast.resize(3);
		mCharge = 0; mSegLength = 0; mFlagFilled = 0; mMarkedForDeletion = false;
	}
	~Point(){};
};

#endif
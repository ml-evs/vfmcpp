#ifndef GUARD_FILAMENT_H
#define GUARD_FILAMENT_H

#include "point.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

const double PI = 3.14159265359;

/* general filament interface class */
class Filament{
public:
	/* member data */
	int mN;
	int mFlagType; // integer showing filament type: 0 for rings, 1 for lines
	vector <Point*> 	mPoints;
	bool mCarriesCharge = false; // default filaments don't carry electrons
	vector <Point*>     mDummies;
	/* member functions */
	Filament(){};
	~Filament(){};
	void CalcMeshLengths();
	void CalcImpulse();
	void CalcSPrime();
	void CalcS2Prime();
	void CalcVelocitySelfNL();
	void CalcDummy();
};
/* ring class */
class Ring : public Filament{
public:
	Ring(){mN = 0; mFlagType = 0;};
	Ring(double res, double r, double x, double y, double z, int alignment){
		res *= 3.0/4.0;			// this replaces multiplying the average mesh length by 4/3 to calculate resolution
		mN = 2*PI*r/res; mFlagType = 0;
		if(alignment==2){
			for(int i=0; i<mN; i++){
				mPoints.push_back(new Point());
				mPoints[i]->mPos[0]=x+r*sin(-i*(2*PI)/mN);
				mPoints[i]->mPos[1]=y+r*cos(-i*(2*PI)/mN);
				mPoints[i]->mPos[2]=z;
			}
		}
		if(alignment==1){
			for(int i=0; i<mN; i++){
				mPoints.push_back(new Point());
				mPoints[i]->mPos[0]=x+r*cos(-i*(2*PI)/mN);
				mPoints[i]->mPos[1]=y;
				mPoints[i]->mPos[2]=z+r*sin(-i*(2*PI)/mN);;
			}
		}
		if(alignment==0){
			for(int i=0; i<mN; i++){
				mPoints.push_back(new Point());
				mPoints[i]->mPos[0]=x;
				mPoints[i]->mPos[1]=y+r*sin(-i*(2*PI)/mN);
				mPoints[i]->mPos[2]=z+r*cos(-i*(2*PI)/mN);
			}
		}
		for(int i(1); i!=mN; i++){(mPoints[i])->mPrev = mPoints[i-1];}
		mPoints[0]->mPrev = mPoints[mN-1];
		for(int i(0); i!=mN-1; i++){(mPoints[i])->mNext = mPoints[i+1];}
		mPoints[mN-1]->mNext = mPoints[0];
		CalcMeshLengths();
	}
};

/* string class */
class String : public Filament{
private:
	double mL;
public:
	String(){
	    mN = 0; mFlagType = 1;
	}
	String(double res, double L, double x, double y, double z){
		res *= 3.0/4.0;			 	// this replaces multiplying the average mesh length by 4/3 to calculate resolution
		mL = L; mN = mL/res;
		mFlagType = 1;
		for (int i = 0; i != 4; i++){
			mDummies.push_back(new Point());
		}
		for (int i = 0; i != mN; i++){
            mPoints.push_back(new Point());
			mPoints[i]->mPos[0] = x;
			mPoints[i]->mPos[1] = y;
			mPoints[i]->mPos[2] = z + i*mL / mN;
		}
		/* add dummy points to the ends */
		for (int i = 1; i != mN; i++){ (mPoints[i])->mPrev = mPoints[i - 1]; }
		for (int i = 0; i != mN-1; i++){ (mPoints[i])->mNext = mPoints[i + 1]; }
		mPoints[mN-1]->mNext = mDummies[0];
		mDummies[0]->mPrev = mPoints[mN-1];
		mDummies[0]->mNext = mDummies[1];
		mDummies[1]->mPrev = mDummies[0];
		mPoints[0]->mPrev = mDummies[2];
		mDummies[2]->mNext = mPoints[0];
		mDummies[2]->mPrev = mDummies[3];
		mDummies[3]->mNext = mDummies[2];

		CalcMeshLengths();
		/* label dummies */
		for (int i=0; i != 4; i++){
			mDummies[i]->mFlagDummy = 1;
		}
		/* calculate the positions of the dummy points */
		CalcDummy();
		CalcMeshLengths();
		/* starting point should remain stationary */
		mPoints[0]->mFlagFilled = 5;
	}
};

#endif
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
	vector <Point*> 	mPoints;
	/* member functions */
	Filament(){};
	~Filament(){};
	void CalcMeshLengths();
	void CalcImpulse();
	void CalcSPrime();
	void CalcS2Prime();
	void CalcVelocitySelfNL();
};
/* ring class */
class Ring : public Filament{
public:
	Ring(){mN = 0;};
	Ring(double r, int N, double x, double y, double z){
		mN = N;
		for(int i=0; i<mN; i++){
			mPoints.push_back(new Point());
			mPoints[i]->mPos[0]=x+r*sin(-i*(2*PI)/mN);
			mPoints[i]->mPos[1]=y+r*cos(-i*(2*PI)/mN);
			mPoints[i]->mPos[2]=z;
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
		mN = 100;
		mL = 2e-6;
		for (int i = 0; i < mN; i++){
			mPoints.push_back(new Point());
			mPoints[i]->mPos[0] = 0;
			mPoints[i]->mPos[1] = 0;
			mPoints[i]->mPos[2] = i*mL / mN;
		}
		for (int i = 1; i != mN; i++){ (mPoints[i])->mPrev = mPoints[i - 1]; }
		for (int i = 1; i != mN; i++){ (mPoints[i])->mNext = mPoints[i + 1]; }
		CalcMeshLengths();
		mPoints.push_back(new Point());
		mPoints.push_back(new Point());
		mPoints[0]->mPrev = mPoints[mN + 1];
		mPoints[mN]->mNext = mPoints[mN + 1];
		mPoints[mN + 1]->mNext = mPoints[0];
		mPoints[mN + 1]->mPrev = mPoints[mN];
		mPoints[mN]->mPos[0] = -1 * (mPoints[2]->mPos[0] - mPoints[0]->mPos[0]);
		mPoints[mN]->mPos[1] = -1 * (mPoints[2]->mPos[1] - mPoints[0]->mPos[1]);
		mPoints[mN]->mPos[2] = -1 * (mPoints[2]->mPos[2] - mPoints[0]->mPos[2]);
		mPoints[mN + 1]->mPos[0] = -1 * mPoints[1]->mSegLast[0];
		mPoints[mN + 1]->mPos[1] = -1 * mPoints[1]->mSegLast[1];
		mPoints[mN + 1]->mPos[2] = -1 * mPoints[1]->mSegLast[2];
		mPoints[0]->mFlagFilled = 5;
	}
	String(double L, int N, double x, double y, double z){
		mL = L, mN = N;
		for (int i = 0; i != mN + 4; i++){
			mPoints.push_back(new Point());
		}
		for (int i = 0; i != mN; i++){
			mPoints[i]->mPos[0] = x;
			mPoints[i]->mPos[1] = y;
			mPoints[i]->mPos[2] = z + i*mL / mN;
		}
		/* add dummy points to the ends */
		for (int i = 1; i != mN+2; i++){ (mPoints[i])->mPrev = mPoints[i - 1]; }
		for (int i = 0; i != mN+1; i++){ (mPoints[i])->mNext = mPoints[i + 1]; }
		mPoints[0]->mPrev = mPoints[mN + 2];
		mPoints[mN + 2]->mPrev = mPoints[0];
		mPoints[mN + 2]->mNext = mPoints[mN + 3];
		mPoints[mN + 3]->mPrev = mPoints[mN + 2];
		CalcMeshLengths();
		/* calculate the positions of the dummy points */
		for (int j = 0; j != 3; j++){
			mPoints[mN]->mPos[j] = mPoints[mN]->mPrev->mPos[j] - mPoints[mN]->mPrev->mSegLast[j]; 
			mPoints[mN + 1]->mPos[j] = mPoints[mN + 1]->mPrev->mPos[j] - mPoints[mN + 1]->mPrev->mPrev->mPrev->mSegLast[j]; 
			mPoints[mN + 2]->mPos[j] = mPoints[mN + 2]->mPrev->mPos[j] + mPoints[mN + 2]->mPrev->mNext->mSegLast[j];
			mPoints[mN + 3]->mPos[j] = mPoints[mN + 3]->mPrev->mPos[j] + mPoints[mN + 3]->mPrev->mPrev->mNext->mNext->mSegLast[j];
		}		
		/* seglengths two away from given point needed to calculate curvature */
		mN = mN + 4;
		CalcMeshLengths();
		mN = mN - 4;
		/* starting point should remain stationary */
		mPoints[0]->mFlagFilled = 5;
	}

	//String(double L, int N, double x, double y, double z, double theta, double phi){
	//	mL = L; mN = N;
	//	mPoints.push_back(new Point());
	//	for (int i = 1; i != mN; i++){
	//		mPoints.push_back(new Point());
	//		mPoints[i]->mPos[0] = x + i*(mL / mN)*sin(-1*phi) * cos(-1*theta);
	//		mPoints[i]->mPos[1] = y + i*(mL / mN)*sin(-1 * phi) * sin(-1 * theta);
	//		mPoints[i]->mPos[2] = z + i*(mL / mN)*cos(-1 * phi);
	//	}
	//	for (int i = 1; i != mN; i++){ (mPoints[i])->mPrev = mPoints[i - 1]; }
	//	for (int i = 0; i != mN; i++){ (mPoints[i])->mNext = mPoints[i + 1]; }
	//	CalcMeshLengths();
	//	/* add points to fix the position of the starting point */
	//	mPoints.push_back(new Point());
	//	mPoints.push_back(new Point());
	//	mPoints[0]->mPrev = mPoints[mN + 1];
	//	mPoints[mN]->mNext = mPoints[mN + 1];
	//	mPoints[mN + 1]->mNext = mPoints[0];
	//	mPoints[mN + 1]->mPrev = mPoints[mN];
	//	mPoints[mN]->mPos[0] = -1 * (mPoints[2]->mPos[0] - mPoints[0]->mPos[0]);
	//	mPoints[mN]->mPos[1] = -1 * (mPoints[2]->mPos[1] - mPoints[0]->mPos[1]);
	//	mPoints[mN]->mPos[2] = -1 * (mPoints[2]->mPos[2] - mPoints[0]->mPos[2]);
	//	mPoints[mN + 1]->mPos[0] = -1 * mPoints[1]->mSegLast[0];
	//	mPoints[mN + 1]->mPos[1] = -1 * mPoints[1]->mSegLast[1];
	//	mPoints[mN + 1]->mPos[2] = -1 * mPoints[1]->mSegLast[2];
	//	/* starting point should remain stationary */
	//	mPoints[0]->mFlagFilled = 5;
	//}
};
/*
class String : public Filament{
private:
	double mL;
public:
	String(){
		mN = 100;
		mL = 2e-7;
		for(int i=0; i<mN; i++){
			mPoints.push_back(new Point());
			mPoints[i]->mPos[0]=0;
			mPoints[i]->mPos[1]=0;
			mPoints[i]->mPos[2]=i*mL/mN;
		}
		for(int i=1; i<mN; i++){(mPoints[i])->mPrev = mPoints[i-1];}
		mPoints[0]->mPrev = mPoints[mN-1];
		CalcMeshLengths();
	}
	String(double L, int N){
		mL = L; mN = N;
		for(int i=0; i<mN; i++){
			mPoints.push_back(new Point());
			mPoints[i]->mPos[0]=3;
			mPoints[i]->mPos[1]=2;
			mPoints[i]->mPos[2]=1;
			cout <<  mPoints[i]->mPos[2] << endl;
		}
		for(int i=1; i<mN; i++){(mPoints[i])->mPrev = mPoints[i-1];}
		mPoints[0]->mPrev = mPoints[mN-1];
		CalcMeshLengths();
	}
};*/

#endif
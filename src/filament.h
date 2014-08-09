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
	Ring(int N, double r, double x, double y, double z){
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
	Ring(int N, double r, double x, double y, double z, double theta){
		mN = N;
		for(int i=0; i<mN; i++){
			mPoints.push_back(new Point());
			mPoints[i]->mPos[0]=r*sin(-i*(2*PI)/mN);
			mPoints[i]->mPos[1]=r*cos(-i*(2*PI)/mN);
			mPoints[i]->mPos[2]=mPoints[i]->mPos[0]*sin(PI*theta/180);
			mPoints[i]->mPos[0]+= x;
			mPoints[i]->mPos[1]+= y;
			mPoints[i]->mPos[2]+= z;
		}
		for(int i(1); i!=mN; i++){(mPoints[i])->mPrev = mPoints[i-1];}
		mPoints[0]->mPrev = mPoints[mN-1];
		for(int i(0); i!=mN-1; i++){(mPoints[i])->mNext = mPoints[i+1];}
		mPoints[mN-1]->mNext = mPoints[0];
		CalcMeshLengths();
	}
};

#endif
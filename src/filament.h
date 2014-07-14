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
	void CalcVelocity();
	void CalcImpulse();
	void CalcSPrime();
	void CalcS2Prime();
	void CalcVelocitySelfNL();
	void PropagatePosAB3(double & dt); 
	void MeshAdjust(double dr);
};
/* ring class */
class Ring : public Filament{
private:
	double 			mRadius0;
	vector <double>	mCentre;
public:
	Ring(){
		mRadius0 = 1e-6; 
		mN = 100;
		mCentre.resize(3,0);
		for(int i=0; i<mN; i++){
			mPoints.push_back(new Point());
			mPoints[i]->mPos[0]=mCentre[0]-mRadius0*sin(i*(2*PI)/mN);
			mPoints[i]->mPos[1]=mCentre[1]-mRadius0*cos(i*(2*PI)/mN);
			mPoints[i]->mPos[2]=mCentre[2];
		}
		for(int i(1); i!=mN; i++){(mPoints[i])->mPrev = mPoints[i-1];}
		mPoints[0]->mPrev = mPoints[mN-1];
		for(int i(0); i!=mN-1; i++){(mPoints[i])->mNext = mPoints[i+1];}
		mPoints[mN-1]->mNext = mPoints[0];
		CalcMeshLengths();
	}
	Ring(double r, int N, double x, double y, double z){
		mRadius0 = r; mN = N;
		mCentre.resize(3);
		mCentre[0] = x; mCentre[1] = y; mCentre[2] = z;
		for(int i=0; i<mN; i++){
			mPoints.push_back(new Point());
			mPoints[i]->mPos[0]=mCentre[0]+mRadius0*sin(-i*(2*PI)/mN);
			mPoints[i]->mPos[1]=mCentre[1]+mRadius0*cos(-i*(2*PI)/mN);
			mPoints[i]->mPos[2]=mCentre[2];
		}
		for(int i(1); i!=mN; i++){(mPoints[i])->mPrev = mPoints[i-1];}
		mPoints[0]->mPrev = mPoints[mN-1];
		for(int i(0); i!=mN-1; i++){(mPoints[i])->mNext = mPoints[i+1];}
		mPoints[mN-1]->mNext = mPoints[0];
		CalcMeshLengths();
	}
};
/* string class */
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
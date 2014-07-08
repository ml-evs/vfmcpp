#ifndef GUARD_FILAMENT_H
#define GUARD_FILAMENT_H

#include "point.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Filament{
public:
	/* member data */
	int mN;
	vector <Point*> 	mPoints;
	/* member functions */
	Filament(){};
	~Filament(){};
	//void CalcMeshLengths();
/*	void CalcVelocity();
	void CalcSPrime();
	void CalcS2Prime();
	void CalcVelocitySelfNL();
	void PropagatePosAB3(double & dt);*/
};

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
			mPoints[i]->mPos.resize(3);
			mPoints[i]->mPos[0]=mCentre[0]+mRadius0*sin(i*(2*M_PI)/mN);
			mPoints[i]->mPos[1]=mCentre[1]+mRadius0*cos(i*(2*M_PI)/mN);
			mPoints[i]->mPos[2]=mCentre[2];
		}
		for(int i=1; i<mN; i++){(mPoints[i])->mLast = mPoints[i-1];}
		mPoints[0]->mLast = mPoints[mN-1];
		//CalcMeshLengths();
	}
	Ring(double r, int N, double x, double y, double z){
		mRadius0 = r; mN = N;
		mCentre.resize(3);
		mCentre[0] = x; mCentre[1] = y; mCentre[2] = z;
		for(int i=0; i<mN; i++){
			mPoints.push_back(new Point());
			mPoints[i]->mPos.resize(3);
			mPoints[i]->mPos[0]=mCentre[0]+mRadius0*sin(i*(2*M_PI)/mN);
			mPoints[i]->mPos[1]=mCentre[1]+mRadius0*cos(i*(2*M_PI)/mN);
			mPoints[i]->mPos[2]=mCentre[2];
		}
		for(int i=1; i<mN; i++){(mPoints[i])->mLast = mPoints[i-1];}
		mPoints[0]->mLast = mPoints[mN-1];
		for(int i=0; i<mN; i++){cout << mPoints[i]->mPos[0] << endl;}
		//CalcMeshLengths();
	}
};

#endif
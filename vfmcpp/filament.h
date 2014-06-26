#ifndef GUARD_FILAMENT_H
#define GUARD_FILAMENT_H

#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Filament{
protected:
	int 						mN;
	vector <vector <double> > 	mPos;
	vector <vector <double> > 	mVel;
	vector <vector <double> > 	mSPrime;
	vector <vector <double> > 	mS2Prime;
	vector <double> 			mSegLengths;
public:
	Filament(){};
	~Filament(){};
	void CalcMeshLengths();
	vector <double> GetMeshLengths(){return mSegLengths;}
	void CalcVelocity();
	void CalcSPrime();
	void CalcS2Prime();
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
		mPos.resize(mN); 
		for(int i=0; i<mN; i++){
			mPos[i].resize(4);
			mPos[i][0]=mCentre[0]+mRadius0*sin(i*(2*M_PI)/mN);
			mPos[i][1]=mCentre[0]+mRadius0*cos(i*(2*M_PI)/mN);
			mPos[i][2]=mCentre[2];
			mPos[i][3]=i;
		}
		CalcMeshLengths();

	}
	Ring(double r, int N, double x, double y, double z){
		mRadius0 = r; mN = N;
		mCentre.resize(3);
		mCentre[0] = x; mCentre[1] = y; mCentre[2] = z;
		mPos.resize(mN);
		for(int i=0; i<mN; i++){
			mPos[i].resize(4);
			mPos[i][0]=mCentre[0]+mRadius0*sin(i*(2*M_PI)/mN);
			mPos[i][1]=mCentre[0]+mRadius0*cos(i*(2*M_PI)/mN);
			mPos[i][2]=mCentre[2];
			mPos[i][3]=i;
			cout << mPos[i][0] << ", " << mPos[i][1] << ", " << mPos[i][2] << endl;
		}
		CalcMeshLengths();

	}
};

#endif
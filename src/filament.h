#ifndef GUARD_FILAMENT_H
#define GUARD_FILAMENT_H

#include <vector>
#include <cmath>
#include <iostream>
#include <list>

using namespace std;

class Filament{
public:
	/* member data */
	int 										mN; 				// number of points 
	list <vector <double> >	mPos;				// positions of each point
	vector <vector <double> > 			mVel; 			// current velocity
	vector <vector <double> > 			mVel1;			// velocity last time step
	vector <vector <double> > 			mVel2;  			// velocity 2 time steps ago
	vector <vector <double> >			mVelNL; 			// non-local velocity
	vector <vector <double> >		 	mSPrime;			// tangent at point
	vector <vector <double> > 			mS2Prime;		// binormal at point
	vector <double> 						mSegLengths;	// distance from ith to (i-1)th point
	/* constructors and destructors */
	Filament(){};
	~Filament(){};
	/* member functions */
	void CalcMeshLengths();
	void CalcVelocity();
	void CalcSPrime();
	void CalcS2Prime();
	void CalcVelocitySelfNL();
	void PropagatePosAB3(double & dt);
	//void PropagatePosRK4(double & dt);

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
		int i(0);
		list <vector <double> >::iterator b, c, e;
		b = mPos.begin(); e = mPos.end();
		for(c=b;c!=e;c++){
			c->resize(3);
			(*c)[0]=mCentre[0]+mRadius0*sin(i*(2*M_PI)/mN);
			(*c)[1]=mCentre[1]+mRadius0*cos(i*(2*M_PI)/mN);
			(*c)[2]=mCentre[2];
			i++;
		}
		CalcMeshLengths();
	}
	Ring(double r, int N, double x, double y, double z){
		mRadius0 = r; mN = N;
		mCentre.resize(3);
		mCentre[0] = x; mCentre[1] = y; mCentre[2] = z;
		mPos.resize(mN);
		int i(0);
		list <vector <double> >::iterator b, c, e;
		b = mPos.begin(); e = mPos.end();
		for(c=b;c!=e;c++){
			c->resize(3);
			(*c)[0]=mCentre[0]+mRadius0*sin(i*(2*M_PI)/mN);
			(*c)[1]=mCentre[1]+mRadius0*cos(i*(2*M_PI)/mN);
			(*c)[2]=mCentre[2];
			i++;
		}
		CalcMeshLengths();
	}
};

#endif
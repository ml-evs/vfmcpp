#ifndef GUARD_FILAMENT_H
#define GUARD_FILAMENT_H

#include "point.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

using namespace std;

const double PI = 3.14159265359;

/* general filament interface class */
class Filament{
public:
	/* member data */
	int mN;
	int mFlagType; // integer showing filament type: 0 for rings, 1 for lines
	vector <Point*> 	mPoints;
	bool mCarriesCharge; // default filaments don't carry electrons
	vector <Point*>     mDummies;
	/* member functions */
	Filament(){};
	~Filament(){};
	void CalcMeshLengths();
	void CalcImpulse();
	void CalcSPrime();
	void CalcS2Prime();
	void CalcDummy();
};
/* ring class */
class Ring : public Filament{
public:
	Ring(){mN = 0; mFlagType = 0; mCarriesCharge = false;};
	Ring(double res, double r, double x, double y, double z, int alignment){
		mN = 2*PI*r/res; mFlagType = 0; mCarriesCharge = false;
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
				mPoints[i]->mPos[0]=x+r*cos(-i*(2*PI)/mN);
				mPoints[i]->mPos[1]=y;
				mPoints[i]->mPos[2]=z+r*sin(-i*(2*PI)/mN);
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
	    mN = 0; mFlagType = 1; mCarriesCharge = false;
	}
	String(double res, double L, double x, double y, double z, int dir){
		mL = L; mN = mL/res; mCarriesCharge = false;
		mFlagType = 1;
		for (int i=0; i!=4; i++){
			mDummies.push_back(new Point());
		}
		for (int i = 0; i != mN; i++){
            mPoints.push_back(new Point());
			mPoints[i]->mPos[0] = x;
			mPoints[i]->mPos[1] = y;
			mPoints[i]->mPos[2] = z;
			mPoints[i]->mPos[dir] += i * mL / mN;
		}
		for(int i(1);i!=mN;i++){
			(mPoints[i])->mPrev = mPoints[i-1];
		}
		for(int i(0);i!=mN-1;i++){
			(mPoints[i])->mNext = mPoints[i+1];
		}
		/* add dummy points to the ends */
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
		/* endpoints should remain stationary */
		mPoints[0]->mFlagFilled = 5;
        mPoints[mN-1]->mFlagFilled = 5;
    }
    /* read string from files named pos.dat and vel.dat
       from folder provided in input file */
    String(string base_filename){
		mFlagType = 1;
		for (int i=0; i!=4; i++){
			mDummies.push_back(new Point());
		}
		string pos_filename(base_filename + "/pos.dat");
		string vel_filename(base_filename + "/vel.dat");
		string line, put;
		stringstream input, convert;
		char* char_pos_filename = (char*)pos_filename.c_str();
		char* char_vel_filename = (char*)vel_filename.c_str();
		ifstream posfile(char_pos_filename);
		ifstream velfile(char_vel_filename);
		if(!posfile.good()){cout << "Line file not found, exiting... " << endl; exit(1);}
		if(!velfile.good()){cout << "Line velocity file not found, exiting... " << endl; exit(1);}
		getline(posfile, line);
		getline(velfile, line);
		int b(0);
		while(posfile){
			put.clear();
			input.clear();
			convert.clear();
			line.clear();
			getline(posfile, line);
			input << line;
			if(line != ""){
				mPoints.push_back(new Point());
				for(int j(0);j<3;j++){
					convert.clear();
					getline(input, put, '\t');
					convert << put;
					convert >> mPoints[b]->mPos[j];
					if(input.eof()) break;
				}
				b++;
			}
			else{break;}
		}
		b = 0;
		while(velfile){
			put.clear();
			input.clear();
			convert.clear();
			line.clear();
			getline(velfile, line);
			input << line;
			if(line != ""){
				for(int j(0);j<3;j++){
					convert.clear();
					getline(input, put, '\t');
					convert << put;
					convert >> mPoints[b]->mVel[j];
					if(input.eof()) break;
				}
				b++;
			}
			else{break;}
		}
		mN = mPoints.size();
		for(int i(1);i!=mN;i++){
			(mPoints[i])->mPrev = mPoints[i-1];
		}
		for(int i(0);i!=mN-1;i++){
			(mPoints[i])->mNext = mPoints[i+1];
		}
		/* add dummy points to the ends */
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
		for(int i(0);i!=4;i++){
			mDummies[i]->mFlagDummy = 1;
		}
		/* calculate the positions of the dummy points */
		CalcDummy();
		CalcMeshLengths();
		/* endpoints should remain stationary */
		mPoints[0]->mFlagFilled = 5;
        mPoints[mN-1]->mFlagFilled = 5;
    }
};

#endif

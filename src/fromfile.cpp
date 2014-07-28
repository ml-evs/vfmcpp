/* Functions to read initial conditions from file and save current state. */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <iostream>
#include <sstream>

void Tangle::FromFile(){
	string base("../bin/init/2_RING");
//	string base("../bin/init/test/Reconnection_");
	string filename;
	int n_fil(1);
	vector <double> CurrentVec(3);
	int n_points(0);
	for(int j(0);j!=n_fil; j++){
		mTangle.push_back(new Ring());
		filename = base + (to_string(j)+".dat");
		n_points = 0;
		fstream pos(filename);
		while(pos){
			for(int i(0); i!=3; i++){
				pos >> CurrentVec[i]; 
			}
			if(pos.eof()) break;
			mTangle.back()->mPoints.push_back(new Point(CurrentVec));
			n_points++;
		}
		pos.close();
		filename = base + (to_string(j)+"vel.dat");
		fstream vel(filename);
		int k(0);
		while(vel){
			for(int i(0); i!=3; i++){
				vel >> CurrentVec[i]; 
			}
			if(vel.eof()) break;
			mTangle.back()->mPoints[k]->mVel = CurrentVec;
			k++;
		}
		vel.close();
		filename = base + (to_string(j)+"vel1.dat");
		fstream vel1(filename);
		k = 0;
		while(vel1){
			for(int i(0); i!=3; i++){
				vel1 >> CurrentVec[i]; 
			}
			if(vel1.eof()) break;
			mTangle.back()->mPoints[k]->mVel1 = CurrentVec;
			k++;
		}		
		vel1.close();
		filename = base + (to_string(j)+"vel2.dat");
		fstream vel2(filename);
		k=0;
		while(vel2){
			for(int i(0); i!=3; i++){
				vel2 >> CurrentVec[i]; 
			}
			if(vel2.eof()) break;
			mTangle.back()->mPoints[k]->mVel2 = CurrentVec;
			k++;
		}
		vel2.close();
		filename = base + (to_string(j)+"vel3.dat");
		fstream vel3(filename);
		k = 0;
		while(vel3){
			for(int i(0); i!=3; i++){
				vel3 >> CurrentVec[i]; 
			}
			if(vel3.eof()) break;
			mTangle.back()->mPoints[k]->mVel3 = CurrentVec;
			k++;
		}
		vel3.close();
	}
	for(unsigned int b(0);b<mTangle.size();b++){
		for(unsigned int i(1); i!=mTangle[b]->mPoints.size(); i++){(mTangle[b]->mPoints[i])->mPrev = mTangle[b]->mPoints[i-1];}
		mTangle[b]->mPoints[0]->mPrev = mTangle[b]->mPoints[mTangle[b]->mPoints.size()-1];
		mTangle[b]->mN = mTangle[b]->mPoints.size();
		for(unsigned int i(0); i!=mTangle[b]->mPoints.size()-1; i++){(mTangle[b]->mPoints[i])->mNext = mTangle[b]->mPoints[i+1];}
		mTangle[b]->mPoints[mTangle[b]->mPoints.size()-1]->mNext = mTangle[b]->mPoints[0];
	}
	for(unsigned int b(0);b<mTangle.size();b++){
		mTangle[b]->CalcMeshLengths();
	}

}

void Tangle::SaveState(){
	int n_fil(0);
	vector <Filament*>::iterator c, b(mTangle.begin()), e(mTangle.end());
	for(c=b; c!=e; c++){
		stringstream ss;
		ss << n_fil;
		string n_fil_str = ss.str();
		string ith_jth_filename = "init/015_09_rec" + n_fil_str + ".dat";
		ofstream outfile(ith_jth_filename.c_str());
		outfile.precision(8);
		int j(0);
		Point* pCurrent = (*c)->mPoints[0];
		while(j!=(*c)->mN){
			for(int m(0); m<3; m++){
				outfile << pCurrent->mNext->mPos[m] << "\t";
			}
			pCurrent = pCurrent->mNext;
			j++;
			outfile << "\n";
		}
		outfile.close();
		ith_jth_filename =  "init/015_09_rec" + n_fil_str + "vel.dat";
		ofstream outfile2(ith_jth_filename.c_str());
		j = 0;
		pCurrent = (*c)->mPoints[0];
		while(j!=(*c)->mN){
			for(int m(0); m<3; m++){
				outfile2 << pCurrent->mNext->mVel[m] << "\t";
			}
			pCurrent = pCurrent->mNext;
			j++;
			outfile2 << "\n";
		}
		outfile2.close();											
		ith_jth_filename =  "init/015_09_rec" + n_fil_str + "vel1.dat";
		ofstream outfile3(ith_jth_filename.c_str());
		j = 0;
		pCurrent = (*c)->mPoints[0];
		while(j!=(*c)->mN){
			for(int m(0); m<3; m++){
				outfile3 << pCurrent->mNext->mVel1[m] << "\t";
			}
			pCurrent = pCurrent->mNext;
			j++;
			outfile3 << "\n";
		}
		outfile3.close();											
		ith_jth_filename =  "init/015_09_rec" + n_fil_str + "vel2.dat";
		ofstream outfile4(ith_jth_filename.c_str());
		j = 0;
		pCurrent = (*c)->mPoints[0];
		while(j!=(*c)->mN){
			for(int m(0); m<3; m++){
				outfile4 << pCurrent->mNext->mVel2[m] << "\t";
			}
			pCurrent = pCurrent->mNext;
			j++;
			outfile4 << "\n";
		}
		outfile4.close();		
		
		ith_jth_filename =  "init/015_09_rec" + n_fil_str + "vel3.dat";
		ofstream outfile5(ith_jth_filename.c_str());
		j = 0;
		pCurrent = (*c)->mPoints[0];
		while(j!=(*c)->mN){
			for(int m(0); m<3; m++){
				outfile5 << pCurrent->mNext->mVel3[m] << "\t";
			}
			pCurrent = pCurrent->mNext;
			j++;
			outfile5 << "\n";
		}
		outfile5.close();		
		n_fil++;
	}
	cout << "!!!!! WROTE CURRENT STATE TO FILE !!!!!" << endl;
}
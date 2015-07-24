/* Functions to read initial conditions from file and save current state. */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <iostream>
#include <sstream>

void Tangle::Output(string filename, int i, int file_no){
	vector <Filament*>::iterator begin, end, current;
	begin = mTangle.begin(); end = mTangle.end();
	int n_fil(0);
	stringstream ss0; ss0 << file_no; string i_str = ss0.str();
	string ith_filename = filename + "/data_" + i_str + "_";
	string ith_mom_filename = filename + "/mom_" + i_str + "_";

	vector <double> EKin;
	
	for(current=begin; current!=end; current++){
		EKin.push_back(0);
		stringstream ss; ss << n_fil;	string n_fil_str = ss.str(); 
		string ith_jth_filename = ith_filename + n_fil_str + ".dat";
		ofstream outfile(ith_jth_filename.c_str());	outfile.precision(8);
		outfile << i*mDt << "\n";
		int j(0);
		Point* pCurrent = (*current)->mPoints[0];
		while(j!=(*current)->mN+1-(*current)->mFlagType){
			for(int m(0); m<3; m++){
				outfile << pCurrent->mPos[m] << "\t";
			}
			pCurrent = pCurrent->mNext; j++; outfile << "\n";
		}
		string ith_jth_mom_filename = ith_mom_filename + n_fil_str + ".dat";
		ofstream momfile(ith_jth_mom_filename.c_str());	momfile.precision(8);
		momfile << i*mDt << "\n";
		for(int i=0;i<(*current)->mN;i++){
			vector <double> rxsp(3);
			double length = sqrt(pow((*current)->mPoints[i]->mSPrime[0],2) + pow((*current)->mPoints[i]->mSPrime[1],2) + pow((*current)->mPoints[i]->mSPrime[2],2));
			rxsp[0] = ((*current)->mPoints[i]->mPos[1] * (*current)->mPoints[i]->mSPrime[2] - (*current)->mPoints[i]->mPos[2] * (*current)->mPoints[i]->mSPrime[1])/length;
			rxsp[1] = ((*current)->mPoints[i]->mPos[2] * (*current)->mPoints[i]->mSPrime[0] - (*current)->mPoints[i]->mPos[0] * (*current)->mPoints[i]->mSPrime[2])/length;
			rxsp[2] = ((*current)->mPoints[i]->mPos[0] * (*current)->mPoints[i]->mSPrime[1] - (*current)->mPoints[i]->mPos[1] * (*current)->mPoints[i]->mSPrime[0])/length;
			for(int q=0;q<3;q++){
				EKin.back() += (*current)->mPoints[i]->mVel[q] * rxsp[q];
				momfile << rxsp[q] << "\t";
			}
			momfile << "\n";

		}
		momfile.close(); outfile.close(); n_fil++;

	}
	string energy_filename = filename + "/energy.dat";
	ofstream energyfile(energy_filename.c_str(),std::ios::app);	energyfile.precision(8);
	for(int q(0); q<n_fil; q++){
		energyfile << EKin[q] << "\t";
	}
	energyfile << "\n";
	energyfile.close();
}
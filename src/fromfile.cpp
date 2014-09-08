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
	for(current=begin; current!=end; current++){
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
		/* save full state of system ~10 times per run */
		if(i%(100*mN_f)==0){
			string state_filename = filename + "/snapshot/data_" + i_str + "_" + n_fil_str + ".dat";
			string vel_filename   = filename + "/snapshot/vel_"  + i_str + "_" + n_fil_str + ".dat";
			string vel1_filename  = filename + "/snapshot/vel1_" + i_str + "_" + n_fil_str + ".dat";
			string vel2_filename  = filename + "/snapshot/vel2_" + i_str + "_" + n_fil_str + ".dat";
			string vel3_filename  = filename + "/snapshot/vel3_" + i_str + "_" + n_fil_str + ".dat";
			ofstream statefile(state_filename.c_str());	ofstream velfile(vel_filename.c_str());
			ofstream vel1file(vel1_filename.c_str());	ofstream vel2file(vel2_filename.c_str());
			ofstream vel3file(vel3_filename.c_str());
			statefile << i*mDt << "\n"; velfile << i*mDt << "\n"; vel1file << i*mDt << "\n";
			vel2file << i*mDt << "\n"; vel3file << i*mDt << "\n";
			int j(0);
			pCurrent = (*current)->mPoints[0];
			while(j!=(*current)->mN+1-(*current)->mFlagType){
				for(int m(0); m<3; m++){
					statefile << pCurrent->mPos[m] << "\t";
					velfile 	<< pCurrent->mVel[m] << "\t";
					vel1file 	<< pCurrent->mVel1[m] << "\t";
					vel2file 	<< pCurrent->mVel2[m] << "\t";
					vel3file 	<< pCurrent->mVel3[m] << "\t";
				}
				pCurrent = pCurrent->mNext; j++; 
				statefile << "\n"; velfile << "\n"; vel1file << "\n"; vel2file << "\n"; vel3file << "\n";
				statefile.close(); velfile.close(); vel1file.close(); vel2file.close(); vel3file.close();				
			}
		}
		outfile.close(); n_fil++;
	}
    if(i%(100*mN_f)==0){
        mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\t\t\twrote full state of system to snapshot/" << endl;
    }
}


void Tangle::FromFile(string base){
	string filename;
	int n_fil(2);
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

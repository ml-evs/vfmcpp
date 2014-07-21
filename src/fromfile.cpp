#include "tangle.h"
#include "point.h"
#include <fstream>

void Tangle::FromFile(){
	string base("../bin/init/test");
	string filename;
	int n_fil;
	cout << "Please enter the number of filaments: " 
	cin >> n_fil;
	Tangle Tangle();
	Tangle.push_back(new Ring());
	int n_points(0);
	for(int j(0);j!=n_fil; j++){
		filename = base.append("_"+to_string(j)+".dat")
		fstream pos(filename);
		vector <double> CurrentPos(3);
		while(pos){
			for(int i(0); i!=3; i++){
				pos >> CurrentVec[i]; 
			}
			Tangle.back().push_back(new Point(CurrentVec));
			n_points++;
		}
		pos.close();
		filename = base.append("_"+to_string(j)+"_vel.dat")
		fstream vel(filename);
		int k(0);
		while(vel){
			for(int i(0); i!=3; i++){
				vel >> CurrentVec[i]; 
			}
			mTangle.back()->mPoints[k]->mVel = CurrentVec;
		}
		vel.close();
		filename = base.append("_"+to_string(j)+"_vel1.dat")
		fstream vel(filename);
		int k(0);
		while(vel){
			for(int i(0); i!=3; i++){
				vel >> CurrentVec[i]; 
			}
			mTangle.back()->mPoints[k]->mVel1 = CurrentVec;
		}		
		vel.close();
		filename = base.append("_"+to_string(j)+"_vel2.dat")
		fstream vel(filename);
		int k(0);
		while(vel){
			for(int i(0); i!=3; i++){
				vel >> CurrentVec[i]; 
			}
			mTangle.back()->mPoints[k]->mVel2 = CurrentVec;
		}
		vel.close();
		filename = base.append("_"+to_string(j)+"_vel3.dat")
		fstream vel(filename);
		int k(0);
		while(vel){
			for(int i(0); i!=3; i++){
				vel >> CurrentVec[i]; 
			}
			mTangle.back()->mPoints[k]->mVel3 = CurrentVec;
		}
		vel.close();
	}

}

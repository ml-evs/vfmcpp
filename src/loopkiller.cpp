
#include "tangle.h"

using namespace std;

bool Tangle::LoopKill(){
	bool Killed(false);
	for(unsigned int i(0); i<mTangle.size(); i++){
		if(mTangle[i]->mN < 6){
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tkilled small loop at (";
			mLog << mTangle[i]->mPoints[0]->mPos[0] << ", ";
			mLog << mTangle[i]->mPoints[0]->mPos[1] << ", ";
			mLog << mTangle[i]->mPoints[0]->mPos[2] << ")." << endl;
			for(unsigned int j(0); j<mTangle[i]->mPoints.size(); j++){
				delete mTangle[i]->mPoints[j];
			}
			delete mTangle[i];
			mTangle.erase(mTangle.begin()+i);
			Killed = true;
		
		}
	}
	if(Killed==true) return true;
	else return false;
}

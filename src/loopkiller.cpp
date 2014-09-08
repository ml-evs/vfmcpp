
#include "tangle.h"

using namespace std;

bool Tangle::LoopKill(){
	bool Killed(false);
	for(unsigned int i(0); i<mTangle.size(); i++){
		if(mTangle[i]->mN < 6){
			for(unsigned int j(0); j<mTangle[i]->mPoints.size(); j++){
				delete mTangle[i]->mPoints[j];
			}
			delete mTangle[i];
			mTangle.erase(mTangle.begin()+i);
			Killed = true;
			mLog << ctime(&(time(0)).substr(10,8) << "\t" << mStep << ":\t\tkilled small loop" << endl;
		}
	}
	if(Killed==true) return true;
	else return false;
}

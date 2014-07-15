
#include "tangle.h"

using namespace std;

void Tangle::LoopKill(){

	for(unsigned int i(0); i<mTangle.size(); i++){
		if(mTangle[i]->mN < 6){
			for(unsigned int j(0); j<mTangle[i]->mPoints.size(); j++){
				delete mTangle[i]->mPoints[j];
			}
			delete mTangle[i];
			mTangle.erase(mTangle.begin()+i);
		}
	}
}
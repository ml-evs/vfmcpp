/* 	Calculates the positions of dummy points used for line filaments */

#include "filament.h"
#include <cstdlib>

using namespace std;

void Filament::CalcDummy(){
    for (int j = 0; j != 3; j++){
        mDummies[0]->mPos[j] = mDummies[0]->mPrev->mPos[j] + mDummies[0]->mPrev->mSegLast[j];
        mDummies[1]->mPos[j] = mDummies[1]->mPrev->mPos[j] + mDummies[1]->mPrev->mPrev->mSegLast[j];
        mDummies[2]->mPos[j] = -1 * (mDummies[2]->mNext->mPos[j] + mDummies[2]->mNext->mNext->mSegLast[j]);
        mDummies[3]->mPos[j] = (mDummies[3]->mNext->mPos[j] - mDummies[3]->mNext->mNext->mNext->mSegLast[j]);
        if (j != 2){
            mDummies[2]->mPos[j] = -1 * mDummies[2]->mPos[j];
            mDummies[3]->mPos[j] = -1 * mDummies[3]->mPos[j];
        }
    }

    /* seglengths two away from given point needed to calculate curvature */
    for(int i(0);i<3;i++){
        mDummies[i]->mSegLength=0;
        for(int j=0;j<3;j++){
            mDummies[i]->mSegLast[j] = mDummies[i]->mPos[j] - mDummies[i]->mPrev->mPos[j];
            mDummies[i]->mSegLength += pow((mDummies[i]->mSegLast[j]),2);
        }
        mDummies[i]->mSegLength = sqrt(mDummies[i]->mSegLength);
    }
}

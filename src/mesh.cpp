/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#include "filament.h"
#include <cstdlib>

void Filament::CalcMeshLengths(){
	// mSegLength stores the distance from Nth to N-1th point
	int a;
	if(mFlagType == 1 && mPoints[0]->mPrev->mFlagDummy != true){a = 1;}
	else{a = 0;}
	for(int i=a;i<mN;i++){
        mPoints[i]->mSegLength=0;
        for(int j=0;j<3;j++){
            mPoints[i]->mSegLast[j] = mPoints[i]->mPos[j] - mPoints[i]->mPrev->mPos[j];
            mPoints[i]->mSegLength += pow((mPoints[i]->mSegLast[j]),2);
        }
        mPoints[i]->mSegLength = sqrt(mPoints[i]->mSegLength);
	}
	a = 0;
}

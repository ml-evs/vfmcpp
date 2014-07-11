/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was 
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#include "filament.h"

void Filament::CalcMeshLengths(){
	// mSegLength stores the distance from Nth to N-1th point
	for(int i=0;i<mN;i++){
		mPoints[i]->mSegLength=0;
		for(int j=0;j<3;j++){
			mPoints[i]->mSegLast[j] = mPoints[i]->mPos[j] - mPoints[i]->mPrev->mPos[j];
			mPoints[i]->mSegLength += pow((mPoints[i]->mSegLast[j]),2); 
		}
		mPoints[i]->mSegLength = sqrt(mPoints[i]->mSegLength);
	}
}
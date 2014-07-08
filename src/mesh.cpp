/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was 
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#include "filament.h"

void Filament::CalcMeshLengths(){
	int k;
	// mSegLength stores the distance from Nth to N-1th point
	for(int i=0;i<mN;i++){
		mPoints[i]->mSegLength=0;
		if(i!=0){k = i;}
		else{k=mN;}
		for(int j=0;j<3;j++){
			mPoints[i]->mSegLength += pow((mPoints[i]->mPos[j] - mPoints[k-1]->mPos[j]),2); 
		}
		mPoints[i]->mSegLength = sqrt(mPoints[i]->mSegLength);
	}
}
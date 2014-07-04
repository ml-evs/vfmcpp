/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was 
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#include "filament.h"

void Filament::CalcMeshLengths(){
	int k;
	// mSegLength stores the distance from Nth to N-1th point
	mSegLengths.resize(mN, 0); 
	for(int i=0;i<mN;i++){
		mSegLengths[i] = 0;
		if(i!=0){k = i;}
		else{k=mN;}
		for(int j=0;j<3;j++){
			mSegLengths[i] += pow((mPos[i][j] - mPos[k-1][j]),2); 
		}
		mSegLengths[i] = sqrt(mSegLengths[i]);
	}
}
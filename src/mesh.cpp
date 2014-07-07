/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was 
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#include "filament.h"

void Filament::CalcMeshLengths(){
	int k;
	// mSegLength stores the distance from Nth to N-1th point
	mSegLengths.resize(mN, 0); 
	forward_list <vector <double> >::iterator b, c, e, d;
	b = mPos.begin(); e = mPos.end(); int i(0);
	for(c=b;c!=e;c++){
		c = 0;
		if(c!=b){d=c-1;}
		else{d=e;}
		for(int j=0;j<3;j++){
			mSegLengths[i] += pow((c[j] - d[j]),2); 
		}
		mSegLengths[i] = sqrt(mSegLengths[i]);
		i++;
	}
}
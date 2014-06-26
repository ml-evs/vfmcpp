/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was 
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#include "filament.h"

void Filament::CalcMeshLengths(){
	// Flag starts at 0->N
	int flag;
	// mSegLength stores the distance from Nth to N-1th point
	mSegLengths.resize(mN, 0); 
	// Iterators to search for N-1
	vector <vector <double> >::iterator begin, current, end, next;
	// Loop to calculate distance between (flag)th and (flag+1)th point 
	for(int i=0;i<mN;i++){
		flag = int(mPos[i][3]+0.5);
		begin = mPos.begin(); end = mPos.end();
		// Find the adjacent point by flag rather than position in array
		for(current=begin; current!=end; current++){
			if(flag == 0){flag = mPos.size();}
			if(int((*current)[3]+0.5)==(flag-1)){next = current; current = end-1;}
		}
		/* 	Calculate the difference between a point and its neighbour
			and add this value to mSegLength */
		for(int j=0;j<3;j++){
			mSegLengths[i] += pow(mPos[i][j] - (*next)[j],2); 
		}
		mSegLengths[i] = sqrt(mSegLengths[i]);
	}
}
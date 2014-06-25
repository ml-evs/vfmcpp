// Calculate velocities of points on each ring
// Adapted from CalcVelMaster.m by Paul Walmsley

#include "vel.h"
#include "mesh.h"
#include <iostream>

using namespace std;

vector <vector <double> > CalcAS(vector <vector <double> > Ring, vector <double> L){
	vector <double> A;
	vector <vector <double> > AS;
	vector <double> blank; blank.push_back(0.0); blank.push_back(0.0); blank.push_back(0.0);
	int j,k,l,m;
	for(int i=0;i<Ring.size();i++){
		A.push_back(0);
		j = i; k = i; l = i; m = i;
		if(j-2==-1){j=Ring.size()+1;}
		if(j-2==-2){j=Ring.size();}
		if(k-1==-1){k=Ring.size();}
		if(l+1==100){l=-1;}
		if(m+1==100){m=-1;}
		if(m+2==100){m=-2;}
		//cout << "(" << j-2 << ", " << k-1 << ", " << i << ", " << l+1 << ", " << m+2 << ")\n";
		A[i] = L[i]*L[l+1]*L[l+1]+L[i]*L[l+1]*L[m+2];
		A[i] = A[i] / (L[k-1]*(L[k-1]+L[i])*(L[k-1]+L[i]+L[l+1])*(L[i]+L[l+1]+L[m+2]));
	}

	for(int i=0;i<Ring.size();i++){
		AS.push_back(blank);
		for(int j=0;j<3;j++){
			AS[i][j] = A[0]*Ring[i][j];
			//cout << AS[i][j] << endl;
		}

	}

	return AS;
}

vector <double> FindFlag(vector <vector <double> > Ring, int f){
	vector <vector <double> >::iterator begin, current, end, wanted;
	for(current=begin;current!=end;current++){
		if(f < 0){f = Ring.size()+f;}
		if(f >= Ring.size()){f = Ring.size()-f;}
		if(int((*current)[3]+0.1)==f){wanted = current; current = end-1;}
	}
	return (*wanted);
}


vector <vector <double> > CalcSPrime(vector <vector <double> > Ring){
	vector <vector <double> > SPrime;
	for(int i=0;i<Ring.size();i++){
		int flag = int(Ring[i][3]+0.5);
		SPrime  = CalcAS(Ring, MeshLengths(Ring));//+CalcBS()+CalcCS()+CalcDS()+CalcES())	
	}
	return SPrime;
}

vector <vector <double> > Velocity(vector <vector <double> > Ring1, vector <vector <double> > Ring2){
	// Length of filaments in each ring
	//FilLength1 = MeshLengths(Ring1); FilLength2 = MeshLengths(Ring2);
	vector <vector <double> > SPrime = CalcSPrime(Ring1);
	return SPrime;

}

/*vector <vector <double> > S2Prime(vector <vector <double> > Ring, vector <vector <double> > FilLength){

}*/



/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was 
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#include <vector>
#include <cmath>

using namespace std;

vector <double> MeshLengths(vector <vector <double> > Ring){
	
	vector <double> Res;
	vector <vector <double> > Diff;
	vector <vector <double> >::iterator begin, current, end, next;
	int flag;

	// Loop to calculate distance between (flag)th and (flag+1)th point 
	for(int i=0;i<Ring.size();i++){
		// First find the adjacent point by flag rather than position in array
		flag = int(Ring[i][3]+0.5);
		begin = Ring.begin()+i; end = Ring.end();
		for(current=begin; current!=end; current++){
			if(flag == Ring.size()){flag = -1;}
			if(int((*current)[3]+0.5)==flag+1){next = current;}
			else if(current==(end-1)){current = Ring.begin();}
		}
		/* 	Calculate the difference between a point and its neighbour
			and add this value to Res */
		Res.push_back(0);
		for(int j=0;j<3;j++){
			Diff[i][j] = Ring[i][j] - (*next)[j];
			Res[i] += pow(Diff[i][j],2); 
		}
		Res[i] = sqrt(Res[i]);
	}
	return Res;
}
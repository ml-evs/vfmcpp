/* 	Calculate mesh lengths
 	Adapted from CalcMeshLenghs.m by Paul Walmsley, which was 
 	based on work by Baggaley and Barenghi, JLTP 166,3 (2011) */

#include <vector>
#include <cmath>

using namespace std;

vector <double> MeshLengths(vector <vector <double> > Ring){
	
	// Res stores the difference between point N and N+1 
	vector <double> Res, Diff;
	// Iterators to search for N+1
	vector <vector <double> >::iterator begin, current, end;
	// Flag starts at 0->N
	int flag;

	// Loop to calculate distance between (flag)th and (flag+1)th point 
	for(int i=0;i<Ring.size();i++){
		// First find the adjacent point by flag rather than position in array
		flag = int(Ring[i][3]+0.5);
		begin = Ring.begin(); end = Ring.end();
		vector <vector <double> >::iterator next;
		for(current=begin; current!=end; current++){
			if(flag == 0){flag = Ring.size();}
			if(int((*current)[3]+0.1)==(flag-1)){next = current; current = end-1;}
		}
		/* 	Calculate the difference between a point and its neighbour
			and add this value to Res */
		Res.push_back(0); Diff.push_back(0); Diff.push_back(0);
		Diff.push_back(0);
		for(int j=0;j<3;j++){
			Diff[j] = Ring[i][j] - (*next)[j];
			Res[i] += pow(Diff[j],2); 
		}
		Res[i] = sqrt(Res[i]);

	}
	return Res;
}
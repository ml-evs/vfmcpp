#include "mesh.h"
#include "vel.h"
#include "velnl.h"
#include "velnl_of.h"
#include <iostream>
#include <cmath>

using namespace std;

	// circulation quantum, core radius, ..., mutual friction
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N1 = 100, N2=100; // number of points on each ring
	double		r0=1e-6; // initial ring radius
	const double pi = 3.14159265358979323846;

int main(){

	// ring's initial x,y,z coordinates
	vector<double> position;
	position.push_back(0);
	position.push_back(0);
	position.push_back(2.0e-6);

	// fill theta from 0 to 2pi for each ring
	double		 diff1=(2*pi)/N1, diff2=(2*pi)/N2;
	double		 theta1[N1], theta2[N2]; theta1[0]=theta2[0]=0;
	for(int i=0; i<N1; i++){
		theta1[i+1] = theta1[i]+diff1;
	}
	for(int i=0; i<N2; i++){
		theta2[i+1] = theta2[i]+diff2;
	}

	// define a vector describing each point, including positions (and flag for neighbours)
	vector < vector <double> > Ring;
	for(int i=0; i<N1; i++){
		vector<double> row;
		row.push_back(0.0);
		row.push_back(0.0);
		row.push_back(0.0);
		row.push_back(0.0);
		Ring.push_back(row);
		for(int j=0; j<3; j++){
			Ring[i][0]=position[0]+r0*sin(theta1[i]);
			Ring[i][1]=position[0]+r0*cos(theta1[i]);
			Ring[i][2]=position[2];
			Ring[i][3]=i;
		}
	}

	vector < vector <double> > Ring2;
	for(int i=0; i<N2; i++){
		vector<double> row;
		row.push_back(0.0);
		row.push_back(0.0);
		row.push_back(0.0);
		row.push_back(0.0);
		Ring2.push_back(row);
		for(int j=0; j<3; j++){
			Ring2[i][0]=position[0]+r0*sin(theta2[i]);
			Ring2[i][1]=position[0]+r0*cos(theta2[i]);
			Ring2[i][2]=position[2]+2.0e-6; // offset
			Ring2[i][3]=i;
		}
	}

/*	for(int r=0; r<N; r++){
		cout << "ring element r = " << r << "is:: (" << Ring[r][0] << ",  " << Ring[r][1] << ",  " << Ring[r][2] << ")." << endl;
	}*/

	vector <double> FilLength = MeshLengths(Ring);
	vector <double> FilLength2 = MeshLengths(Ring2);


/*	for(int r=0; r<N; r++){
		cout << "ring element r = " << r << "is:: (" << FilLength[r] << endl;
	}*/

	vector <vector <double> > SPrime = VelocityNL(Ring); 
	vector <vector <double> > SPrime2 = VelocityNL_OF(Ring, Ring2);
	//cout << "s' = " << endl;
	//for(int r=0; r<N; r++){
	//	cout << "(" << SPrime[r][0] << ", " << SPrime[r][1] << ", " << SPrime[r][2] << ")" << endl;	
	//}

	system("pause");
	return 0;

}


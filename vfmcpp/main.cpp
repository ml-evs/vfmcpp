#include "mesh.h"
#include "vel.h"
#include <iostream>
#include <cmath>

using namespace std;

	// circulation quantum, core radius, ..., mutual friction
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N = 100; // number of points on ring
	double		r0=0.5e-6; // initial ring radius


int main(){

	// ring's initial x,y,z coordinates
	vector<double> position;
	position.push_back(0);
	position.push_back(0);
	position.push_back(2.0e-6);

	// fill theta from 0 to 2pi
	const double pi = 3.14159265358979323846;
	double		 diff=(2*pi)/N;
	double		 theta[N]; theta[0]=0;
	for(int i=0; i<N; i++){
		theta[i+1] = theta[i]+diff;
//		cout << "theta[i]=" << theta[i] << endl;
	}


	// define a vector describing each point, including positions (and flag for neighbours)
	vector < vector <double> > Ring;
	for(int i=0; i<N; i++){
		vector<double> row;
		row.push_back(0.0);
		row.push_back(0.0);
		row.push_back(0.0);
		row.push_back(0.0);
		Ring.push_back(row);
		for(int j=0; j<3; j++){
			Ring[i][0]=position[0]+r0*sin(theta[i]);
			Ring[i][1]=position[0]+r0*cos(theta[i]);
			Ring[i][2]=position[2];
			Ring[i][3]=i;
		}
	}

	vector <vector <double> > SPrime = CalcSPrime(Ring, MeshLengths(Ring));
	cout << "s' = " << endl;
	for(int r=0; r<1; r++){
		cout << "(" << SPrime[r][0] << ", " << SPrime[r][1] << ", " << SPrime[r][2] << ")" << endl;	
	}
	vector <vector <double> > S2Prime = CalcS2Prime(Ring, MeshLengths(Ring));
	cout << "s'' = " << endl;
	for(int r=0; r<1; r++){
		cout << "(" << S2Prime[r][0] << ", " << S2Prime[r][1] << ", " << S2Prime[r][2] << ")" << endl;	
	}

	vector <vector <double> > vel = CalcVelocity(Ring, Ring);
	cout << "vel =" << endl;
	for(int r=0; r<1; r++){
		cout << "(" << vel[r][0] << ", " << vel[r][1] << ", " << vel[r][2] << ")" << endl;	
	}
	return 0;

}


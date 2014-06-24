#include <iostream>
#include <cmath>
#include <vector>
#include "mesh.h"

using namespace std;
using std::vector;

	// circulation quantum, core radius, ..., mutual friction
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N = 100; // number of points on ring
	double		r0=1e-6; // initial ring radius
	const double pi = 3.14159265358979323846;

int main(){

	// ring's initial x,y,z coordinates
	vector<double> position;
	position.push_back(0);
	position.push_back(0);
	position.push_back(2.0e-6);

	// fill theta from 0 to 2pi
	double		 diff=(2*pi)/N;
	double		 theta[N]; theta[0]=0;
	for(int i=0; i<N; i++){
		theta[i+1] = theta[i]+diff;
//		cout << "theta[i]=" << theta[i] << endl;
	}


	// define a vector describing each point, including positions and flag for neighbours
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

	for(int r=0; r<N; r++){
		cout << "ring element r = " << r << "is:	(" << Ring[r][0] << ",	" << Ring[r][1] << ",	" << Ring[r][2] << ")." << endl;
	}

	vector <double> FilLength = MeshLengths(Ring);


	for(int r=0; r<N; r++){
		cout << "ring element r = " << r << "is:	" << FilLength[r] << endl;
	}

	// calculate the timesteps
	double sum = 0;
	for (int s=0; s<FilLength.size(); s++){
		sum += FilLength[s];
	}
	double	delta = 4*sum/(3*N);
	double	maxdt = pow((delta/2),2)/(kappa*log10(delta/(2*pi*a0)));
	double	dt = maxdt/25; // As in Baggaley, Barenghi PRB, 2010*/
//	cout << "sum, delta, maxdt, dt are :: " << sum << ",   " << delta << ",  " << maxdt << ",  " << dt << endl;

/*	double	save=100; // how many iterations between saving to file

	double	t=0;
	int		firstloop=0, count=0, filenum=0,
	double	Nt = Ring.size();

	if count == save; // saving data to files each 100 steps
		count=0;
		filenum=filenum+1;
		ofstream savefile;
		savefile.open("Data_"filenum".txt.");
		savefile << ;
		savefile.close();*/
		system("pause");
	return 0;
}


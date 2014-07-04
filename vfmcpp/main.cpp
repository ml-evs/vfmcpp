#include "filament.h"
#include "tangle.h"
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

	// circulation quantum, core radius, ..., mutual friction
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N = 10; // number of points on ring
	double		r0=1e-6; // initial ring radius

int main(){

	// create filaments
	Ring Ring1(r0,N,0,0,0);
	Ring Ring2(r0,N,0,0,5e-7);
	//Ring Ring3(r0,N,0,0,1.1e-6);

	// add filaments to tangle
	Tangle Tangle(Ring1, Ring2); //Ring3);

	// set resolutions
	double dt, dr(0);
	vector <Filament>::iterator begin, current, end;
	begin = (Tangle.mTangle).begin();
	end = (Tangle.mTangle).end();
	for(current=begin; current!=end; current++){
		for(int j(0); j<current->mN; j++){
			dr += current->mSegLengths[j]/current->mN;
		}
	}
	dt = pow((dr/2),2)/(kappa*log(dr/(2*M_PI*a0)));
	dt = dt/25; 		// Baggaley, Barenghi PRB 2010


	// set number of timesteps and number of steps per save
	int N_t(100000);//1e-3/dt); 	// Number of time steps
	cout << "Number of time steps to be performed: " << N_t << endl;
	int N_f(10000); 			// Number of time steps per save

	string filename = "dat_timing/data_";
	double percent;
	
	for (int i(0); i<N_t; i++){
		Tangle.CalcVelocityNL_OF();

		for (current = begin; current != end; current++){
			current->CalcVelocity();
			current->PropagatePosAB3(dt);
		}

		percent = (100 * i / N_t);
		printf("\r %4.1f %%", percent);
		if (i%N_f == 0){
			string ith_filename = filename + to_string(i) + (".dat");
			ofstream outfile(ith_filename);
			outfile.width(10); outfile.precision(6);

			for (current = begin; current != end; current++){
				for (int j(0); j<current->mN; j++){
					outfile << current->mPos[j][0] << "\t";
					outfile << current->mPos[j][1] << "\t";
					outfile << current->mPos[j][2] << "\n";
				}
			}
			cout << "!!!!!!\t Wrote timestep " << i << " to file. \t!!!!!!" << endl;
			outfile.close();
		}
	}
	return 0;
}


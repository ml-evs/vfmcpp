#include "filament.h"
#include "tangle.h"
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;
	
	/* circulation quantum, core radii, mutual friction */
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N = 100; 		// number of points on ring
	double		r0=1e-6; 		// initial ring radius

int main(){

	/* create filaments */
	Ring Ring1(r0,N,0,0,5e-6);
	Ring Ring2(0.9*r0,N,0,0.05e-6,0);
	//Ring Ring3(r0,N,0,0,1.1e-6);

	/* add filaments to tangle */
	Tangle Tangle(Ring1, Ring2);

	/* set resolutions */
	double dt, dr(0);
	int N(0);
	vector <Filament>::iterator begin, current, end;
	begin = (Tangle.mTangle).begin();
	end = (Tangle.mTangle).end();
	for(current=begin; current!=end; current++){
		for(int j(0); j<current->mN; j++){
			dr += current->mPoints[j]->mSegLength;
		}
		N += current->mN;
	}
	dr /= N;
	dt = pow((dr/2),2)/(kappa*log(dr/(2*M_PI*a0)));
	dt = dt/25; 		// Baggaley, Barenghi PRB 2010
	cout << dr << ", " << dt << endl;

	/* set number of timesteps and number of steps per save */
	int N_t(1e-5/dt); 	// number of time steps
	cout << "Number of time steps to be performed: " << N_t << endl;
	int N_f(10000); 	// number of time steps per save

	string filename = "dat_race/data_"; // location of saves
	
	// prepare to time calculations
	double percent;
	clock_t t;
  	t=clock();

	for(int i(0); i<N_t; i++){
		Tangle.CalcVelocityNL_OF();
		for(current=begin; current!=end; current++){
			current->CalcVelocity();
			current->PropagatePosAB3(dt);
		}
		
		percent = (100*i/N_t);
		printf("\r %4.1f %%",percent);
		if(i%N_f==0){
			string ith_filename = filename + to_string(i) + (".dat");
			ofstream outfile(ith_filename);
			outfile.precision(8);
			for(current=begin; current!=end; current++){
				for(int j(0); j<current->mN; j++){
					for(int m(0); m<3; m++){
						outfile << current->mPoints[j]->mPos[m] << "\t";
					}
					outfile << "\n";
				}
				
			}
			cout << "!!!!!!\t Wrote timestep " << i << " to file. \t!!!!!!" << endl;
			outfile.close();
		}

	}
	t = clock()-t;
  	ofstream timefile(filename+("time.dat"));
	timefile << "Total time taken to iterate " << N_t << " time steps = " << ((float)t)/CLOCKS_PER_SEC << " s." << endl;

	return 0;
}


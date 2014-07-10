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
	const int	N  =  100; 		// number of points on ring
	double		r0 = 1e-6; 		// initial ring radius

int main(){

	/* create filaments */
	Ring Ring1(0.9*r0,N,0,0.05e-6,0);
	Ring Ring2(r0,N,0,0,5e-6);
	//Ring Ring3(r0,N,0,0,1.1e-6);

	/* add filaments to tangle */
	Tangle Tangle(Ring1, Ring2);//, Ring2);

	/* set resolutions */
	double dt, dr(0);
	int N_p(0);
	vector <Filament>::iterator begin, current, end;
	begin = (Tangle.mTangle).begin();
	end = (Tangle.mTangle).end();
	/* calculate mean distance between points */
	for(current=begin; current!=end; current++){
		for(int j(0); j<current->mN; j++){
			dr += current->mPoints[j]->mSegLength;
		}
		N_p += current->mN;
	}
	dr /= N_p;
	/* set resolution as 4/3 average distance for mesh adjust */
	dr = (4.0/3.0)*dr;
	dt = pow((dr/2),2)/(kappa*log(dr/(2*M_PI*a0)));
	dt = dt/25; 		// Baggaley, Barenghi PRB 2010
	cout << dr << ", " << dt << endl;

	/* set number of timesteps and number of steps per save */
	int N_t(1e-3/dt); 	// number of time steps
	cout << "Number of time steps to be performed: " << N_t << endl;
	int N_f(10000); 	// number of time steps per save

	string filename = "data/speedrun/data_"; // location of saves
	
	/* prepare to time calculations */
	double percent;
	clock_t t;
  	t=clock();

  	/* begin time-stepping */
	for(int i(0); i<N_t; i++){
		/* calculate velocities and propagate positions */
		Tangle.CalcVelocityNL_OF();
		for(current=begin; current!=end; current++){
			current->CalcVelocity();
			current->PropagatePosAB3(dt);
			current->MeshAdjust(dr);
		}
		percent = (100*i/N_t); 
		printf("\r %4.1f %% \t",percent); // output percentage completion
		/* save positions to file every N_f steps */
		if(i%N_f==0){
			string ith_filename = filename + to_string(i) + "_";
			int n_fil(0);
			for(current=begin; current!=end; current++){
				string ith_jth_filename = ith_filename + to_string(n_fil) + ".dat";
				ofstream outfile(ith_jth_filename);
				outfile.precision(8);
				int j(0);
				while(j!=current->mN){
					for(int m(0); m<3; m++){
						outfile << current->mPoints[j]->mNext->mPos[m] << "\t";
					}
					j++;
					outfile << "\n";
				}
				outfile.close();
				n_fil++;
			}
			cout << "!!!!!!\t Wrote timestep " << i << " to file. \t!!!!!!" << endl;
			
		}
	}
	/* save total time to file */
	t = clock()-t;
  	ofstream timefile(filename+("time.dat"));
	timefile << "Total time taken to iterate " << N_t << " time steps = " << ((float)t)/CLOCKS_PER_SEC << " s." << endl;

	return 0;
}


#include "filament.h"
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
	Ring Ring2(r0,N,0,1e-6,5e-7);

	double dt, dr(0);
	
<<<<<<< HEAD
	// set number of timesteps and number of steps per save
	int N_t(100);
	int N_f(500);

=======
>>>>>>> 38b40daef470316df85a2ddaf54dddaa6556a714
	// set spatial resolution
	for(int i(0);i<N;i++){
		dr += Ring1.GetMeshLengths()[i];
		dr += Ring2.GetMeshLengths()[i];
	}
	dr = dr / (2*N);
	Ring1.AdjMeshLengths(dr);
		
	// set temporal resolution
	dt = pow((dr/2),2)/(kappa*log(dr/(2*M_PI*a0)));
	dt = dt/25; // Baggaley, Barenghi PRB 2010


	// set number of timesteps and number of steps per save
	int N_t(1e-3/dt);
	cout << "Number of time steps required: " << N_t << endl;
	int N_f(5000);

	string filename = "dat/data_";
	double percent;

	clock_t t;
	t=clock();
	for(int i(0); i<N_t; i++){
		Ring1.CalcVelocityNL_OF(Ring2.GetPos());
<<<<<<< HEAD
		Ring2.CalcVelocityNL_OF(Ring1.GetPos());	
=======
		Ring2.CalcVelocityNL_OF(Ring1.GetPos());
>>>>>>> 38b40daef470316df85a2ddaf54dddaa6556a714
		Ring1.CalcVelocity();
		Ring2.CalcVelocity();
		Ring1.PropagatePosAB3(dt);
		Ring2.PropagatePosAB3(dt);
<<<<<<< HEAD
		//cout << "t = " << i << " steps." << endl;
		
		/*if(i%N_f==0){
=======
		percent = (100*i/N_t);
		printf("\r %4.1f %%",percent);
		if(i%N_f==0){
>>>>>>> 38b40daef470316df85a2ddaf54dddaa6556a714
			string ith_filename = filename + to_string(i) + (".dat");
			ofstream outfile(ith_filename);
			outfile.width(10); outfile.precision(6);
			for(int j(0); j<Ring1.GetN(); j++){
				outfile << Ring1.GetPos()[j][0] << "\t";
				outfile << Ring1.GetPos()[j][1] << "\t";
				outfile << Ring1.GetPos()[j][2] << "\n";
			}
			for(int j(0); j<Ring2.GetN(); j++){
				outfile << Ring2.GetPos()[j][0] << "\t";
				outfile << Ring2.GetPos()[j][1] << "\t";
				outfile << Ring2.GetPos()[j][2] << "\n";
			}
		//	cout << "!!!!!!\t Wrote timestep " << i << " to file. \t!!!!!!" << endl;
			outfile.close();
		}*/
	}
	t = clock()-t;
	ofstream timefile(filename+("time.dat"));
	timefile << "Total time = " << ((float)t)/CLOCKS_PER_SEC << " s.";
	return 0;
}


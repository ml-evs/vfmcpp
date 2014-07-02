#include "filament.h"
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

	// circulation quantum, core radius, ..., mutual friction
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N = 100; // number of points on ring
	double		r0=1e-6; // initial ring radius

int main(){

	// create filaments
	Ring Ring1(r0,N,0,0,0);

	double dt, dr;
	
	// set number of timesteps and number of steps per save
	int N_t(1);
	int N_f(50);

	// set spatial resolution
	for(int i(0);i<N;i++){
		dr += Ring1.GetMeshLengths()[i];
	}
	dr = dr / N;
	cout << dr << endl; 
	
	// set temporal resolution
	dt = pow((dr/2),2)/(kappa*log(dr/(2*M_PI*a0)));
	dt = dt/25; // Baggaley, Barenghi PRB 2010
	cout << dt << endl;
	string filename = "dat/data_";

	for(int i(0); i<N_t; i++){
		Ring1.CalcVelocity();
		Ring1.PropagatePosAB3(dt);
		//cout << "t = " << i << " steps." << endl;
		if(i%N_f==0){
			string ith_filename = filename + to_string(i) + (".dat");
			ofstream outfile(ith_filename);
			outfile.width(10); outfile.precision(6);
			for(int j(0); j<N; j++){
				outfile << Ring1.GetPos()[j][0] << "\t";
				outfile << Ring1.GetPos()[j][1] << "\t";
				outfile << Ring1.GetPos()[j][2] << "\n";
			}
			cout << "Wrote timestep " << i << " to file." << endl;
			outfile.close();
		}
	}
	return 0;
}


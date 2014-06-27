#include "filament.h"

using namespace std;

	// circulation quantum, core radius, ..., mutual friction
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N = 100; // number of points on ring
	double		r0=1e-6; // initial ring radius

int main(){

	// create filaments
	Ring Ring1(r0,N,0,0,2e-6);

	double t(0), dt, dr;
	
	// set number of timesteps
	int N_t(100);

	// set spatial resolution
	for(int i(0);i<N;i++){
		dr += Ring1.GetMeshLengths()[i];
	}
	dr = dr / N;
	
	// set temporal resolution
	dt = pow((dr/2),2)/(kappa*log(dr/(2*M_PI*a0)));
	dt = dt/25; // Baggaley, Barenghi PRB 2010
	
	//Ring1.CalcVelocity();
	Ring1.CalcVelocityNL();
	/*for(int i(0); i<N_t; i++){
		Ring1.CalcVelocity();
		Ring1.PropagatePosAB3(dt);
		cout << "t = " << t+i*dt << " s." << endl;
	}*/


	return 0;

}


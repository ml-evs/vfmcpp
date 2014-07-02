#include "filament.h"

using namespace std;

	// circulation quantum, core radius, ..., mutual friction
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N = 100; // number of points on ring
	double		r0=1e-6; // initial ring radius

int main(){

	// create filaments
	Ring Ring1(r0,N,0,0,0);

	double dt, dr;
	
	// set number of timesteps
	int N_t(10000);

	// set spatial resolution
	for(int i(0);i<N;i++){
		dr += Ring1.GetMeshLengths()[i];
	}
	dr = dr / N;
	cout << dr << endl; 
	
	// set temporal resolution
	dt = pow((dr/2),2)/(kappa*log(dr/(2*M_PI*a0)));
	dt = dt/25; // Baggaley, Barenghi PRB 2010
	
	for(int i(0); i<N_t; i++){
		Ring1.CalcVelocity();
		Ring1.PropagatePosAB3(dt);
		//cout << "t = " << i << " steps." << endl;
	}
	for(int i(0); i<N; i++) cout << "v_" << i << "= (" << Ring1.GetVel()[i][0] << ", " << Ring1.GetVel()[i][1] << ", " << Ring1.GetVel()[i][2] << ")" << endl;
	//cout << "timestep = " << dt;
	


	return 0;

}


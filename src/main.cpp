#include "filament.h"
#include "tangle.h"
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;

	// circulation quantum, core radius, ..., mutual friction
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	const int	N = 100; // number of points on ring
	double		r0=1e-6; // initial ring radius

int main(){

	// create filaments
	Ring Ring1(0.2*r0,N,0,0,5e-6);
	Ring Ring2(r0,N,0,0.05e-6,0);
	//Ring Ring3(r0,N,0,0,1.1e-6);

	// add filaments to tangle
	Tangle Tangle(Ring1, Ring2); //Ring3);

	// set resolutions
	double dt, dr(0);
	int N(0);

	// define required iterators
	vector <Filament>::iterator begin, current, end;
	begin = (Tangle.mTangle).begin();
	end = (Tangle.mTangle).end();
	list <vector <double> >::iterator bpos, cpos, epos;

	for(current=begin; current!=end; current++){
		for(int j(0); j<current->mN; j++){
			dr += current->mSegLengths[j];
			N += current->mN;
		}
	}
	dr /= N;
	dt = pow((dr/2),2)/(kappa*log(dr/(2*M_PI*a0)));
	dt = dt/25; 		// Baggaley, Barenghi PRB 2010
	cout << dr << ", " << dt << endl;

	// set number of timesteps and number of steps per save
	int N_t(1); 	// Number of time steps
	cout << "Number of time steps to be performed: " << N_t << endl;
	int N_f(10000); 	// Number of time steps per save

	string filename = "data/race_data_test/data_";
	double percent;
	

	// prepare to time calculations
  	clock_t t;
  	t=clock();

	for(int i(0); i<N_t; i++){
		/*Tangle.CalcVelocityNL_OF();
		for(current=begin; current!=end; current++){
			current->CalcVelocity();
			current->PropagatePosAB3(dt);
		}*/
		
		percent = (100*i/N_t);
		printf("\r %4.1f %%",percent);
		if(i%N_f==0){
			string ith_filename = filename + to_string(i) + (".dat");
			ofstream outfile(ith_filename);
			outfile.precision(8);
			for(current=begin; current!=end; current++){
				bpos = current->mPos.begin(); epos = current->mPos.end();
				for(cpos=bpos; cpos!=epos; cpos++){
					for(int m(0); m<3; m++){
						outfile << (*cpos)[m] << "\t";
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


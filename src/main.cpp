#include "filament.h"
#include "tangle.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>

using namespace std;
	
	/* circulation quantum, core radii, mutual friction */
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0;
	int			N  = 100; 		// number of points on ring
	double		r0 = 1e-6; 		// initial ring radius

int main(){

	/* add filaments to tangle */
	//Tangle Tangle(new Ring(r0, N, 0, 0, 5e-6), new Ring(0.9*r0, N, 0.28e-6, 0, 0));
	Tangle Tangle;
	Tangle.FromFile();


	/* set resolutions */
	double dt, dr(0);
	int N_p(0);
	vector <Filament*>::iterator begin, current, end;
	begin = Tangle.mTangle.begin();
	end = Tangle.mTangle.end();
	/* calculate mean distance between points */
	for(current=begin; current!=end; current++){
		for(int j(0); j<(*current)->mN; j++){
			dr += (*current)->mPoints[j]->mSegLength;
		}
		N_p += (*current)->mN;
	}
	dr /= N_p;
	/* set resolution as 4/3 average distance for mesh adjust */
	dr = (4.0/3.0)*dr;
	dt = pow((dr/2),2)/(kappa*log(dr/(2*PI*a0)));
	dt = dt/15; 		// Baggaley, Barenghi PRB 2010
	cout << dr << ", " << dt << endl;
	
	dr = 7.95739e-8; dt = 1.3856e-10;

	Tangle.mDr = dr; Tangle.mDt = dt;

	/* set number of timesteps and number of steps per save */
	int N_t(0.8e-3/Tangle.mDt); 				// number of time steps
	cout << "Number of time steps to be performed: " << N_t << endl;
	Tangle.mN_f = 10000; 			// number of time steps per save
	Tangle.mN_slow = 0; 					// counts how many steps have occurred at slow-mo
	string filename = "../bin/data/new_recon_test/data_"; // location of saves
	
	/* prepare to time calculations */
	double percent;
	clock_t t;
  	t=clock();
  	int file_no(0);

  	/* begin time-stepping */
  	int i(0);
	while(i*Tangle.mDt < N_t*dt){
		begin = Tangle.mTangle.begin();
		end = Tangle.mTangle.end();
		percent = (100*i/N_t); 
		printf("\r %4.1f %% \t",percent); 						// output percentage completion
		if(Tangle.mN_slow == 30){Tangle.mN_f = 10;} 			// reset saving after reconnection 
		if(Tangle.mN_slow == 500){Tangle.mN_f = 100;}
		if(Tangle.mN_slow == 50000){Tangle.mN_f = 10000;}
		if(Tangle.mN_f==1||Tangle.mN_f == 10||Tangle.mN_f == 100){Tangle.mN_slow++;}  // increment slow-mo counter
		else{Tangle.mN_slow = 0;} 								// reset slow-mo counter
		/* save positions to file every mN_f steps */
		if(i%Tangle.mN_f==0){
			stringstream ss0;
			ss0 << file_no;
			string i_str = ss0.str();
			string ith_filename = filename + i_str + "_";
			int n_fil(0);

			for(current=begin; current!=end; current++){
				stringstream ss;
				ss << n_fil;
				string n_fil_str = ss.str();
				string ith_jth_filename = ith_filename + n_fil_str + ".dat";
				ofstream outfile(ith_jth_filename.c_str());
				outfile.precision(8);
				outfile << i*Tangle.mDt << "\n";
				int j(0);
				Point* pCurrent = (*current)->mPoints[0];
				while(j!=(*current)->mN){
					for(int m(0); m<3; m++){
						outfile << pCurrent->mPos[m] << "\t";
					}
					pCurrent = pCurrent->mNext;
					j++;
					outfile << "\n";
				}
				outfile.close();
				n_fil++;
			}
			cout << "!!!!!!\t Wrote timestep " << i << " to file. \t!!!!!!" << endl;
			file_no++;
		}
		/* calculate velocities and propagate positions */
		Tangle.LoopKill();
		bool MeshFinished(false);
		while(MeshFinished==false){
			MeshFinished = Tangle.MeshAdjust();
		}
		Tangle.Reconnection();
		Tangle.CalcVelocityNL_OF(); 
		Tangle.CalcVelocity();					// calculates all local contributions and combines with non-local
		Tangle.PropagatePos(Tangle.mDt);
		i++;
	}
	/* save total time to file */
	t = clock()-t;
	filename = filename + "time.dat";
  	ofstream timefile(filename.c_str());
	timefile << "Total time taken to iterate " << N_t << " time steps = " << ((float)t)/CLOCKS_PER_SEC << " s." << endl;

	return 0;
}


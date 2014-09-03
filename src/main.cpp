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
	double		kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0, alpha=0, q_e=1.6e-19;
	int				N  = 100; 		// number of points on ring
	double		r0 = 1e-6; 		// default initial ring radius
	double 		t_total = 1e-3; // default total time


int main(int argc, char* argv[]){

	/* initialise tangle  */
	Tangle Tangle;

	string runfile;
	if(argc!=1){runfile = argv[1];}
	else runfile = "NULL";
	string filename = Tangle.Initialise(runfile);

	cout << "\t    spatial resolution = "<< Tangle.mDr << " m" << endl;
	cout << "\t    time-step = " << Tangle.mDt << " s\n\n";

	/* set number of timesteps and number of steps per save */

	int N_t(t_total/Tangle.mDt); 	// number of time steps
	Tangle.mN_f = 10000; 		// number of time steps per save
	Tangle.mN_slow = 0; 		// counts how many steps have occurred at slow-mo
	Tangle.mDr *= 4.0/3.0;	// augments resolution for mesh adjust stability

	/* prepare to time calculations */
	double percent;
	clock_t t;
	t=clock();
	int file_no(0);

	vector <Filament*>::iterator begin, current, end;
	/* begin time-stepping */
	int i(0);
	int added_rings(0);
	cout << "\t - - - - - - -    BEGINNING SIMULATION    - - - - - - - -\n\n";
	while(i < N_t){
		begin = Tangle.mTangle.begin();
		end = Tangle.mTangle.end();
		
		if(Tangle.mN_slow == 15){Tangle.mN_f = 10;} 	// reset saving after reconnection 
		if(Tangle.mN_slow == 200){Tangle.mN_f = 100;}
		if(Tangle.mN_slow == 5000){Tangle.mN_f = 10000;}
		if(Tangle.mN_f == 1
			||Tangle.mN_f == 10
			||Tangle.mN_f == 100){Tangle.mN_slow++;}  // increment slow-mo counter
		else{Tangle.mN_slow = 0;} 	// reset slow-mo counter

		/* save positions to file every mN_f steps */
		if(i%Tangle.mN_f==0){
			int n_fil(0);
			stringstream ss0; ss0 << file_no; string i_str = ss0.str();
		//	string ith_filename = filename + "/data_" + i_str + "_"; // can use to feed in initial state
			string ith_filename = filename + i_str + "_";
			for(current=begin; current!=end; current++){
				stringstream ss; ss << n_fil;	string n_fil_str = ss.str(); 
				string ith_jth_filename = ith_filename + n_fil_str + ".dat";
				ofstream outfile(ith_jth_filename.c_str());	outfile.precision(8);
				outfile << i*Tangle.mDt << "\n"; 
				int j(0);
				Point* pCurrent = (*current)->mPoints[0];
				while(j!=(*current)->mN+1-(*current)->mFlagType){
					for(int m(0); m<3; m++){
						outfile << pCurrent->mPos[m] << "\t";
					}
					pCurrent = pCurrent->mNext; j++; outfile << "\n";
				}
				outfile.close(); n_fil++;
			}
			percent = (100*i/N_t); 
			printf("\r\t %6.2f %% \t",percent); // output percentage completion
			printf("\t\t wrote step %6u", i);		// note printf does not play well with HPC
			file_no++;
	
		}
		if(added_rings==0){
			if(i*Tangle.mDt > 0.00032){
				Tangle.mTangle.push_back(new Ring(Tangle.mDr, 0.9e-6, 0, 0, 20e-6, 2))
				added_rings++;
			}
		}
		/* calculate velocities and propagate positions */
		Tangle.LoopKill();
		bool MeshFinished(false);	// remove rings smaller than 6 points
		while(MeshFinished==false) MeshFinished = Tangle.MeshAdjust();  // mesh_adjust until finished
		Tangle.Reconnection();		// check for and perform reconnections 
		Tangle.CalcVelocity(); 		// calculates and combines all contributions to velocity
		if(Tangle.mEFieldAmp !=0 && i*Tangle.mDt < Tangle.mEFieldDuration){
			Tangle.CalcField();			// add field contribution to charged point
		}				
		Tangle.PropagatePos(Tangle.mDt);	// propagate positions
		i++;	// step forward

		/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	}
	cout << "\n\t - - - - - - -    SIMULATION FINISHED    - - - - - - - -"; 
	ofstream timefile(filename+"time.dat");
	t = clock()-t;
	timefile	 << "time elapsed = " << ((float)t)/CLOCKS_PER_SEC << " s " << endl;
	timefile << "number of recons = " << Tangle.mN_recon << endl;
	timefile.close();
	return 0;
}

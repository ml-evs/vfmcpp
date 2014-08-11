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
	int				N  = 100; 		// number of points on ring
	double		r0 = 1e-6; 		// default initial ring radius
	double 		t_total = 1e-3; // default total time

int main(int argc, char* argv[]){

	/* initialise tangle and print banner */
	Tangle Tangle;

	string runfile;
	if(argc!=1){runfile = argv[1];}
	else runfile = "NULL";
	string filename = Tangle.Initialise(runfile);

	/*string filename = "../init/init_state/015_09/015_09_rec";
	Tangle.FromFile(filename);*/

	/* set resolutions */
	double dt, dr(0);
	int N_p(0);
	vector <Filament*>::iterator begin, current, end;
	begin = Tangle.mTangle.begin();
	end = Tangle.mTangle.end();

	double perturb = r0/10;	
	for(current=begin; current!=end; current++){
		if((*current)->mFlagType==1){
			(*current)->mPoints[N/2]->mPos[0] = perturb;
			(*current)->mPoints[N/2-1]->mPos[0] = 0.95*perturb;
  		(*current)->mPoints[N/2+1]->mPos[0] = 0.95*perturb;
  		(*current)->mPoints[N/2-2]->mPos[2] += 1.5*r0/N;
			(*current)->mPoints[N/2+2]->mPos[2] -= 1.5*r0/N;
  	}
	}
	
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
	dt = dt/25; 													// Baggaley, Barenghi PRB 2010
	

	cout << "\t    spatial resolution = "<< dr << " m" << endl;
	cout << "\t    time-step = " << dt << " s\n\n";

	Tangle.mDr = dr; Tangle.mDt = dt;

	/* set number of timesteps and number of steps per save */

	int N_t(t_total/Tangle.mDt); 					// number of time steps
	Tangle.mN_f = 10000; 									// number of time steps per save
	Tangle.mN_slow = 0; 									// counts how many steps have occurred at slow-mo
	
	/* prepare to time calculations */
	double percent;
	clock_t t;
  t=clock();
  int file_no(0);

  /* begin time-stepping */
  int i(0);
 	cout << "\t - - - - - - -    BEGINNING SIMULATION    - - - - - - - -\n\n";
	while(i*Tangle.mDt < N_t*dt){
		
		begin = Tangle.mTangle.begin();
		end = Tangle.mTangle.end();
		
		percent = (100*i/N_t); 
		printf("\r\t %6.2f %% \t",percent); 							// output percentage completion
		
		if(Tangle.mN_slow == 30){Tangle.mN_f = 10;} 			// reset saving after reconnection 
		if(Tangle.mN_slow == 2000){Tangle.mN_f = 100;}
		//if(Tangle.mN_slow == 5000){Tangle.mN_f = 10000;}
		if(Tangle.mN_f==1
			||Tangle.mN_f == 10
			||Tangle.mN_f == 100){Tangle.mN_slow++;}  			// increment slow-mo counter
		else{Tangle.mN_slow = 0;} 												// reset slow-mo counter

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
				outfile << i*Tangle.mDt << "\n"; int j(0);
				Point* pCurrent = (*current)->mPoints[0];
				while(j!=(*current)->mN+1-(*current)->mFlagType){
					for(int m(0); m<3; m++){
						outfile << pCurrent->mPos[m] << "\t";
					}
					pCurrent = pCurrent->mNext; j++; outfile << "\n";
				}
				outfile.close(); n_fil++;
			}
			printf("\t\t wrote step %6u", i);;
			file_no++;
		}

		/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
		/* calculate velocities and propagate positions */

		bool MeshFinished(false);
		Tangle.LoopKill();																							// remove rings smaller than 6 points
		while(MeshFinished==false) MeshFinished = Tangle.MeshAdjust();  // mesh_adjust until finished
		Tangle.Reconnection();	 																				// check for and perform reconnections 
		Tangle.CalcVelocity(); 																					// calculates and combines all contributions to velocity
		Tangle.PropagatePos(Tangle.mDt);																// propagate positions

		i++;						// step forward

		/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	}

	t = clock()-t;
	cout << "\n\t - - - - - - -    SIMULATION FINISHED    - - - - - - - -"; 
	cout << "\n\t    time elapsed = " << ((float)t)/CLOCKS_PER_SEC << " s " << endl;
	return 0;
}


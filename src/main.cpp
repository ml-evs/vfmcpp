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
	int				N  = 100; // number of points on ring
	double		r0 = 1e-6; // default initial ring radius
	double 		t_total = 1e-3; // default total time


int main(int argc, char* argv[]){

	/* initialise tangle  */
	Tangle Tangle;
	
	string runfile;
	if(argc!=1){runfile = argv[1];}
	else runfile = "NULL";
	string filename = Tangle.Initialise(runfile);

	/* set number of timesteps and number of steps per save */
	int N_t(t_total/Tangle.mDt); // number of time steps
	Tangle.mN_f = 10000; // number of time steps per save
	Tangle.mN_slow = 0; // counts how many steps have occurred at slow-mo

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
	Tangle.mLog << Tangle.StringTime() << "\t\t\t\tsimulation begins" << endl;
	while(i < N_t){
		Tangle.mStep = i;
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
			Tangle.Output(filename, i, file_no);
			percent = (100*i/N_t);
			printf("\r\t %6.2f %% \t",percent); // output percentage completion
			printf("\t\t wrote step %6u", i);		// note printf does not play well with HPC
			Tangle.mLog << Tangle.StringTime() << "\t" << Tangle.mStep << ":\t\twrote step" << endl;
			file_no++;
		}
		/* SECONDARY, TERTIARY AND QUATERNARY COLLISIONS 
		if(added_rings==0){
			if(i*Tangle.mDt > 0.00032){
				Tangle.mTangle.push_back(new Ring(Tangle.mDr, 0.9e-6, 0, 0.4e-6, 20e-6, 2));
				added_rings++;;
				Tangle.mLog << Tangle.StringTime() << "\tadded a new ring" << endl
			}
		}
		if(added_rings==1){
			if(i*Tangle.mDt > 0.0004){
				Tangle.mTangle.push_back(new Ring(Tangle.mDr, 0.9e-6, 0, 0.6e-6, 20e-6, 2));
				added_rings++;
				Tangle.mLog << Tangle.StringTime() << "\tadded a new ring" << endl
			}
		}
		if(added_rings==2){
			if(i*Tangle.mDt > 0.0005){
				Tangle.mTangle.push_back(new Ring(Tangle.mDr, 0.9e-6, 0, 0.8e-6, 20e-6, 2));
				added_rings++;
				Tangle.mLog << Tangle.StringTime() << "\tadded a new ring" << endl
			}
		}*/
		/* calculate velocities and propagate positions */
		bool LoopKilled = Tangle.LoopKill();
		if(LoopKilled == true) Tangle.mN_loopkills++;
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
	Tangle.mLog << Tangle.StringTime() << "\t\t\t\tsimulation finished" << endl;
	ofstream timefile(filename+"time.dat");
	t = clock()-t;
	timefile << "time elapsed = " << ((float)t)/CLOCKS_PER_SEC << " s " << endl;
	timefile << "number of recons = " << Tangle.mN_recon << endl;
	timefile << "number of loop kills = " << Tangle.mN_loopkills << endl;
	Tangle.mLog	<< Tangle.StringTime() << "\t\t\t\ttime elapsed = " << ((float)t)/CLOCKS_PER_SEC << " s " << endl;
	Tangle.mLog << Tangle.StringTime() << "\t\t\t\tnumber of recons = " << Tangle.mN_recon << endl;
	Tangle.mLog << Tangle.StringTime() << "\t\t\t\tnumber of loop kills = " << Tangle.mN_loopkills << endl;
	timefile.close(); Tangle.mLog.close();
	return 0;
}

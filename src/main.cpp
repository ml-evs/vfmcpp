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
	
int main(int argc, char* argv[]){

	/* initialise tangle  */
	Tangle Tangle;
	
	string runfile;
	if(argc!=1){runfile = argv[1];}
	else runfile = "NULL";
	string filename = Tangle.Initialise(runfile);

/*	string filename = "../init/init_state/015_09/015_09_rec";
	Tangle.FromFile(filename);
	filename = "../data/015_09_old/data_";*/

	/* set number of timesteps and number of steps per save */
	int N_t(Tangle.mTotalTime/Tangle.mDt); // number of time steps
	Tangle.mN_f = 10000; // number of time steps per save
	Tangle.mN_slow = 0; // counts how many steps have occurred at slow-mo

	vector <Filament*>::iterator begin, current, end;

	/* prepare to time calculations */
	double percent;
	double us_Dt(Tangle.mDt * 1e6);
	clock_t t, t_temp;
	t=clock();
	int file_no(0);
	/* begin time-stepping */
	int i(0);
	cout << "\n\t - - - - - - - -    BEGINNING SIMULATION    - - - - - - - -\n\n";
	Tangle.mLog << Tangle.StringTime() << "\t\t\t\t\tsimulation begins" << endl;
	while(i < N_t){
		Tangle.mStep = i;
		begin = Tangle.mTangle.begin();
		end = Tangle.mTangle.end();
		if(Tangle.mN_slow == 15){Tangle.mN_f = 10;} 	// reset saving after reconnection 
		if(Tangle.mN_slow == 200){Tangle.mN_f = 100;}
		if(Tangle.mN_slow == 5000){Tangle.mN_f = 10000;}
		if(Tangle.mN_f == 1
			||Tangle.mN_f == 10
			||Tangle.mN_f == 100){Tangle.mN_slow++;}	// increment slow-mo counter
		else{Tangle.mN_slow = 0;}						// reset slow-mo counter
		if(i%100==0 || i%Tangle.mN_f==0){
			percent = (100*i/N_t);
			printf("\t\t\r %6.2f %% \t",percent); // output percentage completion
		}
		/* save positions to file every mN_f steps */
		if(i%Tangle.mN_f==0){
			Tangle.Output(filename, i, file_no);
			t_temp = clock() -t;
			printf("\t\t wrote step %6u", i);
			Tangle.mLog << Tangle.StringTime() << "\t" << setw(10) << Tangle.mStep;
			Tangle.mLog << "\telapsed: " << ((float)t_temp)/CLOCKS_PER_SEC << " s:\t\twrote to file " << file_no << " for time " << i*us_Dt << " us" << endl;
			file_no++; 
		}
		/* adjust mesh until finished */
		bool MeshFinished(false);	
		while(MeshFinished==false) MeshFinished = Tangle.MeshAdjust(); 
		/* remove rings smaller than 6 points and count them */
		bool LoopKilled = Tangle.LoopKill(); 
		if(LoopKilled == true) Tangle.mN_loopkills++;
		/* check for and perform reconnections if required */
		Tangle.Reconnection();
		/* calculate velocities and propagate positions */
		Tangle.CalcVelocityNL();	// calculates non-local contributions to velocity
		Tangle.CalcVelocity(); 		// calculates local contributions to velocity
/*		if(Tangle.mEFieldAmp !=0 && i*Tangle.mDt < Tangle.mEFieldDuration){
			Tangle.CalcField();			// add field contribution to charged point
		}*/
		Tangle.PropagatePos(Tangle.mDt);	// propagate positions
		i++;	// step forward

	}
	cout << "\n\t - - - - - - -    SIMULATION FINISHED    - - - - - - - -"; 
	Tangle.mLog << Tangle.StringTime() << "\t\t\t\tsimulation finished" << endl;
	ofstream timefile(filename+"/time.dat");
	t = clock()-t;
	timefile << "time elapsed = " << ((float)t)/CLOCKS_PER_SEC << " s " << endl;
	timefile << "number of recons = " << Tangle.mN_recon << endl;
	timefile << "number of loop kills = " << Tangle.mN_loopkills << endl;
	Tangle.mLog	<< Tangle.StringTime() << "\t\t\t\ttime elapsed = " << ((float)t)/CLOCKS_PER_SEC << " s " << endl;
	Tangle.mLog << Tangle.StringTime() << "\t\t\t\tnumber of recons = " << Tangle.mN_recon << endl;
	Tangle.mLog << Tangle.StringTime() << "\t\t\t\tnumber of loop kills = " << Tangle.mN_loopkills << endl;
	timefile.close(); 
	Tangle.mLog.close();
	return 0;
}

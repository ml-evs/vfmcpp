#ifndef GUARD_TANGLE_H
#define GUARD_TANGLE_H

#include "filament.h"
#include <vector>
#include <cmath>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <array>

using namespace std;

class Tangle{
public:
	/* member data */
	vector <Filament*> 	mTangle; // store all active filaments
	vector <Filament*> 	mDelayed; // store filaments to be added later
	vector <double> 	mDelayedTimes; // store times to add filaments
	bool mDelayFlag;
	int mN_f;	// number of timesteps between saves
	int mN_slow; // number of "slow-mo" timesteps for file save
	int mN_recon; // number of reconnections
	int mN_loopkills;	// number of tiny loops deleted
	int mStep; // current step
	double mDr;	// spatial resolution
	double mDt; // temporal resolution
	double mTotalTime; // total time
	double mEFieldAmp; // magnitude of E-field
	double mEFieldDuration; // duration of E-field
	int 	mEFieldDirection; // direction of E-field (0,1,2)->(x,y,z)
	ofstream mLog; // log file records all events
	/* member functions */
	Tangle(){
		mN_recon = 0; mN_loopkills = 0; mDelayFlag = false;
	}
	Tangle(Filament* String1){
		mTangle.push_back(String1);
		mN_f = 10000;
		mN_recon = 0;
	}
	Tangle(Filament* Ring1, Filament* Ring2){
		mTangle.push_back(Ring1);
		mTangle.push_back(Ring2);
		mN_f = 10000;
		mTotalTime = 1e-3; // default to 1 ms
		mN_recon = 0;
		mN_loopkills = 0;
	}
	~Tangle(){};
	string Initialise(string runfile);
	void CalcVelocityNL();
	bool LoopKill();
	void Reconnection();
	void SelfReconnect(int P, int Q, int k, int l);
	void SelfReconnectLine(int P, int Q, int k, int l);
	void Reconnect(int P, int Q, int k, int l);
	void PropagatePos(double & dt); 
	bool MeshAdjust();
	int ReconnectionTest();
	void CalcVelocity();
	void CalcField();
	void Output(string filename, int i, int file_no);
	const string StringTime() {
	    time_t     now = time(0);
	    struct tm  tstruct;
	    char       buf[80];
	    tstruct = *localtime(&now);
	    strftime(buf, sizeof(buf), "%m-%d.%X", &tstruct);
	    return buf;
	}
};

#endif

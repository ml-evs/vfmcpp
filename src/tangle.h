#ifndef GUARD_TANGLE_H
#define GUARD_TANGLE_H

#include "filament.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Tangle{
public:
	/* member data */
	vector <Filament*> 	mTangle;
	int mN_f;				// number of timesteps between saves
	int mN_slow;
	int mN_recon;		// number of reconnections
	double mDr;			// spatial resolution
	double mDt; 		// temporal resolution
	double mTotalTime;	// total time
	/* member functions */
	Tangle(){};
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

	}
	~Tangle(){};
	string Initialise(string runfile);
	void CalcVelocityNL();
	bool LoopKill();
	void Reconnection();
	void CalcField(double eamp, int edir, double dr);
	void SelfReconnect(int P, int Q, int k, int l);
	void SelfReconnectLine(int P, int Q, int k, int l);
	void Reconnect(int P, int Q, int k, int l);
	void PropagatePos(double & dt); 
	bool MeshAdjust();
	int ReconnectionTest();
	void CalcVelocity();
	void FromFile(string base); 	// mostly for debug purposes
	void SaveState(string base);
};

#endif
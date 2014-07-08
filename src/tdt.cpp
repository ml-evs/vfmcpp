// calculate new positions after time step
// adapted from CalcVelMaster.m by Paul Walmsley

#include "filament.h"
using namespace std;

// propagate positions using Adams-Bashforth
void Filament::PropagatePosAB3(double & dt){
	for(int i(0);i<mN;i++){
		for(int j(0);j<3; j++){
			mPoints[i]->mPos[j] += (dt/12)*(23*mPoints[i]->mVel[j] - 16*mPoints[i]->mVel1[j] + 5*mPoints[i]->mVel2[j]);
		}
	}
}




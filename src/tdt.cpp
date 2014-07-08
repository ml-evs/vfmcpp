// calculate new positions after time step
// adapted from CalcVelMaster.m by Paul Walmsley

#include "filament.h"
using namespace std;

// propagate positions using Adams-Bashforth
/*void Filament::PropagatePosAB3(double & dt){
	for(int i(0);i<mN;i++){
		for(int j(0);j<3; j++){
			mPos[i][j] += (dt/12)*(23*mVel[i][j] - 16*mVel1[i][j] + 5*mVel2[i][j]);
		}
	}
}


*/


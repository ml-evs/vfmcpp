// calculate new positions after time step
// adapted from CalcVelMaster.m by Paul Walmsley

#include "filament.h"
using namespace std;

// propagate positions using appropriate order method
void Filament::PropagatePos(double & dt){
	for(int i(0);i<mN;i++){
		/* if velocities have already been filled 3 steps back, do AB4 */
		if(mPoints[i]->mFlagFilled==4){
			for(int j(0);j<3; j++){
				mPoints[i]->mPos[j] += (dt/24)*(55*mPoints[i]->mVel[j] - 59*mPoints[i]->mVel1[j] + 37*mPoints[i]->mVel2[j] - 9*mPoints[i]->mVel3[j]);
			}
		}
		/* if velocities have already been filled 2 steps back, do AB3 */
		if(mPoints[i]->mFlagFilled==3){
			for(int j(0);j<3; j++){
				mPoints[i]->mPos[j] += (dt/12)*(23*mPoints[i]->mVel[j] - 16*mPoints[i]->mVel1[j] + 5*mPoints[i]->mVel2[j]);
			}
		}
		/* if velocities have only been filled 1 step back, do AB2 */
		else if(mPoints[i]->mFlagFilled==2){
			for(int j(0);j<3; j++){
				mPoints[i]->mPos[j] += (dt/2)*(3*mPoints[i]->mVel[j] - mPoints[i]->mVel1[j]);
			}
		}
		/* if no previous velocities, use Euler */
		else if(mPoints[i]->mFlagFilled==1){
			for(int j(0);j<3; j++){
				mPoints[i]->mPos[j] += dt * mPoints[i]->mVel[j];
			}
		}

	}
}




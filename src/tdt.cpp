// calculate new positions after time step
// adapted from CalcVelMaster.m by Paul Walmsley

#include "tangle.h"
using namespace std;

// propagate positions using appropriate order method
void Tangle::PropagatePos(double & dt){
	vector <Filament*>::iterator b, c, e;
	b = mTangle.begin(); e = mTangle.end();
	for(c=b; c!=e; c++){
		for(int i(0);i<(*c)->mN;i++){
			/* if velocities have already been filled 3 steps back, do AB4 */
			if((*c)->mPoints[i]->mFlagFilled==4){
				for(int j(0);j<3; j++){
					(*c)->mPoints[i]->mPos[j] += (dt/24)*(55*(*c)->mPoints[i]->mVel[j] - 59*(*c)->mPoints[i]->mVel1[j] + 37*(*c)->mPoints[i]->mVel2[j] - 9*(*c)->mPoints[i]->mVel3[j]);
				}
			}
			/* if velocities have already been filled 2 steps back, do AB3 */
			if((*c)->mPoints[i]->mFlagFilled==3){
				for(int j(0);j<3; j++){
					(*c)->mPoints[i]->mPos[j] += (dt/12)*(23*(*c)->mPoints[i]->mVel[j] - 16*(*c)->mPoints[i]->mVel1[j] + 5*(*c)->mPoints[i]->mVel2[j]);
				}
			}
			/* if velocities have only been filled 1 step back, do AB2 */
			else if((*c)->mPoints[i]->mFlagFilled==2){
				for(int j(0);j<3; j++){
					(*c)->mPoints[i]->mPos[j] += (dt/2)*(3*(*c)->mPoints[i]->mVel[j] - (*c)->mPoints[i]->mVel1[j]);
				}
			}
			/* if no previous velocities, use Euler */
			else if((*c)->mPoints[i]->mFlagFilled==1){
				for(int j(0);j<3; j++){
					(*c)->mPoints[i]->mPos[j] += dt * (*c)->mPoints[i]->mVel[j];
				}
			}

		}
	}
}




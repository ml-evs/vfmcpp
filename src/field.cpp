/* calculate velocity contribution from electric field, eq(4) from Tsubota and Adachi JLTP 158 p364 (2010) */

#include "tangle.h"

void Tangle::CalcField(){

	/* circulation quantum, superfluid density, charge */
	double		kappa = 9.98e-8, rho = 145, q_e=1.6e-19;
	double      K = 2*q_e/(rho*kappa);

	vector <Filament*>::iterator b, c, e;
	b = mTangle.begin(); e = mTangle.end();

	for(c=b; c!=e; c++){
		if((*c)->mCarriesCharge != 0){
			for(int p(0); p!=(*c)->mN; p++){
				if((*c)->mPoints[p]->mCharge!=0){
					if(mEFieldDirection == 0){
						(*c)->mPoints[p]->mVel[1] += K*((*c)->mPoints[p]->mSPrime[2]*mEFieldAmp) / ((*c)->mPoints[p]->mSegLength + (*c)->mPoints[p]->mSegLength);
						(*c)->mPoints[p]->mVel[2] -= K*((*c)->mPoints[p]->mSPrime[1]*mEFieldAmp) / ((*c)->mPoints[p]->mSegLength + (*c)->mPoints[p]->mSegLength);
					}
					else if(mEFieldDirection == 1){
						(*c)->mPoints[p]->mVel[0] -= K*((*c)->mPoints[p]->mSPrime[2]*mEFieldAmp) / ((*c)->mPoints[p]->mSegLength + (*c)->mPoints[p]->mSegLength);
						(*c)->mPoints[p]->mVel[2] += K*((*c)->mPoints[p]->mSPrime[0]*mEFieldAmp) / ((*c)->mPoints[p]->mSegLength + (*c)->mPoints[p]->mSegLength);
					}
					else if(mEFieldDirection == 2){
						(*c)->mPoints[p]->mVel[0] += K*( (*c)->mPoints[p]->mSPrime[1]*mEFieldAmp) / ((*c)->mPoints[p]->mSegLength + (*c)->mPoints[p]->mSegLength);
						(*c)->mPoints[p]->mVel[1] -= K*( (*c)->mPoints[p]->mSPrime[0]*mEFieldAmp) / ((*c)->mPoints[p]->mSegLength + (*c)->mPoints[p]->mSegLength);
					}
				}
			}
		}
	}
}
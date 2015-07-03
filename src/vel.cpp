/* Calculate velocities of points in mPoints of a filament.
   Adapted from CalcVelMaster.m by Paul Walmsley */

#include "filament.h"
#include "tangle.h"

using namespace std;

const double	kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0;

/* calculate velocity at each point from s' and s'' eq(2) from Hanninen and Baggaley PNAS 111 p4667 (2014) */
void Tangle::CalcVelocity(){
	/* circulation quantum, core radius */
	vector <Filament*>::iterator b, c, e;
	b = mTangle.begin(); e = mTangle.end();
	for(c=b; c!=e; c++){
		
		for(int i(0);i<(*c)->mN;i++){
			for(int j(0);j!=3;j++){
				(*c)->mPoints[i]->mVel3[j] = (*c)->mPoints[i]->mVel2[j];
				(*c)->mPoints[i]->mVel2[j] = (*c)->mPoints[i]->mVel1[j];
				(*c)->mPoints[i]->mVel1[j] = (*c)->mPoints[i]->mVel[j];
			}
		}
		(*c)->CalcMeshLengths(); 
		(*c)->CalcSPrime(); 
		(*c)->CalcS2Prime();
		for(int i=0;i<(*c)->mN;i++){
			if((*c)->mPoints[i]->mFlagFilled==3){(*c)->mPoints[i]->mFlagFilled++;}
			if((*c)->mPoints[i]->mFlagFilled==2){(*c)->mPoints[i]->mFlagFilled++;}
			if((*c)->mPoints[i]->mFlagFilled==1){(*c)->mPoints[i]->mFlagFilled++;}
			if((*c)->mPoints[i]->mFlagFilled==0){(*c)->mPoints[i]->mFlagFilled++;}
			(*c)->mPoints[i]->mVel[0] = (((*c)->mPoints[i]->mSPrime[1])*((*c)->mPoints[i]->mS2Prime[2]) - ((*c)->mPoints[i]->mSPrime[2])*((*c)->mPoints[i]->mS2Prime[1]));
			(*c)->mPoints[i]->mVel[1] = (((*c)->mPoints[i]->mSPrime[2])*((*c)->mPoints[i]->mS2Prime[0]) - ((*c)->mPoints[i]->mSPrime[0])*((*c)->mPoints[i]->mS2Prime[2]));
			(*c)->mPoints[i]->mVel[2] = (((*c)->mPoints[i]->mSPrime[0])*((*c)->mPoints[i]->mS2Prime[1]) - ((*c)->mPoints[i]->mSPrime[1])*((*c)->mPoints[i]->mS2Prime[0]));
			/* apply prefactor and add non-local contributions, resetting mVelNL after */
			for(int q=0;q<3;q++){
				(*c)->mPoints[i]->mVel[q] *= kappa*log(2*sqrt((*c)->mPoints[i]->mSegLength * (*c)->mPoints[i]->mNext->mSegLength)/a1)/(4*PI);
				(*c)->mPoints[i]->mVel[q] += (*c)->mPoints[i]->mVelNL[q];   
				(*c)->mPoints[i]->mVelNL[q] = 0;		
			}
			//cout << i << ", " << (*c)->mPoints[i]->mVel[0] << ", " << (*c)->mPoints[i]->mVel[1] << ", " << (*c)->mPoints[i]->mVel[2] << endl;   
		}
	}
}

// calculate s' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcSPrime(){
	vector <double> A(mN), B(mN), C(mN), D(mN), E(mN);
	double l, l1, l2, lm1;
	for(int i=0;i<mN;i++){
		l = mPoints[i]->mSegLength; l1 = mPoints[i]->mNext->mSegLength;
		l2 = mPoints[i]->mNext->mNext->mSegLength; lm1 = mPoints[i]->mPrev->mSegLength;
		A[i] = l * l1 * l1 + l * l1 * l2;
		A[i] /= (lm1 * (lm1 + l) * (lm1 + l + l1) * (lm1 + l + l1 +l2));

		B[i] = -lm1 * l1 * l1 - l * l1 * l1 - lm1 * l1 * l2 - l * l1 * l2;
		B[i] /= (lm1 * l * (l + l1) * (l + l1 + l2));

		D[i] = lm1 * l * l1 + l * l * l1 + lm1 * l * l2 + l * l * l2;
		D[i] /= (l1 * l2 * (l + l1) * (lm1 + l + l1));

		E[i] = -l1 * l * l - lm1 * l * l1;
		E[i] /= (l2 * (l1 + l2) * (l + l1 + l2) * (lm1 + l + l1 + l2));

		C[i] = -(A[i] + B[i] + D[i] + E[i]);
	}
	for(int p=0;p<mN;p++){
		for(int q=0;q<3;q++){
			mPoints[p]->mSPrime[q]  = A[p]*mPoints[p]->mPrev->mPrev->mPos[q];
			mPoints[p]->mSPrime[q] += B[p]*mPoints[p]->mPrev->mPos[q];
			mPoints[p]->mSPrime[q] += C[p]*mPoints[p]->mPos[q];
			mPoints[p]->mSPrime[q] += D[p]*mPoints[p]->mNext->mPos[q];
			mPoints[p]->mSPrime[q] += E[p]*mPoints[p]->mNext->mNext->mPos[q];
		}
	}
}

// calculate s'' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcS2Prime(){
	vector <double> A2(mN), B2(mN), C2(mN), D2(mN), E2(mN);
	double l, l1, l2, lm1;
	for(int i=0;i<mN;i++){

		l = mPoints[i]->mSegLength; l1 = mPoints[i]->mNext->mSegLength;
		l2 = mPoints[i]->mNext->mNext->mSegLength; lm1 = mPoints[i]->mPrev->mSegLength;

		A2[i] = 2 * (-2 * l * l1  +  l1 * l1  - l * l2  +  l1 * l2 );
		A2[i] = A2[i] / (lm1 * (lm1 +l)*(lm1 + l + l1)*(lm1 + l + l1 + l2));

		B2[i] = 2 * (2 * lm1 * l1  + 2 * l * l1  -  l1 * l1  +  lm1 * l2  + l * l2  -  l1 * l2);
		B2[i] = B2[i] / (lm1 * l *(l + l1) * (l + l1 + l2));

		D2[i] = 2*(-lm1 * l - l * l +  lm1 * l1  + 2 * l * l1  +  lm1 * l2  + 2 * l * l2);
		D2[i] = D2[i] / (l1 * l2 *(l + l1) * (lm1 + l + l1));

		E2[i] = 2*(lm1 * l + l * l -  lm1 * l1  - 2 * l * l1 );
		E2[i] = E2[i] / ( l2  * (l1 + l2) * (l+ l1 + l2) * (lm1 + l + l1 + l2));

		C2[i] = -(A2[i] + B2[i] + D2[i] + E2[i]);
	}
	for(int p=0;p<mN;p++){
		for(int q=0;q<3;q++){
			mPoints[p]->mS2Prime[q]  = A2[p]*mPoints[p]->mPrev->mPrev->mPos[q];
			mPoints[p]->mS2Prime[q] += B2[p]*mPoints[p]->mPrev->mPos[q];
			mPoints[p]->mS2Prime[q] += C2[p]*mPoints[p]->mPos[q];
			mPoints[p]->mS2Prime[q] += D2[p]*mPoints[p]->mNext->mPos[q];
			mPoints[p]->mS2Prime[q] += E2[p]*mPoints[p]->mNext->mNext->mPos[q];
		}
	}
}

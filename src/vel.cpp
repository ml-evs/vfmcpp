/* Calculate velocities of points in mPoints of a filament.
   Adapted from CalcVelMaster.m by Paul Walmsley */

#include "filament.h"

using namespace std;

/* calculate velocity at each point from s' and s'' eq(2) from Hanninen and Baggaley PNAS 111 p4667 (2014) */
void Filament::CalcVelocity(){
	/* circulation quantum, core radius */
	double	kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0;

	for(int i=0;i<mN;i++){
		mPoints[i]->mVel2 = mPoints[i]->mVel1;
		mPoints[i]->mVel1 = mPoints[i]->mVel;
	}
	CalcMeshLengths(); CalcSPrime(); CalcS2Prime(); //CalcVelocitySelfNL();
	for(int i=0;i<mN;i++){
		mPoints[i]->mVel[0] = (mPoints[i]->mSPrime[1]*mPoints[i]->mS2Prime[2] - mPoints[i]->mSPrime[2]*mPoints[i]->mS2Prime[1]);
		mPoints[i]->mVel[1] = (mPoints[i]->mSPrime[2]*mPoints[i]->mS2Prime[0] - mPoints[i]->mSPrime[0]*mPoints[i]->mS2Prime[2]);
		mPoints[i]->mVel[2] = (mPoints[i]->mSPrime[0]*mPoints[i]->mS2Prime[1] - mPoints[i]->mSPrime[1]*mPoints[i]->mS2Prime[0]);
		for(int q=0;q<3;q++){
			mPoints[i]->mVel[q] *= kappa*log(2*sqrt(mPoints[i]->mSegLength * mPoints[i]->mSegLength)/a1)/(4*M_PI);
			mPoints[i]->mVel[q] += mPoints[i]->mVelNL[q];
			mPoints[i]->mVelNL[q] = 0;
		}
	}
	if(mPoints[0]->mVel1.empty()){
		for(int i(0); i!=mN; i++){
			mPoints[i]->mVel1=mPoints[i]->mVel;
		}
	}
	if(mPoints[0]->mVel2.empty()){
		for(int i(0); i!=mN; i++){
			mPoints[i]->mVel2=mPoints[i]->mVel;
		}
	}
}

// calculate s' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcSPrime(){
	vector <double> A, B, C, D, E;
	double l, l1, l2, lm1;
	A.reserve(mN); B.reserve(mN); C.reserve(mN); D.reserve(mN); E.reserve(mN); 
	// funky for loop to generate correct indices for orderered array of lengths
	// produces (98,99,0,1,2) -> (97,98,99,0,1) for N=100.
	for(int i=0;i<mN;i++){
		
		l = mPoints[i]->mSegLength; l1 = mPoints[i]->mNext->mSegLength; l2 = mPoints[i]->mNext->mNext->mSegLength; lm1 = mPoints[i]->mPrev->mSegLength; 
		
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
	vector <double> A2, B2, C2, D2, E2;
	double l, l1, l2, lm1;
	A2.reserve(mN); B2.reserve(mN); C2.reserve(mN); D2.reserve(mN); E2.reserve(mN); 
	for(int i=0;i<mN;i++){
		
		l = mPoints[i]->mSegLength; l1 = mPoints[i]->mNext->mSegLength; l2 = mPoints[i]->mNext->mNext->mSegLength; lm1 = mPoints[i]->mPrev->mSegLength; 

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
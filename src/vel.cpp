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
	CalcSPrime(); CalcS2Prime(); CalcVelocitySelfNL(); CalcMeshLengths();
	int j;
	for(int i=0;i<mN;i++){
		mPoints[i]->mVel[0] = (mPoints[i]->mSPrime[1]*mPoints[i]->mS2Prime[2] - mPoints[i]->mSPrime[2]*mPoints[i]->mS2Prime[1]);
		mPoints[i]->mVel[1] = (mPoints[i]->mSPrime[2]*mPoints[i]->mS2Prime[0] - mPoints[i]->mSPrime[0]*mPoints[i]->mS2Prime[2]);
		mPoints[i]->mVel[2] = (mPoints[i]->mSPrime[0]*mPoints[i]->mS2Prime[1] - mPoints[i]->mSPrime[1]*mPoints[i]->mS2Prime[0]);
		if(i==mN-1){j=-1;}
		else{j=i;}
		for(int q=0;q<3;q++){
			mPoints[i]->mVel[q] *= kappa*log(2*sqrt(mPoints[i]->mSegLength*mPoints[j+1]->mSegLength)/a1)/(4*M_PI);
			mPoints[i]->mVel[q] += mPoints[i]->mVelNL[q];
			mPoints[i]->mVelNL[q] = 0;
		}
		if(mPoints[i]->mVel1.empty()){mPoints[i]->mVel1=mPoints[i]->mVel;}
		if(mPoints[i]->mVel2.empty()){mPoints[i]->mVel2=mPoints[i]->mVel1;}
	}


}

// calculate s' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcSPrime(){
	vector <double> A, B, C, D, E;
	A.resize(mN); B = A; C=A; D=A; E=A;
	// funky for loop to generate correct indices for orderered array of lengths
	// produces (98,99,0,1,2) -> (97,98,99,0,1) for N=100.
	int j,k,l,m;
	for(int i=0;i<mN;i++){
		j = i; k = i; l = i; m = i;
		if(j-2==-1){j=mN+1;}
		if(j-2==-2){j=mN;}
		if(k-1==-1){k=mN;}
		if(l+1==mN){l=-1;}
		if(m+1==mN){m=-1;}
		if(m+2==mN){m=-2;}
		
		A[i] = mPoints[i]->mSegLength*mPoints[l+1]->mSegLength*mPoints[l+1]->mSegLength+mPoints[i]->mSegLength*mPoints[l+1]->mSegLength*mPoints[m+2]->mSegLength;
		A[i] = A[i] / (mPoints[k-1]->mSegLength*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength)*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength+mPoints[l+1]->mSegLength)*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength+mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength));
		
		B[i] = -mPoints[k-1]->mSegLength*mPoints[l+1]->mSegLength*mPoints[l+1]->mSegLength - mPoints[i]->mSegLength*mPoints[l+1]->mSegLength*mPoints[l+1]->mSegLength - mPoints[k-1]->mSegLength*mPoints[l+1]->mSegLength*mPoints[m+2]->mSegLength - mPoints[i]->mSegLength*mPoints[l+1]->mSegLength*mPoints[m+2]->mSegLength;
		B[i] = B[i] / (mPoints[k-1]->mSegLength*mPoints[i]->mSegLength*(mPoints[i]->mSegLength+mPoints[l+1]->mSegLength)*(mPoints[i]->mSegLength+mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength));

		D[i] = mPoints[k-1]->mSegLength*mPoints[i]->mSegLength*mPoints[l+1]->mSegLength+mPoints[i]->mSegLength*mPoints[i]->mSegLength*mPoints[l+1]->mSegLength+mPoints[k-1]->mSegLength*mPoints[i]->mSegLength*mPoints[m+2]->mSegLength+mPoints[i]->mSegLength*mPoints[i]->mSegLength*mPoints[m+2]->mSegLength;
		D[i] = D[i] / (mPoints[l+1]->mSegLength*mPoints[m+2]->mSegLength*(mPoints[i]->mSegLength+mPoints[l+1]->mSegLength)*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength+mPoints[l+1]->mSegLength));

		E[i] = -mPoints[l+1]->mSegLength*mPoints[i]->mSegLength*mPoints[i]->mSegLength - mPoints[k-1]->mSegLength*mPoints[i]->mSegLength*mPoints[l+1]->mSegLength;
		E[i] = E[i] / (mPoints[m+2]->mSegLength*(mPoints[l+1]->mSegLength + mPoints[m+2]->mSegLength) * (mPoints[i]->mSegLength+mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength)*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength+mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength));

		C[i] = -(A[i] + B[i] + D[i] + E[i]);		
		
	}
	for(int p=0;p<mN;p++){
		j = p; k = p; l = p; m = p;
		if(j-2==-1){j=mN+1;}
		if(j-2==-2){j=mN;}
		if(k-1==-1){k=mN;}
		if(l+1==mN){l=-1;}
		if(m+1==mN){m=-1;}
		if(m+2==mN){m=-2;}
		for(int q=0;q<3;q++){
			mPoints[p]->mSPrime[q]  = A[p]*mPoints[j-2]->mPos[q]; 
			mPoints[p]->mSPrime[q] += B[p]*mPoints[k-1]->mPos[q];
			mPoints[p]->mSPrime[q] += C[p]*mPoints[p]->mPos[q];	
			mPoints[p]->mSPrime[q] += D[p]*mPoints[l+1]->mPos[q];
			mPoints[p]->mSPrime[q] += E[p]*mPoints[m+2]->mPos[q];
		}
	}
}

// calculate s'' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcS2Prime(){
	vector <double> A2, B2, C2, D2, E2;
	A2.resize(mN,0); B2 = A2; C2=A2; D2=A2; E2=A2;
	int j,k,l,m;


	for(int i=0;i<mN;i++){
		j = i; k = i; l = i; m = i;
		if(j-2==-1){j=mN+1;}
		if(j-2==-2){j=mN;}
		if(k-1==-1){k=mN;}
		if(l+1==mN){l=-1;}
		if(m+1==mN){m=-1;}
		if(m+2==mN){m=-2;}
			
		A2[i] = 2*(-2*mPoints[i]->mSegLength*mPoints[l+1]->mSegLength + mPoints[l+1]->mSegLength*mPoints[l+1]->mSegLength - mPoints[i]->mSegLength*mPoints[m+2]->mSegLength + mPoints[l+1]->mSegLength*mPoints[m+2]->mSegLength);
		A2[i] = A2[i] / (mPoints[k-1]->mSegLength*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength)*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength+mPoints[l+1]->mSegLength)*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength+mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength));
		
		B2[i] = 2*(2*mPoints[k-1]->mSegLength*mPoints[l+1]->mSegLength + 2*mPoints[i]->mSegLength*mPoints[l+1]->mSegLength - mPoints[l+1]->mSegLength*mPoints[l+1]->mSegLength + mPoints[k-1]->mSegLength*mPoints[m+2]->mSegLength + mPoints[i]->mSegLength*mPoints[m+2]->mSegLength - mPoints[l+1]->mSegLength*mPoints[m+2]->mSegLength);
		B2[i] = B2[i] / (mPoints[k-1]->mSegLength*mPoints[i]->mSegLength*(mPoints[i]->mSegLength + mPoints[l+1]->mSegLength)*(mPoints[i]->mSegLength + mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength));

		D2[i] = 2*(-mPoints[k-1]->mSegLength*mPoints[i]->mSegLength - mPoints[i]->mSegLength*mPoints[i]->mSegLength + mPoints[k-1]->mSegLength*mPoints[l+1]->mSegLength + 2*mPoints[i]->mSegLength*mPoints[l+1]->mSegLength + mPoints[k-1]->mSegLength*mPoints[m+2]->mSegLength + 2*mPoints[i]->mSegLength*mPoints[m+2]->mSegLength);
		D2[i] = D2[i] / (mPoints[l+1]->mSegLength*mPoints[m+2]->mSegLength*(mPoints[i]->mSegLength+mPoints[l+1]->mSegLength)*(mPoints[k-1]->mSegLength+mPoints[i]->mSegLength+mPoints[l+1]->mSegLength));

		E2[i] = 2*(mPoints[k-1]->mSegLength*mPoints[i]->mSegLength + mPoints[i]->mSegLength*mPoints[i]->mSegLength - mPoints[k-1]->mSegLength*mPoints[l+1]->mSegLength - 2*mPoints[i]->mSegLength*mPoints[l+1]->mSegLength);
		E2[i] = E2[i] / (mPoints[m+2]->mSegLength * (mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength) * (mPoints[i]->mSegLength+mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength) * (mPoints[k-1]->mSegLength+mPoints[i]->mSegLength+mPoints[l+1]->mSegLength+mPoints[m+2]->mSegLength));
		
		C2[i] = -(A2[i] + B2[i] + D2[i] + E2[i]);
	}
	for(int p=0;p<mN;p++){
		j = p; k = p; l = p; m = p;
		if(j-2==-1){j=mN+1;}
		if(j-2==-2){j=mN;}
		if(k-1==-1){k=mN;}
		if(l+1==mN){l=-1;}
		if(m+1==mN){m=-1;}
		if(m+2==mN){m=-2;}
		for(int q=0;q<3;q++){
			mPoints[p]->mS2Prime[q] =  A2[p]*mPoints[j-2]->mPos[q];	
			mPoints[p]->mS2Prime[q] += B2[p]*mPoints[k-1]->mPos[q];
			mPoints[p]->mS2Prime[q] += C2[p]*mPoints[p]->mPos[q];		
			mPoints[p]->mS2Prime[q] += D2[p]*mPoints[l+1]->mPos[q];
			mPoints[p]->mS2Prime[q] += E2[p]*mPoints[m+2]->mPos[q];
		}
	}
}
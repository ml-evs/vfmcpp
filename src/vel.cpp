// calculate velocities of points on each mPos
// adapted from CalcVelMaster.m by Paul Walmsley

#include "filament.h"

using namespace std;

// calculate velocity at each point from s' and s'' eq(2) from Hanninen and Baggaley PNAS 111 p4667 (2014)
void Filament::CalcVelocity(){
	// circulation quantum, core radius, ..., mutual friction
	double	kappa = 9.98e-8, a0=1.3e-10, a1=exp(0.5)*a0;
	mVel2 = mVel1;
	mVel1 = mVel;
	mVel.resize(mN);
	CalcSPrime(); CalcS2Prime(); CalcVelocitySelfNL(); CalcMeshLengths();
	int j;
	for(int i=0;i<mN;i++){
		mVel[i].resize(3);
		mVel[i][0] = (mSPrime[i][1]*mS2Prime[i][2] - mSPrime[i][2]*mS2Prime[i][1]);
		mVel[i][1] = (mSPrime[i][2]*mS2Prime[i][0] - mSPrime[i][0]*mS2Prime[i][2]);
		mVel[i][2] = (mSPrime[i][0]*mS2Prime[i][1] - mSPrime[i][1]*mS2Prime[i][0]);
		if(i==mN-1){j=-1;}
		else{j=i;}
		for(int q=0;q<3;q++){
			mVel[i][q] *= kappa*log(2*sqrt(mSegLengths[i]*mSegLengths[j+1])/a1)/(4*M_PI);
			mVel[i][q] += mVelNL[i][q];
			mVelNL[i][q] = 0;
		}
	}
	if(mVel1.empty()){mVel1=mVel;}
	if(mVel2.empty()){mVel2=mVel1;}

}

// calculate s' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcSPrime(){
	mSPrime.resize(mN);
	vector <double> A, B, C, D, E;
	vector <double> &L = mSegLengths;
	A.resize(mN); B = A; C=A; D=A; E=A;
	list <vector <double> >::iterator b, c, e, d, f, g, h;
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
		
		A[i] = L[i]*L[l+1]*L[l+1]+L[i]*L[l+1]*L[m+2];
		A[i] = A[i] / (L[k-1]*(L[k-1]+L[i])*(L[k-1]+L[i]+L[l+1])*(L[k-1]+L[i]+L[l+1]+L[m+2]));
		
		B[i] = -L[k-1]*L[l+1]*L[l+1] - L[i]*L[l+1]*L[l+1] - L[k-1]*L[l+1]*L[m+2] - L[i]*L[l+1]*L[m+2];
		B[i] = B[i] / (L[k-1]*L[i]*(L[i]+L[l+1])*(L[i]+L[l+1]+L[m+2]));

		D[i] = L[k-1]*L[i]*L[l+1]+L[i]*L[i]*L[l+1]+L[k-1]*L[i]*L[m+2]+L[i]*L[i]*L[m+2];
		D[i] = D[i] / (L[l+1]*L[m+2]*(L[i]+L[l+1])*(L[k-1]+L[i]+L[l+1]));

		E[i] = -L[l+1]*L[i]*L[i] - L[k-1]*L[i]*L[l+1];
		E[i] = E[i] / (L[m+2]*(L[l+1] + L[m+2]) * (L[i]+L[l+1]+L[m+2])*(L[k-1]+L[i]+L[l+1]+L[m+2]));

		C[i] = -(A[i] + B[i] + D[i] + E[i]);		
		
	}
/*	for(int p=0;p<mN;p++){
		mSPrime[p].resize(3);
		j = p; k = p; l = p; m = p;
		if(j-2==-1){j=mN+1;}
		if(j-2==-2){j=mN;}
		if(k-1==-1){k=mN;}
		if(l+1==mN){l=-1;}
		if(m+1==mN){m=-1;}
		if(m+2==mN){m=-2;}
		for(int q=0;q<3;q++){
			mSPrime[p][q] = A[p]*mPos[j-2][q];	mSPrime[p][q] += B[p]*mPos[k-1][q];
			mSPrime[p][q] += C[p]*mPos[p][q];	mSPrime[p][q] += D[p]*mPos[l+1][q];
			mSPrime[p][q] += E[p]*mPos[m+2][q];
		}
	}*/

	for(int p(0);p!=mN;p++){
		b = mPos.begin(); e = mPos.end();
		for(c=b;c!=e;c++){
			for(int q(0);q!=3;q++){
				if(c==b){d=(advance(mPos.end(),-2))}
				if(c==b+1){d = mPos.end()-1}
				else{d = c-2;}
				mSPrime[p][q]  = A[p]*(*d))[q];

				if(c==b){f=mPos.end()}
				else{f = c;}
				mSPrime[p][q] += B[p]*(*(f-1))[q];
				mSPrime[p][q] += C[p]*(*c)[q];

				if(c==e-1){g=mPos.begin()}
				else{g = c+1;}
				mSPrime[p][q] += D[p]*(*g)[q];

				if(c==e-2){h = mPos.begin();}
				if(c==e-1){h = mPos.begin()+1;}
				else{h=c+2;}
				mSPrime[p][q] += E[p]*(*h)[q];
			}
		}
		
	}
}

// calculate s'' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcS2Prime(){
	mS2Prime.resize(mN);
	vector <double> A2, B2, C2, D2, E2;
	vector <double> &L = mSegLengths;
	A2.resize(mN,0); B2 = A2; C2=A2; D2=A2; E2=A2;
	list <vector <double> >::iterator b, c, e, d, f, g, h;
	int j,k,l,m;


	for(int i=0;i<mN;i++){
		j = i; k = i; l = i; m = i;
		if(j-2==-1){j=mN+1;}
		if(j-2==-2){j=mN;}
		if(k-1==-1){k=mN;}
		if(l+1==mN){l=-1;}
		if(m+1==mN){m=-1;}
		if(m+2==mN){m=-2;}
			
		A2[i] = 2*(-2*L[i]*L[l+1] + L[l+1]*L[l+1] - L[i]*L[m+2] + L[l+1]*L[m+2]);
		A2[i] = A2[i] / (L[k-1]*(L[k-1]+L[i])*(L[k-1]+L[i]+L[l+1])*(L[k-1]+L[i]+L[l+1]+L[m+2]));
		
		B2[i] = 2*(2*L[k-1]*L[l+1] + 2*L[i]*L[l+1] - L[l+1]*L[l+1] + L[k-1]*L[m+2] + L[i]*L[m+2] - L[l+1]*L[m+2]);
		B2[i] = B2[i] / (L[k-1]*L[i]*(L[i] + L[l+1])*(L[i] + L[l+1]+L[m+2]));

		D2[i] = 2*(-L[k-1]*L[i] - L[i]*L[i] + L[k-1]*L[l+1] + 2*L[i]*L[l+1] + L[k-1]*L[m+2] + 2*L[i]*L[m+2]);
		D2[i] = D2[i] / (L[l+1]*L[m+2]*(L[i]+L[l+1])*(L[k-1]+L[i]+L[l+1]));

		E2[i] = 2*(L[k-1]*L[i] + L[i]*L[i] - L[k-1]*L[l+1] - 2*L[i]*L[l+1]);
		E2[i] = E2[i] / (L[m+2] * (L[l+1]+L[m+2]) * (L[i]+L[l+1]+L[m+2]) * (L[k-1]+L[i]+L[l+1]+L[m+2]));
		
		C2[i] = -(A2[i] + B2[i] + D2[i] + E2[i]);
	}
	for(int p(0);p!=mN;p++){
		b = mPos.begin(); e = mPos.end();
		for(c=b;c!=e;c++){
			for(int q(0);q!=3;q++){
				if(c==b){d=(advance(mPos.end(),-2))}
				if(c==b+1){d = mPos.end()-1}
				else{d = c-2;}
				mSPrime[p][q]  = A[p]*(*d))[q];

				if(c==b){f=mPos.end()}
				else{f = c;}
				mSPrime[p][q] += B[p]*(*(f-1))[q];
				mSPrime[p][q] += C[p]*(*c)[q];

				if(c==e-1){g=mPos.begin()}
				else{g = c+1;}
				mSPrime[p][q] += D[p]*(*g)[q];

				if(c==e-2){h = mPos.begin();}
				if(c==e-1){h = mPos.begin()+1;}
				else{h=c+2;}
				mSPrime[p][q] += E[p]*(*h)[q];
			}
		}
		
	}
}



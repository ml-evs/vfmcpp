// Calculates the non-local contribution to the velocities of points on each ring from another filament
// Adapted from CalcNonLocalVel_OtherFilament.m by Paul Walmsley

#include "filament.h"
#include <vector>
#include <cmath>

using namespace std;

	const double kappa = 9.98e-8;

void Filament::CalcVelocityNL_OF(vector <vector <double> > PosOtherRing){

	int N = PosOtherRing.size();
	mVelNL.resize(mN);
	vector <vector <double> > q(N); // q = s_l+1 - s_l
	vector <vector <double> > p; // p = s_l - s_k
	vector <double> A, B, C, D;
	vector < vector <double> > pxq;
	pxq = q; p = q; 
	A.resize(N); B = A; C = A; D = A;
	int j;

	for(int l=0;l<N;l++){
			//cout << l << endl;
			q[l].resize(3);
			if(l+1==N){j = -1;}
			else{j=l;}
			for(int m=0;m<3;m++){
				q[l][m] = (PosOtherRing[j+1][m]-PosOtherRing[l][m]);
			}
	} // End q calculation

	for (int k=0;k<mN;k++){
		mVelNL[k].resize(3);
		for(int l=0;l<N;l++){
			//cout << l << endl;
			p[l].resize(3);
			for(int m=0;m<3;m++){
				p[l][m] = (PosOtherRing[l][m]-mPos[k][m]);	
			}
		} // End p calculation

	// Calculate coefficients: A=|p|, B=2*p.q, C=|q|
		for(int l=0;l<N;l++){
			pxq[l].resize(3);			
			pxq[l][0] = p[l][1]*q[l][2] - p[l][2]*q[l][1];
			pxq[l][1] = p[l][2]*q[l][0] - p[l][0]*q[l][2];
			pxq[l][2] = p[l][0]*q[l][1] - p[l][1]*q[l][0];

			A[l] = 0; B[l] = 0; C[l] = 0;
			for(int m=0;m<3;m++){
				A[l] += p[l][m]*p[l][m]; 
				B[l] += 2*p[l][m]*q[l][m];
				C[l] += q[l][m]*q[l][m];
			}

			// Total nonlocal contribution at point k is found by summing over all elements l, D.(p x q)
			for(int m=0;m<3;m++){
				mVelNL[k][m] += (kappa*(((2*C[l])+B[l])/(sqrt(A[l]+B[l]+C[l])) - B[l]/sqrt(A[l])) * pxq[l][m])/(2*M_PI*(4*A[l]*C[l]-B[l]*B[l]));	
			}
		}
	}
}
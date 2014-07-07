// Calculates the non-local contribution to the velocities of points on each ring
// Adapted from CalcNonmNocalVel.m by Paul Walmsley

#include "filament.h"

using namespace std;

	const double kappa = 9.98e-8;

void Filament::CalcVelocitySelfNL(){

	vector < vector <double> > pxq;
	vector < vector <double> > p; // p = s_l - s_k
	vector < vector <double> > q; // q = s_l+1 - s_l
	vector <double> A, B, C, D;
	A.resize(mN-2); B = A; C = A; D = A;
	p.resize(mN-2); q = p; 
	pxq = p;
	mVelNL.resize(mN);
	int g, h, i(0), j, s;
	
	for(int k=0;k<mN;k++){
		s = 0; i=0;
		mVelNL[k].resize(3);
		if(k !=0 && k!=mN-1){
			for(int l=0;l<mN;l++){
				if(l==k||l==k-1){s++;}
				else{
					p[l-s].resize(3); q[l-s].resize(3);
					if(l+1==mN){g=-1;}
					else{g=l;}
					for(int m=0;m<3;m++){
						p[l-s][m] = (mPos[l][m]-mPos[k][m]);
						q[l-s][m] = (mPos[g+1][m]-mPos[l][m]);
					}
				}
			}
		}
		else if(k==0){
			for(int l=1;l<mN-1; l++){
				p[i].resize(3); q[i] = p[i];
				g = l+k; h = l+k;
				for(int m=0;m<3;m++){ 
					if(l==k-1){l=k+2;}
					p[i][m] = mPos[l][m] - mPos[k][m];
					if(g+1>=mN){g-=mN;}
					if(h>=mN){h-=mN;}
					q[i][m] = mPos[g+1][m] - mPos[h][m];
				} 
				i++;
			}
		}
		else if(k==mN-1){
			for(int l=1;l<mN-1; l++){
				p[i].resize(3); q[i] = p[i];
				g = l; h = l; j=l;
				for(int m=0;m<3;m++){ 
					if(l==k-1){l=k+2;}
					if(l>=mN){j=0;}
					p[i][m] = mPos[j][m] - mPos[k][m];
					if(g+2>=mN){g=k-g-1;}
					if(h==k-1){h=k-h-1;}
					q[i][m] = mPos[g+1][m] - mPos[h][m];
				} 
				i++;
			}
		}
		else{cout << "Something terrible has happened." << endl;}

		// Calculate coefficients: A=|p|, B=p.q, C=|q|
		for(int l=0;l<mN-2;l++){
			pxq[l].resize(3);
			pxq[l][0] = p[l][1]*q[l][2] - p[l][2]*q[l][1];
			pxq[l][1] = p[l][2]*q[l][0] - p[l][0]*q[l][2];
			pxq[l][2] = p[l][0]*q[l][1] - p[l][1]*q[l][0];
			A[l] = 0; B[l] = 0; C[l] = 0;
			for(int j=0;j<3;j++){
				A[l] += p[l][j]*p[l][j]; 
				B[l] += 2*p[l][j]*q[l][j];
				C[l] += q[l][j]*q[l][j];
			}
			for(int j=0;j<3;j++){
				mVelNL[k][j] += (kappa*((2*C[l]+B[l])/(sqrt(A[l]+B[l]+C[l])) - B[l]/sqrt(A[l])) * pxq[l][j])/(2*M_PI*(4*A[l]*C[l]-B[l]*B[l]));
			}
		}
	}
}
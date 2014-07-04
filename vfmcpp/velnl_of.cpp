// Calculates the non-local contribution to the velocities of points on each ring from another filament
// Adapted from CalcNonLocalVel_OtherFilament.m by Paul Walmsley

#include "tangle.h"


using namespace std;

	const double kappa = 9.98e-8;

void Tangle::CalcVelocityNL_OF(){

	vector <Filament>::iterator begin, current, end;
	vector <vector <double> > q; // q = s_l+1 - s_l
	vector <vector <double> > p; // p = s_l - s_k
	vector <double> A, B, C, D;
	vector < vector <double> > pxq;
	
	begin = mTangle.begin(); end = mTangle.end();

	for(current=begin; current!=end; current++){

		vector <Filament>::iterator other_begin, other_current, other_end;
		other_begin = mTangle.begin(); other_end = mTangle.end();
		int N_k = current->mN;
		(current->mVelNL).resize(N_k);
		for(other_current=other_begin; other_current!=other_end; other_current++){
			if(other_current == current){continue;}
			else{
				int N_l = other_current->mN;
				q.resize(N_l); p = q; pxq = p;
				A.resize(N_l); B = A; C = A; D = A;
				int j;
				for(int l=0;l<N_l;l++){
						q[l].resize(3);
						if(l+1==N_l){j = -1;}
						else{j=l;}
						for(int m=0;m<3;m++){
							q[l][m] = ((other_current->mPos[j+1][m])-(other_current->mPos[l][m]));
						}
				} // End q calculation

				for (int k=0;k<N_k;k++){
					(current->mVelNL[k]).resize(3);
					for(int l=0;l<N_l;l++){
						p[l].resize(3);
						for(int m=0;m<3;m++){
							p[l][m] = (other_current->mPos[l][m]-current->mPos[k][m]);	
						}
					} // End p calculation


				// Calculate coefficients: A=|p|, B=2*p.q, C=|q|
					for(int l=0;l<N_l;l++){
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
						for(int m=0;m<3;m++){
							current->mVelNL[k][m] += (kappa*(((2*C[l])+B[l])/(sqrt(A[l]+B[l]+C[l])) - B[l]/sqrt(A[l])) * pxq[l][m])/(2*M_PI*(4*A[l]*C[l]-B[l]*B[l]));	
						}
					}
				}
			}
		}
	}
}
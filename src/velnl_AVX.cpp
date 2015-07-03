/* 	Calculates the non-local contributions to the velocities of every point.
	Adapted from CalcNonLocalVel_OtherFilament.m by Paul Walmsley. */

#include "tangle.h"

using namespace std;

const double kappa = 9.98e-8;

void Tangle::CalcVelocityNL(){
	
	int N_loop;
	int skip_index_1(12345), skip_index_2(-12345);
	for(unsigned int P(0); P!=mTangle.size(); P++){
		for(unsigned int Q(0); Q!=mTangle.size(); Q++){
			N_loop = mTangle[Q]->mN;
			vector <double> p(3*mTangle[Q]->mN); 	// vector containing p
			vector <double> q(3*mTangle[Q]->mN); 	// vector containing q
			vector <double> pxq(3*mTangle[Q]->mN); 	// vector containing pxq
			vector <double> calc_list(3*mTangle[Q]->mN); 			// vector containing pp, qq, pq
			for(int i(0); i!=mTangle[P]->mN; i++){
				for(int n(0);n!=N_loop; n++){
					p[n]          = mTangle[Q]->mPoints[n]->mPos[0] - mTangle[P]->mPoints[i]->mPos[0];
					p[n+N_loop]   = mTangle[Q]->mPoints[n]->mPos[1] - mTangle[P]->mPoints[i]->mPos[1];
					p[n+2*N_loop] = mTangle[Q]->mPoints[n]->mPos[2] - mTangle[P]->mPoints[i]->mPos[2];
					q[n]          = mTangle[Q]->mPoints[n]->mNext->mPos[0] - mTangle[P]->mPoints[i]->mPos[0];
					q[n+N_loop]   = mTangle[Q]->mPoints[n]->mNext->mPos[1] - mTangle[P]->mPoints[i]->mPos[1];
					q[n+2*N_loop] = mTangle[Q]->mPoints[n]->mNext->mPos[2] - mTangle[P]->mPoints[i]->mPos[2];
					/* calculate cross and dot products */
					pxq[n] 		 	= p[n+N_loop]*q[n+2*N_loop] - p[n+2*N_loop]*q[n+N_loop]; // pxq[x]
					pxq[n+N_loop] 	= p[n+2*N_loop]*q[n] 		- p[n]*q[n+2*N_loop]; 		// pxq[y]
					pxq[n+2*N_loop] = p[n]*q[n+N_loop] 			- p[n+N_loop]*q[n];			// pxq[z]
					/* p.p */
					calc_list[n] += p[n]*p[n];
					calc_list[n] += p[n+N_loop]*p[n+N_loop];
					calc_list[n] += p[n+2*N_loop]*p[n+2*N_loop];
					/* q.q */
					calc_list[n+N_loop] += q[n]*q[n];
					calc_list[n+N_loop] += q[n+N_loop]*q[n+N_loop]; 
					calc_list[n+N_loop] += q[n+2*N_loop]*q[n+2*N_loop]; 
					/* p.q */
					calc_list[n+2*N_loop] += p[n]*q[n]; 
					calc_list[n+2*N_loop] += p[n+N_loop]*q[n+N_loop]; 
					calc_list[n+2*N_loop] += p[n+2*N_loop]*q[n+2*N_loop]; 
				}
				skip_index_1 = 12345;
				skip_index_2 = -12345;
				for(int test(0);test!=N_loop;test++){
					if(mTangle[Q]->mPoints[test] == mTangle[P]->mPoints[i] || mTangle[Q]->mPoints[test] == mTangle[P]->mPoints[i]->mPrev){
						if(test < skip_index_1){skip_index_1 = test;}
						else{skip_index_2 = test;}
					}
				}
				for(int n(0);n!=N_loop;n++){
					double A = sqrt(calc_list[n]); 			// |p|
					double n = calc_list[n+2*N_loop]; 		// p.q
					double C = sqrt(calc_list[n+N_loop]); 	// |q|
					if(n!=skip_index_1 && n!=skip_index_2){
						double D = (A+C)/(A*C*(A*C+n));
						mTangle[P]->mPoints[i]->mVelNL[0] += (kappa/(4*M_PI)) * D * pxq[n];
						mTangle[P]->mPoints[i]->mVelNL[1] += (kappa/(4*M_PI)) * D * pxq[n+N_loop];
						mTangle[P]->mPoints[i]->mVelNL[2] += (kappa/(4*M_PI)) * D * pxq[n+2*N_loop];
					}
				}
			}
		}
	}
}
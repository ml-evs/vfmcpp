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
			vector <double> p(3*mTangle[Q]->mN); 	// vector containing p, q and pxq
			vector <double> q(3*mTangle[Q]->mN); 	// vector containing p, q and pxq
			vector <double> pxq(3*mTangle[Q]->mN); 	// vector containing p, q and pxq
			vector <double> calc_list(3*mTangle[Q]->mN); 			// vector containing pp, qq, pq
			for(int i(0); i!=mTangle[P]->mN; i++){
				for(int n(0);n!=N_loop; n++){
					p[n]          = mTangle[Q]->mPoints[n]->mPos[0] - mTangle[P]->mPoints[i]->mPos[0];
					p[n+N_loop]   = mTangle[Q]->mPoints[n]->mPos[1] - mTangle[P]->mPoints[i]->mPos[1];
					p[n+2*N_loop] = mTangle[Q]->mPoints[n]->mPos[2] - mTangle[P]->mPoints[i]->mPos[2];
					q[n]          = mTangle[Q]->mPoints[n]->mNext->mPos[0] - mTangle[P]->mPoints[i]->mPos[0];
					q[n+N_loop]   = mTangle[Q]->mPoints[n]->mNext->mPos[1] - mTangle[P]->mPoints[i]->mPos[1];
					q[n+2*N_loop] = mTangle[Q]->mPoints[n]->mNext->mPos[2] - mTangle[P]->mPoints[i]->mPos[2];
				}
				skip_index_1 = 12345;
				skip_index_2 = -12345;
				for(int test(0);test!=N_loop;test++){
					if(mTangle[Q]->mPoints[test] == mTangle[P]->mPoints[i] || mTangle[Q]->mPoints[test] == mTangle[P]->mPoints[i]->mPrev){
						if(test < skip_index_1){skip_index_1 = test;}
						else{skip_index_2 = test;}
					}
				}
				/* calculate cross and dot products */
				for(int b(0);b!=N_loop;b++){
					pxq[b] = p[b+N_loop]*q[b+2*N_loop] - p[b+2*N_loop]*q[b+N_loop]; // pxq[x]
					pxq[b+N_loop] = p[b+2*N_loop]*q[b] - p[b]*q[b+2*N_loop]; 		// pxq[y]
					pxq[b+2*N_loop] = p[b]*q[b+N_loop] - p[b+N_loop]*q[b];			// pxq[z]
					/* p.p */
					calc_list[b] += p[b]*p[b];
					calc_list[b] += p[b+N_loop]*p[b+N_loop];
					calc_list[b] += p[b+2*N_loop]*p[b+2*N_loop];
					/* q.q */
					calc_list[b+N_loop] += q[b]*q[b];
					calc_list[b+N_loop] += q[b+N_loop]*q[b+N_loop]; 
					calc_list[b+N_loop] += q[b+2*N_loop]*q[b+2*N_loop]; 
					/* p.q */
					calc_list[b+2*N_loop] += p[b]*q[b]; 
					calc_list[b+2*N_loop] += p[b+N_loop]*q[b+N_loop]; 
					calc_list[b+2*N_loop] += p[b+2*N_loop]*q[b+2*N_loop]; 
				}
				for(int b(0);b!=N_loop;b++){
					double A = sqrt(calc_list[b]); 			// |p|
					double B = calc_list[b+2*N_loop]; 		// p.q
					double C = sqrt(calc_list[b+N_loop]); 	// |q|
					if(b!=skip_index_1 && b!=skip_index_2){
						double D = (A+C)/(A*C*(A*C+B));
						for(int x(0);x<3;x++){
							mTangle[P]->mPoints[i]->mVelNL[x] += (kappa/(4*M_PI)) * D * pxq[b+x*N_loop];
						}
					}
				}
			}
		}
	}
}
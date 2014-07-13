/* Calculates the non-local contribution to the velocities of points on each ring
   Adapted from CalcNonmNocalVel.m by Paul Walmsley */

#include "filament.h"

using namespace std;

	const double kappa = 9.98e-8;

void Filament::CalcVelocitySelfNL(){

	vector <double> q; 		// q = s_l+1 - s_l
	vector <double> p; 		// p = s_l - s_k
	vector <double> pxq; 
	double pp, qq, pq; 		// p.p, q.q, p.q
	p.resize(3); q = p; pxq = p;
	
	vector <Point*>::iterator b, c, e, c2;
	b = mPoints.begin(); e = mPoints.end();
	Point* c3;
	int i(0);
	/* loop over all points */
	for(c=b; c!=e; c++){
		pp = 0; qq = 0; pq = 0;
		/* assign pointer to next point */		
		i = 0;
		c3 = (*c)->mNext;
		while(i<mN-2){
			/* calculate p and q */
			for(int m(0);m<3;m++){
				p[m] = c3->mPos[m] - (*c)->mPos[m]; // does double the calculations it needs to atm
				q[m] = c3->mNext->mSegLast[m];
				pp += p[m]*p[m];
				qq += q[m]*q[m];
				pq += p[m]*q[m];
			}
			/* calculate pxq */
			pxq[0] = p[1]*q[2] - p[2]*q[1];
			pxq[1] = p[2]*q[0] - p[0]*q[2];
			pxq[2] = p[0]*q[1] - p[1]*q[0];
			/* assign values to mVelNL */
			for(int j(0);j<3;j++){
				(*c)->mVelNL[j] += (pxq[j]*kappa/(8*M_PI*(pp*qq-pq))) * (2*(qq+pq)/sqrt(pp+qq+2*pq) - 2*pq/sqrt(pp));
			}
			/* increment pointer to next */
			c3 = c3->mNext;
			i++;
		}
	}
}
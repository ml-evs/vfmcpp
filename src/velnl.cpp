/* 	Calculates the non-local contributions to the velocities of every point.
	Adapted from CalcNonLocalVel_OtherFilament.m by Paul Walmsley. */

#include "tangle.h"

using namespace std;

const double kappa = 9.98e-8;

void Tangle::CalcVelocityNL(){

	vector <double> q(3); 	// q = s_l+1 - s_l
	vector <double> p(3); 	// p = s_l - s_k
	vector <double> pxq(3);
	double pp, qq, pq; 		// p.p, q.q, p.q
	
	/* iterate through tangle */
	vector <Filament*>::iterator begin_tangle, current, end_tangle;	
	begin_tangle = mTangle.begin(); end_tangle = mTangle.end();
	for(current=begin_tangle; current!=end_tangle; current++){
		vector <Filament*>::iterator other_begin, other_current, other_end;
		other_begin = mTangle.begin(); other_end = mTangle.end();
		for(other_current=other_begin; other_current!=other_end; other_current++){
			vector <Point*>::iterator b, ob, oe, oc;
			b = (*current)->mPoints.begin();
			ob = (*other_current)->mPoints.begin(); oe = (*other_current)->mPoints.end();
			/* assign pointer to first field point */
			Point* pField = (*b)->mNext;
			/* loop over all "field points" */
			int i(0);
			while(i<(*current)->mN){
				/* iterate over "source points" */
				for(oc=ob;oc!=oe;oc++){
					/* calculate p and q */
					if((*oc)!=(*current)->mPoints[i]->mNext && (*oc)!=(*current)->mPoints[i]->mPrev && (*oc)!=(*current)->mPoints[i]){
						pp = 0; qq = 0; pq = 0;
						for(int m(0);m<3;m++){
							p[m] = (*oc)->mPos[m] - pField->mPos[m];
							q[m] = (*oc)->mNext->mSegLast[m];
							pp += p[m]*p[m];
							qq += q[m]*q[m];
							pq += p[m]*q[m];
						}
						/* calculate pxq and assign temp variables */
						pxq[0] = p[1]*q[2] - p[2]*q[1];
						pxq[1] = p[2]*q[0] - p[0]*q[2];
						pxq[2] = p[0]*q[1] - p[1]*q[0];
						double sqrt_ppqq2pq = sqrt(pp+qq+2*pq);
						double sqrt_pp = sqrt(pp);
							/* add contribution to mVelNL */
						for(int m(0);m<3;m++){
							pField->mVelNL[m] += pxq[m] * kappa * ( (2*(qq+pq)/sqrt_ppqq2pq) - (2*pq/sqrt_pp) ) / (8*M_PI* (pp*qq - pq*pq) );
						}
					}
				}
				/* increment pointer to next */
				pField = pField->mNext;
				i++;
			}
		}
	}
}

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
			/* if self, then call self-induced nonlocal velocity */
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
					if((*oc)==pField||(*oc)==pField->mPrev){pField->mVelNL[0] += 0;}
					else{
						/* calculate p and q */
						pp = 0; qq = 0; pq = 0;
						for(int m(0);m<3;m++){
							p[m] = (*oc)->mPos[m] - pField->mPos[m]; // does double the calculations it needs to atm
							q[m] = (*oc)->mNext->mPos[m] - pField->mPos[m];
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
						double sqrt_qq = sqrt(qq);
						double A = sqrt_pp;
						double B = pq;
						double C = sqrt_qq;
						double D = (A+C)/(A*C*(A*C+B));
						//double D = ( ( (2*C + B) / sqrt(A+B+C) ) - B/sqrt(A) ) / (4*A*C - B*B) ;
						/* assign values to mVelNL */
						for(int j(0);j<3;j++){
							//(*c)->mVelNL[j] += ((kappa/4*M_PI)*(sqrt_pp + sqrt_qq)*(pxq[j])) / (sqrt_pp*sqrt_qq * (sqrt_pp*sqrt_qq + pq));
							pField->mVelNL[j] += (kappa/(4*M_PI)) * D * pxq[j];
						//	(*c)->mVelNL[j] += kappa/(2*PI) * D * pxq[j];
							//(*c)->mVelNL[j] += pxq[j] * kappa * ( (2*(qq+pq)/sqrt_ppqq2pq) - (2*pq/sqrt_pp) ) / (8*M_PI* (pp*qq - pq*pq) );
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

void Filament::CalcVelocitySelfNL(){

	vector <double> q(3); 		// q = s_l+1 - s_l
	vector <double> p(3); 		// p = s_l - s_k
	vector <double> pxq(3);
	double pp, qq, pq; 			// p.p, q.q, p.q
	/* loop over all points */
	//int w(0); /* used to label current point for debugging */
	vector <Point*>::iterator b, c, e, c2;
	b = mPoints.begin(); e = mPoints.end();
	for(c=b; c!=e; c++){
		int j(0);
		//int i(0); /* used to label field points for debugging */
		for(c2=b; c2!=e; c2++){
			pp = 0; qq = 0; pq = 0;	
			//if((*c2)->mNext->mFlagDummy == 1){ /*cout << "End of the line!" << endl;*/ continue; }
			if(!(  (*c2)==(*c)->mNext||(*c2)==(*c)||(*c2)==(*c)->mPrev)){
			/* calculate p and q */
				for(int m(0);m<3;m++){
					p[m] = (*c2)->mPos[m] - (*c)->mPos[m]; // does double the calculations it needs to atm
					q[m] = (*c2)->mNext->mPos[m] - (*c)->mPos[m];
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
				double sqrt_qq = sqrt(qq);
				double A = sqrt_pp;
				double B = pq;
				double C = sqrt_qq;
				double D = (A+C)/(A*C*(A*C+B));
				//double D = ( ( (2*C + B) / sqrt(A+B+C) ) - B/sqrt(A) ) / (4*A*C - B*B) ;
				/* assign values to mVelNL */
				for(int j(0);j<3;j++){
					//(*c)->mVelNL[j] += ((kappa/4*M_PI)*(sqrt_pp + sqrt_qq)*(pxq[j])) / (sqrt_pp*sqrt_qq * (sqrt_pp*sqrt_qq + pq));
					(*c)->mVelNL[j] += (kappa/(4*M_PI)) * D * pxq[j];
				//	(*c)->mVelNL[j] += kappa/(2*PI) * D * pxq[j];
					//(*c)->mVelNL[j] += pxq[j] * kappa * ( (2*(qq+pq)/sqrt_ppqq2pq) - (2*pq/sqrt_pp) ) / (8*M_PI* (pp*qq - pq*pq) );
				}
			}
		}
	}
}


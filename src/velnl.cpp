// Calculates the non-local contribution to the velocities of points on each ring
// Adapted from CalcNonmNocalVel.m by Paul Walmsley

#include "filament.h"

using namespace std;

	const double kappa = 9.98e-8;

void Filament::CalcVelocitySelfNL(){

	vector <double> p; // p = s_l - s_k
	p.resize(3);
	
	vector <Point*>::iterator b, c, e, c2;
	b = mPoints.begin(); e = mPoints.end();
	Point* c3;
	int i(0);
	/* loop over all points */
	for(c=b; c!=e; c++){
		/* assign pointer to next point */		
		i = 0;
		c3 = (*c)->mNext;
		while(i<mN-2){
			for(int m(0);m<3;m++){
				p[m] = c3->mPos[m] - (*c)->mPos[m];
			}
			(*c)->mVelNL[0] += (kappa*((2*(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2])+(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2])))/(sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2])+(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))+(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2]))) - (2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))/sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2]))) * (p[1]*c3->mNext->mSegLast[2] - p[2]*c3->mNext->mSegLast[1]))/(2*M_PI*(4*(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])*(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2])-(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))*(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))));
			(*c)->mVelNL[1] += (kappa*((2*(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2])+(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2])))/(sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2])+(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))+(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2]))) - (2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))/sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2]))) * (p[2]*c3->mNext->mSegLast[0] - p[0]*c3->mNext->mSegLast[2]))/(2*M_PI*(4*(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])*(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2])-(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))*(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))));
			(*c)->mVelNL[2] += (kappa*((2*(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2])+(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2])))/(sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2])+(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))+(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2]))) - (2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))/sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2]))) * (p[0]*c3->mNext->mSegLast[1] - p[1]*c3->mNext->mSegLast[0]))/(2*M_PI*(4*(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])*(c3->mNext->mSegLast[0]*c3->mNext->mSegLast[0]+c3->mNext->mSegLast[1]*c3->mNext->mSegLast[1]+c3->mNext->mSegLast[2]*c3->mNext->mSegLast[2])-(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))*(2*(p[0]*c3->mNext->mSegLast[0]+p[1]*c3->mNext->mSegLast[1]+p[2]*c3->mNext->mSegLast[2]))));	
			/* increment pointer to next */
			c3 = c3->mNext;
			i++;
		}
	}
}
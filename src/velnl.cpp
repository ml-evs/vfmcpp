// Calculates the non-local contribution to the velocities of points on each ring
// Adapted from CalcNonmNocalVel.m by Paul Walmsley

#include "filament.h"

using namespace std;

	const double kappa = 9.98e-8;

void Filament::CalcVelocitySelfNL(){

	vector <double> p; // p = s_l - s_k
	vector <double> q;
	p.resize(3); q = p; 
	
	vector <Point*>::iterator b, c, e, c2;
	b = mPoints.begin(); e = mPoints.end();
	Point* c3;
	/* loop over all points */
	for(c=b; c!=e; c++){
		int i(0);
		/* assign pointer to next point */		
		c3 = (*c)->mNext;
		while(i<mN-2){
			for(int m(0);m<3;m++){
				p[m] = c3->mPos[m] - (*c)->mPos[m];
				q[m] = c3->mNext->mPos[m] - c3->mPos[m];
			}
			(*c)->mVelNL[0] += (kappa*((2*(q[0]*q[0]+q[1]*q[1]+q[2]*q[2])+(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2])))/(sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2])+(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))+(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]))) - (2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))/sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2]))) * (p[1]*q[2] - p[2]*q[1]))/(2*M_PI*(4*(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])*(q[0]*q[0]+q[1]*q[1]+q[2]*q[2])-(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))*(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))));
			(*c)->mVelNL[1] += (kappa*((2*(q[0]*q[0]+q[1]*q[1]+q[2]*q[2])+(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2])))/(sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2])+(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))+(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]))) - (2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))/sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2]))) * (p[2]*q[0] - p[0]*q[2]))/(2*M_PI*(4*(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])*(q[0]*q[0]+q[1]*q[1]+q[2]*q[2])-(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))*(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))));
			(*c)->mVelNL[2] += (kappa*((2*(q[0]*q[0]+q[1]*q[1]+q[2]*q[2])+(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2])))/(sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2])+(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))+(q[0]*q[0]+q[1]*q[1]+q[2]*q[2]))) - (2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))/sqrt((p[0]*p[0]+p[1]*p[1]+p[2]*p[2]))) * (p[0]*q[1] - p[1]*q[0]))/(2*M_PI*(4*(p[0]*p[0]+p[1]*p[1]+p[2]*p[2])*(q[0]*q[0]+q[1]*q[1]+q[2]*q[2])-(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))*(2*(p[0]*q[0]+p[1]*q[1]+p[2]*q[2]))));	
			/* increment pointer to next */
			c3 = c3->mNext;
			i++;
		}
	}
}
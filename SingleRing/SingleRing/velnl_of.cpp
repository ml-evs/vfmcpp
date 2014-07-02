// Calculates the non-local contribution to the velocities of points on each ring from another filament
// Adapted from CalcNonLocalVel_OtherFilament.m by Paul Walmsley

#include "velnl_of.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

	const double kappa = 9.98e-8, pi = 3.14159265358979323846;

vector <vector <double> > VelocityNL_OF(vector <vector <double> > Ring, vector <vector <double> > Ring2){

	int L1 = Ring.size();
	int L2 = Ring2.size();
	vector <vector <double> > velnl_of(L1), q; // q = s_l+1 - s_l
	vector <double> blank(3);
	
	for(int l=1;l<L2;l++){
			q.push_back(blank);
			for(int m=0;m<3;m++){
				q[l-1][m] = (Ring2[l][m]-Ring2[l-1][m]);
			}
			//cout << "l:	" << l << " has been filled;	q has size " << q.size() << " and q[k][0], q[k][1], q[k][2] are " << q[l-1][0] << ", " << q[l-1][1] << ", " << q[l-1][2] << endl;
	} // End q calculation

	for (int k=0;k<L1;k++){
		vector < vector <double> > p; // p = s_l - s_k
		vector <double> A, B, C, D;
		vector < vector <double> > pxq;
		
		for(int l=0;l<L2;l++){
			p.push_back(blank);
			for(int m=0;m<3;m++){
				p[l][m] = (Ring2[l][m]-Ring[k][m]);	
			}
			//cout << "k:	" << k << ", l:	" << l << " has been filled;	p has size " << p.size() << " and p[k][0], p[k][1], p[k][2] are " << p[l][0] << ", " << p[l][1] << ", " << p[l][2] << endl;
		} // End p calculation
		

	// Calculate coefficients: A=|p|, B=2*p.q, C=|q|
		for(int l=0;l<L1-1;l++){
			pxq.push_back(blank);			
			pxq[l][0] = p[l][1]*q[l][2] - p[l][2]*q[l][1];
			pxq[l][1] = p[l][2]*q[l][0] - p[l][0]*q[l][2];
			pxq[l][2] = p[l][0]*q[l][1] - p[l][1]*q[l][0];
			
			A.push_back(3); B.push_back(3); C.push_back(3);
			A[l] = 0; B[l] = 0; C[l] = 0;
			for(int m=0;m<3;m++){
				A[l] += p[l][m]*p[l][m]; 
				B[l] += 2*p[l][m]*q[l][m];
				C[l] += q[l][m]*q[l][m];
			}
			
			// Total nonlocal contribution at point k is found by summing over all elements l, D.(p x q)
			velnl_of.push_back(blank);
			velnl_of[k].resize(3);
			for(int m=0;m<3;m++){
				velnl_of[k][m] += (kappa*(((2*C[l])+B[l])/(sqrt(A[l]+B[l]+C[l])) - B[l]/sqrt(A[l])) * pxq[l][m])/(2*pi*(4*A[l]*C[l]-B[l]*B[l]));	
			}
			cout << "k:	" << k << ", l:	" << l << ", has velnl_of[k][0]: " << velnl_of[k][0] << endl;
		}
	}

	return velnl_of;
}


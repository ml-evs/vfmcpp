// Calculates the non-local contribution to the velocities of points on each ring
// Adapted from CalcNonLocalVel.m by Paul Walmsley

#include "velnl.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

	const double kappa = 9.98e-8, pi = 3.14159265358979323846;
	

vector <vector <double> > VelocityNL(vector <vector <double> > Ring){
	
	int L = Ring.size();
	vector <vector  <double> > velnl(L), blank_temp(3);
	vector <double> blank(3);

	for (int k=0;k<L;k++){
		vector < vector <double> > cross, velnl_temp, A,B,C,D,A_temp,B_temp,C_temp,D_temp;;
//		vector < vector < vector <double> > > A_temp,B_temp,C_temp,D_temp;
		vector < vector <double> > p; // p = s_l - s_k
		vector < vector <double> > q; // q = s_l+1 - s_l
				
		if(k==0){
			for(int l=1;l<L-1;l++){ 
				//cout << "p:		" << k << ", " << l << endl;
				p.push_back(blank);
					for(int m=0;m<3;m++){
					p[l-1][m] = (Ring[l][m]-Ring[k][m]);
					}
					cout << "k:	" << k << ", l:	" << l << " has been filled;	p has size " << p.size() << " and p[l-1][0] is " << p[l-1][0] << endl;
			}
		}

		else{	for(int l=1;l<L;l++){
					p.push_back(blank);
					if(l==k){ cout << "k: " << k << ",	l: " << l << " has been skipped;	p has size " << p.size() << " and p[l][0] is " << p[l-1][0] << endl;
					}
					else{
					for(int m=0;m<3;m++){
						p[l-1][m] = (Ring[l][m]-Ring[k][m]);
					}
					cout << "k: " << k << ",	l: " << l << " has been filled;	p has size " << p.size() << " and p[l][0] is " << p[l-1][0] << endl;
					}
				}
				/*for(int l=k+1;l<L;l++)
					cout << "p:		" << k << ", " << l << endl;
					p.push_back(blank);
					for(int m=0;m<3;m++){
						p[k][m] = (Ring[l][m]-Ring[k][m]);
					}
				}*/
		} // End filling p
		

		if(k==L-1)	for(int l=0;l<L-1;l++){
						q.push_back(blank);
						for(int m=0;m<3;m++){q[k][m] = (Ring[l+1][m]-Ring[l][m]);						
						}
						cout << "k:	" << k << ", l: " << l << " has been filled; q has size " << q.size() << " and q[l][0] is " << q[l-1][0] << endl;
					}

		else if(k==0) for(int l=1;l<L;l++){
						q.push_back(blank);
						for(int m=0;m<3;m++){q[k][m] = (Ring[l][m]-Ring[l-1][m]);		
						}
						cout << "k:	" << k << ", l: " << l << " has been filled; q has size " << q.size() << " and q[l][0] is " << q[l-1][0] << endl;
					}

		else{	for(int l=0;l<k;l++){
					q.push_back(blank);
					cout << q[k].size();
					cout << k << l << endl;
					for(int m=0;m<3;m++){q[k][m] = (Ring[l][m]-Ring[l-1][m]);
					}
					cout << "k:	" << k << ", l: " << l << " has been filled; q has size " << q.size() << " and q[l][0] is " << q[l-1][0] << endl;
				}

				for(int l=k+1;l<L;l++){
					q.push_back(blank);
					for(int m=0;m<3;m++){q[l-1][m] = (Ring[l][m]-Ring[k][m]);
					}
					cout << "k:	" << k << ", l: " << l << " has been filled; q has size " << q.size() << " and q[l][0] is " << q[l-1][0] << endl;
				}
				
		} // End filling q

	

	// Calculate coefficients: A=|p|, B=p.q, C=|q|
//				A.push_back(blank); B.push_back(blank); C.push_back(blank); D.push_back(blank);
//				A_temp.push_back(blank); B_temp.push_back(blank); C_temp.push_back(blank); D_temp.push_back(blank);
//
//				for(int l=0;l<L;l++){
//					for(int j=0;j<3;j++){
//						A_temp[l][j] = p[l][j]*p[l][j];
//						A_temp[l][j] = sqrt(A_temp[l][j]);
//						B_temp[l][j] = p[l][j]*q[l][j];
//						C_temp[l][j] = q[l][j]*q[l][j];
//						C_temp[l][j] = sqrt(C_temp[l][j]);
//
//						cout << "k, l:	" << k << l << "; " << '\n' << "A_temp, B_temp, C_temp:	" << A_temp[l][j] << ", " << B_temp[l][j] << ", " << C_temp[l][j] << endl;
//						
//						// The total contribution at point k is found by summing over all other points, l
//						A[k][j]=A[k][j]+A_temp[l][j];
//						B[k][j]=B[k][j]+B_temp[l][j];
//						C[k][j]=C[k][j]+C_temp[l][j];
//
//					cout << "A, B, C:	" << A[k][j] << "	" << B[k][j] << "	" << C[k][j] << endl;
//
//	// Expression for coefficient D given by Samuels in Barenghi et al, Quantized Vortex Dynamics and Superfluid Turbulence, Springer (2001)
//					D[k][j] = (kappa/(4*pi))*((A[k][j]+C[k][j])/(A[k][j]*C[k][j]*(A[k][j]*C[k][j]+B[k][j])));
//					cout << "D	" << D[k][j] << endl;
//					}
//				//}
//
//	// Calculate the cross product of p and q
//				cross.push_back(blank);
//				cross[k][0] = p[k][1]*q[k][2] - p[k][2]*q[k][1];
//				cross[k][1] = p[k][2]*q[k][0] - p[k][0]*q[k][2];
//				cross[k][2] = p[k][0]*q[k][1] - p[k][1]*q[k][0];
//				cout << cross[k][0] << "	" << cross[k][1] << "	" << cross[k][2] << endl;
//				}
//	// Total nonlocal contribution is found by summing over all elements, D.(p cross q)
//
////		vector < vector <double> > velnl; vector < vector <double> > velnl_temp;
//				velnl_temp.push_back(blank);
//				for(int j=0;j<3;j++){
//					velnl_temp[k][j] = D[k][j]*cross[k][j]; // Contribution of each element
//					cout << "k: j:	" << k << j << "D, cross, temp	" << D[k][j] << "	" << cross[k][j] << "	" << velnl_temp[k][j] << endl;
//				}
//
//			//for(int j=0;j<3;j++){
//			//	velnl[k] = accumulate(velnl_temp[j].begin(), velnl_temp[j].end(), 0); // Total contribution
//			//}
//
//			velnl[k].resize(L,3);
//
//			for(int j=0;j<3;j++){		
//
//				velnl[k][j] = velnl[k][j] + velnl_temp[k][j];
			//velnl[k][j] = accumulate(velnl_temp[k].begin(), velnl_temp[k].end(), 0); // Total contribution
		//cout << "Finished attempt	" << k << ",	velnl and temp" << j << "is	" << velnl[k][j] << velnl_temp[k][j] << endl;
//			}
		//}

	return velnl;

	}
}
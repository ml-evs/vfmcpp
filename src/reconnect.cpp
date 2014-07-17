
#include "tangle.h"
#include "point.h"

using namespace std;

void Tangle::Reconnect(double dr){
	/* check distance between every point on every filament */
	vector <Filament*>::iterator b, c, e, o_b, o_c, o_e;
	b = mTangle.begin(); e = mTangle.end();
	o_b = b; o_e = e;
	for (c = b; c < e; c++){
		for (o_c = o_b; o_c < o_e; o_c++){
			/* self-reconnection forms new ring at cusp */
			if(o_c == c){
				/* iterate along filament for new test point */
				vector <Point*>::iterator bself, cself, eself;
				Point* pTest;
				bself = (*c)->mPoints.begin(); eself = (*c)->mPoints.begin();
				for(cself=bself;cself!=eself;cself++){
					/* iterate along filament for point to check against */
					pTest = (*cself)->mNext;
					int i(0);
					/* ignore if neighbour - dealt with during mesh adjustment */
					while(i<(*c)->mN-2){
						pTest = pTest->mNext; // start two points away
						/* check if non-neighbouring points are too close */
						if(pow(pTest->mPos[0] - (*cself)->mPos[0], 2) + pow(pTest->mPos[1] - (*cself)->mPos[1], 2) + pow(pTest->mPos[2] - (*cself)->mPos[2], 2) < dr*dr){
							cout << " - - - - Performing self-reconnection - - - - " << endl;
							/* reassign pointers to separate new ring */ 
							(*cself)->mNext->mPrev = pTest->mPrev;  
							pTest->mPrev->mNext = (*cself)->mNext;
							Point* pNew = (*cself)->mNext;
							Point* pNext;
							/* create new ring in tangle */
							mTangle.push_back(new Ring());
							do{
								/* push back position and velocities of new points to tangle */
								mTangle.back()->mPoints.push_back(new Point(pNew));
								mTangle.back()->mN++;
								pNext = pNew->mNext;
								/* delete point from old ring */
								delete pNew;
								pNew = pNext;
							}while(pNew!=(*cself)->mNext);
							/* count number of points on new ring then assign their pointers in order*/
							int N_new = mTangle.back()->mN;
							for(int i(1); i!=N_new; i++){mTangle.back()->mPoints[i]->mPrev = mTangle.back()->mPoints[i-1];}
							mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[mTangle.back()->mN-1];
							for(int i(0); i!=N_new; i++){mTangle.back()->mPoints[i]->mNext = mTangle.back()->mPoints[i+1];}
							mTangle.back()->mPoints[N_new-1]->mNext = mTangle.back()->mPoints[0];
							/* reassign pointers on new ring to close off new ring */
							(*cself)->mNext = pTest;
							pTest->mPrev = (*cself);
						}
						i++;
					}
				}
			}
			///* reconnections involving another filament */
			else{ 
				for (int k(0); k < (*c)->mN; k++){
					for (int l(0); l < (*o_c)->mN; l++){
						if (pow((*c)->mPoints[k]->mPos[0] - (*o_c)->mPoints[l]->mPos[0], 2) + pow((*c)->mPoints[k]->mPos[1] - (*o_c)->mPoints[l]->mPos[1], 2) + pow((*c)->mPoints[k]->mPos[2] - (*o_c)->mPoints[l]->mPos[2], 2) < dr*dr){
							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
							double dot_tangents = (*c)->mPoints[k]->mSPrime[0] * (*o_c)->mPoints[l]->mSPrime[0] +(*c)->mPoints[k]->mSPrime[1] * (*o_c)->mPoints[l]->mSPrime[1] +(*c)->mPoints[k]->mSPrime[2] * (*o_c)->mPoints[l]->mSPrime[2];
							if(dot_tangents < 0){cout << "Parallel lines too close - not reconnecting." << endl;}
							else{
								cout << " - - - - Performing reconnection  - - - - " << endl;
								(*c)->mPoints[k]->mPrev->mNext = (*o_c)->mPoints[l]->mNext;
								(*c)->mPoints[k]->mNext->mPrev = (*o_c)->mPoints[l]->mPrev;
								(*o_c)->mPoints[l]->mPrev->mNext = (*c)->mPoints[k]->mNext;
								(*o_c)->mPoints[l]->mNext->mPrev = (*c)->mPoints[k]->mPrev;
								(*c)->mN--;
								(*o_c)->mN--;
								/* copy points from the other filament to the current filament and delete */
								(*c)->mN = (*c)->mN + (*o_c)->mN;
								int j;
								Point* occ;
								occ = (*o_c)->mPoints[l]->mNext;
								do{
									(*c)->mPoints.push_back(new Point(occ));
									occ = occ->mNext;
								}while(occ!=(*o_c)->mPoints[l]);
								j = (*c)->mN-1;
								while (j > (*c)->mN - (*o_c)->mN + 1){
									(*c)->mPoints[j]->mPrev = (*c)->mPoints[j-1];
									(*c)->mPoints[j]->mNext = (*c)->mPoints[j+1];
									j--;
								}
								(*c)->mPoints[(*c)->mN-(*o_c)->mN+1]->mNext = (*c)->mPoints[(*c)->mN-(*o_c)->mN+2];
								(*c)->mPoints[(*c)->mN-(*o_c)->mN+1]->mPrev = (*c)->mPoints[k]->mPrev;
								(*c)->mPoints.back()->mNext = (*c)->mPoints[k]->mNext;
								(*c)->mPoints.back()->mPrev = (*c)->mPoints[(*c)->mN-1];								
								/* delete the connecting points */
								delete (*c)->mPoints[k];
								(*c)->mPoints.erase((*c)->mPoints.begin() + k);
								for(unsigned int q(0); q<(*o_c)->mPoints.size(); q++){
									delete (*o_c)->mPoints[q];
								}
								delete (*o_c);
								mTangle.erase(o_c);
							}
						}
					}
				}
			}
		}
		o_b++;
	} 
}
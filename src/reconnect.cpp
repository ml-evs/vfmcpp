
#include "tangle.h"
#include "point.h"

using namespace std;

void Tangle::Reconnect(double dr){
	/* check distance between every point on every filament */
	vector <Filament>::iterator b, c, e, o_b, o_c, o_e;;
	b = mTangle.begin(); e = mTangle.end();
	for (c = b; c < e; c++){
		for (o_c = o_b; o_c < o_e; o_c++){
<<<<<<< HEAD
			/* self-reconnection forms new ring at cusp */
			if(o_c == c){
				/* iterate along filament for new test point */
				vector <Point*>::iterator bself, cself, eself;
				Point* pTest;
				bself = c->mPoints.begin(); eself = c->mPoints.begin();
				for(cself=bself;cself!=eself;cself++){
					/* iterate along filament for point to check against */
					pTest = (*cself)->mNext;
					int i(0);
					/* ignore if neighbour - dealt with during mesh adjustment */
					while(i<c->mN-2){
						pTest = pTest->mNext; // start two points away
						/* check if non-neighbouring points are too close */
						if(pow(pTest->mPos[0] - (*cself)->mPos[0], 2) + pow(pTest->mPos[1] - (*cself)->mPos[1], 2) + pow(pTest->mPos[2] - (*cself)->mPos[2], 2) < dr*dr){
							/* reassign pointers to separate new ring */ 
							(*cself)->mNext->mPrev = pTest->mPrev;
							pTest->mPrev->mNext = (*cself)->mNext;
							Point* pNew = (*cself)->mNext;
							Point* pNext;
							/* create new ring in tangle */
							mTangle.push_back(new Ring());
							do{
								/* push back position and velocities of new points to tangle */
								mTangle.back().mPoints.push_back(new Point(pNew));
								mTangle.back().mN++;
								pNext = pNew->mNext;
								/* delete point from old ring */
								delete pNew;
								pNew = pNext;
							}while(pNew!=(*cself)->mNext)
							/* count number of points on new ring then assign their pointers in order*/
							N_new = mTangle.back().mN;
							for(int i(1); i!=N_new; i++){(mPoints[i])->mPrev = mPoints[i-1];}
							mTangle.back().mPoints[0]->mPrev = mTangle.back().mPoints[mN-1];
							for(int i(0); i!=N_new; i++){(mPoints[i])->mNext = mPoints[i+1];}
							mTangle.back().mPoints[N_new-1]->mNext = mTangle.back().mPoints[0];
							/* reassign pointers on new ring to close off new ring */
							(*cself)->mNext = pTest;
							pTest->mPrev = (*cself)
=======
			if (o_c == c){ // Self-reconnection
				for (int k(0); k < c->mN; k++){
					for (int l(0); l < c->mN; l++){
						if (l == k - 1 || l == k || l == k + 1){ continue; }
						else if (sqrt(pow(c->mPoints[k]->mPos[0] - c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[1] - c->mPoints[l]->mPos[1], 2) + pow(c->mPoints[k]->mPos[2] - c->mPoints[l]->mPos[2], 2)) < dr) {
							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
							c->mPoints[k]->mPrev->mNext = c->mPoints[l]->mNext;
							c->mPoints[k]->mNext->mPrev = c->mPoints[l]->mPrev;
							c->mPoints[l]->mNext->mPrev = c->mPoints[k]->mPrev;
							c->mPoints[l]->mPrev->mNext = c->mPoints[k]->mNext;
							/* create a new ring containing points in range k+1 to l-1, delete from current */
							Point* pNext(c->mPoints[k]->mNext);
							int j(0);
							vector <Point*> copy;
							while (pNext != c->mPoints[l]){
								copy[j] = pNext;
								pNext = pNext->mNext;
								j++;
								delete c->mPoints[k]->mPrev;
							}
							c->mPoints.erase(c->mPoints.begin() + k);
							c->mPoints.erase(c->mPoints.begin() + l);
							Ring NewRing(copy);
>>>>>>> 529b47dc1b6dabcd67d13ff35ee4b4c4bbb931c9
						}
						i++;
					}
				}
			}
<<<<<<< HEAD
			/* reconnections involving another filament */
//			else{ 
//				for (int k(0); k < c->mN; k++){
//					for (int l(0); l < o_c->mN; l++){
//						if (sqrt(pow(c->mPoints[k]->mPos[0] - o_c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[1] - o_c->mPoints[l]->mPos[1], 2) + pow(c->mPoints[k]->mPos[2] - o_c->mPoints[l]->mPos[2], 2)) < dr) {
//							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
//							if ((sqrt(pow(c->mPoints[k]->mNext->mPos[0] - o_c->mPoints[l]->mPrev->mPos[0], 2) + pow(c->mPoints[k]->mNext->mPos[1] - o_c->mPoints[l]->mPrev->mPos[1], 2) + pow(c->mPoints[k]->mNext->mPos[2] - o_c->mPoints[l]->mPrev->mPos[2], 2))) < sqrt(pow(c->mPoints[k]->mNext->mPos[0] - o_c->mPoints[l]->mNext->mPos[0], 2) + pow(c->mPoints[k]->mNext->mPos[1] - o_c->mPoints[l]->mNext->mPos[1], 2) + pow(c->mPoints[k]->mNext->mPos[2] - o_c->mPoints[l]->mNext->mPos[2], 2))){
//								c->mPoints[k]->mPrev->mNext = o_c->mPoints[l]->mNext;
//								c->mPoints[k]->mNext->mPrev = o_c->mPoints[l]->mPrev;
//								o_c->mPoints[l]->mPrev->mNext = c->mPoints[k]->mNext;
//								o_c->mPoints[l]->mNext->mPrev = c->mPoints[k]->mPrev;
//							}
//							else{
//								c->mPoints[k]->mPrev->mNext = o_c->mPoints[l]->mPrev;
//								c->mPoints[k]->mNext->mPrev = o_c->mPoints[l]->mNext;
//								o_c->mPoints[l]->mPrev->mNext = c->mPoints[k]->mPrev;
//								o_c->mPoints[l]->mNext->mPrev = c->mPoints[k]->mNext;
//							}
//							c->mN--;
//							o_c->mN--;
//							/* copy points from the other filament to the current filament and delete */
//							c->mN = c->mN + o_c->mN;
//							int j(0);
//							Point* occ;
//							occ = o_c->mPoints[l];
//							while (j < o_c->mN){
//								c->mPoints.push_back(new Point(occ));
//								occ = occ->mNext;
//								j++;
//							}
//							/* check if filaments are rings or strings */
//							/*int b;
//							if (c->mPoints[c->mN]->mNext != c->mPoints[c->mN + 1]){ b = 1; }
//							else{ b = 3; }*/
//							while (j > c->mN - o_c->mN + 1){
//								c->mPoints[l]->mPrev = o_c->mPoints[l]->mPrev;
//								c->mPoints[l]->mNext = o_c->mPoints[l]->mNext;
//								j--;
//							}
//							/* delete the connecting points */
//							c->mPoints.erase(c->mPoints.begin() + k);
//							mTangle.erase(o_c);
//						}
//					}
//				}
//			}
//		}
//		o_b++;
		} 
=======
			else{ // Reconnections involving another filament
				for (int k(0); k < c->mN; k++){
					for (int l(0); l < o_c->mN; l++){
						if (sqrt(pow(c->mPoints[k]->mPos[0] - o_c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[1] - o_c->mPoints[l]->mPos[1], 2) + pow(c->mPoints[k]->mPos[2] - o_c->mPoints[l]->mPos[2], 2)) < dr) {
							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
							if ((sqrt(pow(c->mPoints[k]->mNext->mPos[0] - o_c->mPoints[l]->mPrev->mPos[0], 2) + pow(c->mPoints[k]->mNext->mPos[1] - o_c->mPoints[l]->mPrev->mPos[1], 2) + pow(c->mPoints[k]->mNext->mPos[2] - o_c->mPoints[l]->mPrev->mPos[2], 2))) < sqrt(pow(c->mPoints[k]->mNext->mPos[0] - o_c->mPoints[l]->mNext->mPos[0], 2) + pow(c->mPoints[k]->mNext->mPos[1] - o_c->mPoints[l]->mNext->mPos[1], 2) + pow(c->mPoints[k]->mNext->mPos[2] - o_c->mPoints[l]->mNext->mPos[2], 2))){
								c->mPoints[k]->mPrev->mNext = o_c->mPoints[l]->mNext;
								c->mPoints[k]->mNext->mPrev = o_c->mPoints[l]->mPrev;
								o_c->mPoints[l]->mPrev->mNext = c->mPoints[k]->mNext;
								o_c->mPoints[l]->mNext->mPrev = c->mPoints[k]->mPrev;
							}
							else{
								c->mPoints[k]->mPrev->mNext = o_c->mPoints[l]->mPrev;
								c->mPoints[k]->mNext->mPrev = o_c->mPoints[l]->mNext;
								o_c->mPoints[l]->mPrev->mNext = c->mPoints[k]->mPrev;
								o_c->mPoints[l]->mNext->mPrev = c->mPoints[k]->mNext;
							}
							c->mN--;
							o_c->mN--;
							/* copy points from the other filament to the current filament and delete */
							c->mN = c->mN + o_c->mN;
							int j(0);
							Point* occ;
							occ = o_c->mPoints[l];
							while (j < o_c->mN){
								c->mPoints.push_back(new Point(occ));
								occ = occ->mNext;
								j++;
							}
							/* check if filaments are rings or strings */
							/*int b;
							if (c->mPoints[c->mN]->mNext != c->mPoints[c->mN + 1]){ b = 1; }
							else{ b = 3; }*/
							while (j > c->mN - o_c->mN + 1){
								c->mPoints[l]->mPrev = o_c->mPoints[l]->mPrev;
								c->mPoints[l]->mNext = o_c->mPoints[l]->mNext;
								j--;
							}
							/* delete the connecting points */
							c->mPoints.erase(c->mPoints.begin() + k);
							mTangle.erase(o_c);
						}
					}
				}
			}
		}
		o_b++;
>>>>>>> 529b47dc1b6dabcd67d13ff35ee4b4c4bbb931c9
	}
	for (c = mTangle.begin(); c != mTangle.end(); c++){
		c->CalcMeshLengths(); c->MeshAdjust(dr);
	}
}
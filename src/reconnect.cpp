
#include "tangle.h"
#include "point.h"

using namespace std;

void Tangle::Reconnect(double dr){
	/* check distance between every point on every filament */
	vector <Filament>::iterator b, c, e;
	b = mTangle.begin(); e = mTangle.end();
	vector <Filament>::iterator o_b, o_c, o_e;
	o_b = mTangle.begin(); o_e = mTangle.end();
	for (c = b; c < e; c++){
		for (o_c = o_b; o_c < o_e; o_c++){
			if (o_c == c){ // Self-reconnection
				for (int k(0); k < c->mN; k++){
					for (int l(0); l < c->mN; l++){
						if (l == k - 1 || l == k || l == k + 1){ continue; }
						else if (sqrt(pow(c->mPoints[k]->mPos[0] - c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[1] - c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[2] - c->mPoints[l]->mPos[2], 2)) < dr) {
							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
							c->mPoints[k]->mPrev->mNext = c->mPoints[l]->mNext;
							c->mPoints[k]->mNext->mPrev = c->mPoints[l]->mPrev;
							c->mPoints[l]->mNext->mPrev = c->mPoints[k]->mPrev;
							c->mPoints[l]->mPrev->mNext = c->mPoints[k]->mNext;
							/* create a new ring containing points in range k+1 to l-1, delete from current */
							Point* pNext(c->mPoints[k]->mNext);
							Point* occ;
							int j(0);
							vector <Point*> copy;
							while (pNext != c->mPoints[l]){
//							for (occ = c->mPoints[k]->mNext; occ != c->mPoints[l]->mPrev; occ++){
								//occ = c->mPoints[j];
								copy[j] = pNext; // pNext;
								//pNext = pNext->mNext;
								pNext = pNext->mNext;
								j++;
								delete c->mPoints[k]->mPrev;
							}
							c->mPoints.erase[k]; c->mPoints.erase[l];
						}
					}
				}
			}
			else{ // Reconnections involving another filament
				for (int k(0); k < c->mN; k++){
					for (int l(0); l < o_c->mN; l++){
						if (sqrt(pow(c->mPoints[k]->mPos[0] - o_c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[1] - o_c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[2] - o_c->mPoints[l]->mPos[2], 2)) < dr) {
							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
							c->mPoints[k]->mPrev->mNext = o_c->mPoints[l]->mNext;
							c->mPoints[k]->mNext->mPrev = o_c->mPoints[l]->mPrev;
							o_c->mPoints[l]->mPrev->mNext = c->mPoints[k]->mNext;
							o_c->mPoints[l]->mNext->mPrev = c->mPoints[k]->mPrev;
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
							if (c->mPoints[c->mN]->mNext = c->mPoints.begin || o_c->mPoints[o_c->mN]->mNext = o_c->mPoints.begin){ j = c->mN - 1; } // if either filament is a ring
							else{ j = c->mN + 1; }
							while (j > c->mN - o_c->mN + 1){
								c->mPoints[j]->mPrev = c->mPoints[j - 1];
								c->mPoints[j]->mNext = c->mPoints[j + 1];
								j--;
							}

							/* delete the connecting points */
							c->mPoints.erase[k];
							mTangle.erase(o_c);
						}
					}
				}
			}
		}
		o_b++;
	}
	for (c = mTangle.begin(); c != mTangle.end(); c++){
		c->CalcMeshLengths(); c->MeshAdjust(dr);
	}
}
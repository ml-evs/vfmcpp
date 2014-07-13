
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
						if (sqrt(pow(c->mPoints[k]->mPos[0] - c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[1] - c->mPoints[l]->mPos[0], 2) + pow(c->mPoints[k]->mPos[2] - c->mPoints[l]->mPos[2], 2)) < dr) {
							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
							c->mPoints[k]->mPrev->mNext = c->mPoints[l]->mNext;
							c->mPoints[k]->mNext->mPrev = c->mPoints[l]->mPrev;
							c->mPoints[l]->mNext->mPrev = c->mPoints[k]->mPrev;
							c->mPoints[l]->mPrev->mNext = c->mPoints[k]->mNext;
							/* create a new ring containing points in range k+1 to l-1, delete from current */
							int j(k+1);
							Point* occ;
							while (j < l - k + 1){
								occ = c->mPoints[j];
								c->mPoints.erase[j];
								// Copy point ooc to be passed into ring, possibly as vector?
								occ++;
							}
							c->mPoints.erase[k]; c->mPoints.erase[l];
							int b = 3; 
							string NewRing = "Ring" + b;
							Ring NewRing();
							b++;
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
							j = c->mN - 1;
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
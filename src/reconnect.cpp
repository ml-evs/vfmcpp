
#include "tangle.h"
#include "point.h"

using namespace std;

void Tangle::Reconnect(double dr){

	bool Reconnected = false;
	/* check distance between every point on every filament */
	vector <Filament*>::iterator b, c, e, o_b, o_c, o_e;
	b = mTangle.begin(); e = mTangle.end();
	o_b = b; o_e = e;

	for (c = b; c < e; c++){
		if(Reconnected==true) break;
		for (o_c = o_b; o_c < o_e; o_c++){
			if(Reconnected==true) break;
			/* self-reconnection forms new ring at cusp */
			if(o_c == c){
				/* iterate along filament for new test point */
				vector <Point*>::iterator bself, cself, eself;
				Point* pTest;
				bself = (*c)->mPoints.begin(); eself = (*c)->mPoints.end();
				for(cself=bself;cself!=eself;cself++){
					if(Reconnected==true) break;
					/* iterate along filament for point to check against */
					pTest = (*cself)->mNext;
					int i(0);
					/* ignore if neighbour - dealt with during mesh adjustment */
					while(i<(*c)->mN-3){
						pTest = pTest->mNext; // start two points away
						/* check if non-neighbouring points are too close */
						double dist2 = pow(pTest->mPos[0] - (*cself)->mPos[0], 2) + pow(pTest->mPos[1] - (*cself)->mPos[1], 2) + pow(pTest->mPos[2] - (*cself)->mPos[2], 2);
						if(dist2 < dr*dr){
							mN_f = 1;
							cout << "Point " << i << " is too close to current point, reconnecting." << endl;
							cout << " - - - - Performing self-reconnection - - - - " << endl;
							/* reassign pointers to separate new ring */ 
							(*cself)->mNext->mPrev = pTest->mPrev;  
							pTest->mPrev->mNext = (*cself)->mNext;
							Point* pNew = (*cself)->mNext;
							/* create new ring in tangle */
							mTangle.push_back(new Ring());
							do{
								/* push back position and velocities of new points to tangle */
								mTangle.back()->mPoints.push_back(new Point(pNew));
								mTangle.back()->mN++;
								pNew->mMarkedForDeletion = true;
								pNew = pNew->mNext;
							}while(pNew!=(*cself)->mNext);
							cout << " - - - - Copied points to new ring - - - - " << endl;
							/* count number of points on new ring then assign their pointers in order*/
							int N_new = mTangle.back()->mN;
							cout << " - - - - Assigning pointers - - - - " << endl;
							for(int i(1); i!=N_new; i++){
								mTangle.back()->mPoints[i]->mPrev = mTangle.back()->mPoints[i-1];}
							mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[mTangle.back()->mN-1]; // needs to be done for rings only
							for(int i(0); i!=N_new-1; i++){
								mTangle.back()->mPoints[i]->mNext = mTangle.back()->mPoints[i+1];}
							mTangle.back()->mPoints[N_new-1]->mNext = mTangle.back()->mPoints[0];

							/* reassign pointers on old ring to close off new ring */
							(*cself)->mNext = pTest;
							pTest->mPrev = (*cself);
							Reconnected = true;
							cout << " - - - - RECONNECTION COMPLETE - - - - " << endl;
							break;
						}
						i++;
					}
				}
			}
			///* reconnections involving another filament */
			else{
				if(Reconnected==true) break;  
				for (int k(0); k < (*c)->mN; k++){
					if(Reconnected==true) break;
					for (int l(0); l < (*o_c)->mN; l++){
						if(Reconnected==true) break;
						if (pow((*c)->mPoints[k]->mPos[0] - (*o_c)->mPoints[l]->mPos[0], 2) + pow((*c)->mPoints[k]->mPos[1] - (*o_c)->mPoints[l]->mPos[1], 2) + pow((*c)->mPoints[k]->mPos[2] - (*o_c)->mPoints[l]->mPos[2], 2) < dr*dr){
							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
							double dot_tangents = (*c)->mPoints[k]->mSPrime[0] * (*o_c)->mPoints[l]->mSPrime[0] +(*c)->mPoints[k]->mSPrime[1] * (*o_c)->mPoints[l]->mSPrime[1] +(*c)->mPoints[k]->mSPrime[2] * (*o_c)->mPoints[l]->mSPrime[2];
							if(dot_tangents > 0){cout << " - - - - Parallel lines too close - not reconnecting - - - -" << endl;}
							else{
								cout << " - - - - Performing reconnection - - - - " << endl;
								mN_f = 1;
								cout << " - - - - Assigning connecting pointers - - - - " << endl;
								(*c)->mPoints[k]->mPrev->mNext = (*o_c)->mPoints[l]->mNext;
								(*c)->mPoints[k]->mNext->mPrev = (*o_c)->mPoints[l]->mPrev;
								(*o_c)->mPoints[l]->mPrev->mNext = (*c)->mPoints[k]->mNext;
								(*o_c)->mPoints[l]->mNext->mPrev = (*c)->mPoints[k]->mPrev;
								(*c)->mN--;
								(*o_c)->mN--;
								/* copy points from the other filament to the current filament and delete */
								(*c)->mN = (*c)->mN + (*o_c)->mN;
								Point* occ;
								occ = (*o_c)->mPoints[l]->mNext;
								int i(0);
								while(i<(*o_c)->mN){
									(*c)->mPoints.push_back(new Point(occ));
									cout << "Creating new" << i << "th point." << endl; 
									occ = occ->mNext;
									i++;
								}
								int j = (*c)->mN-1;
								while (j > (*c)->mN - (*o_c)->mN + 1){
									(*c)->mPoints[j]->mPrev = (*c)->mPoints[j-1];
									(*c)->mPoints[j]->mNext = (*c)->mPoints[j+1];
									cout << "Assigning pointer " << j << ". " << endl;
									j--;
								}
								(*c)->mPoints[(*c)->mN-(*o_c)->mN+1]->mNext = (*c)->mPoints[(*c)->mN-(*o_c)->mN+2];
								(*c)->mPoints[(*c)->mN-(*o_c)->mN+1]->mPrev = (*c)->mPoints[k]->mPrev;
								(*c)->mPoints.back()->mNext = (*c)->mPoints[k]->mNext;
								(*c)->mPoints.back()->mPrev = (*c)->mPoints[(*c)->mN-1];								
								/* delete the connecting points */
								cout << " - - - - Deleting points - - - - " << endl;
								(*c)->mPoints[k]->mMarkedForDeletion = true;
								for(unsigned int q(0); q<(*o_c)->mPoints.size(); q++){
									delete (*o_c)->mPoints[q];
								}
								cout << " - - - - Deleting filament - - - - " << endl;
								delete (*o_c);
								mTangle.erase(o_c);
								Reconnected = true;
								cout << " - - - - RECONNECTION COMPLETE - - - - " << endl;
								break;
							}
						}
					}
				}
			}
		}
	} 

	if(Reconnected == true){
		/* dodgy point clean up */
		for(unsigned int n(0); n<mTangle.size(); n++){
			for(unsigned int m(0); m<mTangle[n]->mPoints.size(); m++){
				if(mTangle[n]->mPoints[m]->mMarkedForDeletion == true){
					delete mTangle[n]->mPoints[m];
					mTangle[n]->mPoints.erase(mTangle[n]->mPoints.begin() + m); 
				}
			}
		}
		cout << "- - - - Performing reconnection sweep - - - - " << endl;
		Reconnect(dr);
	}
}
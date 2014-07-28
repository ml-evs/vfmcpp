/* Reconnection algorithm following Baggaley, JLT 168:18-30 (2012). Currently other filament
   reconnections use the Type III algorithm and self-reconnections use Type I. */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <sstream>

using namespace std;

void Tangle::Reconnect(){

	double res = mDr;
	bool Reconnected = false;
	/* check distance between every point on every filament */
	for (unsigned int P(0);P!=mTangle.size();P++){
		if(Reconnected==true) break;
		for (unsigned int Q(0);Q!=mTangle.size(); Q++){
			if(Reconnected==true) break;
			/* self-reconnection forms new ring at cusp */
			if(P == Q){
				/* iterate along filament for new test point */
				vector <Point*>::iterator bself, cself, eself, ocself;
				bself = mTangle[P]->mPoints.begin(); eself = mTangle[P]->mPoints.end();
				for(cself=bself;cself!=eself;cself++){
					if(Reconnected==true) break;
					/* iterate along filament for point to check against */
					for(ocself=bself;ocself!=eself;ocself++){
						if(Reconnected==true) break;
						if((*ocself)==(*cself)||(*ocself)==(*cself)->mNext||(*ocself)==(*cself)->mPrev){continue;}
						else{
							/* check if non-neighbouring points are too close */
							double dist2 = pow((*ocself)->mPos[0] - (*cself)->mPos[0], 2) + pow((*ocself)->mPos[1] - (*cself)->mPos[1], 2) + pow((*ocself)->mPos[2] - (*cself)->mPos[2], 2);
							if(dist2 < 0.25*res*res){
								double dot_tangents = (*cself)->mSPrime[0] * (*ocself)->mSPrime[0] +(*cself)->mSPrime[1] * (*ocself)->mSPrime[1] +(*cself)->mSPrime[2] * (*ocself)->mSPrime[2];
								if(dot_tangents > 0){cout << " - - - - Parallel lines too close - not reconnecting - - - -" << endl;}
								else{
									mN_f = 1; mN_slow = 0;
									cout << " - - - - Performing self-reconnection - - - - " << endl;
									/* reassign pointers to separate new ring */ 
									(*cself)->mNext->mPrev = (*ocself)->mPrev;  
									(*ocself)->mPrev->mNext = (*cself)->mNext;
									Point* pNew = (*cself);
									/* create new ring in tangle */
									mTangle.push_back(new Ring());
									do{
										/* push back position and velocities of new points to tangle */
										mTangle.back()->mPoints.push_back(new Point(pNew));
										mTangle.back()->mN++;
										pNew = pNew->mPrev;
									}while(pNew!=(*ocself)->mPrev);

									/* count number of points on new ring then assign their pointers in order*/
									int N_new = mTangle.back()->mN;
									for(int d(1); d!=N_new-1; d++){
										mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
										mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
									}

									mTangle.back()->mPoints[N_new-2]->mPrev = mTangle.back()->mPoints[N_new-3];
									mTangle.back()->mPoints[N_new-2]->mNext = mTangle.back()->mPoints[0];
									mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
									//mTangle.back()->mPoints.back()->mPrev = mTangle.back()->mPoints[N_new-1];
									//mTangle.back()->mPoints.back()->mNext = mTangle.back()->mPoints.front();
									mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[N_new-2]; // needs to be done for rings only

									/* delete last duplcate point */
									delete mTangle.back()->mPoints.back();
									mTangle.back()->mPoints.erase(mTangle.back()->mPoints.end()-1);
									mTangle.back()->mN--;

									pNew = (*ocself)->mPrev;
									/* create new ring in tangle */
									mTangle.push_back(new Ring());
									do{
										/* push back position and velocities of new points to tangle */
										mTangle.back()->mPoints.push_back(new Point(pNew));
										mTangle.back()->mN++;
										pNew = pNew->mPrev;
									}while(pNew!=(*ocself)->mPrev);

									N_new = mTangle.back()->mN;
									for(int d(1); d!=N_new-1; d++){
										mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
										mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
									}

									mTangle.back()->mPoints[N_new-2]->mPrev = mTangle.back()->mPoints[N_new-3];
									mTangle.back()->mPoints[N_new-2]->mNext = mTangle.back()->mPoints[0];
									mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
									//mTangle.back()->mPoints.back()->mPrev = mTangle.back()->mPoints[N_new-1];
									//mTangle.back()->mPoints.back()->mNext = mTangle.back()->mPoints.front();
									mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[N_new-2]; // needs to be done for rings only

									/* delete last duplicate point */
									delete mTangle.back()->mPoints.back();
									mTangle.back()->mPoints.erase(mTangle.back()->mPoints.end()-1);
									mTangle.back()->mN--;

									for(unsigned int q(0); q<mTangle[P]->mPoints.size(); q++){
										delete mTangle[P]->mPoints[q];
									}
									mTangle.erase(mTangle.begin()+P);
									cout << "Tangle size = " << mTangle.size() << endl; 
									cout << "Filament sizes = " << mTangle.front()->mPoints.size() << endl;			
									cout << "Filament sizes = " << mTangle.back()->mPoints.size() << endl;			
									/* reassign pointers on old ring to close off new ring */
									Reconnected = true;
									cout << " - - - - SELF-RECONNECTION COMPLETE - - - - " << endl;
									break;
								}
							}
						}
					}
				}
			}
			///* reconnections involving another filament */
			else{
				if(Reconnected==true) break;  
				for (int k(0); k < mTangle[P]->mN; k++){
					if(Reconnected==true) break;
					for (int l(0); l < mTangle[Q]->mN; l++){
						if(Reconnected==true) break;
						double dist2 = pow(mTangle[P]->mPoints[k]->mPos[0] - mTangle[Q]->mPoints[l]->mPos[0], 2) + pow(mTangle[P]->mPoints[k]->mPos[1] - mTangle[Q]->mPoints[l]->mPos[1], 2) + pow(mTangle[P]->mPoints[k]->mPos[2] - mTangle[Q]->mPoints[l]->mPos[2], 2);
						if (dist2 < 0.25*res*res){
							/* reassign the neighbouring pointers for those adjacent to the point of reconnection */
							double dot_tangents = mTangle[P]->mPoints[k]->mSPrime[0] * mTangle[Q]->mPoints[l]->mSPrime[0] +mTangle[P]->mPoints[k]->mSPrime[1] * mTangle[Q]->mPoints[l]->mSPrime[1] +mTangle[P]->mPoints[k]->mSPrime[2] * mTangle[Q]->mPoints[l]->mSPrime[2];
							if(dot_tangents > 0){cout << " - - - - Parallel lines too close - not reconnecting - - - -" << endl;}
							else{
								//SaveState();
								cout << " - - - - Performing reconnection - - - - " << endl;
								mN_f = 1; mN_slow = 0;
								cout << " - - - - Assigning connecting pointers - - - - " << endl;
								mTangle[P]->mPoints[k]->mPrev->mNext = mTangle[Q]->mPoints[l]->mNext;
								mTangle[Q]->mPoints[l]->mNext->mPrev = mTangle[P]->mPoints[k]->mPrev;
								cout << mTangle[P]->mN << endl;
								cout << mTangle[Q]->mN << endl;

								/* copy points from the other filament to the current filament and delete */
								Point* occ;
								occ = mTangle[Q]->mPoints[l];
								int i(0);
								while(i<mTangle[Q]->mN){
									mTangle[P]->mPoints.push_back(new Point(occ));
									occ = occ->mPrev;
									i++;
								}
								for(unsigned int j(mTangle[P]->mN+1); j!= mTangle[P]->mPoints.size()-1; j++){
									mTangle[P]->mPoints[j]->mNext = mTangle[P]->mPoints[j-1];
									mTangle[P]->mPoints[j]->mPrev = mTangle[P]->mPoints[j+1];
								}
								mTangle[P]->mPoints[k]->mPrev->mNext = mTangle[P]->mPoints[mTangle[P]->mPoints.size()-1];
								mTangle[P]->mPoints[mTangle[P]->mN]->mPrev = mTangle[P]->mPoints[mTangle[P]->mN+1];
								mTangle[P]->mPoints[mTangle[P]->mN]->mNext = mTangle[P]->mPoints[k];
								mTangle[P]->mPoints.back()->mPrev = mTangle[P]->mPoints[k]->mPrev;
								mTangle[P]->mPoints.back()->mNext = mTangle[P]->mPoints[mTangle[P]->mPoints.size()-2];	
								mTangle[P]->mPoints[k]->mPrev = mTangle[P]->mPoints[mTangle[P]->mN];
								mTangle[P]->mN = mTangle[P]->mPoints.size();

								cout << k << endl;
								cout << l << endl;

								/* delete the connecting points */
								cout << " - - - - Deleting points - - - - " << endl;
								for(unsigned int q(0); q<mTangle[Q]->mPoints.size(); q++){
									delete mTangle[Q]->mPoints[q];
								}
								cout << " - - - - Deleting filament - - - - " << endl;
								delete mTangle[Q];
								mTangle.erase(mTangle.begin()+Q);
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
	/* cleanup points and recalculate mesh lengths and curvatures */
	if(Reconnected == true){
		for(unsigned int n(0); n<mTangle.size(); n++){
			mTangle[n]->CalcMeshLengths();	mTangle[n]->CalcSPrime(); 
			mTangle[n]->CalcS2Prime(); 
		}
		/* iterate through rest of list after reconnection */
		Reconnect();

	}
}
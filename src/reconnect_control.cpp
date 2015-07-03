/* 'Smart' reconnection algorithm that chooses whether to use Type I/II in Baggaley, JLT 168:18-30 (2012)
	depending on line length following reconnection. */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <sstream>

using namespace std;

void Tangle::Reconnection(){
	/* count number of possible reconnections this step */
	int recon_count = ReconnectionTest();
	if(recon_count!=0){mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tnumber of possible reconnections = " << recon_count << endl;}
	bool Reconnected(false);
	bool NeedRecon(false);
	/* keep trying to reconnect until all have been performed */
	begin:
	while(recon_count > 0){
		for (unsigned int P(0);P!=mTangle.size();P++){
			for (unsigned int Q(P);Q!=mTangle.size(); Q++){
				/* iterate along filament for new test point */
				for(int k(0); k<mTangle[P]->mN; k++){
					int l_rec;
					NeedRecon = false;
					/* find points marked for reconnection */
					if(mTangle[P]->mPoints[k]->mMarkedForRecon == true){
						double distlimit2(0.25*mDr*mDr);
						double mindist2(distlimit2);
						/* iterate along filament for point to check against */
						for(int l(0); l<mTangle[Q]->mN; l++){
							/* skip closest points - might lead to cusps */
							if(mTangle[P]->mPoints[k]==mTangle[Q]->mPoints[l]
								|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mNext
								|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mPrev
								|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mNext->mNext
								|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mPrev->mPrev){}
							else{
								double dist2 = pow(mTangle[P]->mPoints[k]->mPos[0] - mTangle[Q]->mPoints[l]->mPos[0],2);
								dist2 +=  pow(mTangle[P]->mPoints[k]->mPos[1] - mTangle[Q]->mPoints[l]->mPos[1], 2);
								dist2 += pow(mTangle[P]->mPoints[k]->mPos[2] - mTangle[Q]->mPoints[l]->mPos[2], 2);
								/* check if non-neighbouring points are too close */
								if(dist2 < distlimit2){
									/* make sure lines aren't parallel */
									double dot_tangents = mTangle[P]->mPoints[k]->mSPrime[0] * mTangle[Q]->mPoints[l]->mSPrime[0];
									dot_tangents += mTangle[P]->mPoints[k]->mSPrime[1] * mTangle[Q]->mPoints[l]->mSPrime[1];
									dot_tangents += mTangle[P]->mPoints[k]->mSPrime[2] * mTangle[Q]->mPoints[l]->mSPrime[2];
									/* find closest point to k inside range and mark it for reconnection, ignoring parallel lines */
									if(dot_tangents > 0.9){
										mLog << StringTime() << "\t" << setw(10) << mStep; 
										mLog << ":\t\t lines were parallel, no reconnection" << endl;
									}
									else if(dist2 < mindist2){mindist2 = dist2; NeedRecon = true; l_rec = l;}
								}
							}
						}
						/* perform reconnections */
						if(NeedRecon==true){
							/* reset recon flags */
							mTangle[P]->mPoints[k]->mMarkedForRecon = false;
							mTangle[Q]->mPoints[l_rec]->mMarkedForRecon = false;
							if(P==Q){
								if(mTangle[P]->mFlagType==0){
									mLog << StringTime() << "\t" << setw(10) << mStep; 
									mLog << ":\t\t calling SelfReconnect(" << P << ", " << Q << ", " << k << ", " << l_rec << ")" << endl; 
									SelfReconnect(P,Q,k,l_rec);
								}
								else{
									mLog << StringTime() << "\t" << setw(10) << mStep; 
									mLog << ":\t\t calling SelfReconnectLine(" << P << ", " << Q << ", " << k << ", " << l_rec << ")" << endl; 
									SelfReconnectLine(P,Q,k,l_rec);
								}
								Reconnected = true;
								recon_count--;
								goto begin;
							}
							else{
								mLog << StringTime() << "\t" << setw(10) << mStep; 
								mLog << ":\t\t calling Reconnect(" << P << ", " << Q << ", " << k << ", " << l_rec << ")" << endl; 
								Reconnect(P,Q,k,l_rec);
								Reconnected = true;
								recon_count--;
								goto begin;
							}
						}
					}
				}
			}
		}
	}
	/* cleanup points and recalculate mesh lengths and curvatures */
	if(recon_count == 0 && Reconnected == true){
		for(unsigned int n(0); n<mTangle.size(); n++){
			mTangle[n]->CalcMeshLengths();	mTangle[n]->CalcSPrime(); 
			mTangle[n]->CalcS2Prime(); 
		}
	}
}

int Tangle::ReconnectionTest(){
	int recon_count(0);
	bool NeedRecon(false);
	double distlimit2(0.25*mDr*mDr);
	int k;
	int l_rec;
  /* FIND MESH POINTS TO BE RECONNECTED */
	/* iterate over all filaments */
	for (unsigned int P(0);P!=mTangle.size();P++){
		for (unsigned int Q(P);Q!=mTangle.size(); Q++){
			/* iterate along filament for new test point */
			Point* pK = mTangle[P]->mPoints[0];
			int i(0);
			while(i<mTangle[P]->mN){
				NeedRecon = false;
				/* iterate along filament for point to check against */
				int j(0); 
				Point* pL = mTangle[Q]->mPoints[0];
				while(j<mTangle[Q]->mN){
					double mindist2(distlimit2);
					if(pK==pL
						|| pL==pK->mNext
						|| pL==pK->mPrev
						|| pL==pK->mNext->mNext
						|| pL==pK->mPrev->mPrev){}
					else{ 
						/* check if non-neighbouring points are too close */
						double dist2 = pow(pK->mPos[0] - pL->mPos[0],2);
						dist2 +=  pow(pK->mPos[1] - pL->mPos[1], 2);
						dist2 += pow(pK->mPos[2] - pL->mPos[2], 2);
						if(dist2 < distlimit2){
							NeedRecon = true;
							double dot_tangents = pK->mSPrime[0] * pL->mSPrime[0];
							dot_tangents += pK->mSPrime[1] * pL->mSPrime[1];
							dot_tangents += pK->mSPrime[2] * pL->mSPrime[2];
							if(dot_tangents > 0.9){mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tignoring parallel filaments" << endl;}
							/* find closest point to k inside range and mark it for reconnection */
							else if(dist2 < mindist2){
								mindist2 = dist2; 
								for(int l(0);l<mTangle[Q]->mN; l++){
									if(mTangle[Q]->mPoints[l] == pL){
										l_rec = l;
									}
								}
								for(int k_t(0);k_t<mTangle[P]->mN; k_t++){
									if(mTangle[P]->mPoints[k_t] == pK){
										k = k_t;
									}
								}
							}
						}
					}
					j++; pL = pL->mNext;
				}
				if(NeedRecon==true){
					if(!(mTangle[Q]->mPoints[l_rec]->mMarkedForRecon == true
						|| mTangle[Q]->mPoints[l_rec]->mMarkedForRecon == true
						|| mTangle[Q]->mPoints[l_rec]->mPrev->mMarkedForRecon == true
						|| mTangle[Q]->mPoints[l_rec]->mPrev->mPrev->mMarkedForRecon == true
						|| mTangle[Q]->mPoints[l_rec]->mNext->mMarkedForRecon == true
						|| mTangle[Q]->mPoints[l_rec]->mNext->mNext->mMarkedForRecon == true
						|| mTangle[P]->mPoints[k]->mMarkedForRecon == true
						|| mTangle[P]->mPoints[k]->mPrev->mMarkedForRecon == true
						|| mTangle[P]->mPoints[k]->mNext->mMarkedForRecon == true
						|| mTangle[P]->mPoints[k]->mPrev->mPrev->mMarkedForRecon == true
						|| mTangle[P]->mPoints[k]->mNext->mNext->mMarkedForRecon == true)){
						pK->mMarkedForRecon = true;
						mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tattempting to reconnect at " << k << ", " << l_rec << endl;
						recon_count++;
					}
				}
				i++; pK = pK->mNext;
			}
		}
	}
	return recon_count;
}
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
	bool NeedRecon(false);
	/* keep trying to reconnect until all have been performed */
	begin:
	while(recon_count != 0){
		for (unsigned int P(0);P!=mTangle.size();P++){
			for (unsigned int Q(P);Q!=mTangle.size(); Q++){
				/* iterate along filament for new test point */
				vector <Point*>::iterator c, oc, b, ob, e, oe;
				b = mTangle[P]->mPoints.begin();
				e = mTangle[P]->mPoints.end();
				for(c=b; c!=e; c++){
					int k, l_rec;
					NeedRecon = false;
					/* find points marked for reconnection */
					if((*c)->mMarkedForRecon == true){
						double distlimit2(0.25*mDr*mDr);
						double mindist2(distlimit2);
						/* iterate along filament for point to check against */
						ob = mTangle[Q]->mPoints.begin();
						oe = mTangle[Q]->mPoints.end();
						for(oc=ob; oc!=oe; oc++){
							/* skip closest points - might lead to cusps */
							if((*c)==(*oc) 
								|| (*oc)==(*c)->mNext
								|| (*oc)==(*c)->mPrev
								|| (*oc)==(*c)->mNext->mNext
								|| (*oc)==(*c)->mPrev->mPrev){}
							else{
								/* check if non-neighbouring points are too close */
								double dist2 = (*c)->Disp2((*oc));
								if(dist2 < distlimit2){
									/* make sure lines aren't parallel */
									double dot_tangents = (*c)->mSPrime[0] * (*oc)->mSPrime[0];
									dot_tangents += (*c)->mSPrime[1] * (*oc)->mSPrime[1];
									dot_tangents += (*c)->mSPrime[2] * (*oc)->mSPrime[2];
									/* find closest point to k inside range and mark it for reconnection, ignoring parallel lines */
									if(dot_tangents > 0.9){
										mLog << StringTime() << "\t" << setw(10) << mStep; 
										mLog << ":\t\tlines were parallel, no reconnection" << endl;
									}
									else if(dist2 < mindist2){mindist2 = dist2; NeedRecon = true; l_rec = oc-ob; k = c-b;}
								}
							}
						}
						/* perform reconnections */
						if(NeedRecon==true){
							mN_f = 100; mN_slow = 0;
							/* reset recon flags */
							mTangle[P]->mPoints[k]->mMarkedForRecon = false;
							mTangle[Q]->mPoints[l_rec]->mMarkedForRecon = false;
							if(P==Q){
								if(mTangle[P]->mFlagType==0){
									mLog << StringTime() << "\t" << setw(10) << mStep; 
									mLog << ":\t\tcalling SelfReconnect(" << P << ", " << Q << ", " << k << ", " << l_rec << ")" << endl; 
									SelfReconnect(P,Q,k,l_rec);
								}
								else{
									mLog << StringTime() << "\t" << setw(10) << mStep; 
									mLog << ":\t\tcalling SelfReconnectLine(" << P << ", " << Q << ", " << k << ", " << l_rec << ")" << endl; 
									SelfReconnectLine(P,Q,k,l_rec);
								}
							}
							else{
								mLog << StringTime() << "\t" << setw(10) << mStep; 
								mLog << ":\t\tcalling Reconnect(" << P << ", " << Q << ", " << k << ", " << l_rec << ")" << endl; 
								Reconnect(P,Q,k,l_rec);
							}
							string filename = "../data/debug/offset/";
							int q(999);
							int file_no(999);
							Output(filename, q, file_no);
							recon_count--;
							/* cleanup points and recalculate mesh lengths and curvatures */
							for(unsigned int n(0); n<mTangle.size(); n++){
								mTangle[n]->CalcMeshLengths();	mTangle[n]->CalcSPrime(); 
								mTangle[n]->CalcS2Prime(); 
							}
							goto begin;
						}
					}
				}
			}
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
						mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tneed to reconnect at " << k << ", " << l_rec << endl;
						recon_count++;
					}
				}
				i++; pK = pK->mNext;
			}
		}
	}
	return recon_count;
}
/* Reconnection algorithm following Type II in Baggaley, JLT 168:18-30 (2012). */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <sstream>

using namespace std;

void Tangle::Reconnection(){

	/* count number of possible reconnections this step */
	int recon_count = ReconnectionTest();
	double res = mDr;
	bool Reconnected(false);
	bool NeedRecon(false);
	bool Finished(false);
	/* keep trying to reconnect until all have been performed */
	if(recon_count!=0){
		while(recon_count > 0){
			if(Finished==true){break;}
			Reconnected = false;
			cout << "\nNumber of possible reconnections = " << recon_count << endl;
			for (unsigned int P(0);P!=mTangle.size();P++){
				if(Reconnected== true){break;}
				for (unsigned int Q(P);Q!=mTangle.size(); Q++){
					if(Reconnected== true){break;}
					/* iterate along filament for new test point */
					for(int k(0); k<mTangle[P]->mN; k++){
						int l_rec;
						NeedRecon = false;
						if(Reconnected== true){break;}
						/* find points marked for reconnection */
						if(mTangle[P]->mPoints[k]->mMarkedForRecon == true){
							double mindist2(0.25*res*res);
							/* iterate along filament for point to check against */
							for(int l(0); l<mTangle[Q]->mN; l++){
								if(Reconnected== true){break;}
								/* prevent reconnection from making loops too small */
								if(l==mTangle[Q]->mN-1){Finished=true; break;}
								if(mTangle[P]->mPoints[k]==mTangle[Q]->mPoints[l]
									|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mNext
									|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mPrev
									|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mNext->mNext
									|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mPrev->mPrev
									|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mNext->mNext->mNext
									|| mTangle[Q]->mPoints[l]==mTangle[P]->mPoints[k]->mPrev->mPrev->mPrev){
									continue;
								}
								else{
									double dist2 = pow(mTangle[P]->mPoints[k]->mPos[0] - mTangle[Q]->mPoints[l]->mPos[0],2);
									dist2 +=  pow(mTangle[P]->mPoints[k]->mPos[1] - mTangle[Q]->mPoints[l]->mPos[1], 2);
									dist2 += pow(mTangle[P]->mPoints[k]->mPos[2] - mTangle[Q]->mPoints[l]->mPos[2], 2);
									/* check if non-neighbouring points are too close */
									if(dist2 < 0.25*res*res){

										/* make sure lines aren't parallel */
										double dot_tangents = mTangle[P]->mPoints[k]->mSPrime[0] * mTangle[Q]->mPoints[l]->mSPrime[0];
										dot_tangents += mTangle[P]->mPoints[k]->mSPrime[1] * mTangle[Q]->mPoints[l]->mSPrime[1];
										dot_tangents += mTangle[P]->mPoints[k]->mSPrime[2] * mTangle[Q]->mPoints[l]->mSPrime[2];
										/* find closest point to k inside range and mark it for reconnection, ignoring parallel lines */
										if(dot_tangents > 0){cout << "\nparallel " << endl; continue;}
										else if(dist2 < mindist2){mindist2 = dist2; NeedRecon = true; l_rec = l;}
									}
								}
							}
							/* perform reconnections */
							if(NeedRecon==true){
								if(P==Q){
									cout << "Want to reconnect " << recon_count << " times" << endl;
									cout << k << ", " << l_rec << endl;
									cout << "Marking points " << endl;
									mTangle[P]->mPoints[k]->mMarkedForRecon = false;
									mTangle[Q]->mPoints[l_rec]->mMarkedForRecon = false;
									cout << "Calling func" << endl;
									if(mTangle[P]->mFlagType==0){
										SelfReconnect(P,Q,k,l_rec);
									}
									else SelfReconnectLine(P,Q,k,l_rec);
									Reconnected = true;
									mN_recon++;
									recon_count--;
									break;
								}
								else{
									cout << "Want to reconnect " << recon_count << " times" << endl;
									mTangle[P]->mPoints[k]->mMarkedForRecon = false;
									mTangle[Q]->mPoints[l_rec]->mMarkedForRecon = false;
									Reconnect(P,Q,k,l_rec);
									Reconnected = true;
									mN_recon++;
									recon_count--;
									break;
								}
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
	}
}

int Tangle::ReconnectionTest(){

	double res = mDr;
	int recon_count(0);
	bool NeedRecon(false);
  /* FIND MESH POINTS TO BE RECONNECTED */
	/* iterate over all filaments */
	for (unsigned int P(0);P!=mTangle.size();P++){
		for (unsigned int Q(P);Q!=mTangle.size(); Q++){
			/* iterate along filament for new test point */
			Point* pK = mTangle[P]->mPoints[0];
			int i(0);
			while(i<mTangle[P]->mN){
				int k;
				int l_rec;
				NeedRecon = false;
				double mindist2(0.25*res*res);
				/* iterate along filament for point to check against */
				int j(0); 
				Point* pL = mTangle[Q]->mPoints[0];
				while(j<mTangle[Q]->mN){
					if(pK==pL
						|| pL==pK->mNext
						|| pL==pK->mPrev
						|| pL==pK->mNext->mNext
						|| pL==pK->mPrev->mPrev){ 
						
					}
					else{ 
						/* check if non-neighbouring points are too close */
						double dist2 = pow(pK->mPos[0] - pL->mPos[0],2);
						dist2 +=  pow(pK->mPos[1] - pL->mPos[1], 2);
						dist2 += pow(pK->mPos[2] - pL->mPos[2], 2);
						if(dist2 < 0.25*res*res){
							NeedRecon = true;
							double dot_tangents = pK->mSPrime[0] * pL->mSPrime[0];
							dot_tangents += pK->mSPrime[1] * pL->mSPrime[1];
							dot_tangents += pK->mSPrime[2] * pL->mSPrime[2];
							if(dot_tangents > 0){continue;}
							/* find closest point to k inside range and mark it for reconnection */
							else if(dist2 < mindist2){
								mindist2 = dist2; 
								for(int l(0);l<mTangle[Q]->mN; l++){
									if(mTangle[Q]->mPoints[l] == pL){
										l_rec = l;
									}
								}
								for(int k_t(0);k<mTangle[P]->mN; k++){
									if(mTangle[P]->mPoints[k] == pK){
										k = k_t;
									}
								}
							}
						}
					}
					j++; pL = pL->mNext;
				}
				if(NeedRecon==true){
					if(mTangle[P]->mPoints[l_rec]->mMarkedForRecon == true
						|| mTangle[P]->mPoints[l_rec]->mPrev->mMarkedForRecon == true
						|| mTangle[P]->mPoints[l_rec]->mPrev->mPrev->mMarkedForRecon == true
						|| mTangle[P]->mPoints[l_rec]->mNext->mMarkedForRecon == true
						|| mTangle[P]->mPoints[l_rec]->mNext->mNext->mMarkedForRecon == true
						|| pK->mPrev->mMarkedForRecon == true
						|| pK->mNext->mMarkedForRecon == true
						|| pK->mPrev->mPrev->mMarkedForRecon == true
						|| pK->mNext->mNext->mMarkedForRecon == true){
					}
					else{
						pK->mMarkedForRecon = true;
						cout << endl << k << ", " << l_rec << endl;
					 	recon_count++;
					}
				}
				i++; pK = pK->mNext;
			}
		}
	}
	return recon_count;
}


void Tangle::SelfReconnectLine(int P, int Q, int k, int l){
 		cout << "Attempting line reconnection" << endl;
    mN_f = 1; mN_slow = 0;
    /* create new ring in tangle */
    mTangle.push_back(new Ring());
    /* separate new wring */
    mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[Q]->mPoints[l]->mPrev;  
		mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k]->mNext;
    Point* pNew = mTangle[P]->mPoints[k]->mNext;
    int i(0);
    do{
    	//	cout << "copied point " << i << endl;
        mTangle.back()->mPoints.push_back(new Point(pNew));
        mTangle.back()->mN++;
        pNew = pNew->mNext;
        i++;
    }while(pNew!=mTangle[Q]->mPoints[k]->mNext);
    /* reassign pointers to separate new ring */
    mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[Q]->mPoints[l]->mPrev;
    mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k]->mNext;
    mTangle[P]->mPoints[k]->mNext = mTangle[Q]->mPoints[l];
    mTangle[Q]->mPoints[l]->mPrev = mTangle[P]->mPoints[k];
    /* count points on new ring and assign pointers */
    int N_new = mTangle.back()->mN;
    for(int d(1); d<N_new-1; d++){
        mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
        mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
    }
    mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
    mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[N_new-1];
    mTangle.back()->mPoints[N_new-1]->mNext = mTangle.back()->mPoints[0];
    mTangle.back()->mPoints[N_new-1]->mPrev = mTangle.back()->mPoints[N_new-2];
    mTangle.back()->CalcMeshLengths();
    /* add new line of remaining points */
    mTangle.push_back(new String());
    int N_rem = mTangle[P]->mN - N_new;
    pNew = mTangle[P]->mPoints[0];
    /* push back position and velocities of new points to tangle */
    int v(0);
    do{
        mTangle.back()->mPoints.push_back(new Point(pNew));
        mTangle.back()->mN++;
        pNew = pNew->mNext;
        v++;
    }while(v!=N_rem);
    /* assign pointers and calculate dummies' positions*/
    for(int d(1); d!=N_rem-1; d++){
    mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
    mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
    }
    for (int i=0; i != 4; i++){
        mTangle.back()->mDummies.push_back(new Point()); // note that default constructor labels as not a dummy!
    }
    mTangle.back()->mPoints[N_rem-1]->mPrev = mTangle.back()->mPoints[N_rem-2];
    mTangle.back()->mPoints[N_rem-1]->mNext = mTangle.back()->mDummies[0];
    mTangle.back()->mDummies[0]->mPrev = mTangle.back()->mPoints[N_rem-1];
    mTangle.back()->mDummies[0]->mNext = mTangle.back()->mDummies[1];
    mTangle.back()->mDummies[1]->mPrev = mTangle.back()->mDummies[0];
    mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
    mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mDummies[2];
    mTangle.back()->mDummies[2]->mNext = mTangle.back()->mPoints[0];
    mTangle.back()->mDummies[2]->mPrev = mTangle.back()->mDummies[3];
    mTangle.back()->mDummies[3]->mNext = mTangle.back()->mDummies[2];
    mTangle.back()->CalcMeshLengths();
    mTangle.back()->CalcDummy();
    /* label dummies for further mesh calculations */
    for (int i=0; i != 4; i++){
        mTangle.back()->mDummies[i]->mFlagDummy = 1;
    }
    mTangle.back()->CalcMeshLengths();
    mTangle.back()->mPoints[0]->mFlagFilled = 5;// starting point should remain stationary
    for(unsigned int q(0); q<mTangle[P]->mPoints.size(); q++){
        delete mTangle[P]->mPoints[q];
    }
    mTangle.erase(mTangle.begin()+P);
    cout << "\t\t !!! LINE RECONNECTION !!! " << endl;
}

void Tangle::SelfReconnect(int P, int Q, int k, int l){
	mN_f = 1; mN_slow = 0;
	/* reassign pointers to separate new ring  */
	mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[Q]->mPoints[l]->mPrev;  
	mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k]->mNext;
	Point* pNew = mTangle[P]->mPoints[l];
	/* copy old ring to tangle */
	mTangle.push_back(new Ring());
	do{
		/* push back position and velocities of new points to tangle */
		mTangle.back()->mPoints.push_back(new Point(pNew));
		mTangle.back()->mN++;
		pNew = pNew->mNext;
	}while(pNew!=mTangle[Q]->mPoints[k]->mNext);	
	/* count number of points on new ring then assign their pointers in order*/
	int N_new = mTangle.back()->mN;
	for(int d(1); d!=N_new-1; d++){
		mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
		mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
	}	
	mTangle.back()->mPoints[N_new-1]->mPrev = mTangle.back()->mPoints[N_new-2];
	mTangle.back()->mPoints[N_new-1]->mNext = mTangle.back()->mPoints[0];
	mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
	mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[N_new-1]; 

	pNew = mTangle[Q]->mPoints[k]->mNext;
	/* create new ring in tangle */
	mTangle.push_back(new Ring());
	do{
		/* push back position and velocities of new points to tangle */
		mTangle.back()->mPoints.push_back(new Point(pNew));
		mTangle.back()->mN++;
		pNew = pNew->mNext;
	}while(pNew!=mTangle[Q]->mPoints[k]->mNext);	
	N_new = mTangle.back()->mN;
	/* reassign pointers */ 
	for(int d(1); d!=N_new-1; d++){
		mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
		mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
	}
	mTangle.back()->mPoints[N_new-1]->mPrev = mTangle.back()->mPoints[N_new-2];
	mTangle.back()->mPoints[N_new-1]->mNext = mTangle.back()->mPoints[0];
	mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
	mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[N_new-1]; 
	
	for(unsigned int q(0); q<mTangle[P]->mPoints.size(); q++){
		delete mTangle[P]->mPoints[q];
	}
	mTangle.erase(mTangle.begin()+P);
	cout << "\n\t\t !!! RECONNECTION !!! " << endl;
}

void Tangle::Reconnect(int P, int Q, int k, int l){

	mN_f = 1; mN_slow = 0;
	mTangle[P]->mPoints[k]->mPrev->mNext = mTangle[Q]->mPoints[l]->mNext;
	mTangle[Q]->mPoints[l]->mNext->mPrev = mTangle[P]->mPoints[k]->mPrev;
	/* copy points from the other filament to the current filament and delete */
	Point* occ;
	occ = mTangle[Q]->mPoints[l];
	int i(0);
	while(i<mTangle[Q]->mN){
		mTangle[P]->mPoints.push_back(new Point(occ));
		occ = occ->mPrev;
		i++;
	}
	/* reassign pointers */
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
	/* delete old ring */
	for(unsigned int q(0); q<mTangle[Q]->mPoints.size(); q++){
		delete mTangle[Q]->mPoints[q];
	}
	delete mTangle[Q];
	mTangle.erase(mTangle.begin()+Q);
	cout << "\n\t\t !!! RECONNECTION !!! " << endl;
}
/* Reconnection algorithm following Type II in Baggaley, JLT 168:18-30 (2012). */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <sstream>

using namespace std;

void Tangle::SelfReconnectLine(int Q, int P, int k, int l){
 		mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tattempting line reconnection" << endl;
    mN_f = 1; mN_slow = 0;
    /* create new ring in tangle */
    mTangle.push_back(new Ring());
    /* separate new wring */
    mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[Q]->mPoints[l]->mPrev;  
	mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k]->mNext;
    Point* pNew = mTangle[P]->mPoints[k]->mNext;
    int i(0);
    do{
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
    mN_recon++;
    mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tsuccessful line reconnection" << endl;
}

void Tangle::SelfReconnect(int P, int Q, int k, int l){
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tattempting self-reconnection" << endl;
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
	mN_recon++;
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tsuccessful self-reconnection" << endl;
}

void Tangle::Reconnect(int Q, int P, int l, int k){
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tattempting reconnection" << endl;
	mN_f = 1; mN_slow = 0;
	if(mTangle[Q]->mFlagType == 1){int swap(Q); Q = P; P = swap; swap = l; l = k; k = swap;}
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
	mN_recon++;
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tsuccessful reconnection" << endl;
}

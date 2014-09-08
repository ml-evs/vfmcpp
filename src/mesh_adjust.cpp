/*  Adds or removes points to keep the resolution within a fixed range
Details given by Baggaley and Barenghi, PRB 83 (2011) */

#include "tangle.h"

using namespace std;

bool Tangle::MeshAdjust(){
    double dr = mDr;
    vector <Filament*>::iterator b, c, e;
    b = mTangle.begin(); e = mTangle.end();
    for(c=b;c!=e;c++){
        for (int k((*c)->mFlagType); k<(*c)->mN; k++){
            /* define to check local curvature */
            double R(0); // 1/|s''| at point k
            for(int j(0);j!=3;j++){
                R += pow(((*c)->mPoints[k]->mNext->mS2Prime[j] + (*c)->mPoints[k]->mPrev->mS2Prime[j])/2,2);
            }
            R = sqrt(R);
            R = 1/R;
            /* point deletion for smoothing and proximity*/
            if((1/R)>1.9/dr || (*c)->mPoints[k]->mSegLength < 0.5*dr){
                //cout << "\nRemoving point at " << (*c)->mPoints[k]->mPos[0] << ", " << (*c)->mPoints[k]->mPos[1] << ", " << (*c)->mPoints[k]->mPos[2] << endl;
                mLog << ctime(&(time(0))).substr(10,8) << "\t" << mStep << ":\t\tremoving point at " << (*c)->mPoints[k]->mPos[0] << ", " << (*c)->mPoints[k]->mPos[1] << ", " << (*c)->mPoints[k]->mPos[2] << " ";
                if((1/R)>1.9/dr){mLog << "for curvature ";}
                mLog << (*c)->mPoints[k]->mSegLength << " vs " << 0.5*dr << endl; 
                if((*c)->mPoints[k]->mCharge != 0){ // don't remove the electron!
                    //cout << "Moving the electron to previous point!" << endl;
                    (*c)->mPoints[k]->mPrev->mCharge = 1.6e-19;
                    (*c)->mPoints[k]->mCharge = 0;
                }
                /* reassign next and last pointers for point to be deleted */
                (*c)->mPoints[k]->mNext->mPrev = (*c)->mPoints[k]->mPrev;
                (*c)->mPoints[k]->mPrev->mNext = (*c)->mPoints[k]->mNext;
                /* erase point from (*c)->mPoints and decrement mN */
                (*c)->mN--;
                delete (*c)->mPoints[k];
                (*c)->mPoints.erase((*c)->mPoints.begin()+k);
                (*c)->CalcMeshLengths(); (*c)->CalcSPrime(); (*c)->CalcS2Prime();
                return false;
            }
            /* point addition */
            else if ((*c)->mPoints[k]->mSegLength > 2*dr){
                /*cout << "\n\nadding at " << k << " / " << (*c)->mN << endl;*/
                /* increment mN */
                (*c)->mN++;
                /* create new point and reassign pointers */
                (*c)->mPoints.push_back(new Point());
                (*c)->mPoints.back()->mPrev = (*c)->mPoints[k]->mPrev;
                (*c)->mPoints.back()->mNext = (*c)->mPoints[k];
                (*c)->mPoints[k]->mPrev->mNext = (*c)->mPoints.back();
                (*c)->mPoints[k]->mPrev = (*c)->mPoints.back();
                /* calculate position of new point a la PRB 2011, Baggaley & Barenghi */
                for(int j(0);j!=3;j++){
                    (*c)->mPoints.back()->mS2Prime[j] = ((*c)->mPoints.back()->mNext->mS2Prime[j] + (*c)->mPoints.back()->mPrev->mS2Prime[j])/2;
                    (*c)->mPoints.back()->mPos[j] = ((*c)->mPoints.back()->mS2Prime[j]) * R * (sqrt(abs(pow(R,2) - 0.25*pow((*c)->mPoints.back()->mNext->mSegLength,2))) - R);
                    (*c)->mPoints.back()->mPos[j] += 0.5*((*c)->mPoints.back()->mNext->mPos[j] + (*c)->mPoints.back()->mPrev->mPos[j]);
                }
                (*c)->CalcMeshLengths(); (*c)->CalcSPrime(); (*c)->CalcS2Prime();
                mLog << ctime(&(time(0))).substr(10,8) << "\t" << mStep << ":\t\tadded point at " << (*c)->mPoints.back()->mPos[0] << ", " << (*c)->mPoints.back()->mPos[1] << ", " << (*c)->mPoints.back()->mPos[2] << endl;
                return false;
            }
        }
    }
    return true;
}

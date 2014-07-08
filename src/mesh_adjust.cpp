/* 	Adds or removes points to keep the resolution within a fixed range
Details given by Baggaley and Barenghi, PRB 83 (2011) */

#include "filament.h"

void Filament::AdjMeshLengths(double dr){
	cout << "Adjusting..." << '\n' << endl;
	for (int k = 0; k < mN; k++){
		cout << "mPos[" << k << "][3] is " << mPos[k][3] << ";	mPos[" << k << "][0] is " << mPos[k][0] << ";	Seg[" << k << "] is " << mSegLengths[k] << endl;
	}
	for (int k = 0; k < mN; k++){
		cout << '\n' << "Beginning " << k << endl;
		//if (mSegLengths[k] < dr / 2){
		//	cout << "Segment " << k << " is too small." << endl;
		//	mPos.erase(mPos.begin() + k);
		//	cout << "Segment " << k << " terminated." << endl;
		//	mN = mN - 1;
		//	cout << "Number of points: " << mN;
		//	mPos.resize(mN); cout << "	mPos.size(): " << mPos.size() << endl;
		//	CalcMeshLengths();
		//	CalcS2Prime();
		//}
		if (mPoints[k]->mSegLength > 1.2*dr){
			cout << "Segment " << k << " is too large." << endl;
			mN++;
			cout << "mN is now " << mN << endl;
			NewPoint = Point();
			int b;
			for (int j = 0; j < 3; j++){
				if (k == mN - 1){ b = 0; }
				else { b = k + 1; }
				mS2PrimeNew[k][j] = (mS2Prime[k][j] + mS2Prime[b][j]) / 2; // *** Fails here somehow ***
				R[j] = 1 / sqrt(pow(mS2PrimeNew[k][j], 2));
				NewPos[j] = (0.5)*(mPos[k][j] + mPos[b][j]) + (sqrt(pow(R[j], 2) - (pow(mSegLengths[k], 2) / 4)) - R[j])*(mS2Prime[k][j] * R[j]);
			}
			if (mS2PrimeNew[k][2] == 0){ NewPos[2] = 0; }
			cout << "New position calculated." << endl;
			for (int l = mN - 1; l > k; l--){
				mPos[l] = mPos[l - 1];
				mS2Prime[l] = mS2Prime[l - 1];
				mPos[l][3] = mPos[l - 1][3] + 1;
			} NewPos[3] = k + 1;
			mPos[k] = NewPos;
			mS2Prime[k] = mS2PrimeNew[k];
			mPos[mN - 1][3] = 0;
			for (int l = 0; l < mN; l++){
				cout << "mPos[" << l << "][3] is now " << mPos[l][3] << ";	mPos[" << l << "][0] is now " << mPos[l][0] << endl;
			}
			cout << "New point is at	(" << NewPos[0] << ", " << NewPos[1] << ", " << NewPos[2] << ")" << endl;
			cout << "Segment " << k << " added. For point l, vector elements are:" << endl;
			for (int l = 0; l < mN; l++){
				cout << l << "	(" << mPos[l][0] << ", " << mPos[l][1] << ", " << mPos[l][2] << "), " << mPos[l][3] << endl;
			}
			cout << "Number of points: " << mN << ";	mPos.size(): " << mPos.size() << endl;
			CalcMeshLengths();
			CalcS2Prime();
		}
		cout << "Finished " << k << endl;

		/*for (int l = 0; l < mN; l++){
			cout << "mPos[" << l << "][3] is now " << mPos[l][3] << ";	mPos[" << l << "][0] is now " << mPos[l][0] << endl;
			}*/
	}
		system("pause");
}

//Alternative method for removing points
//for (int k = 0; k < mN; k++){
	//	cout << "Beginning " << k << endl;
	//	if (k % 9 == 0){ mSegLengths[k] = 1e-10; } // Test points
	//	if (mSegLengths[k] < dr / 2){
	//		cout << "Segment " << k << " is too small." << endl;
	//		for (int l = k + 1; l < mN; l++){
	//			cout << "Pushing back from " << l << endl;
	//			mPos[l - 1] = mPos[l];
	//			cout << "mPos[" << l - 1 << "][3] is now " << mPos[l - 1][3] << endl;
	//		}
	//		mN = mN - 1;
	//		cout << "Number of points: " << mN;
	//		mPos.resize(mN); cout << "	mPos.size(): " << mPos.size() << endl;
	//	}


//if (mSegLengths[k] > dr){
//	cout << "Segment " << k << " is too large." << endl;
//	vector <double> shift(3), R(3);
//	for (int j = 0; j < 3; j++){
//		mS2Prime[k][j] = (mS2Prime[k][j] + mS2Prime[k + 1][j]) / 2;
//		R[j] = 1 / sqrt(pow(mS2Prime[k][j], 2));
//		shift[j] = (0.5)*(mPos[k][j] + mPos[k + 1][j]); +(sqrt(pow(R[j], 2) - (pow(mSegLengths[k], 2) / 4)) - R[j]) *(mS2Prime[k][j] * R[j]);
//	}
//	cout << "New position calculated." << endl;
//	cout << "Position of neighbouring points: " << mPos[k - 1][3] << ", " << mPos[k][3] << ", " << mPos[k + 1][3] << endl;
//	cout << "New point is at	(" << shift[0] << ", " << shift[1] << ", " << shift[2] << ")" << endl;
//	mPos.insert(mPos.begin() + k, shift);
//	mN = mN + 1;
//	cout << "New neighbours at: " << mPos[k - 1][3] << ", " << mPos[k][3] << ", " << mPos[k + 1][3] << endl;
//	mPos.resize(mN);
//	for (int l = k + 1; l < mN; l++){
//		mPos[l] = mPos[l - 1];
//	}
//	cout << "Segment " << k << " added." << endl;
//	for (int l = 0; l < mN; l++){
//		cout << l << "	(" << mPos[l][0] << ", " << mPos[l][1] << ", " << mPos[l][2] << ", " << mPos[l][3] << ")" << endl;
//	}
//	cout << "Number of points: " << mN;
//	mPos.resize(mN); cout << "	mPos.size(): " << mPos.size() << endl;
//}
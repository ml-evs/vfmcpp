/* 	Adds or removes points to keep the resolution within a fixed range
Details given by Baggaley and Barenghi, PRB 83 (2011) */

#include "filament.h"

using namespace std;

void Filament::AdjMeshLengths(double dr){
	cout << "Adjusting..." << '\n' << endl;
	//for (int k = 0; k < mN; k++){
	//	cout << "mPos[" << k << "][3] is " << mPos[k][3] << ";	mPos[" << k << "][0] is " << mPos[k][0] << ";	Seg[" << k << "] is " << mSegLengths[k] << endl;
	//}
	CalcS2Prime();
	for (int k = 0; k < mN; k++){
	//	cout << '\n' << "Beginning " << k << endl;
		if (mSegLengths[k] < dr / 2){
			cout << "Segment " << k << " is too small." << endl;
			int b;
			if (k == mN - 1){ b = 0; }
			else { b = k + 1; }
			mPos.erase(mPos.begin() + k);
			//		cout << "Segment " << k << " terminated." << endl;
			mN = mN - 1;
			//		cout << "Number of points: " << mN;
			mPos.resize(mN);
			//		cout << "	mPos.size(): " << mPos.size() << endl;
			CalcMeshLengths();
			CalcS2Prime();
		}
		else if (mSegLengths[k] > 1.00*dr){
			cout << "Segment " << k << " is too large." << endl;
			mN = mN + 1;
			vector <double> NewPos(4), R(3);
			vector <vector <double> > mS2PrimeNew(mN);
			mPos.resize(mN); mS2Prime.resize(mN); mSegLengths.resize(mN); mS2PrimeNew[k].resize(mN);
			for (int l = mN - 1; l > k; l--){
				mPos[l] = mPos[l - 1];
				mS2Prime[l] = mS2Prime[l - 1];
				mPos[l][3] = mPos[l - 1][3] + 1;
			}
			int b;
			for (int j = 0; j < 3; j++){
				if (k == mN - 1){ b = 0; }
				else { b = k + 1; }
				mS2PrimeNew[k][j] = (mS2Prime[k][j] + mS2Prime[b][j]) / 2;
				R[j] = 1 / sqrt(pow(mS2PrimeNew[k][j], 2));
				NewPos[j] = (0.5)*(mPos[k][j] + mPos[b][j]) + (sqrt(pow(R[j], 2) - (pow(mSegLengths[k], 2) / 4)) - R[j])*(mS2Prime[k][j] * R[j]);
			}
			if (mS2PrimeNew[k][2] == 0){ NewPos[2] = 0; }
	//				cout << "New position calculated." << endl;
			NewPos[3] = k + 1;
			mPos[k] = NewPos;
			mS2Prime[k] = mS2PrimeNew[k];
			mPos[mN - 1][3] = 0;
			for (int l = 0; l < mN; l++){
				//			cout << "mPos[" << l << "][3] is now " << mPos[l][3] << ";	mPos[" << l << "][0] is now " << mPos[l][0] << endl;
			}
			//		cout << "Point " << k << " added at (" << NewPos[0] << ", " << NewPos[1] << ", " << NewPos[2] << ")." << endl;
			//		for (int l = 0; l < mN; l++){
			//			cout << l << "	(" << mPos[l][0] << ", " << mPos[l][1] << ", " << mPos[l][2] << "), " << mPos[l][3] << endl;
			//		}
			//cout << "Number of points: " << mN << ";	mPos.size(): " << mPos.size() << endl;
			CalcMeshLengths();
			CalcS2Prime();
		}
	//	cout << "Finished " << k << endl;
	}
}

// calculate s' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcSPrime(){
	mSPrime.resize(mN);
	vector <double> A, B, C, D, E;
	vector <double> &L = mSegLengths;
	A.resize(mN); B = A; C = A; D = A; E = A;
	// funky for loop to generate correct indices for orderered array of lengths
	// produces (98,99,0,1,2) -> (97,98,99,0,1) for N=100.
	int j, k, l, m;
	for (int i = 0; i<mN; i++){
		j = i; k = i; l = i; m = i;
		if (j - 2 == -1){ j = mN + 1; }
		if (j - 2 == -2){ j = mN; }
		if (k - 1 == -1){ k = mN; }
		if (l + 1 == mN){ l = -1; }
		if (m + 1 == mN){ m = -1; }
		if (m + 2 == mN){ m = -2; }

		A[i] = L[i] * L[l + 1] * L[l + 1] + L[i] * L[l + 1] * L[m + 2];
		A[i] = A[i] / (L[k - 1] * (L[k - 1] + L[i])*(L[k - 1] + L[i] + L[l + 1])*(L[k - 1] + L[i] + L[l + 1] + L[m + 2]));

		B[i] = -L[k - 1] * L[l + 1] * L[l + 1] - L[i] * L[l + 1] * L[l + 1] - L[k - 1] * L[l + 1] * L[m + 2] - L[i] * L[l + 1] * L[m + 2];
		B[i] = B[i] / (L[k - 1] * L[i] * (L[i] + L[l + 1])*(L[i] + L[l + 1] + L[m + 2]));

		D[i] = L[k - 1] * L[i] * L[l + 1] + L[i] * L[i] * L[l + 1] + L[k - 1] * L[i] * L[m + 2] + L[i] * L[i] * L[m + 2];
		D[i] = D[i] / (L[l + 1] * L[m + 2] * (L[i] + L[l + 1])*(L[k - 1] + L[i] + L[l + 1]));

		E[i] = -L[l + 1] * L[i] * L[i] - L[k - 1] * L[i] * L[l + 1];
		E[i] = E[i] / (L[m + 2] * (L[l + 1] + L[m + 2]) * (L[i] + L[l + 1] + L[m + 2])*(L[k - 1] + L[i] + L[l + 1] + L[m + 2]));

		C[i] = -(A[i] + B[i] + D[i] + E[i]);

	}
	for (int p = 0; p<mN; p++){
		mSPrime[p].resize(3);
		j = p; k = p; l = p; m = p;
		if (j - 2 == -1){ j = mN + 1; }
		if (j - 2 == -2){ j = mN; }
		if (k - 1 == -1){ k = mN; }
		if (l + 1 == mN){ l = -1; }
		if (m + 1 == mN){ m = -1; }
		if (m + 2 == mN){ m = -2; }
		for (int q = 0; q<3; q++){
			mSPrime[p][q] = A[p] * mPos[j - 2][q];	mSPrime[p][q] += B[p] * mPos[k - 1][q];
			mSPrime[p][q] += C[p] * mPos[p][q];	mSPrime[p][q] += D[p] * mPos[l + 1][q];
			mSPrime[p][q] += E[p] * mPos[m + 2][q];
		}
	}
}

// calculate s'' using coefficients from Baggaley & Barenghi JLT 166:3-20 (2012)
void Filament::CalcS2Prime(){
	mS2Prime.resize(mN);
	vector <double> A2, B2, C2, D2, E2;
	vector <double> &L = mSegLengths;
	A2.resize(mN, 0); B2 = A2; C2 = A2; D2 = A2; E2 = A2;
	int j, k, l, m;


	for (int i = 0; i<mN; i++){
		j = i; k = i; l = i; m = i;
		if (j - 2 == -1){ j = mN + 1; }
		if (j - 2 == -2){ j = mN; }
		if (k - 1 == -1){ k = mN; }
		if (l + 1 == mN){ l = -1; }
		if (m + 1 == mN){ m = -1; }
		if (m + 2 == mN){ m = -2; }

		A2[i] = 2 * (-2 * L[i] * L[l + 1] + L[l + 1] * L[l + 1] - L[i] * L[m + 2] + L[l + 1] * L[m + 2]);
		A2[i] = A2[i] / (L[k - 1] * (L[k - 1] + L[i])*(L[k - 1] + L[i] + L[l + 1])*(L[k - 1] + L[i] + L[l + 1] + L[m + 2]));

		B2[i] = 2 * (2 * L[k - 1] * L[l + 1] + 2 * L[i] * L[l + 1] - L[l + 1] * L[l + 1] + L[k - 1] * L[m + 2] + L[i] * L[m + 2] - L[l + 1] * L[m + 2]);
		B2[i] = B2[i] / (L[k - 1] * L[i] * (L[i] + L[l + 1])*(L[i] + L[l + 1] + L[m + 2]));

		D2[i] = 2 * (-L[k - 1] * L[i] - L[i] * L[i] + L[k - 1] * L[l + 1] + 2 * L[i] * L[l + 1] + L[k - 1] * L[m + 2] + 2 * L[i] * L[m + 2]);
		D2[i] = D2[i] / (L[l + 1] * L[m + 2] * (L[i] + L[l + 1])*(L[k - 1] + L[i] + L[l + 1]));

		E2[i] = 2 * (L[k - 1] * L[i] + L[i] * L[i] - L[k - 1] * L[l + 1] - 2 * L[i] * L[l + 1]);
		E2[i] = E2[i] / (L[m + 2] * (L[l + 1] + L[m + 2]) * (L[i] + L[l + 1] + L[m + 2]) * (L[k - 1] + L[i] + L[l + 1] + L[m + 2]));

		C2[i] = -(A2[i] + B2[i] + D2[i] + E2[i]);
	}
	for (int p = 0; p<mN; p++){
		mS2Prime[p].resize(3);
		j = p; k = p; l = p; m = p;
		if (j - 2 == -1){ j = mN + 1; }
		if (j - 2 == -2){ j = mN; }
		if (k - 1 == -1){ k = mN; }
		if (l + 1 == mN){ l = -1; }
		if (m + 1 == mN){ m = -1; }
		if (m + 2 == mN){ m = -2; }
		for (int q = 0; q<3; q++){
			mS2Prime[p][q] = A2[p] * mPos[j - 2][q];	mS2Prime[p][q] += B2[p] * mPos[k - 1][q];
			mS2Prime[p][q] += C2[p] * mPos[p][q];		mS2Prime[p][q] += D2[p] * mPos[l + 1][q];
			mS2Prime[p][q] += E2[p] * mPos[m + 2][q];
		}
	}
}
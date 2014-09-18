/* initialise strings via .in files as outlined in documentation */

#include "tangle.h"
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


string Tangle::Initialise(string runfile){
	
  if(runfile == "NULL"){
  	runfile = "run.in";
  }
  ifstream infile(runfile);
	string line;
	stringstream input, convert;
	string put;
	string filename;
	double t_total;
	double res;
	vector <double> param(5);
	cout << "\t - - - - - - -    INITIALISING TANGLE    - - - - - - - -\n\n";
	cout << "\t    init path: " << runfile << endl;
	while(infile){
		getline(infile, line);
		/* ignore comments */
		if(line.substr(0,1) == "#"){
			continue;
		}
		/* set data folder path */
		else if(line.substr(0,4) == "path"){
			input.clear();
			input << line.substr(5);
			input >> filename;
			filename = "../" + filename;
			cout << "\t    data path: " << filename << endl;
			/* initialise log file */
			string logfile = filename + "/events.log";
			mLog.open(logfile.c_str());
			mLog << StringTime() << "\t\t\t\t\tinit path: " << runfile << endl;
			mLog << StringTime() << "\t\t\t\t\tdata path: " << filename << endl;
		}
		/* set simulation length */
		else if(line.substr(0,4) == "time"){
			input.clear();
			input << line.substr(5);
			input >> t_total;
			mTotalTime = t_total;
			cout << "\t    simulation time = " << t_total << " s" << endl;
			mLog << StringTime() << "\t\t\t\t\tsimulation time = " << t_total << " s" << endl;
		}
		/* set an external field */
		else if(line.substr(0,4) == "Eext"){
			put.clear();
			input.clear();
			convert.clear();
			cout << "\t    electric field: ";
			mLog << StringTime() << "\t\t\t\t\telectric field:";
			input << line.substr(5);
			for(int i(0);i<5;i++){
				convert.clear();
				getline(input, put, ' ');
				convert << put;
				convert >> param[i];
				if(input.eof()) break;
			}
			mEFieldAmp = param[0]; mEFieldDuration = param[1];
			mEFieldDirection = int(param[2]);
			cout << " amplitude = " << mEFieldAmp << " V/m, duration = " << mEFieldDuration << " s, in ";
			mLog << " amplitude = " << mEFieldAmp << " V/m, duration = " << mEFieldDuration << " s, in ";
			if(mEFieldDirection==0){cout << "x direction." << endl; mLog << "x direction." << endl;}
			if(mEFieldDirection==1){cout << "y direction." << endl; mLog << "y direction." << endl;}
			if(mEFieldDirection==2){cout << "z direction." << endl; mLog << "z direction." << endl;}
		}
		/* set resolution */
		else if(line.substr(0,4) == "resl"){
			input.clear();
			input << line.substr(5);
			input >> res;
			mDr = float(res); // set simulation resolution in tangle
			mDt = pow((mDr/2),2)/(9.98e-8*log(mDr/(2*PI*1.3e-10)));
			mDt /= 25; 	// Baggaley, Barenghi PRB 2010
			cout << "\t    spatial resolution = "<< mDr << " m" << endl;
			cout << "\t    time-step = " << mDt << " s\n\n";
			mLog << StringTime() << "\t\t\t\t\tspatial resolution = "<< mDr << " m" << endl;
			mLog << StringTime() << "\t\t\t\t\ttime-step = " << mDt << " s\n";
			mDr *= 4.0/3.0; // augments resolution for mesh adjust stability
		}
		/* define a ring */
		else if(line.substr(0,4) == "ring"){
			param[4] = 2;	//default ring alignment is z
			put.clear();
			input.clear();
			convert.clear();
			cout << "\t    ring";
			mLog << StringTime() << "\t\t\t\t\tring";
			input << line.substr(5);
			for(int i(0);i<5;i++){
				convert.clear();
				getline(input, put, ' ');
				convert << put;
				convert >> param[i];
				if(input.eof()) break;
			}
			mTangle.push_back(new Ring(mDr, param[0], param[1], param[2], param[3], param[4]));
			cout << " r = " << param[0] << " m, p = (" << param[1];
			mLog << " r = " << param[0] << " m, p = (" << param[1];
			cout << ", " << param[2] << ", " << param[3] << ") m, aligned in ";
			mLog << ", " << param[2] << ", " << param[3] << ") m, aligned in ";
			if(param[4]==0){cout << "x direction." << endl; mLog << "x direction." << endl;}
			if(param[4]==1){cout << "y direction." << endl; mLog << "y direction." << endl;}
			if(param[4]==2){cout << "z direction." << endl; mLog << "z direction." << endl;}
			for(unsigned int i(0);i<param.size(); i++) param[i] = 0;
		}
		/* define a string */
		else if(line.substr(0,4) == "line"){
			put.clear();
			input.clear();
			convert.clear();
			cout << "\t    string";
			mLog << StringTime() << "\t\t\t\t\tstring";
			input << line.substr(5);
			for(int i(0);i<5;i++){
				convert.clear();
				getline(input, put, ' ');
				convert << put;
				convert >> param[i];
				if(input.eof()) break;
			}
			mTangle.push_back(new String(mDr, param[0], param[1], param[2], param[3], param[4]));
			cout << " L = " << param[0] << " m, p = (" << param[1];
			mLog << " L = " << param[0] << " m, p = (" << param[1];
			cout << ", " << param[2] << ", " << param[3] << ") m. " << endl;
			mLog << ", " << param[2] << ", " << param[3] << ") m. " << endl;
			for(unsigned int i(0);i<param.size(); i++) param[i] = 0;
		}
		/* add a charge */
		else if(line.substr(0,4) == "q_pt"){
			put.clear();
			input.clear();
			convert.clear();
			cout << "\t    charged segment on filament ";
			mLog << StringTime() << "\t\t\t\t\tcharged segment on filament ";
			input << line.substr(5);
			for(int i(0);i<4;i++){
				convert.clear();
				getline(input, put, ' ');
				convert << put;
				convert >> param[i];
				if(input.eof()) break;
			}
			mTangle[param[0]]->mPoints[int(param[1])]->mCharge = param[2]; mTangle[param[0]]->mCarriesCharge = true;
			cout << param[0] << " with size " << param[2] << " C, at mesh point " << param[1] << ". " << endl;
			mLog << param[0] << " with size " << param[2] << " C, at mesh point " << param[1] << ". " << endl;
		}
	}
	infile.close();
	return filename;
}

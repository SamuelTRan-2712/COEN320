#include <pthread.h>
#include <stdio.h>
#include "Plane.h"
#include "Radar.h"
#include "ComputerSystem.h"
#include "Display.h"
#include "ATC.h"
#include <unistd.h>
#include <fstream>
#include <string>

using namespace std;


int main (int argc, char* argv[]) {


	ifstream infile;
	infile.open("./input.txt");

	if(infile.fail()){
		cout << "the file could not be opened";
	}

	else{
		string s;
		while(infile>>s){
			cout<<s<<endl;
		}

	}

//	ATC atc;
//	atc.readInput();

	Display display;
	ComputerSystem compsys;
	Radar radar;

//
//	for(int i=0; i < atc.planes.size(); i++){
//		pthread_join(atc.planes.at(i)->thread_id,NULL);
//	}


	return EXIT_SUCCESS;
}

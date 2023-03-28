#include "ATC.h"
#include <fstream>
#include <iostream>
#include <string>


ATC::ATC(){

}

ATC::~ATC(){

}

int ATC::readInput() {


  int ID = 0;
  int arrivalTime = 0;
  int arrivalPosX = 0;
  int arrivalPosY = 0;
  int arrivalPosZ = 0;
  int arrivalVelX = 0;
  int arrivalVelY = 0;
  int arrivalVelZ = 0;
  std::string separator = " ";


//	std::ofstream outfile("planes.txt");
//
//	if (!outfile.is_open()) {
//	    std::cerr << "Unable to create file input.txt\n";
//	    return -1;
//	  }
//
//
//	  outfile << " 1 1       0     0 12500  500  500 0\n";
//	  outfile << "2 1    4000     0 12500  500  500 0\n";
//	  outfile << "3 1    8000     0 12500  500  500 0\n";
//	  outfile << "4 1   12000     0 12500  500  500 0\n";
//	  outfile << "5 1   16000     0 12500  500  500 0\n";
//
//	  outfile << "6 11      0  4000 12500  500  500 0\n";
//	  outfile << "7 11  94000     0 12500  500  500 0\n";
//	  outfile << "8 11  89000     0 12500  500  500 0\n";
//	  outfile << "9 11  84000     0 12500  500  500 0\n";
//	  outfile << "10 11 79000     0 12500  500  500 0\n";
//
//	  outfile << "11 21     0 99000 12500  500  500 0\n";
//	  outfile << "12 21     0 94000 12500  500  500 0\n";
//	  outfile << "13 21     0 89000 12500  500  500 0\n";
//	  outfile << "14 21     0 84000 12500  500  500 0\n";
//	  outfile << "15 21     0 79000 12500  500  500 0\n";
//
//	  outfile << "16 31 100000 100000  12500 -500 -500 0\n";
//	  outfile << "17 31  96000 100000  12500 -500 -500 0\n";
//	  outfile << "18 31  92000 100000  12500 -500 -500 0\n";
//	  outfile << "19 31  88000 100000  12500 -500 -500 0\n";
//	  outfile << "20 31  84000 100000  12500 -500 -500 0\n";
//	  outfile.close();


	ifstream infile;
	string line;
	infile.open("planes.txt");

	if (!infile) {
	  cerr << "Unable to open file input.txt\n";
	  return -1;
	}

	// Check if the file is empty
	if (infile.peek() == ifstream::traits_type::eof()) {
	  cerr << "The file is empty\n";
	  return -1;
	}

	while (getline (infile,line)) {
		// if (line.empty()) continue;  // skip empty line
		infile >>  ID >> arrivalTime >> arrivalPosX >> arrivalPosY >> arrivalPosZ >> arrivalVelX >> arrivalVelY >> arrivalVelZ;
		//create plane objects and add pointer to each plane to a vector
		Plane *plane = new Plane(ID, arrivalTime, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ);
		planes.push_back(plane);
			if (plane == nullptr) {
					std::cerr << "Failed to create plane object" << std::endl;
					return -1;
				}
		  }
	std::cout << "Number of planes: " << planes.size() << std::endl;
	infile.close();


	for (const auto& plane : planes) {
		std::cout << plane->ID << std::endl;
	}

  return 0;
}

// initialize thread and shm members

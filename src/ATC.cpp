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


  //ID >> arrivalTime >> arrivalPosX >> arrivalPosY >> arrivalPosZ >> arrivalVelX >> arrivalVelY >> arrivalVelZ

	ifstream infile;
	string line;

	std::ofstream outfile("input.txt");

	infile.open("input.txt");

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
		if (line.empty()) continue;  // skip empty line
		cout << line << '\n';
	}
	infile.close();

		// create plane objects and add pointer to each plane to a vector
		//Plane *plane = new Plane(ID, arrivalTime, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ);
		//planes.push_back(plane);
//		    if (plane == nullptr) {
//		            std::cerr << "Failed to create plane object" << std::endl;
//		            return -1;
//		        }
//		  }
//		std::cout << "Number of planes: " << planes.size() << std::endl;





		for (const auto& plane : planes) {
		    std::cout << plane->ID << std::endl;
		}

  return 0;
}

// initialize thread and shm members

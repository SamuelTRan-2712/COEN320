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


	std::ofstream outfile("planerss.txt");

	if (!outfile.is_open()) {
	    std::cerr << "Unable to create file input.txt\n";
	    return -1;
	  }


	  outfile << "1 1   98000     0 17500  500  500 0\n";
	  outfile << "2 1    4000     0 17500  500  500 0\n";
	  outfile << "3 1    8000     0 17500  500  500 1000\n";
	  outfile << "4 1   12000     0 17500  500  500 0\n";
	  outfile << "5 1   16000     0 17500  500  500 0\n";
	  outfile.close();


	ifstream infile;
	string line;
	infile.open("planerss.txt");

	if (!infile) {
	  cerr << "Unable to open file input.txt\n";
	  return -1;
	}

	// Check if the file is empty
	if (infile.peek() == ifstream::traits_type::eof()) {
	  cerr << "The file is empty\n";
	  return -1;
	}

	// Read input from file
	while (infile >>  ID >> arrivalTime >> arrivalPosX >> arrivalPosY >> arrivalPosZ >> arrivalVelX >> arrivalVelY >> arrivalVelZ) {
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
		std::cout << "Plane ID: " << plane->ID << std::endl;
	}

  return 0;
}


#include "ATC.h"

ATC::ATC(){

}

ATC::~ATC(){

}

int ATC::readInput() {
  // open input.txt
  std::string filename = "../input.txt";
  std::ifstream input_file_stream;

  input_file_stream.open(filename);

//  if (!input_file_stream) {
//    std::cout << "Can't find file input.txt" << std::endl;
//    return 1;
//  }


  int ID = 0;
  int arrivalTime = 0;
  int arrivalPosX = 0;
  int arrivalPosY = 0;
  int arrivalPosZ = 0;
  int arrivalVelX = 0;
  int arrivalVelY = 0;
  int arrivalVelZ = 0;

  std::string separator = " ";

  // parse input.txt to create plane objects
  while (input_file_stream >> ID >> arrivalTime >> arrivalPosX >> arrivalPosY >> arrivalPosZ >> arrivalVelX >> arrivalVelY >> arrivalVelZ) {

    // create plane objects and add pointer to each plane to a vector
    Plane *plane = new Plane(ID, arrivalTime, arrivalPosX, arrivalPosY, arrivalPosZ, arrivalVelX, arrivalVelY, arrivalVelZ);
    planes.push_back(plane);
  }

  return 0;
}

// initialize thread and shm members

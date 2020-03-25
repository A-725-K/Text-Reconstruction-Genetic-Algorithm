#include "utils.h"

/*
 * Read dataset directly from file
 *
 * @input [filename]: path of the dataset
 *
 * @output [content]: the dataset in form of a std::string
 */
std::string readFile(char *fileName) {
  std::string content;
  std::ifstream file(fileName);
  
  if (file.is_open()) {
    file.seekg(0, std::ios::end);
    unsigned int contentLength = file.tellg();
    content.resize(contentLength - 1);
    file.seekg(0, std::ios::beg);
    file.read(&content[0], content.size());
    file.close();

    return content;
  }

  throw FILE_NOT_FOUND_EXCEPTION;
}

/*
 * Checks that all command line arguments are correct
 *
 * @input [argc]: number of cli arguments
 * @input [argv]: cli arguments
 * @input [popmax]: size of the initial population
 * @input [mutationRate]: probability of mutation (in percentage)
 * @input [target]: text to be recreated
 */
void checkCliArguments(int argc, char *argv[], unsigned int &popmax, unsigned int &mutationRate, std::string &target) {
  if(argc != 4)
    throw USAGE_EXCEPTION;
   
  mutationRate = std::stoi(argv[1]);
  if (mutationRate <= 0 || mutationRate > 100) 
    throw std::invalid_argument("Mutation rate must be in range [1,100]%");
  
  popmax = std::stoi(argv[2]);
  if(popmax < 2)
    throw std::invalid_argument("Population must be at least 2");
  
  target = readFile(argv[3]);
}

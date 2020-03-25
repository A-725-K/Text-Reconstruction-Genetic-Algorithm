#include "utils.h"

struct DNA {
  std::string genes;
  double fitness;
  std::vector<bool> mask; // boolean mask to guarantee total convergence of the algorithm
};

DNA createDNA(const std::string, const unsigned long);
void calcFitnessDNA(DNA&, const std::string);
void mutate(DNA&, const unsigned int);
DNA crossover(const DNA, const DNA, const std::string);

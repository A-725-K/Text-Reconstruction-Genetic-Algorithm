#include "DNA.h"

#define THRESHOLD 0.0001
#define MAX_FITNESS 1.0
#define MATING_RATE 100

void calcFitnessPop(std::vector<DNA> &, const std::string);
void createPopulation(std::vector<DNA>&, const std::string, const int);
void createGeneration(std::vector<DNA>&, std::vector<DNA>&, unsigned int, const std::string);
void naturalSelectionPop(std::vector<DNA>&, std::vector<DNA>&);
double getAverageFitness(const std::vector<DNA>);
bool evaluate(std::vector<DNA>, const std::string);
void printFitness(const std::vector<DNA>);
  
static long long generations = 0;

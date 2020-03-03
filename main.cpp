#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>
#include <algorithm>

#define THRESHOLD 0.0001
#define MAX_FITNESS 1.0
#define MATING_RATE 100

//const std::string target = "unicorn";
//const std::string target = "to be or not to be";
const std::string target = "nel mezzo del cammin di nostra vita mi ritrovai per una selva oscura che la diritta via era smarrita ahi quanto a dir qual era e cosa dura esta selva selvaggia e aspra e forte che nel pensier rinova la paura";

const int popmax = 500;
const unsigned int mutationRate = 2; //[1, 100] in %
long long generations = 0;

struct DNA {
  std::string genes;
  double fitness;
  std::vector<bool> mask;
};

char genRandomChar() {
  int r = rand() % 27;
  if (r == 26)
    return ' ';
  return 'a' + r;
}

DNA createDNA(const unsigned long targetLength) {
  DNA newDNA;
  newDNA.fitness = 0.0;
  newDNA.genes = std::string(targetLength, '\0');
  for (unsigned int i=0; i<targetLength; i++) {
    newDNA.genes[i] = genRandomChar();
    newDNA.mask.push_back(newDNA.genes[targetLength - i - 1] == target[i] ? true : false);
  }
  return newDNA;
}

void calcFitnessDNA(DNA &dna, const std::string target) {
  double score = 0;
  unsigned int genesLength = dna.genes.size();
  for (unsigned int i=0; i<genesLength; i++)
    if (dna.mask[i])
      score++;
  dna.fitness = pow(score / genesLength, 4);
}

void calcFitnessPop(std::vector<DNA> &population, const std::string target) {
  for (unsigned int i=0; i<population.size(); i++)
    calcFitnessDNA(population[i], target);
}

void createPopulation(std::vector<DNA> &population, const std::string target, const int popmax) {
  for(int i=0; i<popmax; i++)
    population.push_back(createDNA(target.size()));
  calcFitnessPop(population, target);
}

DNA crossover(const DNA partnerA, const DNA partnerB) {
  unsigned int genesLength = partnerA.genes.size();
  unsigned int midpoint = rand() % genesLength;
  DNA child = createDNA(genesLength);

  for(unsigned int i=0; i<genesLength; i++){
    if(i > midpoint)
      child.genes[i] = partnerA.genes[i];
    else
      child.genes[i] = partnerB.genes[i];

    child.mask[i] = child.genes[i] == target[i] ? true : false;
  }
  return child;
}

void mutate(DNA &child, const unsigned int mutationRate) {
  for(unsigned int i=0; i<child.genes.size(); i++)
    if(!child.mask[i] && static_cast<unsigned int>(rand() % 101) <= mutationRate)
      child.genes[i] = genRandomChar();
}

void createGeneration(std::vector<DNA> &population, std::vector<DNA> &matingPool) {
  for(unsigned int i=0; i<population.size(); i++){
    unsigned int a = rand() % matingPool.size();
    unsigned int b = rand() % matingPool.size();
    DNA partnerA = matingPool[a];
    DNA partnerB = matingPool[b];
    DNA child = crossover(partnerA, partnerB);
    mutate(child, mutationRate);
    population[i] = child;
  }
  generations++;
  matingPool.erase(matingPool.begin(), matingPool.end());
}

void naturalSelectionPop(std::vector<DNA> &population, std::vector<DNA> &matingPool) {
  std::vector<DNA>::iterator best =
    std::max_element(population.begin(),
		     population.end(),
		     [](DNA x, DNA y) {return x.fitness < y.fitness;});

  double maxFitness = (*best).fitness;
  for(unsigned int i=0; i<population.size(); i++) {
    double fitness = population[i].fitness/maxFitness;
    int n = fitness * MATING_RATE;
    for(int j=0; j<n; j++)
      matingPool.push_back(population[i]);
  }
}

double getAverageFitness(const std::vector<DNA> population) {
  double avg = 0;
  for (auto dna : population)
    avg += dna.fitness;
  return avg / population.size();
}

bool evaluate(std::vector<DNA> population, const std::string target) {
  std::vector<DNA>::iterator best =
    std::max_element(population.begin(),
		     population.end(),
		     [](DNA x, DNA y) {return x.fitness < y.fitness;});
  //auto bestIdx = std::distance(population.begin(), best);
  if (MAX_FITNESS - (*best).fitness < THRESHOLD) {
    std::cout << std::endl << "*** RESULT ***" << std::endl << (*best).genes << std::endl;
    return true;
  }
  
  std::cout << "BEST GENE: " << (*best).genes
	    << "\t\tFITNESS: " << (*best).fitness << std::endl;
  return false;
}


void printFitness(std::vector<DNA> pop) {
  int i = 0;
  for (auto x : pop)
    std::cout << i++ << " --> " << x.fitness << std::endl;
}


int main(int argc, const char **argv) {
  srand(time(nullptr));
    
  std::vector<DNA> population;
  std::vector<DNA> matingPool;
    
  std::cout << "/*******************\\" << std::endl
	    << "|* HYPERPARAMETERS *|"  << std::endl
	    << "\\*******************/" << std::endl;
  std::cout << "Target: " << target << std::endl;
  std::cout << "Target size: " << target.size() << std::endl;
  std::cout << "Population size: " << popmax << std::endl;
  std::cout << "Mutation rate: " << mutationRate << std::endl << std::endl;
    
  std::cout << "Creating population..." << std::endl;
  createPopulation(population, target, popmax);
  std::cout << std::endl;
  //printFitness(population);
    
  std::cout << "### MAIN LOOP ###" << std::endl;
  do {
    //std::cout << "Selection..." << std::endl;
    naturalSelectionPop(population, matingPool);
    //std::cout << "Generation..." << std::endl;
    createGeneration(population, matingPool);
    //std::cout << "Computing fitness..." << std::endl;
    calcFitnessPop(population, target); 
  } while(!evaluate(population, target));

  std::cout << "Average fitness: " << getAverageFitness(population) << std::endl;
  std::cout << "Generations: " << generations << std::endl;
  return 0;
}

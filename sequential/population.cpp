#include "population.h"

long long generations = 0;

void calcFitnessPop(std::vector<DNA> &population, const std::string target) {
  auto start = std::chrono::system_clock::now();
    
  for (unsigned int i=0; i<population.size(); i++)
    calcFitnessDNA(population[i], target);
    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Fitness elapsed time: " << elapsed.count() << " s" << std::endl;
}

void createPopulation(std::vector<DNA> &population, const std::string target, const int popmax) {
  auto start = std::chrono::system_clock::now();
    
  for(int i=0; i<popmax; i++)
    population.push_back(createDNA(target, target.size()));
  calcFitnessPop(population, target);
    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Create population elapsed time: " << elapsed.count() << " s" << std::endl;
}

void createGeneration(std::vector<DNA> &population, std::vector<DNA> &matingPool, unsigned int mutationRate, const std::string target) {
  auto start = std::chrono::system_clock::now();
    
  for(unsigned int i=0; i<population.size(); i++){
    unsigned int a = rand() % matingPool.size();
    unsigned int b = rand() % matingPool.size();
    DNA partnerA = matingPool[a];
    DNA partnerB = matingPool[b];
    DNA child = crossover(partnerA, partnerB, target);
    mutate(child, mutationRate);
    population[i] = child;
  }
  generations++;
  matingPool.erase(matingPool.begin(), matingPool.end());
    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Create generation elapsed time: " << elapsed.count() << " s" << std::endl;
}

void naturalSelectionPop(std::vector<DNA> &population, std::vector<DNA> &matingPool) {
  auto start = std::chrono::system_clock::now();
    
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
    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Natural selection elapsed time: " << elapsed.count() << " s" << std::endl;
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
    
  if (MAX_FITNESS - (*best).fitness < THRESHOLD) {
    std::cout << std::endl << "*** RESULT ***" << std::endl << (*best).genes << std::endl;
    return true;
  }

#ifdef TRACE_ON
  std::cout << "BEST GENE: " << (*best).genes
	    << "\t\tFITNESS: " << (*best).fitness << std::endl;
#endif
  return false;
}

void printFitness(const std::vector<DNA> pop) {
  int i = 0;
  for (auto x : pop)
    std::cout << i++ << " --> " << x.fitness << std::endl;
}

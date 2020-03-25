#include "population.h"

//number of generations needed to accomplish the task
long long generations = 0;

/*
 * Compute the fitness function on each individual of the population
 *
 * @input [population]: current population
 * @input [target]: text to be recreated
 */
void calcFitnessPop(std::vector<DNA> &population, const std::string target) {
#ifdef DEBUG
  auto start = std::chrono::system_clock::now();
#endif  
  
  for (unsigned int i=0; i<population.size(); i++)
    calcFitnessDNA(population[i], target);

#ifdef DEBUG    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Fitness elapsed time: " << elapsed.count() << " s" << std::endl;
#endif
}

/*
 * Create a new population starting from some hyperparameters
 *
 * @input [population]: new population
 * @input [target]: text to be recreated
 * @input [popmax]: size of the population
 */
void createPopulation(std::vector<DNA> &population, const std::string target, const int popmax) {
#ifdef DEBUG
  auto start = std::chrono::system_clock::now();
#endif    

  for(int i=0; i<popmax; i++)
    population.push_back(createDNA(target, target.size()));
  calcFitnessPop(population, target);

#ifdef DEBUG    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Create population elapsed time: " << elapsed.count() << " s" << std::endl;
#endif
}

/*
 * Create a new generation, starting from the fittest individuals
 *
 * @input [population]: new generation of individuals
 * @input [matingPool]: best individuals chosen from the previous generation
 * @input [mutationRate]: probability of mutation (in percentage)
 * @input [target]: text to be recreated
 */
void createGeneration(std::vector<DNA> &population, std::vector<DNA> &matingPool, unsigned int mutationRate, const std::string target) {
#ifdef DEBUG
  auto start = std::chrono::system_clock::now();
#endif    

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

#ifdef DEBUG    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Create generation elapsed time: " << elapsed.count() << " s" << std::endl;
#endif
}

/*
 * Select the fittest individuals from the current generation
 *
 * @input [population]: current generation
 * @input [matingPool]: array of best individuals populated with a probability proportional 
 *                      to their fitness
 */
void naturalSelectionPop(std::vector<DNA> &population, std::vector<DNA> &matingPool) {
#ifdef DEBUG
  auto start = std::chrono::system_clock::now();
#endif 

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

#ifdef DEBUG    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Natural selection elapsed time: " << elapsed.count() << " s" << std::endl;
#endif
}

/*
 * Get a mean value for the fitness in the current generation
 *
 * @input [population]: current population
 *
 * @output [avg]: average fitness of the population
 */
double getAverageFitness(const std::vector<DNA> population) {
  double avg = 0;
  for (auto dna : population)
    avg += dna.fitness;
  return avg / population.size();
}

/*
 * Check if the fitness of the best DNA sequence is under an acceptable threshold
 * 
 * @input [population]: current generation of the population
 * @input [target]: text to be reconstructed
 *
 * @output [end]: boolean value which determines the termination of the algorithm
 */
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

/*
 * Print the fitness of each individual in the population
 *
 * @input [population]: current generation of the population
 */
void printFitness(const std::vector<DNA> pop) {
  int i = 0;
  for (auto x : pop)
    std::cout << i++ << " --> " << x.fitness << std::endl;
}

#include "src/population.h"

int main(int argc, char *argv[]) {
  srand(time(nullptr));

  std::string target;
  std::vector<DNA> population;
  std::vector<DNA> matingPool;
  unsigned int mutationRate;
  unsigned int popmax;

  //check the input of the user
  try {
    checkCliArguments(argc, argv, popmax, mutationRate, target);
  } catch (MY_EXCEPTIONS genericException) {
    switch(genericException) {
      case FILE_NOT_FOUND_EXCEPTION:
	std::cerr << "ERROR: File not found!" << std::endl;
	break;
      case USAGE_EXCEPTION:
	std::cerr << "Usage: ./genetic.out <mutation_rate> <population_size> <input_file>" << std::endl;
	break;
    }
    
    exit(EXIT_FAILURE);
  } catch (std::invalid_argument& ex) {
    std::cerr << "ERROR: " << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }

  //print some useful information
  std::cout << "/*******************\\" << std::endl
	    << "|* HYPERPARAMETERS *|"  << std::endl
	    << "\\*******************/" << std::endl;
  std::cout << "Target: " << target << std::endl;
  std::cout << "Target size: " << target.size() << std::endl;
  std::cout << "Population size: " << popmax << std::endl;
  std::cout << "Mutation rate: " << mutationRate << std::endl << std::endl;

  auto start = std::chrono::system_clock::now();

#ifdef DEBUG
  std::cout << "Creating population..." << std::endl;
#endif

  //initialization of the algorithm:
  // - phase (0): creating the population
  createPopulation(population, target, popmax);
  std::cout << std::endl;

#ifdef DEBUG
  printFitness(population);
  std::cout << "### MAIN LOOP ###" << std::endl;
#endif

  //core of the algorithm:
  // - phase (1): natural selection
  // - phase (2): creating a new generation
  // - phase (3): compute fitness of the new generation
  // - phase (4): evaluate the generation and go to (1)
  do {
#ifdef DEBUG
    std::cout << "Selection..." << std::endl;
#endif
    naturalSelectionPop(population, matingPool);
#ifdef DEBUG
    std::cout << "Generation..." << std::endl;
#endif
    createGeneration(population, matingPool, mutationRate, target);
#ifdef DEBUG
    std::cout << "Computing fitness..." << std::endl;
#endif
    calcFitnessPop(population, target); 
  } while(!evaluate(population, target));

  //print the results
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;
  std::cout << "Average fitness: " << getAverageFitness(population) << std::endl;
  std::cout << "Generations: " << generations << std::endl;
  
  exit(EXIT_SUCCESS);
}

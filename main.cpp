#include <ctime>
#include <cmath>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <exception>

#define THRESHOLD 0.0001
#define MAX_FITNESS 1.0
#define MATING_RATE 100

enum MY_EXCEPTIONS {FILE_NOT_FOUND_EXCEPTION, USAGE_EXCEPTION};

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

DNA createDNA(const std::string target, const unsigned long targetLength) {
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

DNA crossover(const DNA partnerA, const DNA partnerB, const std::string target) {
  unsigned int genesLength = partnerA.genes.size();
  unsigned int midpoint = rand() % genesLength;
  DNA child = createDNA(target, genesLength);

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

void printFitness(std::vector<DNA> pop) {
  int i = 0;
  for (auto x : pop)
    std::cout << i++ << " --> " << x.fitness << std::endl;
}

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

int main(int argc, char *argv[]) {
  srand(time(nullptr));

  std::string target;
  std::vector<DNA> population;
  std::vector<DNA> matingPool;
  unsigned int mutationRate;
  unsigned int popmax;
  
  try {
    checkCliArguments(argc, argv, popmax, mutationRate, target);
  } catch (MY_EXCEPTIONS genericException) {
    switch(genericException) {
      case FILE_NOT_FOUND_EXCEPTION:
	std::cerr << "ERROR: File not found!" << std::endl;
	break;
      case USAGE_EXCEPTION:
	std::cerr << "Usage: ./a.out <mutation_rate> <population_size> <input_file>" << std::endl;
	break;
    }
    
    exit(EXIT_FAILURE);
  } catch (std::invalid_argument& ex) {
    std::cerr << "ERROR: " << ex.what() << std::endl;
    exit(EXIT_FAILURE);
  }
    
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
  createPopulation(population, target, popmax);
  std::cout << std::endl;

#ifdef DEBUG
  printFitness(population);
  std::cout << "### MAIN LOOP ###" << std::endl;
#endif
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
    
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed = end - start;
  std::cout << "Elapsed time: " << elapsed.count() << " s" << std::endl;
  std::cout << "Average fitness: " << getAverageFitness(population) << std::endl;
  std::cout << "Generations: " << generations << std::endl;
  
  exit(EXIT_SUCCESS);
}

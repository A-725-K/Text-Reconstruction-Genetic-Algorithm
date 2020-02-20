#include <iostream>
#include <ctime>
#include <vector>

std::string target;
const int popmax = 200;
const float mutationRate = 0.01;
long generations = 0;

struct DNA{
    char *genes;
    unsigned long genesLength;
    float fitness;
};

char genRandomChar() {
    return 'a'+(rand()%26);
}

std::vector<DNA> population;

DNA createDNA(unsigned long targetLength) {
    DNA newDNA;
    
    newDNA.fitness = 0;
    newDNA.genes = new char[targetLength];
    newDNA.genesLength = targetLength;
    for (int i=0; i<targetLength; i++)
        newDNA.genes[i] = genRandomChar();
    
    return newDNA;
}

void calcFitnessDNA(DNA &dna, std::string target) {
    float score = 0;
    for (int i=0; i<dna.genesLength; i++)
        if(target[i] == dna.genes[i])
            score++;
    dna.fitness = score / dna.genesLength;
}

void calcFitnessPop(std::vector<DNA> population, std::string target) {
    for(auto dna:population)
        calcFitnessDNA(dna, target);
}

void createPopulation(std::string target, float mutationRate, int popmax) {
    for(int i=0; i<popmax; i++)
        population[i] = createDNA(target.size());
    calcFitnessPop(population, target);
}

int main(int argc, const char **argv) {
    srand(time(nullptr));
    target = "To be or not to be";
    
    return 0;
}



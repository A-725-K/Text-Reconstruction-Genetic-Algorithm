#include <iostream>
#include <ctime>
#include <vector>
#include <cmath>

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
std::vector<DNA> matingPool;

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

DNA crossover(DNA partnerA, DNA partnerB){
    DNA child = createDNA(partnerA.genesLength);
    int midpoint = floor(rand() % partnerA.genesLength);
    for(int i=0; i<partnerA.genesLength; i++){
        if(i>midpoint)
            child.genes[i] = child.genes[i];
        else
            child.genes[i] = partnerB.genes[i];
    }
    return child;
}

void mutate(DNA child, float mutationRate){
    for(int i=0; i<child.genesLength; i++){
        if(rand()%2<mutationRate)
            child.genes[i] = genRandomChar();
    }
}

void createGeneration(std::vector<DNA> population, std::vector<DNA> matingPool){
    for(int i=0; i<population.size(); i++){
        int a = floor(rand() % matingPool.size());
        int b = floor(rand() % matingPool.size());
        DNA partnerA = matingPool[a];
        DNA partnerB = matingPool[b];
        DNA child = crossover(partnerA, partnerB);
        mutate(child, mutationRate);
        population[i] = child;
    }
    generations++;
}

void naturalSelectionPop(std::vector<DNA> population){
    float maxFitness = 0;
    for(int i=0; i<population.size(); i++){
        if(population[i].fitness > maxFitness)
            maxFitness = population[i].fitness;
    }
    
    for(int i=0; i<population.size(); i++){
        float fitness = population[i].fitness/maxFitness;
        int n = floor(fitness * 100);
        for(int j=0; j<n; j++){
            matingPool.push_back(population[i]);
        }
        
    }
}

void createPopulation(std::string target, float mutationRate, int popmax) {
    for(int i=0; i<popmax; i++)
        population[i] = createDNA(target.size());
    calcFitnessPop(population, target);
}

int main(int argc, const char **argv) {
    srand(time(nullptr));
    target = "To be or not to be";
    createPopulation(target, mutationRate, popmax);
    
    return 0;
}



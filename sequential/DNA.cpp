#include "DNA.h"

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

void mutate(DNA &child, const unsigned int mutationRate) {
  for(unsigned int i=0; i<child.genes.size(); i++)
    if(!child.mask[i] && static_cast<unsigned int>(rand() % 101) <= mutationRate)
      child.genes[i] = genRandomChar();
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


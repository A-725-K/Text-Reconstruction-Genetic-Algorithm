#include "DNA.h"

/*
 * Generate a new character randomly
 *
 * @output [c]: char
 */
char genRandomChar() {
  int r = rand() % 27;
  if (r == 26)
    return ' ';
  return 'a' + r;
}

/*
 * Create a new DNA sequence that represent a random string
 *
 * @input [target]: text to be recreated
 * @input [targetLength]: length of the target
 *
 * @output [newDNA]: new DNA sequence
 */
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

/*
 * Compute the fitness of a single DNA cell and store it inside the cell
 *
 * @input [dna]: subject to evaluate
 * @input [target]: text to be recreated
 */
void calcFitnessDNA(DNA &dna, const std::string target) {
  double score = 0;
  unsigned int genesLength = dna.genes.size();
  for (unsigned int i=0; i<genesLength; i++)
    if (dna.mask[i])
      score++;
  dna.fitness = pow(score / genesLength, 4);
}

/*
 * Sometimes, in nature, mutation occur; modify a char randomly with a certain probability
 *
 * @input [child]: DNA sequence of which operate the mutation
 * @input [mutationRate]: probability of mutation (in percentage)
 */
void mutate(DNA &child, const unsigned int mutationRate) {
  for(unsigned int i=0; i<child.genes.size(); i++)
    if(!child.mask[i] && static_cast<unsigned int>(rand() % 101) <= mutationRate)
      child.genes[i] = genRandomChar();
}

/*
 * Create a new child sequence starting from 2 parents
 *
 * @input [partnerA]: first parent
 * @input [partnerB]: second parent
 * @input [target]: text to be recreated
 *
 * @output [child]: new sequence of DNA generatd starting from parents
 */
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


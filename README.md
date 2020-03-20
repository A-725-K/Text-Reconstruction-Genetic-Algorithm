# HPC-Text-Reconstruction-GA
Reconstruction of a text using genetic algorithm technique. This little project is inspired by the very famous ***Infinite Monkey Theorem*** that states that a monkey hitting keys at random on a typewriter keyboard for an infinite amount of time will almost surely type any given text, such as the complete works of William Shakespeare. We can ensure the termination of the program due to the insertion of the *DNA mask* which control the convergence (making it total) of the genetic algorithm.

## Getting started
To run the project is sufficient to clone or download this repository, with the command:
```
git clone https://github.com/A-725-K/Text-Reconstruction-Genetic-Algorithm.git
```
## How to launch the program
In order to compile the project you have simply to run this command from your terminal:
```
make
```
In case you want more information during the execution you can compile with the following commands:
```
make debug
```
or
```
make trace
```
Then you can launch the program with the following statement:
```
./genetic.out mutation_rate population_size input_file
```
where:
<ul>
  <li><i>mutation_rate</i>: a positive integer number between 1 and 100, percentage of mutation</li>
  <li><i>population_size</i>: a positive integer number that represent the dimension of the individuals in the population</li>
  <li><i>input_file</i>: input dataset, a sentence that must be composed only by lowercase letters, there is also an utility, written in Python3, to refactor your text</li>
</ul>


## Authors

* **<i>Andrea Canepa</i>** - Computer Science, UNIGE*
* **<i>Francesco Pellaco</i>** - Computer Science, UNIGE*

#include "cpp_utils/includes.hh"
#include "cpp_utils/read_data.hh"
#include "cpp_utils/read_params.hh"
#include "cpp_utils/write_output.hh"

using namespace std;

// Number of individuals in each generation
#define POPULATION_SIZE 100

// Size of the intersection between two sets
int intersection_size(const set<int> &A, const set<int> &B) {
  int size = 0;
  for (int a : A) {
    if (B.find(a) != B.end()) size++;
  }
  return size;
}

// Function to generate random numbers in given range
int random_num(int start, int end) {
  int range = (end - start) + 1;
  int random_int = start + (rand() % range);
  return random_int;
}

// Create random genes for mutation
char mutated_genes() { return rand() % 2; }

// create chromosome or string of genes
vector<int> create_gnome() {
  int len = mapping.size();
  vector<int> gnome;
  for (int i = 0; i < len; i++) gnome.push_back(mutated_genes());
  return gnome;
}

// Class representing individual in population
class Individual {
 public:
  vector<int> chromosome;
  set<int> chromosome_set;
  int fitness;
  Individual(vector<int> chromosome);
  Individual mate(Individual parent2);
  int cal_fitness();
};

Individual::Individual(vector<int> chromosome) {
  set<int> chromosome_set;
  for (int i : chromosome) {
    if (i == 1) {
      chromosome_set.insert(i);
    }
  }
  this->chromosome = chromosome;
  this->chromosome_set = chromosome_set;
  fitness = cal_fitness();
};

// Perform mating and produce new offspring
Individual Individual::mate(Individual par2) {
  // chromosome for offspring
  vector<int> child_chromosome;

  int len = chromosome.size();
  for (int i = 0; i < len; i++) {
    // random probability
    float p = random_num(0, 100) / 100;

    // if prob is less than 0.45, insert gene
    // from parent 1
    if (p < 0.45) child_chromosome.push_back(chromosome[i]);

    // if prob is between 0.45 and 0.90, insert
    // gene from parent 2
    else if (p < 0.90)
      child_chromosome.push_back(par2.chromosome[i]);

    // otherwise insert random gene(mutate),
    // for maintaining diversity
    else
      child_chromosome.push_back(mutated_genes());
  }

  // create new Individual(offspring) using
  // generated chromosome for offspring
  return Individual(child_chromosome);
};

// Calculate fitness score, it is the number of
// characters in string which differ from target
// string.
int Individual::cal_fitness() {
  int len = mapping.size();
  int fitness = 0;
  for (Person &person : people) {
    if (intersection_size(chromosome_set, person.likes) ==
            person.likes.size() &&
        intersection_size(chromosome_set, person.dislikes) == 0)
      fitness++;
  }
  return fitness;
};

// Overloading < operator
bool operator<(const Individual &ind1, const Individual &ind2) {
  return ind1.fitness < ind2.fitness;
}

// Driver code
int main() {
  srand((unsigned)(time(0)));

  // current generation
  int generation = 0;

  vector<Individual> population;
  bool found = false;

  // create initial population
  for (int i = 0; i < POPULATION_SIZE; i++) {
    vector<int> gnome = create_gnome();
    population.push_back(Individual(gnome));
  }

  while (!found) {
    // sort the population in increasing order of fitness score
    sort(population.begin(), population.end());

    // Stopping criteria
    if (generation == 100) break;

    // Otherwise generate new offsprings for new generation
    vector<Individual> new_generation;

    // Perform Elitism, that mean 10% of fittest population
    // goes to the next generation
    int s = (10 * POPULATION_SIZE) / 100;
    for (int i = 0; i < s; i++) new_generation.push_back(population[i]);

    // From 50% of fittest population, Individuals
    // will mate to produce offspring
    s = (90 * POPULATION_SIZE) / 100;
    for (int i = 0; i < s; i++) {
      int len = population.size();
      int r = random_num(0, 50);
      Individual parent1 = population[r];
      r = random_num(0, 50);
      Individual parent2 = population[r];
      Individual offspring = parent1.mate(parent2);
      new_generation.push_back(offspring);
    }
    population = new_generation;
    cout << "Generation: " << generation << "\t";
    // cout << "String: " << population[0].chromosome << "\t";
    cout << "Fitness: " << population[0].fitness << "\n";
    cout << "Abraçades";

    generation++;
  }
  cout << "Generation: " << generation << "\t";
  // cout << "String: " << population[0].chromosome << "\t";
  cout << "Fitness: " << population[0].fitness << "\n";

  write_output(population[0].chromosome);
}
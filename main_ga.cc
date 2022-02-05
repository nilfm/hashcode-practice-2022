#include "cpp_utils/includes.hh"
#include "cpp_utils/read_data.hh"
#include "cpp_utils/read_params.hh"
#include "cpp_utils/write_output.hh"

using namespace std;

// Size of the intersection between two sets
int intersection_size(const set<int> &A, const set<int> &B) {
  int size = 0;
  for (int a : A) {
    if (B.find(a) != B.end()) size++;
  }
  return size;
}

// Heuristic solution
vector<int> heuristic_solution(map<string, double> &p) {
  const int INF = 1e9;
  int P = people.size();
  vector<int> chosen(N);

  vector<int> ingr_scores(N);
  for (Person person : people) {
    for (int like : person.likes) {
      ingr_scores[like]++;
    }
    for (int dislike : person.dislikes) {
      ingr_scores[dislike]--;
    }
  }

  vector<int> scores(P);
  for (int i = 0; i < P; i++) {
    Person person = people[i];
    int a = person.likes.size();
    int b = person.dislikes.size();
    for (int ingr : person.likes) {
      scores[i] += ingr_scores[ingr];
    }
    for (int ingr : person.dislikes) {
      scores[i] -= ingr_scores[ingr];
    }
    if (a == 0 and b == 0) {
      scores[i] = INF;
    } else {
      scores[i] /= (a + b);
    }
  }

  vector<int> forbidden(N, 0);

  vector<int> people_idx(P);
  for (int i = 0; i < P; i++) {
    people_idx[i] = i;
  }

  sort(people_idx.begin(), people_idx.end(),
       [&](int a, int b) -> bool { return scores[a] > scores[b]; });

  for (int i = 0; i < P; i++) {
    if (scores[people_idx[i]] == INF or rand() % 100 < p["threshold"] * 100) {
      Person person = people[people_idx[i]];
      bool can_do = true;
      for (int ingr : person.dislikes) {
        if (chosen[ingr]) {
          can_do = false;
          break;
        }
      }
      for (int ingr : person.likes) {
        if (forbidden[ingr]) {
          can_do = false;
          break;
        }
      }
      if (can_do) {
        for (int ingr : person.likes) {
          chosen[ingr] = 1;
        }
        for (int ingr : person.dislikes) {
          forbidden[ingr] = 1;
        }
      }
    }
  }
  return chosen;
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
vector<int> create_gnome(vector<int> heu_solution) {
  int len = mapping.size();
  vector<int> gnome;
  for (int i = 0; i < len; i++) {
    gnome.push_back(mutated_genes());
    // if (rand() % 2 == 0)
    //   gnome.push_back(mutated_genes());
    // else
    //   gnome.push_back(heu_solution[i]);
  }
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
  for (int i = 0; i < chromosome.size(); i++) {
    if (chromosome[i] == 1) {
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
  int n_likes, n_dislikes;

  for (Person &person : people) {
    n_likes = intersection_size(chromosome_set, person.likes);
    n_dislikes = intersection_size(chromosome_set, person.dislikes);

    if (n_likes == person.likes.size() && n_dislikes == 0) fitness++;
  }
  return fitness;
};

// Overloading > operator
bool operator<(const Individual &ind1, const Individual &ind2) {
  return ind1.fitness > ind2.fitness;
}

// Driver code
int main(int argc, char **argv) {
  map<string, double> p = read_params(argc, argv);
  read_data(argv[1]);
  srand((unsigned)(time(0)));

  vector<int> heu_solution = heuristic_solution(p);

  int n_generations = p["n_generations"];
  int population_size = p["population_size"];

  // current generation
  int generation = 0;

  vector<Individual> population;
  bool found = false;

  // create initial population
  for (int i = 0; i < population_size; i++) {
    vector<int> gnome = create_gnome(heu_solution);
    population.push_back(Individual(gnome));
  }

  while (!found) {
    // sort the population in increasing order of fitness score
    sort(population.begin(), population.end());

    // Stopping criteria
    if (generation == n_generations) break;

    // Otherwise generate new offsprings for new generation
    vector<Individual> new_generation;

    // Perform Elitism, that mean 10% of fittest population
    // goes to the next generation
    int s1 = (10 * population_size) / 100;
    for (int i = 0; i < s1; i++) new_generation.push_back(population[i]);

    // From 50% of fittest population, Individuals
    // will mate to produce offspring
    int s2 = population_size - s1;
    for (int i = 0; i < s2; i++) {
      int len = population.size();
      int r = random_num(0, (50 * population_size) / 100);
      Individual parent1 = population[r];
      r = random_num(0, (50 * population_size) / 100);
      Individual parent2 = population[r];
      Individual offspring = parent1.mate(parent2);
      new_generation.push_back(offspring);
    }

    population = new_generation;
    cout << "Generation: " << generation << "\t";
    // cout << "String: " << population[0].chromosome << "\t";
    cout << "Fitness: " << population[0].fitness << "\n";

    generation++;
  }
  cout << "Generation: " << generation << "\t";
  // cout << "String: " << population[0].chromosome << "\t";
  cout << "Fitness: " << population[0].fitness << "\n";

  write_output(population[0].chromosome);
}
#include "cpp_utils/includes.hh"
#include "cpp_utils/read_data.hh"
#include "cpp_utils/read_params.hh"
#include "cpp_utils/write_output.hh"

int main(int argc, char** argv) {
  map<string, double> p = read_params(argc, argv);
  read_data(argv[1]);
  srand(time(0));

  // chosen[i] == 1 if the i'th ingredient is chosen, else 0
  vector<int> chosen;

  // assign score to ingredient
  vector<int> scores(N);
  for (Person& person : people) {
    for (int like : person.likes) {
      scores[like]++;
    }
    for (int dislike : person.dislikes) {
      scores[dislike]--;
    }
  }

  vector<int> ingredients(N);
  for (int i = 0; i < N; i++) {
    ingredients[i] = i;
  }

  sort(ingredients.begin(), ingredients.end(),
       [&](int a, int b) -> bool { return scores[a] > scores[b]; });

  int max_score = 1;
  for (int score : scores) max_score = max(score, max_score);

  for (int ingr : ingredients) {
    if (scores[ingr] > 0) {
      // we want the best ingredient to have chance = threshold
      // and the others to have chance = threshold * score / max_score
      int rnd = rand() % max_score;
      if (rnd < scores[ingr] * p["threshold"]) {
        chosen[ingr] = 1;
      }
    }
  }

  write_output(chosen);
}
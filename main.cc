#include "cpp_utils/includes.hh"
#include "cpp_utils/read_data.hh"
#include "cpp_utils/read_params.hh"
#include "cpp_utils/write_output.hh"

const int INF = 1e9;

int main(int argc, char** argv) {
  map<string, double> p = read_params(argc, argv);
  read_data(argv[1]);
  srand(time(0));
  int P = people.size();
  // chosen[i] == 1 if the i'th ingredient is chosen, else 0
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
    }
    else {
      scores[i] /= (a + b);
    }
  }

  vector<int> forbidden(N, 0);

  vector<int> people_idx(P);
  for (int i = 0; i < P; i++) {
    people_idx[i] = i;
  }

  sort(people_idx.begin(), people_idx.end(), [&](int a, int b) -> bool {
    return scores[a] > scores[b];
  });

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

  write_output(chosen);
}
#ifndef __READ_DATA__
#define __READ_DATA__

#include "includes.hh"

map<string, int> mapping;
vector<string> mapping_inv;
int N;

struct Person {
  set<int> likes;
  set<int> dislikes;
};

vector<Person> people;

void read_data(const string& path) {
  ifstream infile(path);
  int sz;
  infile >> sz;
  people.resize(sz);
  for (Person& person : people) {
    for (int k = 0; k < 2; k++) {
      int n;
      infile >> n;
      set<int> v;
      for (int i = 0; i < n; i++) {
        string s;
        infile >> s;
        if (mapping.find(s) == mapping.end()) {
          mapping[s] = mapping.size();
          mapping_inv.push_back(s);
        }
        v.insert(mapping[s]);
      }
      if (k == 0) {
        person.likes = v;
      } else {
        person.dislikes = v;
      }
    }
  }
  N = mapping.size();
}

#endif
#ifndef __WRITE_OUTPUT__
#define __WRITE_OUTPUT__

#include "includes.hh"
#include "read_data.hh"

void write_output(vector<int>& chosen) {
  int count = 0;
  for (int x : chosen) count += x;
  ofstream file("out_temp.txt");
  file << count;
  for (int i = 0; i < chosen.size(); i++) {
    if (chosen[i]) {
      file << " " << mapping_inv[i];
    }
  }
  file << endl;
}

#endif
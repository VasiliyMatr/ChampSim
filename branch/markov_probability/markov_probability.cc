#include "markov_probability.h"

#include <random>

bool markov_probability::predict_branch(champsim::address ip)
{
  auto cnt = counters_table[hash(ip)];

  double taken_probability = 0.5;
  if (auto total = cnt.taken + cnt.not_taken; total != 0) {
    taken_probability = static_cast<double>(cnt.taken) / static_cast<double>(total);
  }

  return probability_distr(mt) <= taken_probability;
}

void markov_probability::last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type)
{
  auto& cnt = counters_table[hash(ip)];
  if (taken) {
    ++cnt.taken;
  } else {
    ++cnt.not_taken;
  }
}

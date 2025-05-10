#include "markov_frequency.h"

bool markov_frequency::predict_branch(champsim::address ip)
{
  auto cnt = counters_table[hash(ip)];
  return cnt.taken >= cnt.not_taken;
}

void markov_frequency::last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type)
{
  auto& cnt = counters_table[hash(ip)];
  if (taken) {
    ++cnt.taken;
  } else {
    ++cnt.not_taken;
  }
}

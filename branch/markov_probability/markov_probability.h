#ifndef MARKOV_PROBABILITY_H
#define MARKOV_PROBABILITY_H

#include <array>
#include <random>

#include "address.h"
#include "modules.h"

class markov_probability : champsim::modules::branch_predictor
{
  [[nodiscard]] static constexpr auto hash(champsim::address ip) { return ip.to<unsigned long>() % PRIME; }

  static constexpr std::size_t TABLE_SIZE = 16384;
  static constexpr std::size_t PRIME = 16381;

  struct counters final {
    std::size_t taken = 0;
    std::size_t not_taken = 0;
  };

  std::array<counters, TABLE_SIZE> counters_table{};

  static constexpr int MT_SEED = 42;
  std::mt19937 mt{MT_SEED};

  std::uniform_real_distribution<> probability_distr{0.0, 1.0};

public:
  using branch_predictor::branch_predictor;

  // void initialize_branch_predictor();
  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif // MARKOV_PROBABILITY_H

#ifndef MARKOV_FREQUENCY_H
#define MARKOV_FREQUENCY_H

#include <array>

#include "address.h"
#include "modules.h"

class markov_frequency : champsim::modules::branch_predictor
{
  [[nodiscard]] static constexpr auto hash(champsim::address ip) { return ip.to<unsigned long>() % PRIME; }

  static constexpr std::size_t TABLE_SIZE = 16384;
  static constexpr std::size_t PRIME = 16381;

  struct counters final {
    std::size_t taken = 0;
    std::size_t not_taken = 0;
  };

  std::array<counters, TABLE_SIZE> counters_table{};

public:
  using branch_predictor::branch_predictor;

  // void initialize_branch_predictor();
  bool predict_branch(champsim::address ip);
  void last_branch_result(champsim::address ip, champsim::address branch_target, bool taken, uint8_t branch_type);
};

#endif // MARKOV_FREQUENCY_H

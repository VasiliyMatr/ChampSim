#include "plru.h"

#include <algorithm>
#include <cassert>

plru::plru(CACHE* cache) : plru(cache, cache->NUM_SET, cache->NUM_WAY) {}

plru::plru(CACHE* cache, long sets, long ways) : replacement(cache), m_trees(sets, plru_tree(ways)) {}

long plru::find_victim(uint32_t triggering_cpu, uint64_t instr_id, long set, const champsim::cache_block* current_set, champsim::address ip,
                       champsim::address full_addr, access_type type)
{
  return m_trees[set].victim();
}

void plru::replacement_cache_fill(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip, champsim::address victim_addr,
                                  access_type type)
{
  m_trees[set].update(way);
}

void plru::update_replacement_state(uint32_t triggering_cpu, long set, long way, champsim::address full_addr, champsim::address ip,
                                    champsim::address victim_addr, access_type type, uint8_t hit)
{
  if (hit && type != access_type::WRITE) // Skip this for writeback hits
    m_trees[set].update(way);
}

plru_tree::plru_tree(std::size_t ways)
{
  for (std::size_t i = 1; i != ways; i *= 2) {
    m_tree.emplace_back(i, victim_dir::LEFT);
  }
}

long plru_tree::victim()
{
  long victim_id = 0;
  for (auto&& level : m_tree) {
    victim_id = victim_id * 2 + level[victim_id];
  }

  return victim_id;
}

void plru_tree::update(long way)
{
  long prev_level_id = way;

  for (auto&& level : m_tree) {
    level[prev_level_id / 2] = static_cast<uint8_t>((prev_level_id + 1) % 2);
    prev_level_id /= 2;
  }
}

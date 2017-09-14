#include "TrussConfig.hpp"

TrussConfig::TrussConfig(unsigned int num_supports, unsigned int num_loads,
                         unsigned int num_nodes, unsigned int num_free,
                         std::vector<unsigned int> support_indices,
                         std::vector<unsigned int> load_indices,
                         std::vector<unsigned int> support_positions_x,
                         std::vector<unsigned int> support_positions_y,
                         std::vector<unsigned int> load_positions_x,
                         std::vector<unsigned int> load_positions_y) :
		Component("TrussConfig") {
	this->num_supports = num_supports;
	this->num_loads = num_loads;
	this->num_nodes = num_nodes;
	this->num_free = num_free;
	this->support_indices = support_indices;
	this->load_indices = load_indices;
	this->support_positions_x = support_positions_x;
	this->support_positions_y = support_positions_y;
	this->load_positions_x = load_positions_x;
	this->load_positions_y = load_positions_y;
}
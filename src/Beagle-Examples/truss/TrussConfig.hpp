#ifndef OPENBEAGLE_TRUSSCONFOIG_H
#define OPENBEAGLE_TRUSSCONFOIG_H

#include "Beagle/EC.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class TrussConfig : public Beagle::Component {
public:
	typedef Beagle::AllocatorT<TrussConfig, Beagle::Component::Alloc> Alloc;
	typedef Beagle::PointerT<TrussConfig, Beagle::Component::Handle> Handle;
	typedef Beagle::ContainerT<TrussConfig, Beagle::Component::Bag> Bag;

	explicit TrussConfig(unsigned int num_supports, unsigned int num_loads,
	                     unsigned int num_nodes, unsigned int num_free,
                         std::vector<unsigned int> support_indices,
                         std::vector<unsigned int> load_indices,
                         std::vector<unsigned int> support_positions_x,
                         std::vector<unsigned int> support_positions_y,
                         std::vector<unsigned int> load_positions_x,
                         std::vector<unsigned int> load_positions_y);

	unsigned int num_supports;
	unsigned int num_loads;
	unsigned int num_nodes;
	unsigned int num_free;
	std::vector<unsigned int> support_indices;
	std::vector<unsigned int> load_indices;
	std::vector<unsigned int> support_positions_x;
	std::vector<unsigned int> support_positions_y;
	std::vector<unsigned int> load_positions_x;
	std::vector<unsigned int> load_positions_y;
};


#endif //OPENBEAGLE_TRUSSCONFOIG_H

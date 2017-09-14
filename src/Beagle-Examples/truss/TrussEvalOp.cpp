#include "TrussEvalOp.hpp"

#include "Beagle/EC.hpp"
#include "Beagle/GP.hpp"
#include "Beagle/BitStr.hpp"

#include <SFML/Graphics.hpp>
#include <Eigen>
#include <cmath>

using namespace Beagle;
typedef sf::Vector2f position;
typedef std::vector<sf::Vector2f> position_array;
typedef std::vector<bool> bit_vector;

TrussEvalOp::TrussEvalOp() : GP::EvaluationOp("TrussEvalOp") {}

/*
 * Bit vector mapping definition:
 * b[a:b]: bits from index a to b inclusive, zero-indexed
 *
 * b[0:7]       0 <= num_nodes <= 255
 * b[8:15]      0 <= num_free <= 255
 * b[16:19]     0 <= num_loads <= 15
 * b[20:23]     0 <= num_supports <= 15
 * b[24:31]     0 <= truss_width_dm <= 255
 * b[32:39]     0 <= truss_height_dm <= 255
 * for each 16 bits num_supports: index of support k
 * for each 16 bits num_loads: index of load k
 * for each
 */
Fitness::Handle TrussEvalOp::evaluate(
		Beagle::Individual &individual, Beagle::Context &context) {
	Beagle_AssertM(individual.size() == 1);
	BitStr::BitString::Handle bit_vec = castHandleT<BitStr::BitString>(individual[0]);



	/*position_array free_node_positions;
	unsigned int bit_ptr = 0;
	for (unsigned int k_free = 0; k_free < config->num_free; k_free++) {
		unsigned int x = 0;
		unsigned int y = 0;
		for (unsigned int ptr = bit_ptr; ptr < bit_ptr + 8; ptr++) {
			x <<= (*bit_vec)[ptr];
		}
		bit_ptr += 8;
		for (unsigned int ptr = bit_ptr; ptr < bit_ptr + 8; ptr++) {
			y <<= (*bit_vec)[ptr];
		}
		bit_ptr += 8;
		position free_node_position(x / 10.0f, y / 10.0f);
		free_node_positions.push_back(free_node_position);
	}


	bit_vector struts;
	unsigned int num_struts = config->num_nodes * (config->num_nodes - 1) / 2;
	for (unsigned int k_strut = bit_ptr; k_strut < bit_ptr + num_struts; k_strut++) {
		struts[k_strut] = (*bit_vec)[bit_ptr];
	}
	bit_ptr += num_struts;
	Beagle_AssertM(bit_ptr == bit_vec->size());*/
}

void TrussEvalOp::init(System &system) {
	GP::EvaluationOp::init(system);
	config = castHandleT<TrussConfig>(system.getComponent("TrussConfig"));
	if (config == NULL) {
		throw Beagle_RunTimeExceptionM("TrussConfig not found in system.");
	}
	Beagle_AssertM(config->num_supports == 2);
	Beagle_AssertM(config->num_nodes - config->num_free
	               == config->num_loads + config->num_supports);
}

void TrussEvalOp::registerParams(System &system) {
	GP::EvaluationOp::registerParams(system);
	std::stringstream config;
	config << "An example configuration value.";
	Register::Description desc(
			"Example config",
			"String",
			"example.config",
			config.str()
	);
	this->string_config = castHandleT<String>(
			system.getRegister().insertEntry("example.config", new String("example.config"), desc));
}
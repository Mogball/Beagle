//
// Created by Jeff Niu on 7/17/2017.
//

#include "TrussConfig.h"
#include <string>

TrussConfig::TrussConfig() :
		loads(NULL),
		struts(NULL),
		strut_indices(NULL),
		strut_inverse(NULL) {}

TrussConfig::~TrussConfig() {
	delete[] loads;
	for (int32 k = 0; k < num_nodes; k++) {
		delete[] struts[k];
		delete[] strut_indices[k];
	}
	for (int32 k = 0; k < num_struts; k++) {
		delete[] strut_inverse[k];
	}
	delete[] struts;
	delete[] strut_indices;
	delete[] strut_inverse;
}

void TrussConfig::init(const std::string &config_file) {
	this->config_file = config_file;
	std::ifstream config_in(config_file);
	if (!config_in.good()) {
		throw "Configuration file (" + config_file + ") does not exist";
	}
	// Read truss length
	config_in >> truss_length;
	// Read fixed nodes
	config_in >> num_fixed_nodes;
	float32 spacing = (float32) (truss_length / (num_fixed_nodes - 1));
	for (int32 k = 0; k < num_fixed_nodes; k++) {
		float32 p;
		if (k == 0) p = 0;
		else if (k == 1) p = 3;
		else if (k == 2) p = 6;
		else if (k == 3) p = 8;
		else if (k == 4) p = 11;
		else p = 14;
		FixedNode fixed_node(k, p, 0);
		fixed_nodes.push_back(fixed_node);
	}
	// Read free nodes
	config_in >> num_free_nodes;
	for (int32 k = 0; k < num_free_nodes; k++) {
		FreeNode free_node(k);
		free_nodes.push_back(free_node);
	}
	// Remaining parameters
	num_nodes = num_free_nodes + num_fixed_nodes;
	float32 x_lower, x_upper, y_lower, y_upper;
	config_in >> x_lower >> x_upper >> y_lower >> y_upper;
	x_span.first = x_lower;
	x_span.second = x_upper;
	y_span.first = y_lower;
	y_span.second = y_upper;
	float32 total_force = (float32) (WEIGHT_PER_LENGTH * truss_length);
	load_force = total_force / num_fixed_nodes;
	support_force = -total_force / 2.0f;
	num_bits_total = (uint32) (NODE_BITS * num_free_nodes * 2 + 0 * NODE_BITS * (num_fixed_nodes - 2));
	// Read in struts
	config_in >> num_struts;
	struts = new bool *[num_nodes];
	strut_indices = new int32 *[num_nodes];
	strut_inverse = new int32 *[num_struts];
	for (int32 k = 0; k < num_nodes; k++) {
		struts[k] = new bool[num_nodes]();
		strut_indices[k] = new int32[num_nodes]();
	}
	for (int32 k = 0; k < num_struts; k++) {
		strut_inverse[k] = new int32[2]();
	}
	for (int32 k = 0; k < num_struts; k++) {
		int32 i, j;
		config_in >> i >> j;
		struts[i][j] = 1;
		struts[j][i] = 1;
		strut_indices[i][j] = k;
		strut_indices[j][i] = k;
		strut_inverse[k][0] = i;
		strut_inverse[k][1] = j;
	}
	// Set loads
	config_in >> support_a >> support_b;
	loads = new float32[num_nodes]();
	loads[support_a] += support_force;
	loads[support_b] += support_force;
	for (int32 k = 0; k < num_fixed_nodes; k++) {
		FixedNode fixed_node = fixed_nodes[k];
		loads[fixed_node.index] += load_force;
	}
	while (!config_in.eof()) {
		std::string line;
		std::getline(config_in, line);
		if (!line.size() || line.at(0) == '-') {
			continue;
		}
		genotypes.push_back(line);
	}
	config_in.close();
}
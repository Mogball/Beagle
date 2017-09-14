//
// Created by Jeff Niu on 7/17/2017.
//

#ifndef OPENBEAGLE_TRUSSCONFIG_H
#define OPENBEAGLE_TRUSSCONFIG_H

#include "GlobalConfig.hpp"


/*
 * Configuration object
 */
struct FixedNode {
	int32 index;
	float32 x;
	float32 y;

	FixedNode(int32 index, float32 x, float32 y) :
			index(index), x(x), y(y) {}
};

struct FreeNode {
	int32 index;

	FreeNode(int32 index) : index(index) {}
};

class TrussConfig {
public:
	static TrussConfig &get_conf() {
		static TrussConfig conf;
		return conf;
	}

	std::vector<FixedNode> fixed_nodes;
	std::vector<FreeNode> free_nodes;

	int32 num_fixed_nodes;
	int32 num_free_nodes;
	int32 num_nodes;
	int32 num_struts;

	int32 support_a;
	int32 support_b;

	uint32 num_bits_total;

	std::pair<float32, float32> x_span;
	std::pair<float32, float32> y_span;

	bool **struts;
	int32 **strut_indices;
	int32 **strut_inverse;
	float32 *loads;

	float32 support_force;
	float32 load_force;
	float32 truss_length;

	std::string config_file;

	std::vector<std::string> genotypes;

	TrussConfig();
	~TrussConfig();

	void init(const std::string &config_file);
};

#endif //OPENBEAGLE_TRUSSCONFIG_H

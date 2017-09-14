#ifndef OPENBEAGLE_GLOBAL_CONFIG_H
#define OPENBEAGLE_GLOBAL_CONFIG_H

#include <vector>
#include <tuple>
#include <fstream>
#include <SFML/System.hpp>
#include "vector.h"

#define HEUR(c, excess, unbalance, floorjoint, memberL) 30000/c - 5*excess - 15*unbalance - 5*floorjoint - 10 * memberL

/**
 * Genertic algorithm parameters
 */
#define MAX_GENERATIONS 300
#define MAX_POPULATION 1500
#define TOURNAMENT_SIZE 2
#define MIGRATION_SIZE 2
#define WRITING_INTERVAL 50

/**
 * Genotype parameters
 */
#define NODE_BITS 16

/*
 * Global truss constants
 */
#define COST_GUSSET_PLATE 6.0;
#define COST_MEMBER_LENGTH 12.0;
#define MIN_FORCE -13.0
#define MAX_FORCE 8.0
#define TOL 1e-6
#define FTOL 0.005
#define WEIGHT_PER_LENGTH 2.0

/*
 * Global types
 */
typedef int int32;
typedef unsigned int uint32;
typedef float float32;
typedef double float64;

/*
 * Helper Functions
 */
inline point unit_vector(point *nodes, __int32 i, __int32 j) {
	point r = nodes[j] - nodes[i];
	return r / r.norm();
}

inline double node_length(point *nodes, __int32 i, __int32 j) {
	point r = nodes[j] - nodes[i];
	return r.norm();
}

inline int32 str2bin(std::string s) {
	int32 i = 0;
	for (uint32 k = 0; k < s.length(); k++) {
		i += (1 << k) * (s.at(k) == '1');
	}
	return i;
}

inline float32 map_range(int32 raw, int32 max, std::pair<float32, float> bounds) {
	float32 range = bounds.second - bounds.first;
	float32 scale = float32(raw) / float32(max);
	return scale * range + bounds.first;
}

inline sf::Vector2f scale_pixel(sf::Vector2f pos) {
	float32 x = pos.x * 75 + 225;
	float32 y = 400 - pos.y * 75;
	return sf::Vector2f(x, y);
}

#endif

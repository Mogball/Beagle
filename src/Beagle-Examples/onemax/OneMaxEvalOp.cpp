#include "Beagle/EC.hpp"
#include "Beagle/BitStr.hpp"
#include "OneMaxEvalOp.hpp"
#include "GlobalConfig.hpp"
#include "TrussConfig.h"

#include <Eigen>

typedef Eigen::SparseMatrix<double> sparse_matrix;
typedef Eigen::VectorXd eigen_vector;
typedef Eigen::LeastSquaresConjugateGradient<sparse_matrix> linear_solver;
using std::tuple;
using std::tie;
using std::make_tuple;
using namespace Beagle;

OneMaxEvalOp::OneMaxEvalOp() : EvaluationOp("OneMaxEvalOp") {}

Fitness::Handle OneMaxEvalOp::evaluate(Individual &inIndividual, Context &ioContext) {
	Beagle_AssertM(inIndividual.size() == 1);
	BitStr::BitString::Handle bit_vec = castHandleT<BitStr::BitString>(inIndividual[0]);
	TrussConfig *conf = &TrussConfig::get_conf();
	int32 num_nodes = conf->num_nodes;
	int32 num_struts = conf->num_struts;
	int32 num_fixed_nodes = conf->num_fixed_nodes;

	point nodes[num_nodes];
	int32 bit_ptr = 0;
	float64 loads[conf->num_nodes]{0};
	loads[conf->support_a] += conf->support_force;
	loads[conf->support_b] += conf->support_force;
	for (int32 k = 0; k < num_fixed_nodes; k++) {
		FixedNode fixed_node = conf->fixed_nodes[k];
		nodes[fixed_node.index].x = fixed_node.x;
		nodes[fixed_node.index].y = fixed_node.y;
		/*if (k != 0 && k != num_fixed_nodes - 1) {
			int32 int_x = 0;
			for (int32 ptr = bit_ptr; ptr < bit_ptr + NODE_BITS; ptr++) {
				int_x += (1 << (ptr - bit_ptr)) * (*bit_vec)[ptr];
			}
			bit_ptr += NODE_BITS;
			double val_x = double(int_x) / (1 << NODE_BITS) * conf->truss_length;
			nodes[fixed_node.index].x = val_x;
		}*/
	}
	for (int32 k = num_fixed_nodes; k < num_nodes; k++) {
		int32 int_x = 0, int_y = 0;
		for (int32 ptr = bit_ptr; ptr < bit_ptr + NODE_BITS; ptr++) {
			int_x += (1 << (ptr - bit_ptr)) * (*bit_vec)[ptr];
		}
		bit_ptr += NODE_BITS;
		for (int32 ptr = bit_ptr; ptr < bit_ptr + NODE_BITS; ptr++) {
			int_y += (1 << (ptr - bit_ptr)) * (*bit_vec)[ptr];
		}
		bit_ptr += NODE_BITS;
		double val_x = double(int_x) / (1 << NODE_BITS)
		               * (conf->x_span.second - conf->x_span.first) + conf->x_span.first;
		double val_y = double(int_y) / (1 << NODE_BITS)
		               * (conf->y_span.second - conf->y_span.first) + conf->y_span.first;
		if ((k == conf->support_a || k == conf->support_b) && (val_y > 0)) {
			val_y = 0.01;
		}
		nodes[k].x = val_x;
		nodes[k].y = val_y;
		if (k == conf->support_a) {
			nodes[k].x = 0;
		} else if (k == conf->support_b) {
			nodes[k].x = conf->truss_length;
			nodes[k].y = nodes[conf->support_b].y;
		}
	}
	float64 length_penalty = 0;
	for (int32 k = 0; k < conf->num_fixed_nodes - 1; k++) {
		float64 length = fabs(nodes[k + 1].x - nodes[k].x);
		if (length > 3) {
			length_penalty += length - 3;
		}
		float64 overallforce = length * WEIGHT_PER_LENGTH;
		float64 jointforce = overallforce / 2.0;
		loads[k] += jointforce;
		loads[k + 1] += jointforce;
	}
	int32 num_eq = 2 * num_nodes;
	int32 index_eq = 0;
	sparse_matrix coeff_matrix(num_eq, num_struts);
	eigen_vector const_vector(num_eq);
	eigen_vector forces(num_struts);
	for (int32 i = 0; i < num_eq; i++) {
		const_vector[i] = 0.0;
	}
	for (int32 i = 0; i < num_nodes; i++) {
		if (fabs(loads[i]) >= TOL) {
			const_vector[2 * i + 1] = -loads[i];
		}
		for (int32 j = 0; j < num_nodes; j++) {
			if (!conf->struts[i][j]) {
				continue;
			}
			point u_vec = unit_vector(nodes, i, j);
			if (fabs(u_vec.x) < TOL) {
				u_vec.x = 0.0;
			}
			if (fabs(u_vec.y) < TOL) {
				u_vec.y = 0.0;
			}
			coeff_matrix.insert(index_eq, conf->strut_indices[i][j]) = u_vec.x;
			coeff_matrix.insert(index_eq + 1, conf->strut_indices[i][j]) = u_vec.y;
		}
		index_eq += 2;
	}
	coeff_matrix.makeCompressed();

	linear_solver solver;
	solver.analyzePattern(coeff_matrix);
	solver.factorize(coeff_matrix);
	forces = solver.solve(const_vector);

	float32 member_length_penalty = 0;
	float32 excess_force_penalty = 0;
	double total_cost = num_nodes * COST_GUSSET_PLATE;
	point force_sum[num_nodes];
	for (int32 k = 0; k < num_nodes; k++) {
		force_sum[k].x = 0.0;
		force_sum[k].y = loads[k];
	}
	for (int32 k = 0; k < num_struts; k++) {
		const int32 *strut_index = conf->strut_inverse[k];
		int32 i = strut_index[0];
		int32 j = strut_index[1];
		double strut_length = node_length(nodes, i, j);
		point fij = unit_vector(nodes, i, j) * forces[k];
		force_sum[i].x += fij.x;
		force_sum[i].y += fij.y;
		force_sum[j].x -= fij.x;
		force_sum[j].y -= fij.y;
		if (strut_length < 1) {
			member_length_penalty += 1 - strut_length;
		}
		float64 member_cost = strut_length * COST_MEMBER_LENGTH;
		total_cost += member_cost;
		if (forces[k] < MIN_FORCE * 2 - FTOL) {
			excess_force_penalty += 2 * MIN_FORCE - forces[k];
			total_cost += member_cost;
		} else if (forces[k] < MIN_FORCE - FTOL) {
			total_cost += member_cost;
		}
		if (forces[k] > MAX_FORCE * 2 + FTOL) {
			excess_force_penalty += forces[k] - 2 * MAX_FORCE;
			total_cost += member_cost;
		} else if (forces[k] > MAX_FORCE + FTOL) {
			total_cost += member_cost;
		}
	}
	double nonzero_sum_penalty = 0;
	for (int32 k = 0; k < num_nodes; k++) {
		nonzero_sum_penalty += fabs(force_sum[k].x);
		nonzero_sum_penalty += fabs(force_sum[k].y);
	}

	double reduc = 0;
	reduc += HEUR(total_cost, excess_force_penalty, nonzero_sum_penalty, length_penalty, member_length_penalty);



	return new EC::FitnessSimple(reduc);
}

#include "Beagle/EC.hpp"
#include "Beagle/BitStr.hpp"
#include "OneMaxEvalOp.hpp"

#include "GlobalConfig.hpp"
#include "TrussConfig.h"
#include "ZeroMinEvalOp.hpp"

#include <numeric>
#include <Eigen>
#include <SFML/Graphics.hpp>

typedef Eigen::SparseMatrix<float64> sparse_matrix;
typedef Eigen::VectorXd eigen_vector;
typedef Eigen::LeastSquaresConjugateGradient<sparse_matrix> linear_solver;
using namespace std;
using namespace Beagle;

void start_evolution(int argc, char **argv) {
	TrussConfig *conf = &TrussConfig::get_conf();
	uint32 num_bits_total = conf->num_bits_total;
	try {
		System::Handle lSystem = new System;
		lSystem->addPackage(new BitStr::Package(num_bits_total));
		lSystem->setEvaluationOp("OneMaxEvalOp", new OneMaxEvalOp::Alloc);
		Evolver::Handle lEvolver = new Evolver;
		lEvolver->initialize(lSystem, argc, argv);
		Vivarium::Handle lVivarium = new Vivarium;
		lEvolver->evolve(lVivarium, lSystem);

		HallOfFame::Handle hallOfFame = lVivarium->getHallOfFame();
		if (hallOfFame != NULL) {
			hallOfFame->sort();
			fstream out_file;
			out_file.open(conf->config_file, fstream::out | fstream::app);
			out_file << "- Best of run" << endl;
			for (unsigned int i = 0; i < hallOfFame->size(); i++) {
				BitStr::BitString::Handle bit_vec = castHandleT<BitStr::BitString>(
						(*(*hallOfFame)[i].mIndividual)[0]
				);
				for (unsigned int k = 0; k < bit_vec->size(); k++) {
					out_file << (*bit_vec)[k];
				}
				out_file << endl;
			}
		}
	} catch (Exception &inException) {
		inException.terminate(cerr);
	} catch (std::exception &inException) {
		cerr << "Standard exception caught:" << endl << flush;
		cerr << inException.what() << endl << flush;
	}
}

void solve_eqn(int argc, char **argv) {
	uint32 num_bits_total = 20 * 5;
	try {
		System::Handle lSystem = new System;
		lSystem->addPackage(new BitStr::Package(num_bits_total));
		lSystem->setEvaluationOp("ZeroMinEvalOp", new ZeroMinEvalOp::Alloc);
		Evolver::Handle lEvolver = new Evolver;
		lEvolver->initialize(lSystem, argc, argv);
		Vivarium::Handle lVivarium = new Vivarium;
		lEvolver->evolve(lVivarium, lSystem);

		HallOfFame::Handle hallOfFame = lVivarium->getHallOfFame();
		if (hallOfFame != NULL) {
			hallOfFame->sort();
			fstream out_file;
			out_file.open("OUTPUT.out", fstream::out | fstream::app);
			out_file << "- Best of run" << endl;
			for (unsigned int i = 0; i < hallOfFame->size(); i++) {
				BitStr::BitString::Handle bit_vec = castHandleT<BitStr::BitString>(
						(*(*hallOfFame)[i].mIndividual)[0]
				);
				for (unsigned int k = 0; k < bit_vec->size(); k++) {
					out_file << (*bit_vec)[k];
				}
				out_file << endl;
			}
		}
	} catch (Exception &inException) {
		inException.terminate(cerr);
	} catch (std::exception &inException) {
		cerr << "Standard exception caught:" << endl << flush;
		cerr << inException.what() << endl << flush;
	}
}

void visualize_truss(const std::string &file) {
	sf::RenderWindow window(sf::VideoMode(1600, 900, 32), "Truss Visualizer");
	sf::Font arial;
	arial.loadFromFile("arial.ttf");
	TrussConfig *conf = &TrussConfig::get_conf();
	conf->init(file);
	int32 num_genotypes = conf->genotypes.size();
	int32 index_genotype = 0;
	vector<sf::Text> text[num_genotypes];
	vector<sf::CircleShape> shapes[num_genotypes];
	vector<sf::VertexArray> lines[num_genotypes];
	for (int32 k = 0; k < num_genotypes; k++) {
		point nodes[conf->num_nodes];
		vector<sf::Vector2f> positions;
		float64 loads[conf->num_nodes]{0};
		loads[conf->support_a] += conf->support_force;
		loads[conf->support_b] += conf->support_force;
		string genotype = conf->genotypes[k];
		uint32 bits = NODE_BITS;
		int32 bit_max = (1 << bits);
		uint32 bit_ptr = 0;
		std::pair<float32, float32> range(0.0, conf->truss_length);
		for (int32 o = 0; o < conf->num_fixed_nodes; o++) {
			FixedNode fixed_node = conf->fixed_nodes[o];
			sf::CircleShape node(4);
			node.setFillColor(sf::Color::Black);
			sf::Vector2f position(fixed_node.x, fixed_node.y);
			/*if (o != 0 && o != conf->num_fixed_nodes - 1) {
				std::string str_x = genotype.substr(bit_ptr, bits);
				bit_ptr += bits;
				int32 int_x = str2bin(str_x);
				float32 x_val = map_range(int_x, bit_max, range);
				position.x = x_val;
			}*/
			printf("(%.6f, %.6f)\n", position.x, position.y);
			nodes[o].x = position.x;
			nodes[o].y = position.y;
			position = scale_pixel(position);
			positions.push_back(position);
			node.setPosition(position - sf::Vector2f(2, 2));
			shapes[k].push_back(node);
			sf::Text label(std::to_string(o), arial, 14);
			label.setPosition(position.x - 20, position.y - 20);
			label.setFillColor(sf::Color::Black);
			text[k].push_back(label);
		}
		float64 length_penalty = 0;
		for (int32 kk = 0; kk < conf->num_fixed_nodes - 1; kk++) {
			float64 length = nodes[kk + 1].x - nodes[kk].x;
			float64 overallforce = length * WEIGHT_PER_LENGTH;
			float64 jointforce = overallforce / 2.0;
			loads[kk] += jointforce;
			loads[kk + 1] += jointforce;
			if (length > 3) {
				length_penalty += length - 3;
			}
		}
		for (int32 o = 0; o < conf->num_free_nodes; o++) {
			std::string str_x = genotype.substr(bit_ptr, bits);
			bit_ptr += bits;
			std::string str_y = genotype.substr(bit_ptr, bits);
			bit_ptr += bits;
			int32 int_x = str2bin(str_x);
			int32 int_y = str2bin(str_y);
			sf::CircleShape node(4);
			node.setFillColor(sf::Color::Black);
			float32 x_val = map_range(int_x, bit_max, conf->x_span);
			float32 y_val = map_range(int_y, bit_max, conf->y_span);
			if (o + conf->num_fixed_nodes == conf->support_a) {
				x_val = 0;
			} else if (o + conf->num_fixed_nodes == conf->support_b) {
				x_val = 14;
				y_val = (float32) nodes[conf->support_a].y;
			}
			sf::Vector2f position(x_val, y_val);
			nodes[o + conf->num_fixed_nodes].x = x_val;
			nodes[o + conf->num_fixed_nodes].y = y_val;

			printf("(%.6f, %.6f)\n", x_val, y_val);
			position = scale_pixel(position);
			positions.push_back(position);
			node.setPosition(position - sf::Vector2f(2, 2));
			shapes[k].push_back(node);
			sf::Text label(std::to_string(o + conf->num_fixed_nodes), arial, 14);
			label.setPosition(position.x - 20, position.y - 20);
			label.setFillColor(sf::Color::Black);
			text[k].push_back(label);
		}
		for (int32 kk = 0; kk < conf->num_nodes; kk++) {
			printf("(%.6f)\n", loads[kk]);
		}
		for (int32 o = 0; o < conf->num_struts; o++) {
			int32 *strut = conf->strut_inverse[o];
			sf::VertexArray line(sf::LineStrip, 2);
			int32 i = strut[0];
			int32 j = strut[1];
			line[0].position = positions[i];
			line[1].position = positions[j];
			line[0].color = sf::Color::Black;
			line[1].color = sf::Color::Black;
			lines[k].push_back(line);
		}
		int32 num_eq = 2 * conf->num_nodes;
		int32 index_eq = 0;
		sparse_matrix coeff_matrix(num_eq, conf->num_struts);
		eigen_vector const_vector(num_eq);
		eigen_vector forces(conf->num_struts);
		for (int32 i = 0; i < num_eq; i++) {
			const_vector[i] = 0.0;
		}
		for (int32 i = 0; i < conf->num_nodes; i++) {
			if (fabs(loads[i]) >= TOL) {
				const_vector[2 * i + 1] = -loads[i];
			}
			for (int32 j = 0; j < conf->num_nodes; j++) {
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

		float32 excess_force = 0;
		float32 member_length_penalty = 0;
		float64 total_cost = conf->num_nodes * COST_GUSSET_PLATE;
		point force_sum[conf->num_nodes];
		for (int32 t = 0; t < conf->num_nodes; t++) {
			force_sum[t].x = 0.0;
			force_sum[t].y = loads[t];
		}
		for (int32 l = 0; l < conf->num_struts; l++) {
			const int32 *strut_index = conf->strut_inverse[l];
			int32 i = strut_index[0];
			int32 j = strut_index[1];
			float64 strut_length = node_length(nodes, i, j);
			point fij = unit_vector(nodes, i, j) * forces[l];
			force_sum[i].x += fij.x;
			force_sum[i].y += fij.y;
			force_sum[j].x -= fij.x;
			force_sum[j].y -= fij.y;
			if (strut_length < 1) {
				member_length_penalty += 1 - strut_length;
			}
			float64 member_cost = strut_length * COST_MEMBER_LENGTH;
			total_cost += member_cost;
			printf("(%i, %i) : %.6f, %.6f\n", i, j, forces[l], strut_length);
			if (forces[l] < MIN_FORCE * 2 - FTOL) {
				excess_force += 2 * MIN_FORCE - forces[l];
				total_cost += member_cost;
				printf("OVER\n");
			} else if (forces[l] < MIN_FORCE - FTOL) {
				total_cost += member_cost;
				printf("OVER\n");
			}
			if (forces[l] > MAX_FORCE * 2 + FTOL) {
				excess_force += forces[l] - 2 * MAX_FORCE;
				total_cost += member_cost;
				printf("OVER\n");
			} else if (forces[l] > MAX_FORCE + FTOL) {
				total_cost += member_cost;
				printf("OVER\n");
			}
		}
		float64 nonzero_sum = 0;
		for (int32 h = 0; h < conf->num_nodes; h++) {
			nonzero_sum += fabs(force_sum[h].x);
			nonzero_sum += fabs(force_sum[h].y);
		}
		sf::Text cost_text("nil", arial, 11);
		sf::Text penalty_text("nil", arial, 11);
		stringstream cost_ss, penalty_ss;
		cost_ss << fixed << setprecision(2) << "Cost: $" << total_cost;
		penalty_ss << fixed << setprecision(5) << "Excess: " << excess_force << ", Nonzero: " << nonzero_sum << ", Spacing: " << length_penalty << ", " << member_length_penalty;
		cost_text.setString(cost_ss.str());
		cost_text.setPosition(25, 25);
		cost_text.setFillColor(sf::Color::Black);
		penalty_text.setString(penalty_ss.str());
		penalty_text.setPosition(25, 50);
		penalty_text.setFillColor(sf::Color::Black);

		text[k].push_back(cost_text);
		text[k].push_back(penalty_text);
		float64 reduc = HEUR(total_cost, excess_force, nonzero_sum, length_penalty, member_length_penalty);
		sf::Text reduc_text("nil", arial, 11);
		stringstream reduc_ss;
		reduc_ss << fixed << setprecision(3) << "Reduc: " << reduc;
		reduc_text.setString(reduc_ss.str());
		reduc_text.setPosition(25, 75);
		reduc_text.setFillColor(sf::Color::Black);
		text[k].push_back(reduc_text);
	}
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				if (index_genotype > 0) {
					index_genotype--;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				if (index_genotype < num_genotypes - 1) {
					index_genotype++;
				}
			}
			window.clear(sf::Color::White);
			for (int32 k = 0; k < text[index_genotype].size(); k++) {
				window.draw(text[index_genotype][k]);
			}
			for (int32 k = 0; k < shapes[index_genotype].size(); k++) {
				window.draw(shapes[index_genotype][k]);
			}
			for (int32 k = 0; k < lines[index_genotype].size(); k++) {
				window.draw(lines[index_genotype][k]);
			}
			window.display();
		}
	}
}

int main(int argc, char **argv) {
	printf("a - [config_file] evolve bridge\n");
	printf("b - [result_file] visualize bridge\n");
	printf("c - solve equations\n");
	char choice;
	std::string file;
	std::cin >> choice >> file;
	if (choice == 'a') {
		TrussConfig::get_conf().init(file);
		start_evolution(argc, argv);
	} else if (choice == 'b') {
		visualize_truss(file);
	} else if (choice == 'c') {
		solve_eqn(argc, argv);
	} else {
		printf("Unknown command\n");
	}
}

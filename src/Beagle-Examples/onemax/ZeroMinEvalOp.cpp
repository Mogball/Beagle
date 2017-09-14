/*
 *  ZeroMin (zeromin):
 *  Minimization GA example with Open BEAGLE
 *
 *  Copyright (C) 2003-2006 by Christian Gagne and Marc Parizeau
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   ZeroMinEvalOp.cpp
 *  \brief  Implementation of the class ZeroMinEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11 $
 *  $Date: 2007/08/08 19:26:48 $
 */

#include "Beagle/EC.hpp"
#include "Beagle/BitStr.hpp"
#include "ZeroMinEvalOp.hpp"

#include <cmath>

using namespace Beagle;

/*!
 *  \brief Construct the individual evaluation operator for the ZeroMin problem.
 */
ZeroMinEvalOp::ZeroMinEvalOp() :
		EvaluationOp("ZeroMinEvalOp") {}

double compute_distance(double x1, double x2, double y1, double y2, double y3) {
	double xA = 7 - x2;
	double xC = x2 - 4.2;
	double xD = x2 - x1;
	double yD = y2 - y1;
	double xE = 4.2 - x1;
	double xG = x1 - 1.4;
	double yH = y1 - y3;
	double y2sq = y2 * y2;
	double y1sq = y1 * y1;
	double dAsq = xA * xA + y2sq;
	double dB = 2.8;
	double dCsq = xC * xC + y2sq;
	double dDsq = xD * xD + yD * yD;
	double dEsq = xE * xE + y1sq;
	double dF = 2.8;
	double dGsq = xG * xG + y1sq;
	double dHsq = x1 * x1 + yH * yH;
	double dIsq = 1.4 * 1.4 + y3 * y3;
	double dJ = 2.8;
	double d = sqrt(dAsq) + dB + sqrt(dCsq) + sqrt(dDsq) + sqrt(dEsq) + dF + sqrt(dGsq) + sqrt(dHsq) + sqrt(dIsq) + dJ;
	return d;
}

double FA(double x1, double x2, double y1, double y2, double y3) {
	double xx = 7 - x2;
	double y2sq = y2 * y2;
	return 14 / 3 / y2 * sqrt(xx * xx + y2sq);
}

double FB(double x1, double x2, double y1, double y2, double y3) {
	return 14 / 3 / y2 * (7 - x2);
}

double FC(double x1, double x2, double y1, double y2, double y3) {
	double p1 = 14 * ((x1 - x2) / (y2 - y1) + (x2 - 7) / y2);
	double xx = x2 - 4.2;
	double p2 = sqrt(xx * xx + y2 * y2);
	double p3 = 3 * (x2 - 4.2 + y2 * (x1 - x2) / (y2 - y1));
	return p1 * p2 / p3;
}

double FD(double x1, double x2, double y1, double y2, double y3) {
	double xx = x2 - x1;
	double yy = y2 - y1;
	double p1 = sqrt(xx * xx + yy * yy) / yy;
	double p2 = 14 * (-xx / yy + (x2 - 7) / y2);
	double p3 = 3 * ((x2 - 4.2) / y2 - xx / yy);
	return p1 * p2 / p3;
}

double FE(double x1, double x2, double y1, double y2, double y3) {
	double xx = 4.2 - x1;
	double p1 = sqrt(xx * xx + y1 * y1);
	double p2 = 28 / 3 / y1 -
	            14 * y2 / 3 / y1 * (((x1 - x2) / (y2 - y1) + (x2 - 7) / y2) / (x2 - 4.2 + y2 * (x1 - x2) / (y2 - y1)));
	return p1 * p2;
}

double FG(double x1, double x2, double y1, double y2, double y3) {
	double xx = x1 - 1.4;
	return 14 * sqrt(xx * xx + y1 * y1) / 3 / y1;
}

double FF(double x1, double x2, double y1, double y2, double y3) {
	return 14 * (x1 - 1.4) / 3 / y1;
}

/*!
 *  \brief Evaluate the fitness of the given individual.
 *  \param inIndividual Current individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness value of the GA individual.
 */
Fitness::Handle ZeroMinEvalOp::evaluate(Individual &inIndividual, Context &ioContext) {
	Beagle_AssertM(inIndividual.size() == 1);
	BitStr::BitString::Handle bits = castHandleT<BitStr::BitString>(inIndividual[0]);
	// 20 * 5
	double x1 = 0;
	double x2 = 0;
	double y1 = 0;
	double y2 = 0;
	double y3 = 0;
	int bit_ptr = 0;
	for (int i = bit_ptr; i < bit_ptr + 20; i++) {
		x1 += (1 << i) * (*bits)[i];
	}
	bit_ptr += 20;
	for (int i = bit_ptr; i < bit_ptr + 20; i++) {
		x2 += (1 << i) * (*bits)[i];
	}
	bit_ptr += 20;
	for (int i = bit_ptr; i < bit_ptr + 20; i++) {
		y1 += (1 << i) * (*bits)[i];
	}
	bit_ptr += 20;
	for (int i = bit_ptr; i < bit_ptr + 20; i++) {
		y2 += (1 << i) * (*bits)[i];
	}
	bit_ptr += 20;
	for (int i = bit_ptr; i < bit_ptr + 20; i++) {
		y3 += (1 << i) * (*bits)[i];
	}
	bit_ptr += 20;
	Beagle_AssertM(bit_ptr = 20 * 5);

	double max = 1 << 20;
	x1 = x1 / max * 0.1 + 2.7;
	x2 = x2 / max * 0.1 + 4.6;
	y1 = y1 / max * 0.1 + 0.9;
	y2 = y2 / max * 0.1 + 1.4;
	y3 = y3 / max * 0.2 + 0.6;

	double cost = compute_distance(x1, x2, y1, y2, y3);
	double fa = FA(x1, x2, y1, y2, y3);
	double fb = FB(x1, x2, y1, y2, y3);
	double fc = FC(x1, x2, y1, y2, y3);
	double fd = FD(x1, x2, y1, y2, y3);
	double fe = FE(x1, x2, y1, y2, y3);
	double ff = FF(x1, x2, y1, y2, y3);
	double fg = FG(x1, x2, y1, y2, y3);
	double forces[]{fa, fb, fc, fd, fe, ff, fg};
	double offset = 0;
	for (int i = 0; i < 7; i++) {
		if (forces[i] < -13) {
			offset += 12 * (-13 - forces[i]);
		} else if (forces[i] > 8) {
			offset += 12 * (forces[i] - 8);
		}
	}
	double value = 30000 / cost - offset;

	return new EC::FitnessSimpleMin(float(value));
}

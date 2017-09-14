#ifndef GSM_l_vector_H
#define GSM_l_vector_H

#define __float32 double

#include <cmath>
#include <iostream>

class point {
public:
	__float32 x;
	__float32 y;

	inline point() : x(0.0), y(0.0) {}

	inline explicit point(__float32 x, __float32 y) : x(x), y(y) {}

	inline __float32 norm() const {
		return sqrt(x * x + y * y);
	}
};

inline point operator-(const point &a, const point &b) {
	return point(a.x - b.x, a.y - b.y);
}

inline point operator/(const point &a, const __float32 &s) {
	return point(a.x / s, a.y / s);
}

inline point operator*(const point &a, const __float32 &s) {
	return point(a.x * s, a.y * s);
}

inline std::ostream &operator<<(std::ostream &os, const point &v) {
	os << '(' << v.x << ',' << ' ' << v.y << ')';
}

#endif //GSM_l_vector_H

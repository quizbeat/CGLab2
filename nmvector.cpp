#include "nmvector.h"

NMVector::NMVector() {
    x = 0; y = 0; z = 0; w = 1;
}

NMVector::NMVector(float _x, float _y, float _z, float _w) {
    x = _x; y = _y; z = _z; w = _w;
}

NMVector NMVector::operator+(const NMVector &v) {
	return NMVector(x + v.x, y + v.y, z + v.z, w + v.w);
}

NMVector NMVector::operator-(const NMVector &v) {
	return NMVector(x - v.x, y - v.y, z - v.z, w - v.w);
}

void NMVector::operator=(const NMVector &v) {
    x = v.x; y = v.y; z = v.z; w = v.w;
}

NMVector NMVector::crossProduct(const NMVector &v1, const NMVector &v2) {
    return NMVector(v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x,
                    1.0f);
}

float NMVector::dotProduct(const NMVector &v1, const NMVector &v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

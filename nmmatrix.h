#ifndef NMMATRIX_H
#define NMMATRIX_H

#include "nmvector.h"
#include <cmath>

class NMMatrix
{
public:
    float a[4][4];
    NMMatrix();
    NMMatrix(float arg[4][4]);
    void rotateXY(float alpha);
    void rotateXZ(float alpha);
    void rotateYZ(float alpha);
    void setScale(float scale);
    NMVector operator*(const NMVector &v);
    NMMatrix operator*(const NMMatrix &m);
};

#endif // NaATRIX_H

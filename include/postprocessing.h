#pragma once

#include "types.h"

// Post processing image with filters
Image postprocessing(Image src);

class unaryOp
{
    Matrix<double> kernel;
public:
    const uint radius;
    unaryOp(const Matrix<double>& ker): kernel(ker), radius((ker.n_rows-1)/2) {}
    std::tuple<uint,uint,uint> operator()(const Image& neighbourhood) const;

};

// Check on out of bounds
std::tuple<uint,uint,uint> overflow_crop(const std::tuple<uint,uint,uint>& pix);

// Autocontrast filter
Image autocontrast(Image src_image, double fraction);

// Sharping filter
Image unsharp(Image src_image);
#pragma once

#include "types.h"

// Post processing image with filters
Image postprocessing(Image src);

// Check on out of bounds
std::tuple<uint,uint,uint> overflow_crop(const std::tuple<uint,uint,uint>& pix);

// Autocontrast filter
Image autocontrast(Image src_image, double fraction);

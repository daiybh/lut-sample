#pragma once
#include <CubeLUT.hpp>
#include <unsupported/Eigen/CXX11/Tensor>
#include <cstdint>
struct WorkerData;

namespace Trilinear
{
int applyTrilinear(unsigned char*srcImage, unsigned char*destImage, int cols, int rows, int channels, const Table3D& lut, float opacity, int threadPool);
int applyTrilinear_noThread(unsigned char*srcImage, unsigned char*destImage, int cols, int rows, int channels, const Table3D& lut, float opacity, int threadPool);

void calculateArea(int x, const Table3D& lut, float opacity, const WorkerData& data, int segWidth);

void calculatePixel(int x, int y, const Table3D& lut, float opacity, const WorkerData& data);
} // namespace Trilinear

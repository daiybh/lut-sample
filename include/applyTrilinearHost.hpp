#pragma once

class GpuTrilinear
{
public:
	GpuTrilinear();
	int init(int cols, int rows, int channels, const Table3D& lut);
	int applyTrilinearGpu(uint8_t* srcImage, uint8_t* destImage,   const float opacity, const int threads);
private:
	int lutdimension = 0;
	int imgSize;
	int lutSize;
	float* lutPtr{ nullptr };
	uint8_t* imgPtr{ nullptr };
	int width{ 0 }, height{ 0 };
};

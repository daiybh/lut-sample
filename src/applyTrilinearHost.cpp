#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <CubeLUT.hpp>
#include <applyTrilinearGpu.cuh>
#include <applyTrilinearHost.hpp>
#include <CudaUtils.hpp>
#include <tuple>
#include <cstdint>
#include <iostream>
#define uchar uint8_t

GpuTrilinear::GpuTrilinear()
{

}

int GpuTrilinear::init(int cols, int rows, int channels, const Table3D& lut)
{
	auto deviceInfo = CudaUtils::getCudaDeviceInfo();
	for (auto info :deviceInfo)
	{
		std::cout << info.first << ": " << info.second << std::endl;
	}
	width = { cols };
	height = { rows };
	imgSize = width * height * 3 * sizeof(unsigned char);
	lutSize = static_cast<int>(
		pow(lut.dimension(0), 3) * 3 * sizeof(float));
	cudaErrorChk(cudaMalloc(reinterpret_cast<void**>(&lutPtr), lutSize));
	cudaErrorChk(cudaMemcpy(lutPtr, lut.data(), lutSize, cudaMemcpyHostToDevice));
	cudaErrorChk(cudaMallocManaged(&imgPtr, imgSize));
	lutdimension = lut.dimension(0);
	return 0;
}

int GpuTrilinear::applyTrilinearGpu(uint8_t* srcImage, uint8_t* destImage, const float opacity, const int threads)
{
	// Declare device (or/and host) pointers

	// Copy data to GPU

	memcpy(imgPtr, srcImage, imgSize);

	const int blocksX = (width + threads - 1) / threads;
	const int blocksY = (height + threads - 1) / threads;
	const dim3 threadsGrid(threads, threads);
	const dim3 blocksGrid(blocksX, blocksY);

	// Process data
	GpuTrilinearDevice::run(threadsGrid, blocksGrid, imgPtr, 3, lutPtr, lutdimension, opacity, { width, height });

	// Free memory and copy data back to host
	//auto finalImg = cv::Mat(height, width, CV_8UC3, imgPtr);
	//return finalImg;
	// Copy data back to host
	cudaErrorChk(cudaMemcpy(destImage, imgPtr, imgSize, cudaMemcpyDeviceToHost));
	return 0;
}

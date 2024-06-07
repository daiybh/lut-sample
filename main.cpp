#include <iostream>
#include <fstream>
#include <functional>  // std::cref
#include <vector>
#include <algorithm>  // std::for_each
#include <iosfwd>

#include "CubeLUT.hpp"
#include "applyTrilinear.hpp"

class Worker {
public:
	Worker() {
		cube = std::make_unique<CubeLUT>();
	}
	void run() {
		int w = 1920;
		int h = 1080;
		int channels = 3;

		destRGBImage = new uint8_t[w * h * channels];
		loadLut();
		loadRGB();

		std::cout << "[INFO] start applyTrilinear.....\n";
		auto start = std::chrono::high_resolution_clock::now();
		for(int i=0;i<1000;i++)
		{
			Trilinear::applyTrilinear(srcRGBImage, destRGBImage, w, h, channels, 
				std::get<Table3D>(cube->getTable()), 1.0f, 3);
		}
		auto end2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> duration2 = end2 - start;

		std::cout << "function2() executed in " << duration2.count() << " ms" << std::endl;
		std::cout << "function2() executed in " << duration2.count()/1000 << " ms/1000" << std::endl;
		std::ofstream outfile("out.rgb", std::ios::binary);
		outfile.write((char*)destRGBImage, w * h * channels);
		outfile.close();

		std::cout << "[INFO] Done applyTrilinear\n";
	}
private:
	bool loadRGB() {
		std::cout << "[INFO] Importing RGB...\n";

		FILE* fp = fopen(rgbPath.data(), "rb");
		if (fp == nullptr) {
			std::cout << "[ERROR] Could not open input RGB file: " << rgbPath << std::endl;
			return false;
		}

		fseek(fp, 0, SEEK_END);
		int xfileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		srcRGBImage = new uint8_t[xfileSize];
		memset(srcRGBImage, 1, xfileSize);
		int x = fread(srcRGBImage, xfileSize, 1, fp);
		fclose(fp);

		std::cout << "[INFO] Done.\n";
		return true;
	}
	bool loadLut()
	{
		bool success = true;
		std::cout << "[INFO] Importing LUT...\n";

		std::ifstream infile(lutPath);
		if (!infile.good())
		{
			std::cerr << std::format("[ERROR] Could not open input LUT file: {}\n", lutPath);
			return false;
		}
		std::cout << "[INFO] Parsing LUT...\n";
		try {
			cube->loadCubeFile(infile);
		}
		catch (const std::runtime_error& ex) {
			std::cerr << std::format("[ERROR] {}\n", ex.what());
			success = false;
		}
		infile.close();
		std::cout << "[INFO] Done.\n";
		return success;
	}

private:
	std::string lutPath = R"(C:\Codes\TheThird\Cube-LUT-Loader\src\test\resources\xt3_flog_bt709.cube)";
	std::string rgbPath = R"(C:\Users\daiyb\Desktop\yg\a.rgb)";
	uint8_t* srcRGBImage = nullptr;
	uint8_t* destRGBImage = nullptr;
	std::unique_ptr<CubeLUT> cube;
};
int main() {
	Worker w;
	w.run();

	return 0;
}
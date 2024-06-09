#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <filesystem>

class Generator
{
public:
	void Generate()
	{
		std::vector<float> intakeLobeHeight;
		std::vector<float> exhaustLobeHeight;

		float crankThrow = stroke / 2;

		for (int i = 0; i <= 360; i++)
		{
			float pistonHeight = rodLength + crankThrow - (crankThrow * std::cos(i * 0.0174533) +
				std::sqrt((rodLength * rodLength) - (crankThrow * crankThrow) * (std::sin(i * 0.0174533) * std::sin(i * 0.0174533))));

			float intakePort = pistonHeight - (stroke - (transferPortHeight + transferPortOffset));

			if (intakePort <= 0) { intakeLobeHeight.push_back(0); }
			else if (intakePort >= transferPortHeight) { intakeLobeHeight.push_back(transferPortHeight); }
			else { intakeLobeHeight.push_back(intakePort); }
		}

		for (int i = 0; i <= 360; i++)
		{
			float pistonHeight = rodLength + crankThrow - (crankThrow * std::cos(i * 0.0174533) + 
				std::sqrt((rodLength * rodLength) - (crankThrow * crankThrow) * (std::sin(i * 0.0174533) * std::sin(i * 0.0174533))));

			float exhaustPort = pistonHeight - (stroke - (stroke - exhaustPortOffset));

			if (exhaustPort <= 0) { exhaustLobeHeight.push_back(0); }
			else if (exhaustPort >= exhaustPortHeight) { exhaustLobeHeight.push_back(exhaustPortHeight); }
			else { exhaustLobeHeight.push_back(exhaustPort); }
		}

		std::size_t middleIntake = intakeLobeHeight.size() / 2;
		std::size_t middleExhaust = exhaustLobeHeight.size() / 2;

		std::rotate(intakeLobeHeight.begin(), intakeLobeHeight.begin() + middleIntake, intakeLobeHeight.end());
		std::rotate(exhaustLobeHeight.begin(), exhaustLobeHeight.begin() + middleExhaust, exhaustLobeHeight.end());

		int size = intakeLobeHeight.size();
		for (int i = 0; i < size; i++)
		{
			intakeLobeHeight.push_back(intakeLobeHeight[i]);
		}
		size = exhaustLobeHeight.size();
		for (int i = 0; i < size; i++)
		{
			exhaustLobeHeight.push_back(exhaustLobeHeight[i]);
		}

		std::ofstream file(path + "/2t_cams_" + std::to_string(time(NULL)) + ".txt");
		file << "\n\nINTAKE:\n\n";
		for (int i = 0; i < intakeLobeHeight.size(); i++)
		{
			file << ".add_sample(" << ((float)i / 2) - 180 << " * units.deg, " << intakeLobeHeight[i] << " * units.mm" << ")\n";
			//std::cout << "Angle: " << (i / 2) - 180 << " Intake: " << intakeLobeHeight[i] << "\t\tExhaust: " << exhaustLobeHeight[i] << std::endl;
		}
		file << "\n\nEXHAUST:\n\n";

		for (int i = 0; i < exhaustLobeHeight.size(); i++)
		{
			file << ".add_sample(" << ((float)i / 2) - 180 << " * units.deg, " << exhaustLobeHeight[i] << " * units.mm" << ")\n";
			//std::cout << "Angle: " << (i / 2) - 180 << " Intake: " << intakeLobeHeight[i] << "\t\tExhaust: " << exhaustLobeHeight[i] << std::endl;
		}
		file.close();
	}

	float stroke = 108;
	float rodLength = 180;
	float exhaustPortHeight = 25;
	float exhaustPortOffset = 62;
	float transferPortHeight = 15;
	float transferPortOffset = 6;
	std::string path = "";
};

int main()
{
	std::string inputBuffer;	
	bool quit = false;

	Generator gen;

	gen.Generate();
	while (!quit)
	{
		std::string buffer;
		std::cout << "Stroke: ";
		std::cin >> buffer;
		
		for (int i = 0; i < buffer.size(); i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != '.') { std::cout << "NaN\n"; return -1;}
		}
		gen.stroke = std::stof(buffer);
		buffer.clear();

		if (gen.stroke <= 0) { std::cout << "Stroke can not be zero or negative\n"; return -3; }

		std::cout << "Rod length: ";
		std::cin >> buffer;
		
		for (int i = 0; i < buffer.size(); i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != '.') { std::cout << "NaN\n"; return -1;}
		}
		gen.rodLength = std::stof(buffer);
		buffer.clear();

		if (gen.rodLength <= 0) { std::cout << "Rod length can not be zero or negative\n"; return -3; }

		std::cout << "Exhaust port height: ";
		std::cin >> buffer;
		
		for (int i = 0; i < buffer.size(); i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != '.') { std::cout << "NaN\n"; return -1;}
		}
		gen.exhaustPortHeight = std::stof(buffer);
		buffer.clear();

		if (gen.exhaustPortHeight < 0) { std::cout << "Port height can not be negative\n"; return -3; }

		if (gen.exhaustPortHeight > gen.stroke) { std::cout << "Height can not be larger than stroke\n"; return -1; }

		std::cout << "Exhaust port offset: ";
		std::cin >> buffer;
		
		for (int i = 0; i < buffer.size(); i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != '.') { std::cout << "NaN\n"; return -1; }
		}
		gen.exhaustPortOffset = std::stof(buffer);
		buffer.clear();

		if (gen.exhaustPortOffset < 0) { std::cout << "Port offset can not be negative\n"; return -3; }

		if (gen.exhaustPortOffset > gen.stroke - gen.exhaustPortHeight) { std::cout << "Offset can not be larger than stroke\n"; return -1; }

		std::cout << "Transfer port height: ";
		std::cin >> buffer;
		
		for (int i = 0; i < buffer.size(); i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != '.') { std::cout << "NaN\n"; return -1; }
		}
		gen.transferPortHeight = std::stof(buffer);
		buffer.clear();

		if (gen.transferPortHeight < 0) { std::cout << "Port height can not be negative\n"; return -3; }

		if(gen.transferPortHeight > gen.stroke) { std::cout << "Height can not be larger than stroke\n"; return -1; }

		std::cout << "Transfer port offset: ";
		std::cin >> buffer;
		
		for (int i = 0; i < buffer.size(); i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != '.') { std::cout << "NaN\n"; return -1; }
		}
		gen.transferPortOffset = std::stof(buffer);
		buffer.clear();

		if (gen.transferPortOffset < 0) { std::cout << "Port offset can not be negative\n"; return -3; }

		if (gen.transferPortOffset > gen.stroke - gen.transferPortHeight) { std::cout << "Offset can not be larger than stroke\n"; return -1; }

		std::cout << "Output path: ";
		std::cin >> buffer;
		gen.path = buffer;
		buffer.clear();

		if (!std::filesystem::exists(gen.path)) { std::cout << "Path does not exist\n"; return -2; }

		gen.Generate();

		std::cout << "\n\n";
	}
}
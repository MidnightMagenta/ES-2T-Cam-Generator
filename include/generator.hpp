#ifndef GENERATOR_H
#define GENERATOR_H

#include <CLI11.hpp>
#include <string>

struct GeneratorOptions {
	float stroke = 100.f;
	float rodLength = 200.f;
	float ep_height = 25.f;
	float ep_offset = 60.f;
	float tp_height = 15.f;
	float tp_offset = 10.f;
	int stepCount = 360;

	std::string path = "out.mr";
};

CLI::App *make_generator(CLI::App &app, GeneratorOptions *options);
int generate(CLI::App *app, GeneratorOptions &options);
void calculate(const GeneratorOptions &options, std::vector<float> &intake, std::vector<float> &exhaust);

#endif// !GENERATOR_H

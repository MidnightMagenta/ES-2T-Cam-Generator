#include <MidnightMath/include/constants.hpp>
#include <MidnightMath/include/units.hpp>
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <generator.hpp>

CLI::App *make_generator(CLI::App &app, GeneratorOptions *options) {
	CLI::App *appGenerate = app.add_subcommand(
			"generate", "Generates a camshaft profile, and writes the result into the specified file");
	appGenerate->footer("Generator math developed by ArchangelMotors");
	appGenerate->add_option("--stroke", options->stroke, "Used to specify the stroke of the engine in milimeters");
	appGenerate->add_option("--rod-len", options->rodLength,
							"Used to specify the length of the connecting rod in milimeters");
	appGenerate->add_option("--ep-height", options->ep_height,
							"Used to specify the height (size) of the exhaust port in milimeters");
	appGenerate->add_option(
			"--ep-offset", options->stroke,
			"Used to specify the offset of the exhaust port from the top of the cylinder in milimeters");
	appGenerate->add_option("--tp-height", options->stroke,
							"Used to specify the height (size) of the transfer port in milimeters");
	appGenerate->add_option(
			"--tp-offset", options->stroke,
			"Used to specify the offset of the transfer port from the bottom of the cylinder in milimeters");
	appGenerate->add_option(
			"-o,--out", options->path,
			"Used to specify the path to a file to which the output will be written. If the file doesn't exist, it "
			"will be created. If the file exists, the --overwrite option must be specified for the app to overwrite "
			"the contents of the file, or the --append option for the app to append the result to the end of the file");
	appGenerate->add_option("--steps", options->stepCount,
							"Used to specify the number of samples that will be generated");
	appGenerate->add_flag("--overwrite", "If specified, the file will be overwritten, if the file already exists");
	appGenerate
			->add_flag("--append", "If specified, the generated samples will be appended to the specified file, if the "
								   "file already exists")
			->excludes("--overwrite");

	appGenerate->add_flag("--interactive");

	return appGenerate;
}

int generate(CLI::App *app, GeneratorOptions &options) {
	std::vector<float> intakeLobeHeight;
	std::vector<float> exhaustLobeHeight;

	if (app->count("--interactive")) {
		std::cout << "Stroke: ";
		std::cin >> options.stroke;
		std::cout << "Rod length: ";
		std::cin >> options.rodLength;
		std::cout << "Exhaust port height (size): ";
		std::cin >> options.ep_height;
		std::cout << "Exhaust port offset (from the top of the cylinder): ";
		std::cin >> options.ep_offset;
		std::cout << "Transfer port height (size): ";
		std::cin >> options.tp_height;
		std::cout << "Transfer port offset (from the bottom of the cylinder): ";
		std::cin >> options.tp_offset;
		std::cout << "Step count (number of samples = (360 / stepCount) * 2 + 1): ";
		std::cin >> options.stepCount;
		std::cout << "Output path: ";
		std::cin >> options.path;
	}

	assert((options.stroke > 0, "Stroke must be larger than 0"));
	assert((options.rodLength > 0, "Rod length must be larger than 0"));

	assert((options.ep_height >= 0, "Exhaust port height must be larger or equal to 0"));
	assert((options.ep_height <= options.stroke, "Exhaust port height must not be larger than stroke"));
	assert((options.ep_offset >= 0, "Exhaust port offset must be larger or equal to 0"));
	assert((options.ep_offset <= options.stroke, "Exhaust port offset must not be larger than stroke"));

	assert((options.tp_height >= 0, "Transfer port height must be larger or equal to 0"));
	assert((options.tp_height <= options.stroke, "Transfer port height must not be larger than stroke"));
	assert((options.tp_offset >= 0, "Transfer port offset must be larger or equal to 0"));
	assert((options.tp_offset <= options.stroke - options.tp_height,
			"Exhaust port offset must not be larger than stroke"));

	std::ofstream file;

	if (std::filesystem::exists(options.path)) {
		if (app->count("--interactive")) {
			std::cout << "The specified file already exists. Do you want to overwrite? (Y/N): ";
			while (true) {
				std::string opt;
				std::cin >> opt;
				if (opt == "Y") {
					file.open(options.path, std::ios::trunc);
					break;
				} else if (opt == "N") {
					return 1;
				} else {
					std::cout << "Please specify Y or N: ";
				}
			}
		} else {
			if (app->count("--overwrite")) {
				file.open(options.path, std::ios::trunc);
			} else if (app->count("--append")) {
				file.open(options.path, std::ios::app);
			} else {
				std::cerr << "The file " << options.path
						  << " already exists. If you want to overwrite the file, specify the "
							 "--overwrite flag. If you want to append to the end of the file, specify the --append "
							 "flag.\n";
				return 1;
			}
		}
	} else {
		file.open(options.path, std::ios::trunc);
	}

	if (!file.is_open()) {
		std::cerr << "Could not open file " << options.path << "\n";
		return 1;
	}

	calculate(options, intakeLobeHeight, exhaustLobeHeight);

	float stepSize = 360.f / (options.stepCount * 2);

	file << "function Intake_Lobe_Profile(" << stepSize << " * units.deg)\n\t\tIntake_Lobe_Profile\n";
	for (size_t i = 0; i < intakeLobeHeight.size(); i++) {
		file << "\t\t.add_sample(" << 180.f - i * stepSize << " * units.deg, " << intakeLobeHeight[i]
			 << " * units.mm)\n";
	}

	file << "\nfunction Exhaust_Lobe_Profile(" << stepSize << " * units.deg)\n\t\Exhaust_Lobe_Profile\n";
	for (size_t i = 0; i < exhaustLobeHeight.size(); i++) {
		file << "\t\t.add_sample(" << 180.f - i * stepSize << " * units.deg, " << exhaustLobeHeight[i]
			 << " * units.mm)\n";
	}

	file.close();

	std::cout << "Written " << intakeLobeHeight.size() + exhaustLobeHeight.size() << " samples to " << options.path
			  << "\nIntake samples written: " << intakeLobeHeight.size()
			  << "\nExhaust samples written: " << exhaustLobeHeight.size() << "\n";

	if (app->count("--interactive")) { 
		system("pause");
	}

	return 0;
}

void calculate(const GeneratorOptions &options, std::vector<float> &intake, std::vector<float> &exhaust) {
	const float crankThrow = options.stroke / 2;
	const float stepSize = md_math::tau_f / options.stepCount;

	for (int i = 0; i <= options.stepCount; i++) {
		float a = i * stepSize;
		float sin_a = std::sin(a);
		float cos_a = std::cos(a);

		float pistonHeight = options.rodLength + crankThrow -
							 (crankThrow * cos_a + std::sqrt((options.rodLength * options.rodLength) -
															 (crankThrow * crankThrow) * (sin_a * sin_a)));

		//calculate the intake port
		float intakePort = pistonHeight - (options.stroke - (options.tp_height + options.tp_offset));

		if (intakePort <= 0) {
			intake.push_back(0);
		} else if (intakePort >= options.tp_height) {
			intake.push_back(options.tp_height);
		} else {
			intake.push_back(intakePort);
		}

		//calculate the exhaust port
		float exhaustPort = pistonHeight - (options.stroke - (options.stroke - options.ep_offset));

		if (exhaustPort <= 0) {
			exhaust.push_back(0);
		} else if (exhaustPort >= options.ep_height) {
			exhaust.push_back(options.ep_height);
		} else {
			exhaust.push_back(exhaustPort);
		}
	}

	std::rotate(intake.begin(), intake.begin() + (intake.size() / 2), intake.end());
	std::rotate(exhaust.begin(), exhaust.begin() + (exhaust.size() / 2), exhaust.end());

	intake.insert(intake.end(), intake.begin(), intake.end() - 1);
	exhaust.insert(exhaust.end(), exhaust.begin(), exhaust.end() - 1);
}
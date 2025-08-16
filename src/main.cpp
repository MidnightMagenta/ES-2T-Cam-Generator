#include <CLI11.hpp>
#include <generator.hpp>

int main(int argc, char **argv) {
	CLI::App app;
	app.require_subcommand(1);

	GeneratorOptions genOpts;
	CLI::App *genApp = make_generator(app, &genOpts);

	CLI11_PARSE(app, argc, argv);
	if (genApp->parsed()) { return generate(genApp, genOpts); }
}
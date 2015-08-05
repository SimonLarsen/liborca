#include <iostream>
#include <fstream>
#include "StdOutput.hpp"
#include <tclap/CmdLine.h>
#include <graph/Graph.hpp>
#include <graph/Algorithms.hpp>
#include <graph/GraphReader.hpp>
#include <orca/Orca.hpp>

int main(int argc, const char **argv) {
	TCLAP::CmdLine cmd("Count graphlet degree vectors.", ' ', "0.1");
	StdOutput std_output("orca", "Simon Larsen <simonhffh@gmail.com>");
	cmd.setOutput(&std_output);

	TCLAP::ValueArg<int> graphletSizeArg("s", "size", "Graphlet size. 2-5 supported. Default: 4", false, 4, "size", cmd);
	TCLAP::UnlabeledValueArg<std::string> graphArg("graph", "Path to graph file", true, "", "GRAPH", cmd);
	TCLAP::UnlabeledValueArg<std::string> outputArg("output", "Output file", true, "", "FILE", cmd);

	cmd.parse(argc, argv);

	int graphlet_size = graphletSizeArg.getValue();

	graph::Graph<> g;
	graph::readGraph(graphArg.getValue(), g);
	std::vector<std::pair<size_t,size_t>> edges;

	graph::get_edges(g, edges);

	orca::Orca orca(g.vertexCount(), edges, graphlet_size);
	orca.compute();

	const auto &orbits = orca.getOrbits();

	std::ofstream file(outputArg.getValue());
	for(auto it1 = orbits.begin1(); it1 != orbits.end1(); ++it1) {
		for(auto it = it1.begin(); it != it1.end(); ++it) {
			file << *it << " ";
		}
		file << "\n";
	}
	file.close();

	return 0;
}
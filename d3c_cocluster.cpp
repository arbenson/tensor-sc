#include "common.hpp"
#include "community.hpp"
#include "cut_tools.hpp"
#include "file_io.hpp"
#include "network.hpp"

#include <iostream>

int main(int argc, char **argv) {
    if (argc == 1) {
	PrintHelp();
	return 0;
    }
    int network = atoi(argv[1]);
    int num_comms = 3;
    int triple_type = D3CS;
    int cut_type = D3C_COND;
    std::string name = NetworkStr(network);

    {
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_cocluster_u");

	std::string file = "data/" + name + "_u";
	std::vector<double> vals = ReadVector<double>(file);
	Vector<double> u_vals(vals.size());
	for (int i = 0; i < vals.size(); ++i) {
	    u_vals(i) = vals[i];
	}

	std::vector<int> order = GetSortedOrder(u_vals);
	WriteVector(order, name + "_cocluster_u_order.txt");
	//std::vector<int> cut = cutter.GetCut(order);
    }

    {
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_cocluster_v");

	std::string file = "data/" + name + "_v";
	std::vector<double> vals = ReadVector<double>(file);
	Vector<double> v_vals(vals.size());
	for (int i = 0; i < vals.size(); ++i) {
	    v_vals(i) = vals[i];
	}

	std::vector<int> order = GetSortedOrder(v_vals);
	WriteVector(order, name + "_cocluster_v_order.txt");
	//std::vector<int> cut = cutter.GetCut(order);
    }

    std::cout << "END ---------" << std::endl;
}

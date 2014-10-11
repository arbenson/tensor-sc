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
    std::string name = NetworkStr(network);
    std::string order_dir = "orders/";

    int cut_type = D3C_COND;
    std::cout << "TENSOR ----";
    {
	std::string alg = AlgStr(TENSOR);
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_" + alg);

	std::string file = order_dir + name + "_" + alg + "_order.txt";
	std::vector<int> order = ReadVector<int>(file);
	std::vector<int> cut = cutter.GetCut(order);
    }
    std::cout << "DIRLAP ----";
    {
	std::string alg = AlgStr(DIRLAP);
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_" + alg);

	std::string file = order_dir + name + "_" + alg + "_order.txt";
	std::vector<int> order = ReadVector<int>(file);
	std::vector<int> cut = cutter.GetCut(order);
    }
    std::cout << "LAP ----";
    {
	std::string alg = AlgStr(LAP);
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_" + alg);

	std::string file = order_dir + name + "_" + alg + "_order.txt";
	std::vector<int> order = ReadVector<int>(file);
	std::vector<int> cut = cutter.GetCut(order);
    }
    std::cout << "ALAP ----";
    {
	std::string alg = AlgStr(ASYMMETRIC_LAP);
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_" + alg);

	std::string file = order_dir + name + "_" + alg + "_order.txt";
	std::vector<int> order = ReadVector<int>(file);
	std::vector<int> cut = cutter.GetCut(order);
    }
    std::cout << "RANDOM ----";
    {
	std::string alg = AlgStr(RANDOM);
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_" + alg);

	std::string file = order_dir + name + "_" + alg + "_order.txt";
	std::vector<int> order = ReadVector<int>(file);
	std::vector<int> cut = cutter.GetCut(order);
    }
    std::cout << "Co-U ----";
    {
	std::string alg = AlgStr(CO_U);
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_" + alg);

	std::string file = order_dir + name + "_" + alg + "_order.txt";
	std::vector<int> order = ReadVector<int>(file);
	std::vector<int> cut = cutter.GetCut(order);
    }
    std::cout << "Co-V ----";
    {
	std::string alg = AlgStr(CO_V);
	Network net = GetNetwork(network, triple_type);
	Cutter cutter(net, 0, cut_type, name + "_" + alg);

	std::string file = order_dir + name + "_" + alg + "_order.txt";
	std::vector<int> order = ReadVector<int>(file);
	std::vector<int> cut = cutter.GetCut(order);
    }

    std::cout << "END ---------" << std::endl;
}

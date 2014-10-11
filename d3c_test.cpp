#include "common.hpp"
#include "community.hpp"
#include "network.hpp"

#include <iostream>

int main(int argc, char **argv) {
    if (argc == 1) {
	PrintHelp();
	return 0;
    }
    int network = atoi(argv[1]);
    int num_comms = 2;
    int triple_type = D3CS;
    int cut_type = D3C_COND;
    std::string name = NetworkStr(network);

    std::cout << "TENSOR ---------" << std::endl;
    {
	Network net = GetNetwork(network, triple_type);
	CommDetection comm(net, num_comms, TENSOR, cut_type, name);
	comm.Run();
    }
    std::cout << "DIRLAP ---------" << std::endl;
    {
	Network net = GetNetwork(network, triple_type);
	CommDetection comm(net, num_comms, DIRLAP, cut_type, name);
	comm.Run();
    }
    std::cout << "LAP ---------" << std::endl;
    {
	Network net = GetNetwork(network, triple_type);
	CommDetection comm(net, num_comms, LAP, cut_type, name);
	comm.Run();
    }
    std::cout << "RANDOM ---------" << std::endl;
    {
	Network net = GetNetwork(network, triple_type);
	CommDetection comm(net, num_comms, RANDOM, cut_type, name);
	comm.Run();
    }
    std::cout << "ALAP ---------" << std::endl;
    {
	Network net = GetNetwork(network, triple_type);
	CommDetection comm(net, num_comms, ASYMMETRIC_LAP, cut_type, name);
	comm.Run();
    }
    std::cout << "END ---------" << std::endl;
}

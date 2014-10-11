#include "common.hpp"
#include "community.hpp"
#include "network.hpp"

#include <iostream>


int main(int argc, char **argv) {
    int num_comms = 2;
    int triple_type = D3CS_NOBACK;
    int cut_type = BIPARTITE;
    std::string name = "tripartite_flow";

    std::cout << "DIRLAP ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, DIRLAP, cut_type, name);
	comm.Run();
    }
    std::cout << "D3C_ONLY ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, D3C_ONLY_DIRLAP, cut_type, name);
	comm.Run();
    }
    std::cout << "TENSOR ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, TENSOR, cut_type, name);
	comm.Run();
    }
    std::cout << "END ---------" << std::endl;
}

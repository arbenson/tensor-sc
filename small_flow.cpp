#include "common.hpp"
#include "community.hpp"
#include "network.hpp"

#include <iostream>


int main(int argc, char **argv) {
    int num_comms = 2;
    int triple_type = D3CS;
    int cut_type = D3C_COND;
    std::string name = "flow";

    std::cout << "DIRLAP ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, DIRLAP, cut_type, name);
	comm.Run();
    }
    std::cout << "LAP ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, LAP, cut_type, name);
	comm.Run();
    }
    std::cout << "MULTILINEAR ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, MULTILINEAR, cut_type, name);
	comm.Run();
    }
    std::cout << "END ---------" << std::endl;
}

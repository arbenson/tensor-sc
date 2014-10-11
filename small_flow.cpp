#include "common.hpp"
#include "community.hpp"
#include "file_io.hpp"
#include "network.hpp"

#include <iostream>


int main(int argc, char **argv) {
    int num_comms = 2;
    int triple_type = D3CS;
    int cut_type = D3C_COND;
    std::string name = "small_flow";

    std::cout << "DIRLAP ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, DIRLAP, cut_type, name);
	comm.Run();
	std::vector<int> comms = comm.Communities();
	WriteVector(comms, "small_dl_comms.txt");
    }
    std::cout << "TENSOR ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, TENSOR, cut_type, name);
	comm.Run();
	std::vector<int> comms = comm.Communities();
	WriteVector(comms, "small_tsc_comms.txt");
    }
    std::cout << "END ---------" << std::endl;
}

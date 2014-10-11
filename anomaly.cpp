#include "common.hpp"
#include "community.hpp"
#include "file_io.hpp"
#include "network.hpp"

#include <iostream>


int main(int argc, char **argv) {
    int num_comms = 2;
    int triple_type = D3CS_NOBACK;
    int cut_type = D3C_NOBACK_COND;
    std::string name = "anomaly";

    std::cout << "DIRLAP ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, DIRLAP, cut_type, name);
	comm.Run();
	std::vector<int> comms = comm.Communities();
	WriteVector(comms, "anomaly_dl_comms.txt");
    }
    std::cout << "D3C NO BACK DIRLAP ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, D3C_ONLY_NOBACK_DIRLAP, cut_type, name);
	comm.Run();
	std::vector<int> comms = comm.Communities();
	WriteVector(comms, "anomaly_subdl_comms.txt");
    }
    std::cout << "MULTILINEAR ---------" << std::endl;
    {
	Network net = Network("data", name, triple_type);
	CommDetection comm(net, num_comms, MULTILINEAR, cut_type, name);
	comm.Run();
	std::vector<int> comms = comm.Communities();
	WriteVector(comms, "anomaly_msc_comms.txt");
    }
    std::cout << "END ---------" << std::endl;
}

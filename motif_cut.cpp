#include "common.hpp"
#include "community.hpp"
#include "file_io.hpp"
#include "network.hpp"

#include <iostream>

int main(int argc, char **argv) {
    if (argc < 3) {
	return -1;
    }

    int num_comms = 2;
    int cut_type = D3C_COND;
    
    std::string network_path(argv[1]);
    std::string trips_path(argv[2]);
    std::string name(argv[3]);

    Network net(network_path, trips_path);
    Cutter cutter(net, TENSOR, cut_type, name);
    std::vector<int> order = cutter.GetOrder();
    WriteVector(order, name + "-TSC-order.txt");
}

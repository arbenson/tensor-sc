tensor-sc
=========

Tensor Spectral Clustering.


Getting started
=========

What you need:
* Compiler supporting C++11
* [SNAP](http://snap.stanford.edu/snap/index.html) for C++

Edit the Makefile for your compiler and the location of SNAP.


Layered flow example
=========

First, build the example:

    make layered_flow

Run the algorithms:

    ./layered_flow

Look at the communities for tensor spectral clustering, the directed Laplacian, and the subgraph directed Laplacian:

    cat layered_flow_tsc_comms.txt
    cat layered_flow_dl_comms.txt
    cat layered_flow_subdl_comms.txt


Big network directed 3-cycle cut example
=========

We will now go through the steps for running the directed 3-cycle cut algorithms on wiki-Vote, one of the SNAP networks.
First, download the data:

    wget http://snap.stanford.edu/data/wiki-Vote.txt.gz
    gunzip wiki-Vote.txt.gz
    mv wiki-Vote.txt data/

Now, filter the graph:

    bash scripts/filter_data.sh wiki-Vote

Build the partitioning code:

    make d3c_test

Run the algorithm:

    ./d3c_test 0

Visualize the number of directed 3-cycles cut as a function of the smaller partition size:

    python scripts/d3c_plot.py wiki-Vote-filter num_cut

Scripts for processing networks and counting third-order moments.

Re-index the nodes so that they are 0-index-based.  This is necessary for the algorithm:

    python re_index.py ../data/Email-Enron.txt   # overwrites Email-Enron.txt

Save the maximal bi-connected component of a graph:

    python biconn_component.py ../data/Email-Enron.txt  # creates Email-Enron-bicon.txt

Enumerate all of the directed 3 cycles in a graph:

    python dir3cycles.py ../data/Email-Enron-bicon.txt  # creates Email-Enron-bicon-dir3cycles.txt

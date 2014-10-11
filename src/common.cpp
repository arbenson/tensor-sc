#include "common.hpp"

#include <algorithm>
#include <cassert>
#include <map>
#include <stdexcept>
#include <vector>
#include <iostream>


std::vector<int> Range(int start, int end) {
    if (start >= end) {
        return std::vector<int>();
    }
    std::vector<int> range(end - start);
    for (int i = 0; i < range.size(); ++i) {
        range[i] = start + i;
    }
    return range;
}

std::vector<int> Range(int end) {
    if (end < 0) {
        throw std::logic_error("range must end at a positive number");
    }
    return Range(0, end);
}

std::string AlgStr(int algorithm) {
    switch (algorithm) {
    case TENSOR:
        return "tsc";
    case DIRLAP:
        return "dl";
    case LAP:
        return "lap";
    case ASYMMETRIC_LAP:
        return "alap";
    case RANDOM:
        return "random";
    case CO_U:
        return "cocluster_u";
    case CO_V:
        return "cocluster_v";
    case D3C_ONLY_DIRLAP:
        return "d3c-only-dirlap";
    case D3C_ONLY_RECIP_DIRLAP:
        return "d3c-only-recip-dirlap";
    case D3C_ONLY_NOBACK_DIRLAP:
        return "d3c-only-noback-dirlap";
    default:
        throw std::logic_error("Unknown algorithm");
    }
}

std::string NetworkStr(int network) {
    switch (network) {
    case WIKI_VOTE:
        return "wiki-Vote-filter";
    case EMAIL_EU:
        return "email-EuAll-filter";
    case WIKI_TALK:
        return "wiki-Talk-filter";
    case SOC_EPINIONS:
        return "soc-Epinions1-filter";
    case SLASHDOT:
        return "soc-Slashdot0811-filter";
    case AS_CAIDA:
        return "as-caida20071105-filter";
    case AMAZON:
        return "amazon0312-filter";
    case TWITTER:
        return "twitter_combined-filter";
    case WIKI_RFA:
        return "wiki-RfA-net-filter";
    case CIT_HEP_PH:
        return "cit-HepPh-filter";
    case WEB_STANFORD:
        return "web-Stanford-filter";
    case EMAIL_ENRON:
        return "email-Enron-filter";
    case DBLP:
        return "com-dblp.ungraph-filter";
    default:
        throw std::logic_error("Unknown network");
    }
}

void PrintHelp() {
    std::cout << "Usage: ./test network" << std::endl
              << "-----" << std::endl;
    for (int i = 0; i < kNumNetworks; ++i) {
	std::cout << NetworkStr(i) << ": " << i << std::endl;
    }
}

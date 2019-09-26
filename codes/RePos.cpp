#include "ScaffInfo.h"
#include <iostream>
#include <fstream>

int main(int argc , char ** argv)
{
    if( argc != 3 )
    {
        std::cerr<<"Usage "<<argv[0]<<" scaff_infos min_n "<<std::endl;
        return 0 ;
    }

    std::string si(argv[1]);
    std::ifstream si_ifs(si);
    if( ! si_ifs.is_open() )
    {
        std::cerr<<"Failed to open "<<si<<" for read !!! exit ..."<<std::endl;
        return 0 ;
    }

    int min_n = std::stoi(std::string(argv[2]));
    std::cerr<<" Load scaff_infos now ... "<<std::endl;
    BGIQD::stLFR::ScaffInfoHelper helper ;
    helper.LoadAllScaff(si_ifs);
    si_ifs.close() ;
    // re-set gap size

    for ( auto & pair : helper.all_scaff )
    {
        auto & a_scaff = pair.second.a_scaff ;
        for( auto & contig_detail : a_scaff )
        {
            if( contig_detail.gap_size < min_n )
                contig_detail.gap_size = min_n ;
        }
    }
    helper.FormatAllIndex();
    helper.FormatAllStartPos();
    helper.PrintAllScaff(std::cout);
    return 0 ;
}

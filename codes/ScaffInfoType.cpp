#include "ScaffInfoType.h"

#include <cassert>
#include <sstream>
namespace BGIQD {
    namespace stLFR {

        int Type2Int(const std::string & type )
        {
            if ( type == "UnKnow" ) return 0 ;
            if ( type == "UnMatch" ) return 1 ;
            if ( type == "WrongRef" ) return 2 ;
            if ( type == "InRef" ) return 3 ;
            if ( type == "WrongOrder" ) return 4 ;
            if ( type == "WrongOrientation" ) return 5 ;
            if ( type == "OOCorrect" ) return 6 ;
            assert(0);
        }

        bool Later(const std::string type1 , const std::string & type2)
        {
            return Type2Int(type1) < Type2Int(type2) ;
        }

        void ContigDetail::InitFromString(const std::string &line)
        {
            std::istringstream ist(line);
            char pos ;
            ist>>type
                >>step_num
                >>step_num_1
                >>ref_index
                >>ref_name
                >>ref_pos
                >>ref_pos_end ;
            ist >>contig_id
                >>pos
                >>gap_size
                >>contig_len
                >>start_pos
                >>scaff_index
                >>scaff_id;
            orientation = ( pos == '+' );
            std::string extra_item ;
        }

        void ScaffInfoHelper::LoadAllScaff(  std::istream &ist )
        {
            int id ;
            std::string line ;
            while( ! std::getline( ist , line).eof() )
            {
                if( line[0] == '>' )
                {
                    sscanf(line.c_str(),">scaffold_%d",&id);
                    all_scaff[id].scaff_id = id ;
                }
                else
                {
                    ContigDetail tmp ;
                    tmp.InitFromString(line);
                    all_scaff[id].a_scaff.push_back(tmp);
                    contig_indexs[tmp.contig_id] = ContigIndex{ id ,int( all_scaff[id].a_scaff.size() -1) } ;
                }
            }
        }

        ContigDetail & ScaffInfoHelper::GetContig( unsigned int contig)
        {
            try
            {
                auto index = contig_indexs.at(contig);
                return all_scaff.at(index.scaffold_id).a_scaff.at(index.contig_index);
            }
            catch(...)
            {
                assert(0);
            }
            assert(0);
            static ContigDetail invalid ;
            return invalid ;;
        }
    }
}

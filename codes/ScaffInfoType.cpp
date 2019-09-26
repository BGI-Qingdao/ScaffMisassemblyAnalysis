#include "ScaffInfoType.h"

#include <cassert>
#include <sstream>
namespace BGIQD {
    namespace stLFR {

        //const std::string ContigDetail::ONT_FILL = "ONT_FILL" ;

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

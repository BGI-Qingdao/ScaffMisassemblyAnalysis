#ifndef __STLFR_SCAFFINFO_H__
#define __STLFR_SCAFFINFO_H__

#include <vector>
#include <iostream>
#include <map>

namespace BGIQD {
    namespace stLFR {

        // detect which contig_error_type happened later ;
        bool Later(const std::string type1 , const std::string & type2);

        struct ContigDetail
        {
            std::string type ;
            std::string step_num ;
            std::string step_num_1 ;
            std::string ref_index ;
            std::string ref_name ;
            std::string ref_pos ;
            std::string ref_pos_end ;
            ///////////////
            unsigned int contig_id ;
            bool orientation ;
            int gap_size ;
            int contig_len ;
            int start_pos ;
            int scaff_index ;
            int scaff_id ;
            void InitFromString(const std::string & line) ;

        };

        struct ScaffInfo
        {
            int scaff_id ;
            std::vector<ContigDetail> a_scaff ;
        };

        struct ScaffInfoHelper
        {

            void LoadAllScaff( std::istream & ist ) ;

            std::map<int , ScaffInfo> all_scaff ;

            struct ContigIndex
            {
                int scaffold_id ;
                int contig_index ;
            } ;

            std::map<unsigned int , ContigIndex>  contig_indexs ;

            ContigDetail & GetContig( unsigned int contig_id ) ;
        };
    }
}
#endif

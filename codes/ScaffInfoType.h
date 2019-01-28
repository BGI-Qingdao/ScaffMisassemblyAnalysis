#ifndef __STLFR_SCAFFINFO_H__
#define __STLFR_SCAFFINFO_H__

#include <vector>
#include <iostream>
#include <map>

namespace BGIQD {
    namespace stLFR {

        struct ContigDetail
        {
             ///////////////
            //InRef   0       1       1     NC_000004.12_Homo_sapiens_chromosome_4__GRCh38.p12_Primary_Assembly     51815107        193363  -       1       27101   1       1       1
            std::string type ;
            std::string step_num ;
            std::string step_num_1 ;
            std::string ref_index ;
            std::string ref_name ;
            std::string ref_pos ;
            ///////////////
            unsigned int contig_id ;
            bool orientation ;
            int gap_size ;
            int contig_len ;
            int start_pos ;
            int scaff_index ;
            int scaff_id ;
            void InitFromString(const std::string & line) ;

            //std::string ToString() const ;

            //std::map<std::string , std::string> extra ;

            //static const std::string ONT_FILL  ;
        };

        struct ScaffInfo
        {
            int scaff_id ;
            std::vector<ContigDetail> a_scaff ;

            //void PrintScaff( std::ostream & ost ) const;

            //void FormatIndex();
            //void FormatStartPos();
        };

        struct ScaffInfoHelper
        {
            //void PrintAllScaff( std::ostream & ost ) const ;

            void LoadAllScaff( std::istream & ist ) ;

            std::map<int , ScaffInfo> all_scaff ;

            struct ContigIndex
            {
                int scaffold_id ;
                int contig_index ;
            } ;

            std::map<unsigned int , ContigIndex>  contig_indexs ;

            ContigDetail & GetContig( unsigned int contig_id ) ;
            //void FormatAllIndex()
            //{
            //    for( auto & pair : all_scaff )
            //        pair.second.FormatIndex();
            //}
            //void FormatAllStartPos()
            //{
            //    for( auto & pair : all_scaff )
            //        pair.second.FormatStartPos();
            //}
        };
    }
}
#endif

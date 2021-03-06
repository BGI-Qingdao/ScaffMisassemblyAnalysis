#ifndef __STLFR_SCAFFINFO_H__
#define __STLFR_SCAFFINFO_H__

#include <vector>
#include <iostream>
#include <map>

namespace BGIQD {
    namespace stLFR {

        struct ContigDetail
        {
            unsigned int contig_id ;
            bool orientation ;
            int gap_size ;
            int contig_len ;
            int start_pos ;
            int scaff_index ;
            int scaff_id ;
            void InitFromString(const std::string & line) ;

            std::string ToString() const ;

            std::map<std::string , std::string> extra ;

            enum Type
            {
                UnKnow = 0 ,
                UnMatch = 1 ,
                WrongRef = 2 ,
                InRef = 3 ,
                WrongOrder = 4 ,
                WrongOrientation = 5 ,
                SeedContigRepeat = 6 ,
                OOCorrect = 7
            } type ;

            std::string ref ;
            int ref_start ;
            int ref_end ;
            int ref_index ;
            int step ;
            int step_1;
        };

        struct ScaffInfo
        {
            bool positive ;
            int scaff_id ;
            std::vector<ContigDetail> a_scaff ;

            void PrintScaff( std::ostream & ost ) const;

            void FormatIndex();
            void FormatStartPos();
        };

        struct ScaffInfoHelper
        {
            void PrintAllScaff( std::ostream & ost ) const ;

            void LoadAllScaff( std::istream & ist ) ;

            std::map<int , ScaffInfo> all_scaff ;

            struct ContigIndex
            {
                int scaffold_id ;
                int contig_index ;
            } ;

            std::map<unsigned int , ContigIndex>  contig_indexs ;

            ContigDetail & GetContig( unsigned int contig_id ) ;
            void FormatAllIndex()
            {
                for( auto & pair : all_scaff )
                    pair.second.FormatIndex();
            }
            void FormatAllStartPos()
            {
                for( auto & pair : all_scaff )
                    pair.second.FormatStartPos();
            }
        };
    }
}
#endif 

#include <string>

namespace BGIQD {
    namespace DEBUG {

        struct BreakArea
        {
            //
            // Zone 1 , 13 columns
            // 
            int scaff_id ;
            int start_pos ;
            int end_pos ;

            long prev_contig ;
            std::string prev_contig_type ;
            char prev_o ;
            int prev_ref_start_pos;
            int next_ref_start_pos;

            long next_contig ;
            std::string next_contig_type ;
            char next_o ;
            int prev_ref_end_pos;
            int next_ref_end_pos;

            std::string  prev_ref_name ;
            std::string  next_ref_name ;
            //
            // Zone 2 , 5 columns
            //
            char is_N ; 
            int  break_size_scaff ;
            std::string  break_size_ref ;
            std::string  size_diff ;
            std::string break_type;
            long break_contig ;
            std::string quast_missassembly_info ;

            // true means parse correct
            // false means parse error , maybe this is a contig break
            bool InitFromStr_v1( const std::string & line ) ;

            std::string ToString_v4 () const ;

        };
    }
}

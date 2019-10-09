#include "Break.h"
#include "stringtools.h"
#include <sstream>

namespace BGIQD{
    namespace DEBUG {

        bool BreakArea::InitFromStr_v1( const std::string & line )
        {
            auto items = BGIQD::STRING::split(line,'\t');
            if ( items.size() < 12 )
                return false ;
            auto sids = BGIQD::STRING::split(items[0],"_");
            if(sids[0] != "scaffold" )
                return false ;
            scaff_id = std::stoi(sids[1]);
            start_pos = std::stoi(items[1]);
            end_pos = std::stoi(items[2]);
            prev_o = items[3][0];
            next_o = items[4][0];
            prev_ref_start_pos= std::stoi(items[5]);
            prev_ref_end_pos= std::stoi(items[6]);
            prev_ref_name=items[7];
            next_ref_start_pos= std::stoi(items[8]);
            next_ref_end_pos= std::stoi(items[9]);
            prev_ref_name=items[10];
            quast_missassembly_info = items[11] ;
            return true ;
        }


        std::string BreakArea::ToString_v4() const 
        {
            std::ostringstream ost ;
            ost<<"scaffold_"<<scaff_id<<'\t'
                <<start_pos<<'\t'
                <<end_pos<<'\t';

            ost<<prev_contig<<'\t'
                <<prev_contig_type<<'\t'
                <<prev_o<<'\t'
                <<prev_ref_start_pos<<'\t'
                <<prev_ref_end_pos<<'\t';

            ost<<next_contig<<'\t'
                <<next_contig_type<<'\t'
                <<next_o<<'\t'
                <<next_ref_start_pos<<'\t'
                <<next_ref_end_pos<<'\t';

            // V2
            ost<<is_N<<'\t'
                <<break_size_scaff<<'\t'
                <<break_size_ref<<'\t'
                <<size_diff<<'\t'
                <<break_contig<<'\t'
                <<break_type<<'\t'
                <<quast_missassembly_info;
            return ost.str();
        }
    }
}

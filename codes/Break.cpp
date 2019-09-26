#include "Break.h"
#include "stringtools.h"
#include <sstream>

namespace BGIQD{
    namespace DEBUG {

        bool BreakArea::InitFromStr_v1( const std::string & line )
        {
            auto items = BGIQD::STRING::split(line,'\t');
            if ( items.size() < 10 )
                return false ;
            auto sids = BGIQD::STRING::split(items[0],"_");
            if(sids[0] != "scaffold" )
                return false ;
            scaff_id = std::stoi(sids[1]);
            start_pos = std::stoi(items[1]);
            end_pos = std::stoi(items[2]);
            prev_o = items[3][0];
            end_o = items[4][0];
            prev_ref_start_pos= std::stoi(items[5]);
            prev_ref_end_pos= std::stoi(items[6]);
            next_ref_start_pos= std::stoi(items[7]);
            next_ref_end_pos= std::stoi(items[8]);
            quast_missassembly_info = items[9] ;
            return true ;
        }


        std::string BreakArea::ToString_v1() const 
        {
            std::ostringstream ost ;
            ost<<"scaffold_"<<scaff_id<<'\t'
                <<start_pos<<'\t'
                <<end_pos<<'\t'
                <<prev_o<<'\t'
                <<end_o<<'\t'
                <<prev_ref_start_pos<<'\t'
                <<prev_ref_end_pos<<'\t'
                <<next_ref_start_pos<<'\t'
                <<next_ref_end_pos<<'\t'
                <<quast_missassembly_info;
            return ost.str();
        }

        bool BreakArea::InitFromStr_v2( const std::string & line )
        {
            auto items = BGIQD::STRING::split(line);
            if ( items.size() < 13 )
                return false ;
            auto sids = BGIQD::STRING::split(items[0],"_");
            if(sids[0] != "scaffold" )
                return false ;
            scaff_id = std::stoi(sids[1]);
            start_pos = std::stoi(items[1]);
            end_pos = std::stoi(items[2]);
            prev_o = items[3][0];
            end_o = items[4][0];
            prev_ref_start_pos= std::stoi(items[5]);
            prev_ref_end_pos= std::stoi(items[6]);
            next_ref_start_pos= std::stoi(items[7]);
            next_ref_end_pos= std::stoi(items[8]);
            // V2
            is_N = items[9][0] ;
            break_contig = std::stoi(items[10]);
            prev_contig = std::stoi(items[11]);
            next_contig = std::stoi(items[12]);
            return true ;
        }

        std::string BreakArea::ToString_v2() const 
        {
            std::ostringstream ost ;
            ost<<"scaffold_"<<scaff_id<<'\t'
                <<start_pos<<'\t'
                <<end_pos<<'\t'
                <<prev_o<<'\t'
                <<end_o<<'\t'
                <<prev_ref_start_pos<<'\t'
                <<prev_ref_end_pos<<'\t'
                <<next_ref_start_pos<<'\t'
                <<next_ref_end_pos;
            // V2
            ost<<'\t'<<is_N<<'\t'
                <<break_contig<<'\t'
                <<prev_contig<<'\t'
                <<next_contig;
            return ost.str();
        }

        bool BreakArea::InitFromStr_v3( const std::string & line )
        {
            auto items = BGIQD::STRING::split(line);
            if ( items.size() < 15 )
                return false ;
            auto sids = BGIQD::STRING::split(items[0],"_");
            if(sids[0] != "scaffold" )
                return false ;
            scaff_id = std::stoi(sids[1]);
            start_pos = std::stoi(items[1]);
            end_pos = std::stoi(items[2]);
            prev_o = items[3][0];
            end_o = items[4][0];
            prev_ref_start_pos= std::stoi(items[5]);
            prev_ref_end_pos= std::stoi(items[6]);
            next_ref_start_pos= std::stoi(items[7]);
            next_ref_end_pos= std::stoi(items[8]);

            // V2
            is_N = items[9][0] ;
            break_contig = std::stoi(items[10]);
            prev_contig = std::stoi(items[11]);
            next_contig = std::stoi(items[12]);

            // V3
            break_contig_step   = items[13];
            break_contig_step_1 = items[14];
            return true ;
        }

        std::string BreakArea::ToString_v3() const 
        {
            std::ostringstream ost ;
            ost<<"scaffold_"<<scaff_id<<'\t'
                <<start_pos<<'\t'
                <<end_pos<<'\t'
                <<prev_o<<'\t'
                <<end_o<<'\t'
                <<prev_ref_start_pos<<'\t'
                <<prev_ref_end_pos<<'\t'
                <<next_ref_start_pos<<'\t'
                <<next_ref_end_pos;

            // V2
            ost<<'\t'<<is_N<<'\t'
                <<break_contig<<'\t'
                <<prev_contig<<'\t'
                <<next_contig;

            // V3
            ost<<'\t'<<break_contig_step<<'\t'
               <<break_contig_step_1;
            return ost.str();
        }

        bool BreakArea::InitFromStr_v4( const std::string & line )
        {
            auto items = BGIQD::STRING::split(line);
            if ( items.size() < 17 )
                return false ;
            auto sids = BGIQD::STRING::split(items[0],"_");
            if(sids[0] != "scaffold" )
                return false ;
            scaff_id = std::stoi(sids[1]);
            start_pos = std::stoi(items[1]);
            end_pos = std::stoi(items[2]);
            prev_o = items[3][0];
            end_o = items[4][0];
            prev_ref_start_pos= std::stoi(items[5]);
            prev_ref_end_pos= std::stoi(items[6]);
            next_ref_start_pos= std::stoi(items[7]);
            next_ref_end_pos= std::stoi(items[8]);

            // V2
            is_N = items[9][0] ;
            break_contig = std::stoi(items[10]);
            prev_contig = std::stoi(items[11]);
            next_contig = std::stoi(items[12]);

            // V3
            break_contig_step   = items[13];
            break_contig_step_1 = items[14];

            //V4 
            detect_type = ParseFromStr(items[15]);
            n_size = std::stoi(items[16]);

            return true ;
        }

        std::string BreakArea::ToString_v4() const 
        {
            std::ostringstream ost ;
            ost<<"scaffold_"<<scaff_id<<'\t'
                <<start_pos<<'\t'
                <<end_pos<<'\t'
                <<prev_o<<'\t'
                <<end_o<<'\t'
                <<prev_ref_start_pos<<'\t'
                <<prev_ref_end_pos<<'\t'
                <<next_ref_start_pos<<'\t'
                <<next_ref_end_pos;

            // V2
            ost<<'\t'<<is_N<<'\t'
                <<break_contig<<'\t'
                <<prev_contig<<'\t'
                <<next_contig;

            // V3
            ost<<'\t'<<break_contig_step<<'\t'
               <<break_contig_step_1;

            //V4
            ost<<'\t'<<BreakType2Str(detect_type)<<'\t'
               <<n_size;
            return ost.str();
        }
    }
}

#include "ScaffInfoType.h"
#include "Break.h"
#include "freq.h"
#include <iostream>
#include <fstream>

typedef BGIQD::FREQ::Freq<int> GapFreq ;
GapFreq gap_freq ;
int cross_err = 0 ;
int succ_log=0;
int scaff_log = 0 ;
int total_log = 0 ;
int min_n ;
int min_c ;

int big_nc = 0;
int in_c = 0  ;
int in_n = 0  ;
int in_nc = 0 ;
int in_ncn = 0 ;
int in_ncc = 0 ;
int no_found =0;

bool AssignBreak( const BGIQD::stLFR::ScaffInfoHelper & helper
        , BGIQD::DEBUG::BreakArea & tmp )
{
    auto itr =  helper.all_scaff.find(tmp.scaff_id);
    if( itr == helper.all_scaff.end() )
    {
        no_found++;
        return false ;
    }
    const auto & a_scaff = itr->second.a_scaff ;
    int contig_start = 1 ;
    int contig_end = 0 ;
    int n_start = 0 ;
    int n_end = 0 ;
    bool checked = false ;
    int target_start = tmp.start_pos ;
    int target_end = tmp.end_pos ;

    for( size_t i = 0 ; i < a_scaff.size() ; i++ )
    {
        const auto & a_c = a_scaff.at(i);
        if( a_c.contig_len + a_c.gap_size <= 0 )
        {
            contig_start ++ ;
            continue;
        }

        int n_size = 0 ;
        if( a_c.gap_size >= 0) 
        {
            int gap_size =  a_c.gap_size ;
            if( gap_size < min_n )
                gap_size = min_n ;
            contig_end = contig_start + a_c.contig_len -1  ;
            n_start = contig_start + a_c.contig_len ;
            n_end = contig_start + a_c.contig_len + a_c.gap_size -1;
            n_size = gap_size ;
        }
        else
        {
            contig_end = contig_start + a_c.contig_len -1 +  a_c.gap_size  ;
            n_start =    contig_start + a_c.contig_len + a_c.gap_size ;
            n_end =      contig_start + a_c.contig_len + a_c.gap_size + min_c -1 ;
            n_size = min_c ;
        }

        bool this_checked = false ;
        bool is_n = false ;

        if( target_start <= contig_start && target_end >= contig_end )
        { //break area bigger than contig 
            this_checked = true ;
            is_n = false ;
            big_nc ++ ;
        }
        else if( contig_start-1 <= target_start && contig_end+1 >= target_end )
        {
            this_checked = true ;
            is_n = false ;
            in_c ++ ;
        }
        else if( n_start <= target_start+1 && n_end +1 >= target_end  )
        {
            this_checked = true ;
            is_n = true ;
            in_n ++ ;
        }
        else if (  contig_start -1  <= target_start && n_end +1  >= target_end )
        {
            this_checked = true ;
            is_n = true ;
            in_nc ++;
        }
        else if ( n_start -1 <= target_start && n_end +1 >= target_start && n_end +1 < target_end )
        {
            this_checked = true ;
            is_n = true ;
            in_ncn ++;
        } 
        else if ( contig_start -1 <= target_start && contig_end +1 >= target_start && n_end + 1 <target_end )
        {
            this_checked = true ;
            is_n = false ;
            in_ncc ++;
        } 
        else
        { ; }

        if( this_checked )
        {
            tmp.n_size = n_size ;
            if( is_n )
            {
                tmp.is_N = 'N' ;
                size_t prev = i ;
                size_t next = i < a_scaff.size() -1 ?  i +1 : i ;
                tmp.break_contig = a_c.contig_id ;
                tmp.prev_contig = a_scaff.at(prev).contig_id;
                tmp.next_contig = a_scaff.at(next).contig_id;
                tmp.break_contig_step = a_c.step_num ;
                tmp.break_contig_step_1 = a_c.step_num_1 ;
            }
            else 
            {
                tmp.is_N = '*';
                tmp.break_contig = a_c.contig_id ;
                size_t prev = i > 0 ? i -1 : i;
                size_t next = i < a_scaff.size() -1 ?  i +1 : i ;
                tmp.prev_contig = a_scaff.at(prev).contig_id;
                tmp.next_contig = a_scaff.at(next).contig_id;
                tmp.break_contig_step = a_c.step_num ;
                tmp.break_contig_step_1 = a_c.step_num_1 ;
            }
            if( this_checked && checked )
            {
                cross_err ++ ;
                break ;
            }
            checked = true ;
        }
        contig_start = n_end + 1 ;
    }
    if(checked )
        return true ;
    return false;
}

void UpdateBreakType( BGIQD::DEBUG::BreakArea & tmp )
{
    if( tmp.break_contig_step == "*" || tmp.break_contig_step_1 == "*" )
    {
        // Check contig error
        tmp.detect_type = BGIQD::DEBUG::BreakArea::BreakType::OrderUnknow ;
    }
    else 
    {
        if( std::abs(std::stoi(tmp.break_contig_step)) > 1 
                || std::abs(std::stoi(tmp.break_contig_step_1)) > 1 )
        {
            // Check order error
            tmp.detect_type = BGIQD::DEBUG::BreakArea::BreakType::OrderError ;
        }
        else
        {
            if( tmp.prev_o != tmp.end_o )
            {
                // Check orientation error
                tmp.detect_type = BGIQD::DEBUG::BreakArea::BreakType::OrientationError ;
            }
            else
            {
                // Check gap error
                int gap_scaff = tmp.end_pos - tmp.start_pos ;
                int gap_ref = 0;
                if( tmp.prev_ref_start_pos < tmp.next_ref_start_pos )
                    gap_ref = tmp.next_ref_start_pos - tmp.prev_ref_end_pos ;
                else 
                    gap_ref = tmp.prev_ref_start_pos - tmp.next_ref_end_pos ;
                int gap_diff_0  = gap_scaff - gap_ref ;
                int gap_diff = std::abs(gap_diff_0);
                if( gap_diff >= 9990 )
                {
                    tmp.detect_type = BGIQD::DEBUG::BreakArea::BreakType::GapError10K ;
                    gap_freq.Touch(gap_diff_0);
                }
                else if ( gap_diff >= 990 && ( tmp.is_N == '*' || tmp.n_size < 10 ) )
                {
                    tmp.detect_type = BGIQD::DEBUG::BreakArea::BreakType::GapError1K ;
                    gap_freq.Touch(gap_diff_0);
                }
                else
                    tmp.detect_type = BGIQD::DEBUG::BreakArea::BreakType::Other ;
            }
        }
    }
}

int main(int argc , char ** argv)
{
    if( argc != 5 )
    {
        std::cerr<<"Usage "<<argv[0]<<" scaff_infos all_xxx_6 min_n min_c "<<std::endl;
        return 0 ;
    }

    std::string si(argv[1]);
    std::ifstream si_ifs(si);
    if( ! si_ifs.is_open() )
    {
        std::cerr<<"Failed to open "<<si<<" for read !!! exit ..."<<std::endl;
        return 0 ;
    }

    std::string a6(argv[2]);
    std::ifstream a6_ifs(a6);
    if( ! a6_ifs.is_open() )
    {
        std::cerr<<"Failed to open "<<a6<<" for read !!! exit ..."<<std::endl;
        return 0 ;
    }
    min_n = std::stoi(std::string(argv[3]));
    min_c = std::stoi(std::string(argv[4]));
    std::cerr<<" Load scaff_infos now ... "<<std::endl;
    BGIQD::stLFR::ScaffInfoHelper helper ;
    helper.LoadAllScaff(si_ifs);
    si_ifs.close() ;

    std::cerr<<" Progress breaking now ... "<<std::endl;
    std::string line ;
    while( ! std::getline( a6_ifs , line).eof() )
    {
        total_log ++ ;
        BGIQD::DEBUG::BreakArea tmp ;
        if( tmp.InitFromStr_v1(line) )
        {
            scaff_log ++ ;
            if( AssignBreak( helper , tmp ))
            {
                UpdateBreakType(tmp);
                succ_log ++ ;
                std::cout<<tmp.ToString_v4()<<std::endl;
            }
        }
    }
    std::cerr<<" Total  "<<total_log<<std::endl;
    std::cerr<<" Scaff  "<<scaff_log<<std::endl;
    std::cerr<<" Succ   "<<succ_log<<std::endl;
    std::cerr<<" ERR    "<<cross_err<<std::endl;
    std::cerr<<" big_nc "<<big_nc<<std::endl;
    std::cerr<<" in_c   "<<in_c<<std::endl;
    std::cerr<<" in_n   "<<in_n<<std::endl;
    std::cerr<<" in_nc  "<<in_nc<<std::endl;
    std::cerr<<" in_ncn "<<in_ncn<<std::endl;
    std::cerr<<" in_ncc "<<in_ncc<<std::endl;
    std::cerr<<"no_found "<<no_found<<std::endl;
    std::cerr<<"gap_diff freq :\n"<<gap_freq.ToString()<<std::endl;
}

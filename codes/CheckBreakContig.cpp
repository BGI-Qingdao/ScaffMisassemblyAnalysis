#include "ScaffInfoType.h"
#include "Break.h"
#include "freq.h"
#include <iostream>
#include <fstream>

typedef BGIQD::FREQ::Freq<int> GapFreq ;
int error_log = 0 ;
int succ_log=0;
int scaff_log = 0 ;
int total_log = 0 ;
int min_n ;
int no_found =0;

bool AssignBreak( const BGIQD::stLFR::ScaffInfoHelper & helper
        , BGIQD::DEBUG::BreakArea & tmp )
{
    auto itr =  helper.all_scaff.find(tmp.scaff_id);
    if( itr == helper.all_scaff.end() )
    {
        error_log++;
        return false ;
    }

    const auto & a_scaff = itr->second.a_scaff ;

    int target_start = tmp.start_pos  ;
    int target_end = tmp.end_pos  ;

    bool left_check = false ;
    bool right_check = false ;
    int left_index = -1 ;
    int right_index = -1 ;

    for( size_t i = 0 ; i < a_scaff.size() ; i++ )
    {
        if( left_check && right_check ) 
            break ;
        const auto & a_c = a_scaff.at(i);
        int contig_start = a_c.start_pos ;
        int contig_end =   contig_start + a_c.contig_len -1 ;
        if( ! left_check && !right_check )
        {
            if( contig_start<= target_start && contig_end >= target_start )
            {
                left_index = i ; 
                left_check = true ;
            }
        }
        if ( left_check && !right_check )
        {
            if( contig_start<= target_end  && contig_end >= target_end )
            {
                right_index = i ; 
                right_check = true ;
            }
        }
    }

    if( left_check && right_check ) 
    {
        const auto & left = a_scaff.at(left_index);
        const auto & right = a_scaff.at(right_index);

        tmp.prev_contig = left.contig_id ;
        tmp.next_contig = right.contig_id ;

        tmp.prev_contig_type = left.type ;
        tmp.next_contig_type = right.type ;

        tmp.break_size_scaff = target_end - target_start -1 ;

        if( left_index == right_index )
            tmp.is_N = '*';
        else if ( right_index - left_index == 1 )
            tmp.is_N = 'N' ;
        else
            tmp.is_N = '+' ;

        tmp.break_size_ref="*";
        tmp.size_diff = "*" ;
        if( tmp.is_N == '+' )
        {
            tmp.break_type = "UnMatch" ;
            const auto mid = a_scaff.at(left_index+1);
            tmp.break_contig = mid.contig_id ;
        }
        else
        {
            if( BGIQD::stLFR::Later( tmp.prev_contig_type ,tmp.next_contig_type) )
            {
                tmp.break_type = tmp.prev_contig_type ;
                tmp.break_contig = tmp.prev_contig ;
            }
            else
            {
                tmp.break_type = tmp.next_contig_type ;
                tmp.break_contig = tmp.next_contig ;
            }
            // check aligned map changing 
            if( tmp.break_type == "OOCorrect" && left_index != right_index )
            {
                if( tmp.prev_ref_name != left.ref_name )
                {
                    tmp.break_type = "NewWrongRef" ;
                    tmp.break_contig = tmp.prev_contig ;
                }
                else if ( tmp.next_ref_name != right.ref_name )
                {
                    tmp.break_type = "NewWrongRef" ;
                    tmp.break_contig = tmp.next_contig ;
                }
            }
        }
        if( tmp.is_N != '+' && tmp.break_type == "OOCorrect" )
        {
            if( tmp.is_N == '*' )
            {
                tmp.break_type = "NewContigMissassembly" ;
            }
            else if ( tmp.is_N == 'N' )
            {
                int inref = 0 ;
                if( tmp.prev_ref_start_pos < tmp.next_ref_start_pos )
                    inref = tmp.next_ref_start_pos - tmp.prev_ref_end_pos -1 ;
                else
                    inref = tmp.prev_ref_start_pos - tmp.next_ref_end_pos -1 ;
                int diff = inref - tmp.break_size_scaff ;
                if( diff < -999 )
                {
                    tmp.break_type = "Overlap1K" ;
                }
                else if( tmp.break_size_scaff < 10 && std::abs(diff) >=999 )
                    tmp.break_type = "GapError1K" ;
                else if ( tmp.break_size_scaff >= 10 && std::abs(diff)>=9990 )
                    tmp.break_type = "GapError10K" ;
                else
                    tmp.break_type = "GapError?" ;
                tmp.break_size_ref= std::to_string(inref);
                tmp.size_diff = std::to_string(diff);
            }
            else
            {
                    tmp.break_type = "GapError?" ;
            }
        }
        return true ;
    }
    else
    {
        error_log ++ ;
        return false ;
    }
}


int main(int argc , char ** argv)
{
    if( argc != 3 )
    {
        std::cerr<<"Usage "<<argv[0]<<" scaff_infos all_xxx_5 "<<std::endl;
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
                succ_log ++ ;
                std::cout<<tmp.ToString_v4()<<std::endl;
            }
            else
            {
            }
        }
        else
        {

            no_found++;
        }
    }
    std::cerr<<" Total  "<<total_log<<std::endl;
    std::cerr<<" InScaff  "<<scaff_log<<std::endl;
    std::cerr<<" InContig "<<no_found<<std::endl;
    std::cerr<<" SuccDetect   "<<succ_log<<std::endl;
    std::cerr<<" ErrorDetect  "<<error_log<<std::endl;

    return 0 ;
}

#include "ScaffInfo_2.h"
#include "contigPairInfo.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cassert>

template< class Key >
class Freq
{
    public:
        void Touch( const Key k , int num = 1)
        {
            if( data.find(k) != data.end() )
                data[k] += num  ;
            else
                data[k] = num ;
        }

        std::string ToString() const
        {
            std::ostringstream ret ;
            for( const auto i : data )
                ret<<i.first<<'\t'<<i.second<<std::endl;
            return ret.str();
        }
        std::map<Key, long> data;
};

struct ContigChecker 
{
    unsigned int contig_id ;
    int scaff_index ;
    int ref_index ;
    int ref_pos ;
    int step ;
    int step_1;
    bool operator<(const ContigChecker & other ) const 
    {
        return ref_pos < other.ref_pos ;
    }
};


struct QuastInfo
{
    int ref_start ;
    int ref_end ;
    int query_start ;
    int query_end ;
    std::string ref ;
    unsigned int contig ;

    void InitFromString(const std::string & line )
    {
        std::string tmp1 ;
        std::istringstream ist(line);
        ist>>ref_start>>ref_end>>query_start>>query_end>>ref>>tmp1;
        std::string ctg ;
        for( auto i : tmp1 )
        {
            if( std::isdigit(i) )
            {
                ctg += i;
            }
            else
                break ;
        }
        contig = std::stoul(ctg);
    }
};

std::map<unsigned int , QuastInfo> map_cache ;

int main(int argc , char **argv)
{
    if ( argc != 2 )
    {
        std::cerr<<"Usage :  "<<argv[0]<<" contig_ref   < scaff_info >scaff_item_type "<<std::endl;
        return 1;
    }

    // load cache map ;

    std::ifstream ifs;
    std::string contig_ref(argv[1]);
    ifs.open( contig_ref );
    if ( ifs.bad() )
    {
        std::cerr<<" Read "<<contig_ref<<" failed !!! exit ..."<<std::endl;
        return 1;
    }
    std::string line ;
    while( ! std::getline( ifs , line).eof() )
    {
        QuastInfo tmp ;
        tmp.InitFromString(line);
        map_cache[tmp.contig] = tmp ;
    }
    // load all scaff
    BGIQD::stLFR::ScaffInfoHelper helper ;
    helper.LoadAllScaff(std::cin);
    long unmatch = 0 ;
    long unref = 0 ;
    long p= 0 ;
    long n= 0 ;
    Freq<int> step_freq_all;
    long total = 0;

    long Orientation_total = 0 ;
    long Orientation_err = 0 ;
    long orientation_but_contain = 0 ;

    // check all scaff
    for ( auto & pair : helper.all_scaff )
    {
        Freq<std::string> ref_fre;
        auto & a_scaff = pair.second.a_scaff ;
        // step1 mask unmatch
        for( auto & contig_detail : a_scaff )
        {
            total ++ ;
            contig_detail.type = BGIQD::stLFR::ContigDetail::Type::UnKnow;
            if( map_cache.find(contig_detail.contig_id) == map_cache.end() )
            {
                contig_detail.type = BGIQD::stLFR::ContigDetail::Type::UnMatch ;
                unmatch ++ ;
            }
            else
            {
                const auto & map_info = map_cache.at(contig_detail.contig_id);
                ref_fre.Touch ( map_info.ref );
                contig_detail.ref = map_info.ref ;
                contig_detail.ref_start = map_info.ref_start ;
            }
        }
        // step2 mask unref
        std::string  ref;
        int max = 0 ;
        for( const auto & pair : ref_fre.data )
        {
            if ( pair.second > max )
            {
                ref = pair.first ;
                max = pair.second ;
            }
        }
        for( auto & contig_detail : a_scaff )
        {
            if( contig_detail.type != BGIQD::stLFR::ContigDetail::Type::UnMatch && contig_detail.ref != ref )
            {
                contig_detail.type = BGIQD::stLFR::ContigDetail::Type::WrongRef ;
                unref ++ ;
            }
        }
        // step 3 , make scaff_order 
        std::vector<ContigChecker> sdata ;
        int  i = 1 ;
        for ( auto & contig_detail : a_scaff )
        {
            if( contig_detail.type != BGIQD::stLFR::ContigDetail::Type::WrongRef 
                    && contig_detail.type != BGIQD::stLFR::ContigDetail::Type::UnMatch )
            {
                sdata.push_back( { contig_detail.contig_id , i , 0 , contig_detail.ref_start , 0 ,0 } );
                contig_detail.type = BGIQD::stLFR::ContigDetail::Type::InRef ;
                i++ ;
            }
        }
        // step 4 make ref order
        std::sort( sdata.begin() , sdata.end() );
        std::map< unsigned int , int > contig_2_ref_index ;
        std::map< int , int > scaff_index_2_ref_index ;
        i = 1 ;
        for( auto & j : sdata )
        {
            j.ref_index  = i ;
            scaff_index_2_ref_index[j.scaff_index] = i -1 ;
            contig_2_ref_index[j.contig_id] = i - 1 ;
            i++ ;
        }
        // step 5 , get a err freq
        Freq<int> step_freq ;
        if( scaff_index_2_ref_index.size() < 3)
        {
            for ( auto & contig_detail : a_scaff )
            {
                contig_detail.type = BGIQD::stLFR::ContigDetail::Type::WrongRef ;
            }
            continue ;
        }
        for( int i = 2 ; i <= (int)sdata.size() ; i ++ )
        {
            auto & prev = sdata.at( scaff_index_2_ref_index.at(i-1) );
            auto & next= sdata.at( scaff_index_2_ref_index.at(i) );
            step_freq.Touch(next.ref_index-prev.ref_index);
            next.step = next.ref_index-prev.ref_index ;
            prev.step_1  = next.step;
        }
        // step 6 , chek scaff orientation
        int positive = 0 ;
        for( const auto & pair : step_freq.data )
        {
            if( pair.first > 0 )
                positive += pair.second  ;
            else
                positive -= pair.second  ;
        }
        if( positive > 0 )
        {
            for( const auto & pair : step_freq.data )
            {
                step_freq_all.Touch(pair.first,pair.second);
            }
            p ++ ;
        }
        else
        {
            for( const auto & pair : step_freq.data )
            {
                step_freq_all.Touch(-pair.first,pair.second);
            }
            n ++ ;
            // re-order 
            for( auto & j : sdata )
            {
                j.ref_index  = sdata.size() + 1 - j.ref_index ;
                j.step = - j.step ;
                j.step_1 = - j.step_1 ;
            }
        }
        // assign info back
        pair.second.positive = ( positive > 0 );
        for ( auto & contig_detail : a_scaff )
        {
            if( contig_detail.type == BGIQD::stLFR::ContigDetail::Type::InRef )
            {
                assert( contig_2_ref_index.find(contig_detail.contig_id) 
                        != contig_2_ref_index.end() );
                const auto & info = sdata.at( contig_2_ref_index.at(contig_detail.contig_id));
                contig_detail.step = info.step ;
                contig_detail.step_1 = info.step_1 ;
                contig_detail.ref_index = info.ref_index ;
                contig_detail.scaff_index = info.scaff_index ;
            }
        }
        int  prev_i = -1 ;
        // check the orientation of step 1 && step -1 
        for(int i = 0 ; i < (int) a_scaff.size() ; i++ )
        {
            const auto & curr = a_scaff.at(i);
            if( curr.type != BGIQD::stLFR::ContigDetail::Type::InRef )
            {
                continue ;
            }
            if( prev_i != -1 )
            {
                if( curr.step == 1 || curr.step == -1 )
                {
                    Orientation_total ++ ;
                    const auto & prev =  a_scaff.at(prev_i);
                    const auto & prev_ref = map_cache.at(prev.contig_id);
                    const auto & next_ref = map_cache.at(curr.contig_id);

                    BGIQD::stLFR::PairPN from_Scaff ;
                    BGIQD::stLFR::PairPN from_ref ;

                    from_Scaff.InitFromRef( prev.contig_id, (prev.orientation ? '+' : '-')
                            , curr.contig_id , ( curr.orientation ? '+' : '-' ) , 100);

                    if ( prev_ref.ref_start < next_ref.ref_start 
                        &&
                        prev_ref.ref_end < next_ref.ref_end )
                    {
                        from_ref.InitFromRef( prev_ref.contig
                                , ( prev_ref.query_end > prev_ref.query_start ? '+' : '-' )
                                , next_ref.contig
                                , ( next_ref.query_end > next_ref.query_start ? '+' : '-' )
                                , 100 ) ;
                    }
                    else if ( prev_ref.ref_start > next_ref.ref_start
                            &&
                            prev_ref.ref_end > next_ref.ref_end )
                    {
                        from_ref.InitFromRef(next_ref.contig
                                , ( next_ref.query_end > next_ref.query_start ? '+' : '-' )
                                , prev_ref.contig 
                                , ( prev_ref.query_end > prev_ref.query_start ? '+' : '-' )
                                , 100 ) ;
                    }
                    else
                    {
                        orientation_but_contain ++ ;
                    }
                    if( from_Scaff.type != from_ref.type )
                    {
                        Orientation_err ++ ;
                    }
                }
            }
            prev_i = i ;
        }
    }
    std::cerr<<"total contig count "<<total<<std::endl;
    std::cerr<<"unmatch contig count "<<unmatch<<std::endl;
    std::cerr<<"unref contig count "<<unref<<std::endl;
    std::cerr<<"positive scaff count "<<p<<std::endl;
    std::cerr<<"negative scaff count "<<n<<std::endl;
    std::cerr<<"step 1/-1 pair num\t"<<Orientation_total<<std::endl;
    std::cerr<<"err orientation in step 1/-1 pairs\t"<<Orientation_err<<std::endl;
    std::cerr<<"orientation but contained\t"<<orientation_but_contain<<std::endl;
    std::cerr<<" step freq\n"<<step_freq_all.ToString();

    helper.PrintAllScaff(std::cout);
}

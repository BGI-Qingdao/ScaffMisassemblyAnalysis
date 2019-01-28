#include <string>

namespace BGIQD {
    namespace DEBUG {

        struct BreakArea
        {
            //
            // Zone 1 , 9 columns
            // 
            int scaff_id ;
            int start_pos ;
            int end_pos ;
            char prev_o ;
            char end_o ;
            int prev_ref_start_pos;
            int next_ref_start_pos;
            int prev_ref_end_pos;
            int next_ref_end_pos;

            //
            // Zone 2 , 4 columns
            // 
            char is_N ; 
            long break_contig ;
            long prev_contig ;
            long next_contig ;

            //
            // Zone 3 , 2 columns
            //
            std::string break_contig_step ;
            std::string break_contig_step_1;

            //
            // Zone 4 , 2 columns
            //
            enum BreakType
            {
                Unknow = 0 ,
                OrderUnknow = 1 ,
                OrderError = 2 ,
                OrientationError = 3 ,
                GapError1K = 4 ,
                GapError10K = 5 ,
                Other = 6 
            };
            BreakType  detect_type;
            int n_size ;

            std::string BreakType2Str(const BreakType t ) const
            {
                if( t == Unknow )
                    return "Unknow";
                else if ( t == OrderUnknow )
                    return "OrderUnknow" ;
                else if ( t == OrderError )
                    return "OrderError";
                else if ( t == OrientationError )
                    return "OrientationError";
                else if ( t == GapError1K )
                    return "GapError1K" ;
                else if ( t == GapError10K )
                    return "GapError10K" ;
                else 
                    return "Other";
            }

            BreakType ParseFromStr(const std::string & t) 
            {
                if( t == "Unknow" )
                    return Unknow;
                else if ( t == "OrderUnknow" )
                    return OrderUnknow ;
                else if ( t == "OrderError" )
                    return OrderError;
                else if ( t == "OrientationError" )
                    return OrientationError;
                else if ( t == "GapError1K" )
                    return GapError1K ;
                else if ( t == "GapError10K" )
                    return GapError10K ;
                else 
                    return Other;
            }
            // true means parse correct
            // false means parse error , maybe this is a contig break
            bool InitFromStr_v1( const std::string & line ) ;
            std::string ToString_v1 () const ;

            // true means parse correct
            // false means parse error
            bool InitFromStr_v2( const std::string & line ) ;
            std::string ToString_v2 () const ;

            // true means parse correct
            // false means parse error
            bool InitFromStr_v3( const std::string & line ) ;
            std::string ToString_v3 () const ;

            // true means parse correct
            // false means parse error
            bool InitFromStr_v4( const std::string & line ) ;
            std::string ToString_v4 () const ;

        };
    }
}

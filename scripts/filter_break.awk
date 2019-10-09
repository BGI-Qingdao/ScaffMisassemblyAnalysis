BEGIN {
# name store scaffold_name
    prev_name="";
    prev_ref="";
# store prev line 
    prev_extra="";
# store prev info
    prev_scaff_end_pos=0;
    prev_orientation="+"
    prev_break=0;
    prev_ref_start=0;
    prev_ref_end=0;
}
{
    curr_scaff_start_pos=0+$3;
    curr_scaff_end_pos=0+$4;
    curr_ref_start=0+$1;
    curr_ref_end=0+$2;
    if( curr_scaff_start_pos > curr_scaff_end_pos )
    {
        tmp=curr_scaff_start_pos;
        curr_scaff_start_pos=curr_scaff_end_pos;
        curr_scaff_end_pos=tmp;
        curr_orientation="-"
    }
    else
    {
        curr_orientation="+"
    }
    curr_name=$6;
    curr_ref=$5;
    err_type=$9;
    curr_break=0;
    if( err_type ~/relocation/  || err_type ~/inversion/ || err_type ~/translocation/ )
    {
        curr_break = 1 ;
    }
    if( prev_break==1 )
    {
        if( curr_name == prev_name )
        {
            printf("%s\t%d\t%d\t%s\t%s\t%d\t%d\t%s\t%d\t%d\t%s\t%s\n",
                   curr_name,
                   prev_scaff_end_pos,curr_scaff_start_pos,
                   prev_orientation,curr_orientation,
                   prev_ref_start,prev_ref_end, prev_ref,
                   curr_ref_start,curr_ref_end, curr_ref,
                   prev_extra);
        }
    }
    prev_name=curr_name;
    prev_extra=$9" "$10"_"$11"_"$12"_"$13"_"$14"_"$15"_"$16;
    prev_scaff_end_pos=curr_scaff_end_pos;
    prev_orientation=curr_orientation;
    prev_break=curr_break;
    prev_ref=curr_ref;
    prev_ref_start=curr_ref_start;
    prev_ref_end=curr_ref_end;
}

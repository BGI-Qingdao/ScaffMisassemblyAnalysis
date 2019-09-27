BEGIN {
    prev_name="";
    prev_line="";
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
    err_type=$9;
    curr_break=0;
    if( err_type ~/relocation/  || err_type ~/inversion/ || err_type ~/translocation/ )
    {
        #if( curr_name == prev_name )
        #{
        #    printf("%s\t%d\t%d\t%s\t%s\t%d\t%d\t%d\t%d\n",
        #           curr_name,
        #           prev_scaff_end_pos,curr_scaff_start_pos,
        #           prev_orientation,curr_orientation,
        #           prev_ref_start,prev_ref_end,
        #           curr_ref_start,curr_ref_end);
        #}
        curr_break = 1 ;
    }
    if( prev_break==1 ) # && curr_break==0)
    {
        if( curr_name == prev_name )
        {
            printf("%s\t%d\t%d\t%s\t%s\t%d\t%d\t%d\t%d\t%s\n",
                   curr_name,
                   prev_scaff_end_pos,curr_scaff_start_pos,
                   prev_orientation,curr_orientation,
                   prev_ref_start,prev_ref_end,
                   curr_ref_start,curr_ref_end,
                   err_type);
        }
        else
        {
            printf("ERROR!!! %s\t%s\n",prev_line,$0);
        }
    }
    prev_name=curr_name;
    prev_line=$0;
    prev_scaff_end_pos=curr_scaff_end_pos;
    prev_orientation=curr_orientation;
    prev_break=curr_break;
    prev_ref_start=curr_ref_start;
    prev_ref_end=curr_ref_end;
}

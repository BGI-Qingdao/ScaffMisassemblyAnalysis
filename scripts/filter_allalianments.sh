#!/bin/bash

if [[ $# != 1 ]] ; then
    echo "Usage : $0 all_alignments_xxx.tsv"
    exit
fi

file0=$1;
file_base=`basename $file0`
file1=$file_base"_1.tsv"
file2=$file_base"_2.tsv"
file3=$file_base"_3.tsv"
file4=$file_base"_4.tsv"
file5=$file_base"_5.tsv"
file6=$file_base"_6.tsv"


exe_path=`dirname $0`

if [[ ! -e $exe_path/filter_break.awk && ! -e filter_break.awk ]] ; then 
    echo "$exe_path/filter_break.awk is not exsist !!!  exit ... "
    exit
fi
if [[ -e $exe_path/filter_break.awk ]] ; then 
    cp $exe_path/filter_break.awk ./
fi
# delete lines which has no aligned infomations .
grep -vw unaligned $file0 | grep -v mis_unaligned | tail -n +2  > $file1
# 2 line --> 1 line
awk '{if (NR%2==0){print $0} else {printf"%s\t",$0}}'  $file1 > $file2
# delete useless line 
grep -v "unknown"  $file2  > $file3
# file mis-assembly info
awk -f filter_break.awk < $file3 > $file5

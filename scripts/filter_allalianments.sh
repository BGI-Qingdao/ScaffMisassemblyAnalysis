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

grep -vw unaligned $file0 | grep -v mis_unaligned > $file1
tail -n +2 $file1 > $file2
awk '{if (NR%2==0){print $0} else {printf"%s\t",$0}}'  $file2 > $file3
grep -v "unknown"  $file3  > $file4
#egrep "relocation|inversion|translocation|CONTIG" <$file4  >$file5
awk -f filter_break.awk < $file4 >$file5
#awk '{printf("%s\t%s\t%s\t%s\t%s\n",$1,$2,$3,$4,$5);}' <$file5 >$file6

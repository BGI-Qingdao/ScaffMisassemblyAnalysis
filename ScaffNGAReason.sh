#!/bin/bash
#
#   @Brief
#       this script used to classify each missassmbly of quast_NGA of scaffold.
#       current support types are :
#           +--contig missassmbly
#           |
#           +--scaffold missassbbly
#             +
#             |
#             +-- Order Unknow (contig repeat/contig unaligned/translocate...)
#             |
#             +-- Order Error  ( scaffold rank 1 but not ref rank 1/-1 )
#             |
#             +-- Orientation Error (Order correct but orientation diff in ref)
#             |
#             +-- Gap 1K (Order & orientation correct , n < 10 , gap_diff>1K)
#             |
#             +-- Gap 10K (Order & orientation correct , n >=10 ,gap_diff>10K)
#
#       each missambly will belong to one of those types .
#
#   @Parameters
#       quast_contig
#       quast_scaffold
#       scaff_infos
#
#   @Output
#
#

#>>>step 0, check parameters.
Uasge="Usage : $0 quast_contig.tsv quast_scaffold.tsv xxx.scaff_infos min_n min_c";
if [[ $1 == "--help" || $1 == "-h" ]] ; then 
    echo $Uasge 
    echo "exit ... "
    exit -1 ;
fi
if [[ $# != 5 ]] ; then 
    echo $Uasge 
    echo "exit ... "
    exit -1 ;
fi

contig_quast=$1
scaff_quast=$2
scaff_infos=$3
min_n=$4
min_c=$5

if [[ ! -f $contig_quast ]] ; then
    echo "ERROR :   $contig_quast is not exist !!! "
    echo "exit ... "
    exit -1 ;
fi

if [[ ! -f $scaff_quast ]] ; then
    echo "ERROR :   $scaff_quast is not exist !!! "
    echo "exit ... "
    exit -1 ;
fi

if [[ ! -f $scaff_infos ]] ; then
    echo "ERROR :   $scaff_infos is not exist !!! "
    echo "exit ... "
    exit -1 ;
fi

if [[ $min_n -lt 0 ]] ; then 
    echo "ERROR :   $min_n is invalid !!! "
    echo "exit ... "
    exit -1 ;
fi

if [[ $min_c -lt 0 ]] ; then 
    echo "ERROR :   $min_c is invalid !!! "
    echo "exit ... "
    exit -1 ;
fi

#>>>step 0.5 , check environment .
#todo
TOOL_ROOT=`dirname $0`
echo $TOOL_ROOT
CTGAZ_DIR=$TOOL_ROOT"/SZContigAnalysis"
SCIPT_DIR=$TOOL_ROOT"/scripts"
TOOLS_DIR=$TOOL_ROOT"/codes"
#>>>step 1, get contig information
cp $CTGAZ_DIR/* ./
./order_contig.sh $contig_quast >log_oc 2>&1 
#>>>step 2, get scaffold order informtion
awk -f $SCIPT_DIR/filter_contig_name.awk sorted_unique_contig.txt >sorted_unique_contig1.txt
$TOOLS_DIR/Scaff_Order_Detect sorted_unique_contig1.txt < $scaff_infos >scaff_type.txt 2>log_sod
#>>>step 3, get break point information
cp $scaff_quast ./scaffold_quast_tmp.tsv
$SCIPT_DIR/filter_allalianments.sh ./scaffold_quast_tmp.tsv >log_faa 2>&1 
#>>>step 4, get missassmbly type information
$TOOLS_DIR/CheckBreakContig scaff_type.txt  scaffold_quast_tmp.tsv_5.tsv  $min_n $min_c >missassembly_type.txt 2>log_cbc
#>>>step 5, show the classify information
awk '{print $16}' <missassembly_type.txt | sort |uniq -c

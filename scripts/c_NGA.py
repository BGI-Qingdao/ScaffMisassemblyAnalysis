#!/usr/bin/python
# scripts by wangwenchao@genomics.cn
from optparse import OptionParser
import re
parser = OptionParser()
parser.add_option("--in", dest="infile", help="give a all_alignment_xxx", metavar="FILE")
parser.add_option("--rlen", dest="num", help="ref len", type="int")
(options, args) = parser.parse_args()
align = file(options.infile,"r")
m = options.num
list1=[]
list2=[]
la=['relocation','inversion','translocation','CONTIG']
l=0
i=0
for line in align:
	info=re.split('\t',line.strip())
	sctig=info[5]
	s=info[2]
	e=info[3]
	mis=re.split('[,]+',info[9])[0]
#       print mis
	if i==0:
		psctig=sctig
	if (mis in la) and (psctig==sctig):
		l+=abs(int(e)-int(s))+1
		list1.append(l)
		list2.append(i)
#               print l
		l=0


	elif (mis in la) and (psctig!=sctig):
		l=abs(int(e)-int(s))+1
		list1.append(l)
		list2.append(i)
#               print l
		l=0
	elif mis not in la:
		l+=abs(int(e)-int(s))+1

	i+=1
	psctig=sctig

align.close()
lista=sorted(list1,reverse=True)

s=0
for f in lista:
	if s<m/2:
		s+=f
	else:
		print "NGA50	"+str(f)+"\n"
		break
print list1
print list2


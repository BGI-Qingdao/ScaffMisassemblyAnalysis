.PHONY:	all

all:	Scaff_Order_Detect	CheckBreakContig 

CheckBreakContig: CheckBreakContig.cpp ScaffInfoType.cpp ScaffInfoType.h Break.h Break.cpp stringtools.cpp stringtools.h
	g++ -g -std=c++11 -c Break.cpp -o Break.o
	g++ -g -std=c++11 -c stringtools.cpp -o stringtools.o
	g++ -g -std=c++11 -c ScaffInfoType.cpp -o ScaffInfoType.o
	g++ -g -std=c++11 CheckBreakContig.cpp Break.o ScaffInfoType.o stringtools.o -o CheckBreakContig 

Scaff_Order_Detect: Scaff_Order_Detect.cpp ScaffInfo_2.cpp ScaffInfo_2.h contigPairInfo.h contigPairInfo.cpp
	g++ -std=c++11 -g -c ScaffInfo_2.cpp -o ScaffInfo_2.o
	g++ -std=c++11 -g -c contigPairInfo.cpp -o contigPairInfo.o
	g++ -std=c++11 -g Scaff_Order_Detect.cpp ScaffInfo_2.o contigPairInfo.o -o Scaff_Order_Detect

clean:
	rm -rf CheckBreakContig  Scaff_Order_Detect  *.o

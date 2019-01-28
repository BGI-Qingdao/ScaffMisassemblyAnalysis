.PHONY:	all

all:	CheckBreakContig 

CheckBreakContig: CheckBreakContig.cpp ScaffInfoType.cpp ScaffInfoType.h Break.h Break.cpp stringtools.cpp stringtools.h
	g++ -g -std=c++11 -c Break.cpp -o Break.o
	g++ -g -std=c++11 -c stringtools.cpp -o stringtools.o
	g++ -g -std=c++11 -c ScaffInfoType.cpp -o ScaffInfoType.o
	g++ -g -std=c++11 CheckBreakContig.cpp Break.o ScaffInfoType.o stringtools.o -o CheckBreakContig 


clean:
	rm -rf CheckBreakContig *.o

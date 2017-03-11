CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`

SRCS1 = FindFiles.cxx
OBJS1 = $(SRCS1:.c=.o)

MAIN1 = FindFiles 

SRCS2 = CountRecoInfo.cxx
OBJS2 = $(SRCS2:.c=.o)

MAIN2 = CountRecoInfo

all:    $(MAIN1)
	$(MAIN2)
	@echo  FindFiles and CountRecoInfo have been compiled successfully.

$(MAIN1): $(OBJS1)
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $(MAIN1) $(OBJS1)

$(MAIN2): $(OBJS2)
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $(MAIN2) $(OBJS2)

clean:
	$(RM) *.o *~ $(MAIN1)
	$(RM) *.o *~ $(MAIN2)

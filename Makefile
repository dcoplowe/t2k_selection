CXX = g++
CXXFLAGS = -g -Wall -fPIC
ROOTFLAGS = `root-config --cflags --glibs`

SRCS1 = FindFiles.cxx
OBJS1 = $(SRCS1:.c=.o)

MAIN1 = FindFiles 

all:    $(MAIN1)
	@echo  CompareMC has compiled successfully.

$(MAIN1): $(OBJS1)
	$(CXX) $(CXXFLAGS) $(ROOTFLAGS) -o $(MAIN1) $(OBJS1)

clean:
	$(RM) *.o *~ $(MAIN)

# Architecture

EXEC = exec

SRCDIR = src
OBJDIR = obj
BINDIR = bin
RULESDIR = rules
STRATDIR = trickstrat
BIDSTRATDIR = bidstrat

CFILES = $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/$(RULESDIR)/*.cpp $(SRCDIR)/$(STRATDIR)/*.cpp $(SRCDIR)/$(BIDSTRATDIR)/*.cpp)
OFILES = 	$(subst $(SRCDIR)/,$(OBJDIR)/,$(CFILES:.cpp=.o))
DFILES = $(OFILES:.o=.d)

VPATH = $(SRCDIR)

# Flags

CXXFLAGS = -Wall
CXXFLAGS += -DRULES_CONTREE
CXXFLAGS += -g -O2

# Inference rules

$(OBJDIR)/%.o : %.cpp | $(OBJDIR) 
	$(CXX) -o $@ -c $< $(CXXFLAGS)
	
#	Rules

all : $(EXEC)

$(OBJDIR) :
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(RULESDIR)
	mkdir -p $(OBJDIR)/$(STRATDIR)
	mkdir -p $(OBJDIR)/$(BIDSTRATDIR)

$(BINDIR) :
	mkdir -p $(BINDIR)

-include $(DFILES)
	
$(EXEC) : $(OFILES) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$@ $^ $(LDFLAGS)
	
run : $(EXEC) # runs the binary
	./$(BINDIR)/$(EXEC)

sure : mrproper
	make run

.PHONY : clean mrproper

clean : # remove binary and dependencies
	$(RM) $(BINDIR)/$(EXEC) $(DFILES)

mrproper : clean # remove obj and bin folders
	rm -r $(OBJDIR) $(BINDIR)
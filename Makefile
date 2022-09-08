# Architecture

EXEC = exec

SRCDIR = src
OBJDIR = obj
FOXDIR = fox
BINDIR = bin
RULESDIR = rules
STRATDIR = trickstrat

CFILES = $(wildcard $(SRCDIR)/*.cpp $(SRCDIR)/$(RULESDIR)/*.cpp $(SRCDIR)/$(FOXDIR)/*.cpp $(SRCDIR)/$(STRATDIR)/*.cpp $(SRCDIR)/$(BIDSTRATDIR)/*.cpp)
OFILES = $(subst $(SRCDIR)/,$(OBJDIR)/,$(CFILES:.cpp=.o))
DFILES = $(OFILES:.o=.d)

VPATH = $(SRCDIR)

# Flags

# CXXFLAGS = -Wall
CXXFLAGS += -DRULES_CONTREE
CXXFLAGS += -g

DEFINES    += -DBELOTE
# DEFINES    += -DINTERACTPLAYER=4

DEFINES += -DCOPYRIGHT='"\nBelote playing program using expert knowledge, belief states, and-or search (c) 2022, Stefan Edelkamp, Samuel Bounan\n"'

# Print Information about Knowledge
# DEFINES     += -DKNOWLEDGEPRINT
# DEFINES     += -DALLPRINT
# DEFINES     += -DSHOWBELIEF
# Unicode instead of Numbers
# DEFINES     += -DDISPLAY
# DEFINES     += -DCERTAINTY
# AI bids
# DEFINES     += -DBIDDING
# Switches for Factorized Search
# DEFINES     += -DASTRUMP
# DEFINES     += -DFACTORED
# Switch for Hope Cards
# DEFINES     += -DNOHOPE
# Switch for Glassbox Analysis 
# DEFINES     += -DONLYGLAS
# Switch for online/offline play
# DEFINES     += -DFROMFILE
# Switches for Endgame play
DEFINES     += -DSUGGEST
# Switches for Analysis Starting Cards 
DEFINES     += -DFULLIITRICK=18
DEFINES     += -DDIFFERENCE=3
DEFINES     += -DBSDEPTH=7
# Belief Space Size and Confidence Level
DEFINES     += -DBSSIZE=20000
DEFINES     += -DBSCONF=0.90
# "Killer" Cards 
DEFINES     += -DKILLER

# Inference rules

$(OBJDIR)/%.o : %.cpp | $(OBJDIR) 
	$(CXX) -o $@ -c $< $(CXXFLAGS) $(DEFINES)
	
#	Rules

all : $(EXEC)

$(OBJDIR) :
	mkdir -p $(OBJDIR)
	mkdir -p $(OBJDIR)/$(RULESDIR)
	mkdir -p $(OBJDIR)/$(FOXDIR)
	mkdir -p $(OBJDIR)/$(STRATDIR)

$(BINDIR) :
	mkdir -p $(BINDIR)

-include $(DFILES)
	
$(EXEC) : $(OFILES) | $(BINDIR)
	$(CXX) $(CXXFLAGS) $(DEFINES) -o $(BINDIR)/$@ $^ $(LDFLAGS)
	
run : $(EXEC) # runs the binary
	./$(BINDIR)/$(EXEC)

sure : mrproper
	make run

.PHONY : clean mrproper

clean : # remove binary and dependencies
	$(RM) $(BINDIR)/$(EXEC) $(DFILES)

mrproper : clean # remove obj and bin folders
	rm -r $(OBJDIR) $(BINDIR)

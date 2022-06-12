# Bridge playing program using expert knowledge, expert games, belief states, and-or search
# (c) 2021-22, Stefan Edelkamp 
#
#######################################
# CONFIG
#######################################
VERSION     = 2.1.3
#######################################
# CONSTANTS
#######################################
CC      	= g++ -O2
EXECUTABLE 	= kiclientpool 
TMPDIR      = /tmp
ARCHIVE     = kiclientpool-v$(VERSION)
EXECDIR 	= /usr/bin
#######################################
# Compiler Flags
#######################################
CFLAGS 		 = -std=c++1y
#######################################
# Defines
#######################################
# Print Information about Knowledge
# DEFINES     += -DKNOWLEDGEPRINT
# DEFINES     += -DALLPRINT
# DEFINES     += -DSHOWBELIEF
# Unicode instead of Numbers
DEFINES     += -DDISPLAY
# DEFINES     += -DCERTAINTY

# AI bids
# DEFINES     += -DBIDDING

# Switches for Worldsearch
# DEFINES     += -DPARANOIA
# DEFINES     += -DWORSTCASE
# DEFINES     += -DBESTCASE

# DEFINES     += -DGOESSL	

# Switches for Factorized Search

# DEFINES     += -DASTRUMP
# DEFINES     += -DFACTORED

# Switch for Hope Cards
# DEFINES     += -DNOHOPE

# Switch for Glassbox Analysis 
# DEFINES     += -DONLYGLAS

DEFINES     += -DCLIENTPOOL_VERSION='"$(VERSION)"'

# Switch for online/offline play
DEFINES     += -DFROMFILE

# Switches for Endgame play
DEFINES     += -DGSSUGGEST
DEFINES     += -DASSUGGEST

# Switches for Analysis Starting Cards 
DEFINES     += -DSTARTTRICK=7
DEFINES     += -DFULLPARANOIATRICK=10
DEFINES     += -DDIFFERENCE=3
DEFINES     += -DWORSTCASEDEPTH=14
DEFINES     += -DBSDEPTH=10

# Belief Space Size and Confidence Level
DEFINES     += -DBSSIZE=3000
DEFINES     += -DBSCONF=0.90

# "Killer" Cards 
DEFINES     += -DKILLER

# Switch Optimization vs Decision Variants
# DEFINES   += -DOPTCARD
#######################################
# Linker Flags
#######################################
LDFLAGS      += -lstdc++
#######################################
# BUILD OBJECTS
#######################################
CPP_FILES = $(wildcard *.cpp)
OBJS = $(CPP_FILES:.cpp=.o)
#######################################
# RULES
#######################################
$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -I. -o $(EXECUTABLE) $(LDFLAGS)

%.o: %.cpp
	$(CC) -L. -I. $(VERBOSITY) -c $(CFLAGS) $(DEFINES) $< -o $@

clean:
	@-rm -f $(EXECUTABLE) $(OBJS) *.tgz *.txt

install:
	@cp $(EXECUTABLE) $(EXECDIR)

dist: clean
	@-mkdir $(TMPDIR)/$(ARCHIVE)
	@cp -a * $(TMPDIR)/$(ARCHIVE)
	@tar czf $(ARCHIVE).tgz -C $(TMPDIR) $(ARCHIVE)
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@echo Distribution package created as $(ARCHIVE).tgz

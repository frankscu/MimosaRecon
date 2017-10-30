TARGET = MimosaRecon

TARGET_EXE := $(TARGET)

################### Define Path #########################
TARGET_DIR := ./
SRC_DIR := ./src/
INC_DIR := ./include/
BUILD_DIR := ./build/

SRCS         := $(wildcard $(TARGET_DIR)*.cxx $(SRC_DIR)*.cxx)
SRCSFILE     := $(notdir $(SRCS))
OBJS         := $(patsubst %.cxx,%.o,$(SRCSFILE))
DEPS         := $(patsubst %.cxx,%.d,$(SRCSFILE))

################## Path to look for specific files ####################
vpath %.cxx $(TARGET_DIR)
vpath %.cxx $(SRC_DIR)
vpath %.h $(INC_DIR)
vpath %.o $(BUILD_DIR)
vpath %.d $(BUILD_DIR)

##################### Compile Options ###################
CXX          := g++
DEBUG        := -ggdb3
WARNING      := -Wall
CXXFLAGS     := $(DEBUG) $(WARNING) -D_REENTRANT -D_GNU_SOURCE -std=c++11

ROOTCONFIG   := root-config
ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)
HASTHREAD    := $(shell $(ROOTCONFIG) --has-thread)

CLHEPCONFIG   := clhep-config
CLHEPINCLUDE   := $(shell $(CLHEPCONFIG) --include)
CLHEPLIBS     := $(shell $(CLHEPCONFIG) --libs)

CXXFLAGS     += $(ROOTCFLAGS) $(CLHEPINCLUDE)/CLHEP -I$(INC_DIR)

LIBS         := $(ROOTGLIBS) $(CLHEPLIBS)

$(BUILD_DIR)%.d: %.cxx
	@echo \*\*\*\*\*\* Creating dependence file: $@; 
	@set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($(notdir $*)\)\.o[ :]*,\1.o $(notdir $*).d: ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$; 

$(TARGET_EXE): $(OBJS)
	@echo ; 
	@echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\* Linking: $@ \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*; 
	$(CXX) $(CXXFLAGS) $(LIBS) $(addprefix $(BUILD_DIR),$(notdir $^)) -o $@
	@echo \*\*\*\*\*\*\*\*\*\*\*\*\*\* Installing $@ \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*; 

sinclude $(addprefix $(BUILD_DIR),$(DEPS))

%.o: %.cxx
	@echo ; 
	@echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\* Creating obj file: $@ \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*; 
	$(CXX) $(CXXFLAGS) -c $< -o $(addprefix $(BUILD_DIR),$(notdir $@))

.PHONY: clean
clean:
	@rm -f ./build/*
	@rm -f ./*.exe

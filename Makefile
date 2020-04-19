TARGET 				= 	$(CUR_DIR_NAME)

ifeq ($(OS), Windows_NT)
  RM          =   CMD /C del /Q /F
  RRM         =   CMD /C rmdir /Q /S
  BACKSLASH   :=  \$(strip)
  LFLAGS			=		-LC:\cygwin64\lib
	LLIBS				=		-lstdc++fs
else
  RM          =   rm -f
  RRM         =   rm -f -r
	BACKSLASH   :=  /$(strip)
  LFLAGS			=   -L../lib -L/usr/lib/x86_64-linux-gnu/
	LLIBS				=		-lstdc++fs
endif

CXX 					= 	g++
CXXFLAGS 			= 	-g -Wall -Werror -MD -MP -I$(INC_DIR) -std=c++17
DIRS					=		binaries objects
SRC_DIR				=		./source
BIN_DIR 			=		./binaries
OBJ_DIR				=		./objects
INC_DIR				=		./include
MKF_PTH 			:= 	$(abspath $(lastword $(MAKEFILE_LIST)))
CUR_DIR				:=	$(strip $(subst Makefile,, $(MKF_PTH)))
RUN_DIR				:= 	$(addprefix $(CUR_DIR), $(strip $(subst .\,, $(BIN_DIR))))
CUR_DIR_NAME	:= 	$(notdir $(patsubst %/,%,$(dir $(MKF_PTH))))
SRCS 					=		$(wildcard $(SRC_DIR)/*.cpp)
OBJS 					=		$(subst $(SRC_DIR),$(OBJ_DIR),$(SRCS:.cpp=.o))

.PHONY: all
.PHONY: clean
.PHONY: directories
.PHONY: run
.PHONY: compile
.PHONY: console
.PHONY: runConsoleOutput
.PHONY: valgrind
.PHONY: memoryCheck

all: directories $(BIN_DIR)/$(TARGET) run

compile: directories $(BIN_DIR)/$(TARGET)

valgrind: directories $(BIN_DIR)/$(TARGET) memoryCheck

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(LFLAGS) $^ $(LLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

directories: $(DIRS)

$(DIRS):
	mkdir $(DIRS)

clean:
	$(RRM) $(subst /,$(BACKSLASH), $(BIN_DIR))
	$(RRM) $(subst /,$(BACKSLASH), $(OBJ_DIR))

run:
	$(RUN_DIR)/$(TARGET)

memoryCheck:
	valgrind --leak-check=full $(RUN_DIR)/$(TARGET)

-include $(OBJS:.o=.d)

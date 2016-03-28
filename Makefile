NAME = 'FieldTheory'
SRC = src
TGT = obj
PRG = application
INCLUDES = -Iinclude
CXXFLAGS = -Wall -O3 -std=c++0x $(INCLUDES)
SOURCES = $(wildcard $(SRC)/*.cpp)
OBJS = $(addprefix $(TGT)/, $(notdir $(SOURCES:.cpp=.o)))

$(TGT)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PRG)/$(NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@


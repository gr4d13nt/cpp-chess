CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = main
OBJ = main.o engine.o evaluation.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp engine.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

engine.o: engine.cpp engine.hpp evaluation.hpp
	$(CXX) $(CXXFLAGS) -c engine.cpp

evaluation.o: evaluation.cpp evaluation.hpp
	$(CXX) $(CXXFLAGS) -c evaluation.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

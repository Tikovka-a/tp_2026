CXX = g++
CXXFLAGS = -Wall -std=c++17
SRCS = main.cpp dict.cpp utils.cpp

# Определяем операционную систему и настраиваем команды
ifeq ($(OS),Windows_NT)
    TARGET = main.exe
    RUN_CMD = $(TARGET) < test_input.txt > test_output.txt
    CLEAN_CMD = if exist $(TARGET) del /Q $(TARGET) && if exist test_output.txt del /Q test_output.txt && if exist dict.txt del /Q dict.txt
else
    TARGET = main.out
    RUN_CMD = ./$(TARGET) < test_input.txt > test_output.txt
    CLEAN_CMD = rm -f $(TARGET) test_output.txt dict.txt
endif

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	$(RUN_CMD)

clean:
	$(CLEAN_CMD)
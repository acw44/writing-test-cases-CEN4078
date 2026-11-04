CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -O2

ifeq ($(OS),Windows_NT)
TARGET := secure_login.exe
TEST_TARGET := secure_login_tests.exe
else
TARGET := secure_login
TEST_TARGET := secure_login_tests
endif

SOURCES := main.cpp Validator.cpp Cryptographer.cpp DefaultPassword.cpp
HEADERS := Validator.hpp Cryptographer.hpp DefaultPassword.hpp
TEST_SOURCES := tests.cpp Validator.cpp Cryptographer.cpp DefaultPassword.cpp

.PHONY: all test clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# UPDATED (v2.0.0): Build and run the assignment's minimum tests.
$(TEST_TARGET): $(TEST_SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

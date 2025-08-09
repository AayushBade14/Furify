CXX = g++

LIBS = -lglfw -lGL

BUILD = ./Build
INCLUDE = ./Include/ThirdParty

CXXFLAGS = -Wall -g -I$(INCLUDE)

OBJS = ./Main.o \
			 ./Include/Shader/Shader.o \
			 ./Include/Memory/Buffer/Buffer.o \
			 ./Include/Memory/VAO/VAO.o \
			 ./Include/ThirdParty/STB/stb_image.o \
			 ./Include/ThirdParty/glad/glad.o \
			 ./Include/Primitives/Primitives.o

TARGET = $(BUILD)/furify

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS) $(CXXFLAGS)

%.o : %.cpp
	$(CXX) -c $< -o $@ $(LIBS) $(CXXFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)


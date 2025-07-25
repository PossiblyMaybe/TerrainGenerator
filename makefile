TARGET = TerrainEngine

SRC_DIR = ./src

SRCS = ${shell find $(SRC_DIR) -name '*.cpp'}
OBJS = ${SRCS:.cpp=.o}


	
${TARGET}: ${OBJS}
	@echo 'Compiling $@'
	@g++ -o $@ $^  -lGL -lglfw -lGLU -Llib -lglad	
	@echo 'Compiled'
	@echo 'Cleaning'
	@rm src/*.o
	@echo 'Cleaned'
	
	
%.o: %.cpp
	@echo 'Compiling $@'
	@g++ -c $^ -o $@  

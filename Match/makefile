COMP = g++

OBJS = main.o MatchManager.o Utils.o Maze.o GameManager.o AlgorithmRegistration.o MazeInfo.o
EXEC = match
CPP_COMP_FLAG = -std=c++17 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -lstdc++fs -ldl -lpthread -export-dynamic

$(EXEC): $(OBJS)
	$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@ 
	
AlgorithmRegistration.o: AlgorithmRegistration.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

Maze.o: Maze.cpp
		$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

MazeInfo.o: MazeInfo.cpp
		$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

GameManager.o: GameManager.cpp
		$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

Utils.o: Utils.cpp Utils.h
		$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

MatchManager.o: MatchManager.cpp
		$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

main.o: main.cpp
		$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
	
clean:
	rm -f $(OBJS) $(EXEC)

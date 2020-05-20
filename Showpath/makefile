COMP = g++
OBJS = main.o CUtils.o
EXEC = showpath
CPP_COMP_FLAG = -std=c++17 -g -Wall -Wextra -Werror -pedantic-errors -DNDEBUG
CPP_LINK_FLAG = -lstdc++fs -lncurses

$(EXEC): $(OBJS)
		$(COMP) $(OBJS) $(CPP_LINK_FLAG) -o $@

CUtils.o: CUtils.cpp
		$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
main.o: main.cpp CUtils.cpp
		$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
clean:
		rm -f $(OBJS) $(EXEC)
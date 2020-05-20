COMP = g++

SHARED_OBJS = solver1.so solver2.so
OBJS_A = UnlimitedBookmarksAlgo.o solver1.o
OBJS_B = UnlimitedBookmarksAlgo.o solver2.o
CPP_COMP_FLAG = -std=c++17 -Wall -Wextra -Werror -pedantic-errors -DNDEBUG -fPIC
CPP_LINK_FLAG = -shared

all: $(SHARED_OBJS)

solver1.so: $(OBJS_A)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

solver1.o: solver1.cpp 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

solver2.so: $(OBJS_B)
	$(COMP) $(CPP_LINK_FLAG) -o $@ $^

solver2.o: solver2.cpp 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

UnlimitedBookmarksAlgo.o: UnlimitedBookmarksAlgo.cpp 
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
	
clean:
	rm -f $(OBJS_A) $(OBJS_B) $(SHARED_OBJS)

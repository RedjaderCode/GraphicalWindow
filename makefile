
comp   = g++


#inclpath = -IC:/code/redjader/engineGL/src/include
flags    = -w -fstack-usage -DUNICODE
link     = -lopengl32 -lgdi32 -lole32 -loleaut32

out      = engineGL

all: main.cpp
	$(comp) -c main.cpp $(flags) $(link)
	$(comp) *.o $(link) -o $(out)
	rm *.o

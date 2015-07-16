
objects=conv.o testencoding.o 
targets=test

# Compile th projects
$(targets):$(objects)
	g++ -o $(targets) $(objects)

conv.o : conv.h
testencoding.o : conv.h

# Clean all the projects 
.PHONY : clean
clean :
	-rm $(targets) $(objects)


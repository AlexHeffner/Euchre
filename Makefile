CFLAGS = -Wall 
CC = g++

info:
	@echo "Available Targets"
	@echo " - pmain: evaluate heuristic statisicly using ompenmp"
	@echo " - seqmain: evaluate heuristic statisicly sequentialy"
	@echo " - example: used to show how game runs, shows proper game"
	@echo " - clean: remove generated files"

pmain: pmain.cc game.cc
	$(CC) $(CFLAGS) -o $@  -fopenmp $^ 

seqmain: seqmain.cc game.cc
	$(CC) $(CFLAGS) -o $@   $^

example: example.cc exampleGame.cc
	$(CC) $(CFLAGS) -o $@ $^
clean:
	-rm -f *.o
	-rm -f a.out pmain example seqmain

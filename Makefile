CC = g++
OBJS = corpus.o lda.o
LINKPARAM =
EXEC = main
main.o:main.cpp corpus.h lda.h
	  $(CC) -c -g $<
corpus.o:corpus.cpp corpus.h 
	  $(CC) -c -g $<
lda.o:lda.cpp lda.h corpus.h
	  $(CC) -c -g $<

all : $(EXEC)
main:$(OBJS) main.o
	  $(CC)  -o main -g main.o  $(OBJS) $(LINKPARAM)

clean: -rm *.o

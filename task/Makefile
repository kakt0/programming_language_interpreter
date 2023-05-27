PROG = task10
CFLAGS = -g
CC=g++
task9: main.cpp Lex.h Ident.h Scaner.h Parser.h Executer.h Interpretator.h
	$(CC) $(CFLAGS) Scaner.cpp Parser.cpp Executer.cpp main.cpp -o $(PROG)
clean:
	rm -f *.o $(PROG)
a_tests:
	./$(PROG) prog.txt
	./$(PROG) prog1.txt
	./$(PROG) prog3.txt
b_test_1:
	./$(PROG) prog4.txt
b_test_2:
	./$(PROG) prog2.txt
b_test_3:
	./$(PROG) prog5.txt
b_test_4:
	./$(PROG) prog6.txt
b_test_5:
	./$(PROG) prog7.txt
run:
	./$(PROG)

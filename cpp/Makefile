#/**************************************************************/
#/* José Eduardo R. M. Peres y Peres       No. USP:8945332     */
#/* Exercicio-Programa EP1				       */
#/* Curso MAC323 - Prof. Carlos Eduardo Ferreira	       */
#/*                                                            */
#/* Compilador: GNU GCC Compiler                               */
#/**************************************************************/
CXXFLAGS = -Wall -pedantic -Wno-unused-result
CC = g++
EXECUTABLE = unit_test
CLASSES = tabeladesimbolos.hpp vetorDes.hpp vetorOrd.hpp listaDes.hpp\
listaOrd.hpp arvoreBin.hpp treap.hpp arvore23.hpp arvoreRN.hpp\
hashTable.hpp Par.hpp 
MODE = AB
###########################################################################

release: CXXFLAGS += -O2
release: run

debug:	CXXFLAGS += -g -O0
debug:	$(EXECUTABLE)
	valgrind --leak-check=full ./$^ Uncle_Wiggily_and_The_Pirates.txt $(MODE)
	@printf "Executado: $(MODE)\n"

run:	$(EXECUTABLE)
	./$^ Uncle_Wiggily_and_The_Pirates.txt $(MODE)

$(EXECUTABLE): util.o unit_test.o 
	$(CC) $(CXXFLAGS) $^ -o $@

unit_test.o: unit_test.cpp $(CLASSES)
	$(CC) $(CXXFLAGS) -c $< -o $@

util.o: util.cpp util.hpp
	$(CC) $(CXXFLAGS) -c $< -o $@

.PHONY:	clean
clean:
	rm -f *.o $(EXECUTABLE)

.PHONY:	backup
backup:
	rm -rf backup
	mkdir backup
	cp *.cpp backup
	cp *.hpp backup
	cp Makefile backup
	cp relatorio.txt backup

.PHONY:	pack
pack:
	mkdir ep1;\
	cp *.cpp ep1;\
	cp *.txt ep1;\
	cp *.hpp ep1;\
	cp Makefile ep1;\
	cd ep1;\
	tar -cf ep1_nusp_8945332.tar *;\
	cd ..;\
	mv ep1/ep1_nusp_8945332.tar .;\
	rm -rf ep1

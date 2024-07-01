JA = java
JC = javac
FLAGS = -Xlint
EXEC = UnitTest
CLASSES = VetorDes Pair SymbolTable VetorOrd\
 ListaDes ListCell ListaOrd BinTreeCell \
 ArvoreBin Treap TreapCell
JAVA_CLASSES = $(addsuffix .java,$(CLASSES))
MODE = TR
FILE = Uncle_Wiggily_and_The_Pirates.txt

#################################################

.PHONY:	debug
debug:	$(EXEC)
	$(JA) $< $(FILE) $(MODE) 

$(EXEC): $(EXEC).class

.DELETE_ON_ERROR:
$(EXEC).class: $(EXEC).java $(JAVA_CLASSES)
	$(JC) $<

$(JAVA_CLASSES):
	printf "Missing $@\n\n"

.PHONY:	clean
clean:
	rm -f *.class

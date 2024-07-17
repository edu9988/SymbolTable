public class Arvore23 implements SymbolTable{
    TwoThreeCell root;

    public Arvore23( String filename ){
	root = new TwoThreeCell();
	readFile( filename );
    }
}

public class Arvore23 implements SymbolTable{
    TwoThreeCell root;

    public Arvore23( String filename ){
	root = new TwoThreeCell();
	root.threeNode = false;
	readFile( filename );
    }

    private void downward( TwoThreeCell tree , String key ){
	int aux;
	aux = tree.p1.getKey().compareToIgnoreCase( key );
	if( aux < 0 ){
	    if( tree.threeNode ){
		aux = tree.p2.getKey().compareToIgnoreCase( key );
		if( aux < 0 ){
		    if( tree.right == null )
			upward( tree , key );
		    else
			downward( tree.right , key );
		}
		else if( aux == 0 ){
		    tree.p2.valInc();
		}
		else{ /* aux > 0 */
		    if( tree.middle == null )
			upward( tree , key );
		    else
			downward( tree.middle , key );
		}
	    }
	    else{
		if( tree.right == null )
		    upward( tree , key );
		else
		    downward( tree.right , key );
	    }
	}
	else if( aux == 0 ){
	    tree.p1.valInc();
	}
	else{ /* aux > 0 */
	    if( tree.left == null )
		upward( tree , key );
	    else
		downward( tree.left , key );
	}
    }

    public void insert( String key ){
	downward( root, key );
    }

    public int retrieve ( String key ){
	return 0;
    }

    public void remove( String Key ){
    }

    public int rank( String Key ){
	return 0;
    }

    public String select( int rank ){
	return "";
    }
}

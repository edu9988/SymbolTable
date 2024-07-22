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
		    if( tree.right == null ){
			TwoThreeCell flyingTree = new TwoThreeCell();
			flyingTree.threeNode = false;
			flyingTree.p1.setKey( key );
			upward( tree , flyingTree );
		    }
		    else
			downward( tree.right , key );
		}
		else if( aux == 0 ){
		    tree.p2.valInc();
		}
		else{ /* aux > 0 */
		    if( tree.middle == null ){
			upward( tree , key );
		    }
		    else
			downward( tree.middle , key );
		}
	    }
	    else{ /* not threeNode */
		if( tree.right == null )
		    tree.p2.setKey( key );
		else
		    downward( tree.right , key );
	    }
	}
	else if( aux == 0 ){
	    tree.p1.valInc();
	}
	else{ /* aux > 0 */
	    if( tree.left == null ){
		if( tree.threeNode ){
		    TwoThreeCell flyingNode = new TwoThreeCell();
		    flyingNode.threeNode = false;
		    flyingNode.p1.setKey( tree.p1.getKey() );
		    flyingNode.p1.setVal( tree.p1.getVal() );
		}
		else{ /* not threeNode */
		    tree.p2 = new Pair();
		    tree.p2.setKey( tree.p1.getKey() );
		    tree.p2.setVal( tree.p1.getVal() );
		    tree.p1.setKey( key );
		    tree.p1.setVal( 1 );
		}
	    }
	    else
		downward( tree.left , key );
	}
    }

    public void insert( String key ){
	if( root.p1 == null ){
	    root.p1 = new Pair();
	    root.p1.setKey( key );
	    root.p1.setVal( 1 );
	}
	else
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

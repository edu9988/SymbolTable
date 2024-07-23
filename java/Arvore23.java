public class Arvore23 implements SymbolTable{
    TwoThreeCell root;

    public Arvore23( String filename ){
	root = new TwoThreeCell();
	root.threeNode = false;
	readFile( filename );
    }

    private void upward( TwoThreeCell parent , TwoThreeCell flyingNode ){
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
			flyingTree.p1 = new Pair();
			flyingTree.p1.setKey( tree.p2.getKey() );
			flyingTree.p1.setVal( tree.p2.getVal() );
			tree.p2 = null;
			tree.threeNode = false;
			flyingTree.parent = tree.parent;
			tree.parent = flyingTree;
			flyingTree.left = tree;
			flyingTree.right = new TwoThreeCell();
			flyingTree.right.threeNode = false;
			flyingTree.right.parent = flyingTree;
			flyingTree.right.p1 = new Pair();
			flyingTree.right.p1.setKey( key );
			flyingTree.right.p1.setVal( 1 );
			upward( flyingTree.parent , flyingTree );
		    }
		    else
			downward( tree.right , key );
		}
		else if( aux == 0 ){
		    tree.p2.valInc();
		}
		else{ /* aux > 0 */
		    if( tree.middle == null ){
			TwoThreeCell flyingNode = new TwoThreeCell();
			flyingNode.threeNode = false;
			flyingNode.p1 = new Pair();
			flyingNode.p1.setKey( key );
			flyingNode.p1.setVal( 1 );
			flyingNode.left = tree;
			flyingNode.parent = tree.parent;
			tree.parent = flyingNode;
			flyingNode.right = new TwoThreeCell();
			flyingNode.right.parent = flyingNode;
			flyingNode.right.threeNode = false;
			flyingNode.right.p1 = new Pair();
			flyingNode.right.p1.setKey( tree.p2.getKey() );
			flyingNode.right.p1.setVal( tree.p2.getVal() );
			tree.threeNode = false;
			tree.p2 = null;
			upward( flyingNode.parent , flyingNode );
		    }
		    else
			downward( tree.middle , key );
		}
	    }
	    else{ /* not threeNode */
		if( tree.right == null ){
		    tree.threeNode = true;
		    tree.p2 = new Pair();
		    tree.p2.setKey( key );
		    tree.p2.setVal( 1 );
		}
		else
		    downward( tree.right , key );
	    }
	}
	else if( aux == 0 )
	    tree.p1.valInc();
	else{ /* aux > 0 */
	    if( tree.left == null ){
		if( tree.threeNode ){
		    TwoThreeCell flyingNode = new TwoThreeCell();
		    flyingNode.threeNode = false;
		    flyingNode.p1 = new Pair();
		    flyingNode.p1.setKey( tree.p1.getKey() );
		    flyingNode.p1.setVal( tree.p1.getVal() );
		    flyingNode.right = tree;
		    flyingNode.parent = tree.parent;
		    tree.parent = flyingNode;
		    tree.p1.setKey( tree.p2.getKey() );
		    tree.p1.setVal( tree.p2.getVal() );
		    tree.threeNode = false;
		    tree.p2 = null;
		    flyingNode.left = new TwoThreeCell();
		    flyingNode.left.parent = flyingNode;
		    flyingNode.left.threeNode = false;
		    flyingNode.left.p1 = new Pair();
		    flyingNode.left.p1.setKey( key );
		    flyingNode.left.p1.setVal( 1 );
		    upward( flyingNode.parent , flyingNode );
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
	    downward( root , key );
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

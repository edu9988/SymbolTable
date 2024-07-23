public class Arvore23 implements SymbolTable{
    private TwoThreeCell root;

    public Arvore23( String filename ){
	root = new TwoThreeCell();
	root.threeNode = false;
	readFile( filename );
	debug_print( root );
    }

    private void upward( TwoThreeCell parent , TwoThreeCell flyingNode ){
	int aux;
	if( parent.threeNode ){
	    aux = parent.p1.getKey().compareToIgnoreCase( flyingNode.p1.getKey() );
	    if( aux < 0 ){ /* parent.p1 to the left of flyingNode.p1 */
		aux = parent.p2.getKey().compareToIgnoreCase( flyingNode.p1.getKey() );
		if( aux < 0 ){ /* parent.p2 to the left of flyingNode.p1 */
		    TwoThreeCell midNode = new TwoThreeCell();
		    midNode.threeNode = false;
		    midNode.p1 = new Pair();
		    midNode.p1.setKey( parent.p2.getKey() );
		    midNode.p1.setVal( parent.p2.getVal() );
		    midNode.parent = parent.parent;
		    midNode.left = parent;
		    midNode.right = flyingNode;
		    parent.right = parent.middle;
		    parent.threeNode = false;
		    parent.p2 = null;
		    parent.parent = midNode;
		    if( parent != root )
			upward( parent.parent , parent );
		    else
			root = midNode;
		}
		else{ /* parent.p2 to the right of flyingNode.p1 */
		    TwoThreeCell rightNode = new TwoThreeCell();
		    rightNode.threeNode = false;
		    rightNode.p1 = new Pair();
		    rightNode.p1.setKey( parent.p2.getKey() );
		    rightNode.p1.setVal( parent.p2.getVal() );
		    rightNode.left = flyingNode.right;
		    rightNode.right = parent.right;
		    rightNode.parent = flyingNode;
		    flyingNode.right = rightNode;
		    parent.right = flyingNode.left;
		    flyingNode.left = parent;
		    parent.p2 = null;
		    parent.threeNode = false;
		    flyingNode.parent = parent.parent;
		    parent.parent = flyingNode;
		    if( parent != root )
			upward( parent.parent , parent );
		    else
			root = flyingNode;
		}
	    }
	    else{ /* parent.p1 to the right of flyingNode.p1 */
		TwoThreeCell rightNode = new TwoThreeCell();
		rightNode.threeNode = false;
		rightNode.p1 = new Pair();
		rightNode.p1.setKey( parent.p2.getKey() );
		rightNode.p1.setVal( parent.p2.getVal() );
		rightNode.left = parent.middle;
		rightNode.right = parent.right;
		rightNode.parent = parent;
		parent.right = rightNode;
		parent.p2 = null;
		parent.threeNode = false;
		parent.left = flyingNode;
		if( parent != root )
		    upward( parent.parent , parent );
	    }
	}
	else{ /* parent is 2-node */
	    parent.threeNode = true;
	    parent.p2 = new Pair();
	    aux = parent.p1.getKey().compareToIgnoreCase( flyingNode.p1.getKey() );
	    if( aux < 0 ){ /* parent.p1 to the left of flyingNode.p1 */
		parent.p2.setKey( flyingNode.p1.getKey() );
		parent.p2.setVal( flyingNode.p1.getVal() );
		parent.middle = flyingNode.left;
		parent.right = flyingNode.right;
	    }
	    else{ /* parent.p1 to the right of flyingNode.p1 */
		parent.p2.setKey( parent.p1.getKey() );
		parent.p2.setVal( parent.p1.getVal() );
		parent.middle = flyingNode.right;
		parent.left = flyingNode.left;
		parent.p1.setKey( flyingNode.p1.getKey() );
		parent.p1.setVal( flyingNode.p1.getVal() );
	    }
	}
    }

    private void downward( TwoThreeCell tree , String key ){
	int aux;
	TwoThreeCell flyingNode;
	aux = tree.p1.getKey().compareToIgnoreCase( key );
	if( aux < 0 ){
	    if( tree.threeNode ){
		aux = tree.p2.getKey().compareToIgnoreCase( key );
		if( aux < 0 ){
		    if( tree.right == null ){
			flyingNode = new TwoThreeCell();
			flyingNode.threeNode = false;
			flyingNode.p1 = new Pair();
			flyingNode.p1.setKey( tree.p2.getKey() );
			flyingNode.p1.setVal( tree.p2.getVal() );
			tree.p2 = null;
			tree.threeNode = false;
			flyingNode.parent = tree.parent;
			tree.parent = flyingNode;
			flyingNode.left = tree;
			flyingNode.right = new TwoThreeCell();
			flyingNode.right.threeNode = false;
			flyingNode.right.parent = flyingNode;
			flyingNode.right.p1 = new Pair();
			flyingNode.right.p1.setKey( key );
			flyingNode.right.p1.setVal( 1 );
			if( tree == root )
			    root = flyingNode;
			else
			    upward( flyingNode.parent , flyingNode );
		    }
		    else
			downward( tree.right , key );
		}
		else if( aux == 0 ){
		    tree.p2.valInc();
		}
		else{ /* aux > 0 */
		    if( tree.middle == null ){
			flyingNode = new TwoThreeCell();
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
			if( tree == root )
			    root = flyingNode;
			else
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
		    flyingNode = new TwoThreeCell();
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
		    if( tree == root )
			root = flyingNode;
		    else
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

    private void debug_print( TwoThreeCell c ){
	if( c != null ){
	    debug_print( c.left );
	    if( c.p1 != null )
		System.out.println( c.p1.getKey() + "\t" + c.p1.getVal() );
	    debug_print( c.middle );
	    if( c.p2 != null )
		System.out.println( c.p2.getKey() + "\t" + c.p2.getVal() );
	    debug_print( c.right );
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

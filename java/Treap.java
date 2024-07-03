public class Treap implements SymbolTable{
    TreapCell root;

    public Treap( String filename ){
	root = new TreapCell();
	root.genPri();
	readFile( filename );
    }

    private TreapCell split( TreapCell node , String key ){
	TreapCell ret = new TreapCell();
	if( node != null ){
	    int aux = node.getKey().compareToIgnoreCase( key );
	    if( aux < 0 ){
		ret.left = node;
		TreapCell bogusNode = split( node.right , key );
		ret.left.right = bogusNode.left;
		ret.right = bogusNode.right;
	    }
	    else{
		ret.right = node;
		TreapCell bogusNode = split( node.left , key );
		ret.right.left = bogusNode.right;
		ret.left = bogusNode.left;
	    }
	}
	return ret;
    }

    public void insert( String key ){
	int aux = 0;
	boolean found = false;
	TreapCell c = root, prev = root, 
	    newC, target = root, targetParent = root;
	if( c.getKey() == null ){
	    c.setKey( key );
	    c.setVal( 1 );
	    return;
	}

	newC = new TreapCell();
	newC.genPri();
	while( c != null ){
	    if( !found && newC.getPri() > c.getPri() ){
		found = true;
		target = c;
		targetParent = prev;
	    }
	    else
		prev = c;
	    aux = c.getKey().compareToIgnoreCase( key );
	    if( aux < 0 )
		c = c.right;
	    else if( aux == 0 ){
		c.valInc();
		return;
	    }
	    else
		c = c.left;
	}

	if( found ){
	    if( targetParent == target )
		root = newC;
	    else if( targetParent.left == target )
		targetParent.left = newC;
	    else /* targetParent.right == target */
		targetParent.right = newC;
	    target = split( target, key );
	    newC.left = target.left;
	    newC.right = target.right;
	}
	else{
	    if( aux < 0 )
		prev.right = newC;
	    else
		prev.left = newC;
	}
	newC.setKey( key );
	newC.setVal( 1 );
    }

    public int retrieve( String key ){
	int aux;
	TreapCell c = root;

	if( c.getKey() == null )
	    return 0;

	while( c != null ){
	    aux = c.getKey().compareToIgnoreCase( key );
	    if( aux < 0 )
		c = c.right;
	    else if( aux == 0 )
		return c.getVal();
	    else
		c = c.left;
	}

	return 0;
    }

    public void debug_print( TreapCell c ){
	if( c != null ){
	    debug_print( c.left );
	    System.out.println( c.getKey() + ":\t" + c.getVal() + "\tpri:" + c.getPri() );
	    debug_print( c.right );
	}
    }

    public void remove( String key ){
	if( root.getKey() == null )
	    return;

	if( root.getKey().compareToIgnoreCase( key ) == 0 ){
	    if( root.left == null && root.right == null ){
		root = new TreapCell();
	    }
	    else if( root.left == null ){
		root = root.right;
	    }
	    else if( root.right == null ){
		root = root.left;
	    }
	    else{
		TreapCell maxLeft = root.left;
		while( maxLeft.right != null )
		    maxLeft = maxLeft.right;
		maxLeft.right = root.right;
		root = root.left;
	    }
	    return;
	}

	int aux;
	TreapCell c = root, prev = root;

	while( c != null ){
	    aux = c.getKey().compareToIgnoreCase( key );
	    if( aux < 0 ){
		prev = c;
		c = c.right;
	    }
	    else if( aux == 0 ){
		if( c.left == null && c.right == null ){
		    if( c == prev.left )
			prev.left = null;
		    else
			prev.right = null;
		}
		else if( c.left == null ){
		    if( c == prev.left )
			prev.left = c.right;
		    else
			prev.right = c.right;
		}
		else if( c.right == null ){
		    if( c == prev.left )
			prev.left = c.left;
		    else
			prev.right = c.left;
		}
		else{ /* both children not null */
		    TreapCell maxLeft = c.left;
		    while( maxLeft.right != null )
			maxLeft = maxLeft.right;
		    maxLeft.right = c.right;
		    if( c == prev.left )
			prev.left = c.left;
		    else
			prev.right = c.left;
		}
		return;
	    }
	    else{
		prev = c;
		c = c.left;
	    }
	}
	System.out.printf( "Not " );
    }

    public int rank( String key ){
	if( root.getKey() == null )
	    return 0;

	int aux, count = 0;
	TreapCell c = root;

	while( c != null ){
	    aux = c.getKey().compareToIgnoreCase( key );
	    if( aux < 0 ){
		count += 1 + countR( c.left );
		c = c.right;
	    }
	    else if( aux == 0 )
		return count + countR( c.left );
	    else
		c = c.left;
	}
	return count;
    }

    private int countR( TreapCell c ){
	if( c != null )
	    return countR( c.left ) + 1 + countR( c.right );
	return 0;
    }

    public String select( int rank ){
	if( rank < 0 || root.getKey() == null )
	    return "";

	int count = 0, testCount;
	TreapCell c = root;

	while( c!= null ){
	    testCount = countR( c.left );
	    if( count + testCount < rank ){
		count += 1 + testCount;
		c = c.right;
	    }
	    else if( count + testCount == rank )
		return c.getKey();
	    else /* count + testCount > rank */
		c = c.left;
	}
	return "";
    }
}

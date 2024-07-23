public class ArvoreBin implements SymbolTable{
    private BinTreeCell root;

    public ArvoreBin( String filename ){
	root = new BinTreeCell();
	readFile( filename );
    }

    public void insert( String key ){
	int aux = 0;
	BinTreeCell c = root, prev = root;
	if( c.getKey() == null ){
	    c.setKey( key );
	    c.setVal( 1 );
	    return;
	}

	while( c != null ){
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

	if( aux < 0 ){
	    prev.right = new BinTreeCell();
	    prev.right.setKey( key );
	    prev.right.setVal( 1 );
	}
	else{
	    prev.left = new BinTreeCell();
	    prev.left.setKey( key );
	    prev.left.setVal( 1 );
	}
    }

    public int retrieve( String key ){
	int aux;
	BinTreeCell c = root;

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

    public void debug_print( BinTreeCell c ){
	if( c != null ){
	    debug_print( c.left );
	    System.out.println( c.getKey() + ":\t" + c.getVal() );
	    debug_print( c.right );
	}
    }

    public void remove( String key ){
	if( root.getKey() == null )
	    return;

	if( root.getKey().compareToIgnoreCase( key ) == 0 ){
	    if( root.left == null && root.right == null ){
		root = new BinTreeCell();
	    }
	    else if( root.left == null ){
		root = root.right;
	    }
	    else if( root.right == null ){
		root = root.left;
	    }
	    else{
		BinTreeCell maxLeft = root.left;
		while( maxLeft.right != null )
		    maxLeft = maxLeft.right;
		maxLeft.right = root.right;
		root = root.left;
	    }
	    return;
	}

	int aux;
	BinTreeCell c = root, prev = root;

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
		    BinTreeCell maxLeft = c.left;
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
	BinTreeCell c = root;

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

    private int countR( BinTreeCell c ){
	if( c != null )
	    return countR( c.left ) + 1 + countR( c.right );
	return 0;
    }

    public String select( int rank ){
	if( rank < 0 || root.getKey() == null )
	    return "";

	int count = 0, testCount;
	BinTreeCell c = root;

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

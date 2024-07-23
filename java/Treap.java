/*
https://cp-algorithms.com/data_structures/treap.html
*/

public class Treap implements SymbolTable{
    private TreapCell root;

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

    private TreapCell merge( TreapCell T1 , TreapCell T2 ){
	if( T1 == null && T2 == null )
	    return null;
	else if( T1 == null )
	    return T2;
	else if( T2 == null )
	    return T1;
	else{ /* T1 != null && T2 != null */
	    if( T1.getPri() > T2.getPri() ){
		T1.right = merge( T1.right , T2 );
		return T1;
	    }
	    else{
		T2.left = merge( T1 , T2.left );
		return T2;
	    }
	}
    }

    public void remove( String key ){
	if( root.getKey() == null )
	    return;

	if( root.getKey().compareToIgnoreCase( key ) == 0 ){
	    root = merge( root.left , root.right );
	    if( root == null ){
		root = new TreapCell();
		root.genPri();
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
		if( prev.left == c )
		    prev.left = merge( c.left , c.right );
		else /* prev.right == c */
		    prev.right = merge( c.left , c.right );
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

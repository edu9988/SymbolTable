public class ListaDes implements SymbolTable{
    ListCell head;

    public ListaDes( String filename ){
	head = new ListCell();
	readFile( filename );
    }

    public void insert( String key ){
	ListCell c = head;

	while( c.next != null ){
	    c = c.next;
	    if( c.getKey().equals( key ) ){
		c.valInc();
		return;
	    }
	}

	c.next = new ListCell();
	c = c.next;
	c.setKey( key );
	c.setVal( 1 );
    }

    public int retrieve( String key ){
	ListCell c = head;

	while( c.next != null ){
	    c = c.next;
	    if( c.getKey().equals( key ) ){
		return c.getVal();
	    }
	}

	return 0;
    }
    
    public void debug_print(){
	int i = 0;
	ListCell c = head;

	while( c.next != null ){
	    c = c.next; i++;
	    System.out.println(i+"\t"+c.getKey()
		+"\t"+c.getVal() );
	}
    }

    public void remove( String key ){
	ListCell c = head, ant;

	while( c.next != null ){
	    ant = c;
	    c = c.next;
	    if( c.getKey().equals( key ) ){
		ant.next = c.next;
		return;
	    }
	}
	System.out.printf( "Not " );
    }

    public int rank( String key ){
	int count = 0;
	ListCell c = head;
	
	while( c.next != null ){
	    c = c.next;
	    if( c.getKey().compareToIgnoreCase( key ) < 0 )
		count++;
	}

	return count;
    }

    public String select( int rank ){
	if( rank < 0 )
	    return "";
	int n = 0;
	ListCell c = head;

	while( c.next != null ){
	    c = c.next;
	    n++;
	}
	if( rank >= n )
	    return "";

	ListCell[] pointers = new ListCell[n]; 
	c = head.next;
	for( int i=0 ; i<n ; i++ ){
	    pointers[i] = c;
	    c = c.next;
	}
	qS( pointers , rank , 0 , n-1 );
	return pointers[rank].getKey();
    }

    private void qS( ListCell[] pointer , int target , int p , int r ){
	if( p <= r ){
	    int q = separate( pointer , p , r );
	    if( q < target )
		qS( pointer , target , q+1 , r );
	    else
		qS(pointer , target , p , q-1 );
	}
    }

    private int separate( ListCell[] pointer, int p , int r ){
	int i, j, diff;
	ListCell aux;
	for( j=p-1 , i=p ; i<=r ; i++ ){
	    diff = pointer[i].getKey().compareToIgnoreCase( pointer[r].getKey() );
	    if( diff <= 0 ){
		aux = pointer[++j];
		pointer[j] = pointer[i];
		pointer[i] = aux;
	    }
	}
	return j;
    }
}

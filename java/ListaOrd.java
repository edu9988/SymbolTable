public class ListaOrd implements SymbolTable{
    ListCell head;

    public ListaOrd( String filename ){
	head = new ListCell();
	readFile( filename );
    }

    public void insert( String key ){
	int aux;
	ListCell c = head, prev;

	while( c.next != null ){
	    prev = c;
	    c = c.next;
	    aux = c.getKey().compareToIgnoreCase( key );
	    if( aux == 0 ){
		c.valInc();
		return;
	    }
	    else if( aux > 0 ){
		c = new ListCell();
		c.next = prev.next;
		prev.next = c;
		c.setKey( key );
		c.setVal( 1 );
		return;
	    }
	}

	c.next = new ListCell();
	c = c.next;
	c.setKey( key );
	c.setVal( 1 );
    }

    public int retrieve( String key ){
	int aux;
	ListCell c = head;

	while( c.next != null ){
	    c = c.next;
	    aux = c.getKey().compareToIgnoreCase( key );
	    if( aux == 0 ){
		return c.getVal();
	    }
	    else if( aux > 0 )
		break;
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
	int aux;
	ListCell c = head, ant;

	while( c.next != null ){
	    ant = c;
	    c = c.next;
	    aux = c.getKey().compareToIgnoreCase( key );
	    if( aux == 0 ){
		ant.next = c.next;
		return;
	    }
	    if( aux >= 0 )
		break;
	}
	System.out.printf( "Not " );
    }

    public int rank( String key ){
	int count = 0, aux;
	ListCell c = head;
	
	while( c.next != null ){
	    c = c.next;
	    aux = c.getKey().compareToIgnoreCase( key );
	    if( aux < 0 )
		count++;
	    else
		break;
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
	    if( n == rank )
		return c.getKey();
	    n++;
	}
	return "";
    }
}

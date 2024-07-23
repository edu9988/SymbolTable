public class VetorDes implements SymbolTable{
    private Pair[] v;
    private int last; //first empty position

    public VetorDes( String filename ){
	v = new Pair[4];
	readFile( filename );
    }

    public void insert( String key ){
	int i;

	for( i=0 ; i<last ; i++ ){
	    if( v[i].getKey().equals( key ) ){
		v[i].valInc();
		return;
	    }
	}

	if( last == v.length )
	    resize();
	v[last] = new Pair();
	v[last].setKey( key );
	v[last++].setVal( 1 );
    }

    public int retrieve( String key ){
	for( int i=0 ; i<last ; i++ ){
	    if( v[i].getKey().equals( key ) ){
		return v[i].getVal();
	    }
	}
	return 0;
    }
    
    public void debug_print(){
	System.out.println("\nlast:"+last);
	System.out.println("v.length:"+v.length);
	for( int i=0 ; i<last ; i++ )
	    System.out.println(i+"\t"+v[i].getKey()
		+"\t"+v[i].getVal() );
    }

    public void remove( String key ){
	int i;

	for( i=0 ; i<last ; i++ ){
	    if( v[i].getKey().equals( key ) ){
		v[i] = v[--last];
		return;
	    }
	}
	System.out.printf( "Not " );
    }

    public int rank( String key ){
	int i, count = 0;
	
	for( i=0 ; i<last ; i++ ){
	    if( v[i].getKey().compareToIgnoreCase( key ) < 0 )
		count++;
	}
	return count;
    }

    public String select( int rank ){
	if( rank<0 || rank>= last )
	    return "";
	int[] pointers = new int[last]; 
	for( int i=0 ; i<last ; i++ )
	    pointers[i] = i;
	qS( pointers , rank , 0 , last-1 );
	return v[pointers[rank]].getKey();
    }

    private void resize(){
	int i;
	Pair[] aux = new Pair[2*v.length];
	for( i=0; i<last ; i++ )
	    aux[i] = v[i];
	v = aux;
    }

    private void qS( int[] pointer , int target , int p , int r ){
	if( p <= r ){
	    int q = separate( pointer , p , r );
	    if( q < target )
		qS( pointer , target , q+1 , r );
	    else
		qS(pointer , target , p , q-1 );
	}
    }

    private int separate( int[] pointer, int p , int r ){
	int i, j, diff, aux;
	for( j=p-1 , i=p ; i<=r ; i++ ){
	    diff = v[pointer[i]].getKey().compareToIgnoreCase( v[pointer[r]].getKey() );
	    if( diff <= 0 ){
		aux = pointer[++j];
		pointer[j] = pointer[i];
		pointer[i] = aux;
	    }
	}
	return j;
    }
}

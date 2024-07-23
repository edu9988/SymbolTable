public class VetorOrd implements SymbolTable{
    private Pair[] v;
    private int last; //first empty position

    public VetorOrd( String filename ){
	v = new Pair[4];
	readFile( filename );
    }

    public void insert( String key ){
	int aux, i = rank( key );

	if( i<last && v[i].getKey().equals( key ) )
	    v[i].valInc();
	else{
	    if( last == v.length )
		resize();
	    for( aux=last ; aux>i ; aux-- )
		v[aux] = v[aux-1];
	    v[i] = new Pair();
	    v[i].setKey( key );
	    v[i].setVal( 1 );
	    last++;
	}
    }

    public int retrieve( String key ){
	if( last > 0 ){
	    int i = rank( key );
	    if( i<last && v[i].getKey().equals( key ) )
		return v[i].getVal();
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
	if( last == 0 )
	    return;
	int i = rank( key );

	if( i<last && v[i].getKey().equals( key ) ){
	    while( i < last-1 )
		v[i] = v[++i];
	    last--;
	}
	else
	    System.out.printf( "Not " );
    }

    public int rank( String key ){
	int i, left, right, aux;
	
	left = 0;
	right = last-1;
	while( left <= right ){/*binary search*/
	    i = (left+right)/2;
	    aux = v[i].getKey().compareToIgnoreCase( key );
	    if( aux == 0 )
		return i;
	    else if( aux < 0 )
		left = i+1;
	    else
		right = i-1;
	}
	return left;
    }

    public String select( int rank ){
	if( rank<0 || rank>= last )
	    return "";
	return v[rank].getKey();
    }

    private void resize(){
	int i;
	Pair[] aux = new Pair[2*v.length];
	for( i=0; i<last ; i++ )
	    aux[i] = v[i];
	v = aux;
    }
}

public class Tester{
    public static void main( String[] args ){
	BinTreeCell root = new BinTreeCell();
	if( root.getKey() == null )
	    System.out.println( "null: "+root.getKey() );
	else
	    System.out.println( "not null: "+root.getKey() );
    }
}

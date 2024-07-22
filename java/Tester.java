public class Tester{
    public static void main( String[] args ){
	TwoThreeCell root = new TwoThreeCell();
	if( root.left == null )
	    System.out.println( "root.left null:"+root.left );
	else
	    System.out.println( "root.left not null:"+root.left  );
    }
}

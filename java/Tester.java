public class Tester{
    public static void main( String[] args ){
	TwoThreeCell root = new TwoThreeCell();
	root.p1 = new Pair();
	System.out.println( "p1 value:" + root.p1.getVal() );
	/*
	if( root.left == null )
	    System.out.println( "root.left null:"+root.left );
	else
	    System.out.println( "root.left not null:"+root.left  );
	*/
    }
}

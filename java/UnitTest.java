import java.util.Scanner; // Import the Scanner class to read user input
import java.util.StringTokenizer;

public class UnitTest{
    public static void main( String[] args ){
	SymbolTable ST;
	long start, end;
	double elapsed;

	if( args.length == 2){
	    System.out.println( "creating ST..." );

	    start = System.nanoTime();
	    if( args[1].equals( "VD" ) )
		ST = new VetorDes( args[0] );
	    else if( args[1].equals( "VO" ) )
		ST = new VetorOrd( args[0] );
	    else if( args[1].equals( "LD" ) )
		ST = new ListaDes( args[0] );
	    else if( args[1].equals( "LO" ) )
		ST = new ListaOrd( args[0] );
	    else if( args[1].equals( "AB" ) )
		ST = new ArvoreBin( args[0] );
	    /*
	    else if( args[1].equals( "TR" ) )
		ST = new Treap( args[0] );
	    */
	    else{
		System.out.println( "Unkown type" );
		return;
	    }
	    end = System.nanoTime();
	    elapsed = (end-start)/1.0e+9;

	    System.out.printf( "file read and ST built in "
		+"%.9f s\n",
		elapsed
	    );

	    testOperations( ST );
	}

	else{
	    System.out.println( "Usage:\n"
		+ "prompt> java "
		+ UnitTest.class.getSimpleName()
		+ " file_name table_type"
		+ "\n\t file_name = name of file containing text to be read"
		+ "\n\ttable_type = initials of data structure to be used"
		+ "\n\t             (VD, VO, LD, LO, AB, TR, A23, RN, HS)" );
	}
    }

    private static void testOperations( SymbolTable ST ){
	long start, end;
	double elapsed;

	System.out.println( "Interactive test operations:\n"
	    + "minST, delminST, getST <key>, rankST <key>"
	    + ", deleteST <key>, selectST <int>\n"
	    + "CTRL-D to finish"
	);

	try{
	    System.out.printf( ">>>" );
	    Scanner myReader = new Scanner( System.in );
	    while( myReader.hasNextLine() ){
		String data = myReader.nextLine();
		StringTokenizer tok = new StringTokenizer( data );
		if( tok.hasMoreTokens() ){
		    String op = tok.nextToken();

		    if( op.isEmpty() ){
			System.out.println( "Error: Expected operation" );
		    }

		    else if( op.equals( "minST" ) ){
			start = System.nanoTime();
			String result = ST.select(0);
			end = System.nanoTime();
			elapsed = (end-start)/1.0e+9;
			if( result.isEmpty() )
			    result = "Empty ST";
			System.out.printf(
			    result
			    +"\t\t\t"
			    + "%.2e s\n",
			    elapsed
			);
		    }

		    else if( op.equals( "delminST" ) ){
			start = System.nanoTime();
			String result = ST.select(0);
			if( result.isEmpty() ){
			    System.out.println( "ST already empty" );
			}
			else{
			    ST.remove(result);
			    end = System.nanoTime();
			    elapsed = (end-start)/1.0e+9;
			    System.out.printf(
				"\""+result
				+"\" removed \t"
				+"%.2e s\n",
				elapsed
			    );
			}
		    }

		    else if ( tok.hasMoreTokens() ){
			/* operation needs key argument */
			String key = tok.nextToken();
			if( key.isEmpty() )
			    System.out.println( "empty argument" );
			else{ /* key not empty */ 
			    if( op.equals( "getST" ) ){
				int frequency;
				start = System.nanoTime();
				frequency = ST.retrieve( key );
				end = System.nanoTime();
				elapsed = (end-start)/1.0e+9;
				System.out.printf(
				    key+": "+frequency
				    +"\t\t\t%.2e s\n",
				    elapsed
				);
			    }

			    else if( op.equals( "rankST" ) ){
				int r;
				start = System.nanoTime();
				r = ST.rank( key );
				end = System.nanoTime();
				elapsed = (end-start)/1.0e+9;
				System.out.printf(
				    r+"\t\t\t%.2e s\n",
				    elapsed
				);
			    }

			    else if( op.equals( "deleteST" ) ){
				start = System.nanoTime();
				ST.remove( key );
				end = System.nanoTime();
				elapsed = (end-start)/1.0e+9;
				System.out.printf(
				    "\""+key+"\" removed\t\t\t%.2e s\n",
				    elapsed
				);
			    }

			    else if( op.equals( "selectST" ) ){
				int pos;
				try{ pos = Integer.parseInt( key ); }
				catch( Exception e ){ pos = 0; }
				start = System.nanoTime();
				String res = ST.select( pos );
				end = System.nanoTime();
				elapsed = (end-start)/1.0e+9;
				System.out.printf(
				    "Position "+key+" = "+res+"\t\t%.2e s\n",
				    elapsed
				);
			    }

			    else{
				System.out.println( "ERROR: Unkown command" );
			    }
			}
		    }
		    else{ /* missing key argument */
			System.out.println( "Missing operation argument" );
		    }

		}
		System.out.printf( ">>>" );
	    }
	    System.out.printf( "\n" );
	    myReader.close();
	} 
	catch( Exception e ){
	    System.out.println( "An error occurred." );
	    e.printStackTrace();
	}
    }
}

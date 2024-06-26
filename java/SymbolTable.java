import java.io.File;  // Import the File class
import java.io.FileNotFoundException;  // Import this class to handle errors
import java.util.Scanner; // Import the Scanner class to read text files

public interface SymbolTable{
    /* no need to use 'public', since
    it is the default in interfaces */
    void insert( String key );
    int retrieve( String key );
    void remove( String key );
    int rank( String key );
    String select( int rank );

    default void readFile( String filename ){
	try{
	    File myObj = new File( filename );
	    Scanner myReader = new Scanner( myObj ).useDelimiter( "([^a-zA-Z]|\\s)" );
	    while( myReader.hasNext() ){
		String data = myReader.next();
		if( !( data.isEmpty() ) )
		    insert( data.toLowerCase() );
	    }
	    myReader.close();
	} 
	catch( FileNotFoundException e ){
	    e.printStackTrace();
	    System.out.println( "\nWARNING: Symbol table empty\n" );
	}
    }
}

public class Pair{
    private String key;
    private int value;

    public void setKey( String in ){
	key = in;
    }

    public void setVal( int in ){
	value = in;
    }

    public String getKey(){
	return key;
    }

    public int getVal(){
	return value;
    }

    public void valInc(){
	value++;
    }
}

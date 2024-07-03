import java.util.Random;

public class TreapCell extends Pair{
    public TreapCell left, right;
    private int priority;

    public void genPri(){
	Random r = new Random();
	priority = r.nextInt();
    }

    public int getPri(){
	return priority;
    }
}


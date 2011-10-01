public class testprog {
	
	public static void testfunc( jnitest z ) {
		System.out.println("Func Wert: " + z.getNumber());
	}
	

	public static void main (String[] args) {
		
		jnitest x = new jnitest(5);
		System.out.println("Wert: "+x.getNumber());
        
        x.setNumber(10);
        System.out.println("Wert: "+x.getNumber());


		jnitest y = new jnitest(50);
		System.out.println("Wert: "+y.getNumber());
		
		testfunc(y);
		
		x.dispose();
		y.dispose();
	}
}

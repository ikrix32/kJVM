package platform;

public class Touch{
	static boolean[] touched = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};
	public Touch(){
		
	}
	
	public static boolean isTouched(int i){
		return System.platform.isButtonPressed(i);
	}
	
	public static boolean[] getTouched(){
	  for(int i=0; i<=15; i++)
	    touched[i] = isTouched(i);
	    return touched;
	}
}

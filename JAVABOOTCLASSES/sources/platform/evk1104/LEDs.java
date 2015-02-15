package platform;

public class LEDs{
	
	boolean [] leds = {false,false,false,false};
	int bitmask = 0;
	
	public LEDs(){
    System.platform.setLEDoff(0xf);
	}
	
	public void setLEDs(boolean [] led){
	  leds = led;
	  bitmask = 0x0;
	  if(leds[0])
	    bitmask += 1;
	  if(leds[1])
	    bitmask += 2;
	  if(leds[2])
	    bitmask += 4;
	  if(leds[3])
	    bitmask += 8;
	}
	
	public void activateBits(int bitmask){
	  leds[0] = (bitmask & 0x1) == 1;
    leds[1] = (bitmask & 0x2) == 1;
    leds[2] = (bitmask & 0x4) == 1;
    leds[3] = (bitmask & 0x8) == 1;
	  System.platform.setLEDon(bitmask);
    System.platform.setLEDoff(bitmask^0xf);
	}
	
	public void activate(){
	  System.platform.setLEDon(bitmask);
    System.platform.setLEDoff(bitmask^0xf);
	}
	
	public void activate(boolean [] leds){
	  setLEDs(leds);
	  activate();
	}
	
	public void activate(int i){
	   leds[i] = true;
	   activate(leds);
	}
	
	public void deactivate(int i){
	  leds[i] = false;
	  activate(leds);
	}
	
	public void toggle(int i){
	  if(0 < i && i < 5){
	    leds[i-1] = leds[i-1]^true;
	    activate(leds);
	  }
	}
	
}

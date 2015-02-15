package platform;

public class ADC
{
  public ADC(){
    System.platform.adcInit();
  }
  
  public void enable(){
    System.platform.adcEnable();
  }
  
  public void disable(){
    System.platform.adcDisable();
  }
  
  public int getValue(){
    return System.platform.adcGetValue();
  }


}

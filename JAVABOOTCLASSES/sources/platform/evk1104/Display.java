package platform;

public class Display
{

	private int xMax		= 320;
	private int yMax		= 240;	
	private int color, bgColor;

    public Display() {
      System.platform.initDisplay();
    	color = 0x0000;
    	bgColor = 0xffff;
    	clearDisplay();
    }        

    public void setColor(int i)
    {
    	color = i;
    }

    public void setBackgroundColor(int i)
    {
    	bgColor = i;
    	clearDisplay();
    }
    
    public void drawPoint(int i, int j) 
    {
    	System.platform.drawPixel(i, j, color);		
	  }
	  
    public void drawPoint(int i, int j, int color) 
    {
    	System.platform.drawPixel(i, j, color);		
	  }

    public void drawRect(int x1, int y1, int width, int height, int lineWidth)
    {
    	drawFillRect(x1, y1, width, height);
    	drawClearRect(x1+lineWidth, y1+lineWidth, width-2*lineWidth, height-2*lineWidth);
    }
    
    public void drawRect(int x1, int y1, int width, int height, int lineWidth, int color)
    {
    	System.platform.drawFillRect(x1, y1, width, height, color);
    	System.platform.drawFillRect(x1+lineWidth, y1+lineWidth, width-2*lineWidth, height-2*lineWidth, bgColor);
    }
    
    public void drawRect(int x1, int y1, int width, int height, int lineWidth, int color, int bgColor)
    {
    	System.platform.drawFillRect(x1, y1, width, height, color);
    	System.platform.drawFillRect(x1+lineWidth, y1+lineWidth, width-2*lineWidth, height-2*lineWidth, bgColor);
    }

    public void drawFillRect(int x1, int y1, int width, int height)
    {
    	System.platform.drawFillRect(x1, y1, width, height, color);
    }
    
    public void drawFillRect(int x1, int y1, int width, int height, int color)
    {
    	System.platform.drawFillRect(x1, y1, width, height, color);
    }

    public void drawClearRect(int x1, int y1, int width, int height)
    {
    	System.platform.drawFillRect(x1, y1, width, height, bgColor);  	
    }

    public void clearDisplay()
    {
    	System.platform.clearDisplay(bgColor);
    }
    
    public void drawCircle(int x, int y, int r, int lineWidth)
    {
    	drawFillCircle(x, y, r);
    	drawClearCircle(x, y, r-lineWidth);
    }
    
    public void drawCircle(int x, int y, int r, int lineWidth, int color, int bgColor)
    {
    	System.platform.drawFilledCircle(x, y, r, color, 0xFF);
    	System.platform.drawFilledCircle(x, y, r-lineWidth, bgColor, 0xFF);
    }
    
    public void drawCircle(int x, int y, int r, int lineWidth, int color)
    {
    	System.platform.drawFilledCircle(x, y, r, color, 0xFF);
    	System.platform.drawFilledCircle(x, y, r-lineWidth, bgColor, 0xFF);
    }
    
    public void drawFillCircle(int x, int y, int r)
    {
    	System.platform.drawFilledCircle(x, y, r, color, 0xFF);
    }
    
    public void drawFillCircle(int x, int y, int r, int color)
    {
    	System.platform.drawFilledCircle(x, y, r, color, 0xFF);
    }
    
    public void drawPartialFillCircle(int x, int y, int r, int bitMask)
    {
    	System.platform.drawFilledCircle(x, y, r, color, bitMask);
    }
    
    public void drawPartialFillCircle(int x, int y, int r, boolean[] array)
    {
    	System.platform.drawFilledCircle(x, y, r, color, arrayToBitmask(array));
    }
    
    public void drawPartialCircle(int x, int y, int r, int lineWidth, int bitMask)
    {
    	System.platform.drawFilledCircle(x, y, r, color, bitMask);
    	System.platform.drawFilledCircle(x, y, r-lineWidth, bgColor, bitMask);
    }
    
    public void drawPartialCircle(int x, int y, int r, int lineWidth, boolean[] array)
    {
      int bitMask = arrayToBitmask(array);
    	System.platform.drawFilledCircle(x, y, r, color, bitMask);
    	System.platform.drawFilledCircle(x, y, r-lineWidth, bgColor, bitMask);
    }
    
    public void drawPartialFillCircle(int x, int y, int r, int bitMask, int color)
    {
    	System.platform.drawFilledCircle(x, y, r, color, bitMask);
    }
    
    public void drawPartialFillCircle(int x, int y, int r, boolean[] array, int color)
    {
    	System.platform.drawFilledCircle(x, y, r, color, arrayToBitmask(array));
    }
    
    public void drawPartialCircle(int x, int y, int r, int lineWidth, int bitMask, int color)
    {
    	System.platform.drawFilledCircle(x, y, r, color, bitMask);
    	System.platform.drawFilledCircle(x, y, r-lineWidth, bgColor, bitMask);
    }
    
    public void drawPartialCircle(int x, int y, int r, int lineWidth, boolean[] array, int color)
    {
      int bitMask = arrayToBitmask(array);
    	System.platform.drawFilledCircle(x, y, r, color, bitMask);
    	System.platform.drawFilledCircle(x, y, r-lineWidth, bgColor, bitMask);
    }
    
    public void drawPartialCircle(int x, int y, int r, int lineWidth, int bitMask, int color, int bgColor)
    {
    	System.platform.drawFilledCircle(x, y, r, color, bitMask);
    	System.platform.drawFilledCircle(x, y, r-lineWidth, bgColor, bitMask);
    }
    
    public void drawPartialCircle(int x, int y, int r, int lineWidth, boolean[] array, int color, int bgColor)
    {
      int bitMask = arrayToBitmask(array);
    	System.platform.drawFilledCircle(x, y, r, color, bitMask);
    	System.platform.drawFilledCircle(x, y, r-lineWidth, bgColor, bitMask);
    }
       
    public void drawClearCircle(int x, int y, int r)
    {
    	System.platform.drawFilledCircle(x, y, r, bgColor, 0xFF);
    }
    
    public void drawLine(int x1, int y1, int x2, int y2)
    {
    	System.platform.drawLine(x1, y1, x2, y2, color);
    }
    
    public void drawLine(int x1, int y1, int x2, int y2, int color)
    {
    	System.platform.drawLine(x1, y1, x2, y2, color);
    }
    
    public void drawHorizLine(int x1, int y1, int length, int color)
    {
    	System.platform.drawHorizLine(x1, y1, length, color);
    }
    
    public void drawHorizLine(int x1, int y1, int length)
    {
    	System.platform.drawHorizLine(x1, y1, length, color);
    }
    
    public void drawVertLine(int x1, int y1, int length, int color)
    {
    	System.platform.drawVertLine(x1, y1, length, color);
    }
    
    public void drawVertLine(int x1, int y1, int length)
    {
    	System.platform.drawVertLine(x1, y1, length, color);
    }
    
    public void drawString(String s, int font, int x, int y, int color, int bgColor){
      for (int i = 0 ; i < s.length() ; i++) {
			  drawCharIndex(s.charAt(i),font,x,y,i,color,bgColor);
		  }
    }
    
    public void drawString(String s, int font, int x, int y, int color){
      for (int i = 0 ; i < s.length() ; i++) {
			  drawCharIndex(s.charAt(i),font,x,y,i,color,bgColor);
		  }
    }
    
    public void drawString(String s, int font, int x, int y){
      drawString(s,font,x,y,color,bgColor);
    }
    
    public void drawChar(char s, int font, int x, int y, int color, int bgColor){
      drawCharIndex(s,font,x,y,0,color,bgColor);
    }
    
    public void drawChar(char s, int font, int x, int y, int color){
      drawCharIndex(s,font,x,y,0,color,bgColor);
    }
    
    public void drawChar(char s, int font, int x, int y){
      drawCharIndex(s,font,x,y,0,color,bgColor);
    }
    
    public void drawCharIndex(char s, int font, int x, int y, int index, int color, int bgColor){
      System.platform.drawChar(s,font,x,y,color,bgColor,index);
    }
    
    public void drawCharIndex(char s, int font, int x, int y, int index, int color){
      System.platform.drawChar(s,font,x,y,color,bgColor,index);
    }
    
    public void drawCharIndex(char s, int font, int x, int y, int index){
      System.platform.drawChar(s,font,x,y,color,bgColor,index);
    }
    
    /*public boolean[] bitmaskToArray(int bitMask){
      boolean[] array = {false, false, false, false};
      if((bitMask & 0x03) != 0x00)
        array[0] = true;
      if((bitMask & 0x0C) != 0x00)
        array[1] = true;
      if((bitMask & 0x30) != 0x00)
        array[2] = true;
      if((bitMask & 0xC0) != 0x00)
        array[3] = true;
      return array;
    }*/
    
    public int arrayToBitmask(boolean[] array){
      int bitMask = 0x00;
      if(array[0])
        bitMask += 0x03;
      if(array[1])
        bitMask += 0x0C;
      if(array[2])
        bitMask += 0x30;
      if(array[3])
        bitMask += 0xC0;
      return bitMask;
    }
}

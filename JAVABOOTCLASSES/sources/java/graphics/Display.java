package java.graphics;

public class Display implements DisplayKonstanten
{
	public Font font=new Font();
	int color, bgColor;

    public Display()
    {
    	color = 0x000000;
    	bgColor = 0xffffff;
    	clearDisplay();
    }        

    public void setColor(int i)
    {
    	color = i;
    }

    public void setBackground(int i)
    {
    	bgColor = i;
    	clearDisplay();
    }
    
    public void drawPoint(int i, int j) 
    {
    	System.platform.drawPointRGB(i, j, color);		
	}

    public void drawRectangle(int x1, int y1, int x2, int y2)
    {
    	System.platform.drawRectangleRGB(x1, y1, x2, y2, color, bgColor);
    }

    public void drawFillRect(int x1, int y1, int x2, int y2)
    {
    	System.platform.drawFillRectRGB(x1, y1, x2, y2, color);
    }

    public void drawClearRect(int x1, int y1, int x2, int y2)
    {
    	System.platform.drawFillRectRGB(x1, y1, x2, y2, bgColor);  	
    }

    public void clearDisplay()
    {
    	System.platform.clearScreenRGB(bgColor);
    }
    
    public void drawRoundRect(int x1, int y1, int x2, int y2, int a, int b)
    {
    	System.platform.drawRoundRectRGB(x1, y1, x2, y2, a, b, color, bgColor);
    }
    
    public void drawEllipse(int x1, int y1, int x2, int y2)
    {
    	System.platform.drawEllipseRGB(x1, y1, x2, y2, color, bgColor);
    }
    
    public void drawCircle(int x, int y, int r)
    {
    	System.platform.drawCircleRGB(x, y, r, color, bgColor);
    }
          
    /*public void drawLine(int x0, int y0, int x1, int y1)
    {
        //http://www.cs.unc.edu/~mcmillan/comp136/Lecture6/Lines.html
    	
    	//int pix = color.getRGB();
        int dx = x1 - x0;
        int dy = y1 - y0;

        //raster.setPixel(pix, x0, y0);
        drawPoint(x0, y0);	
        if (Math.abs(dx) > Math.abs(dy)) {          // slope < 1
            float m = (float) dy / (float) dx;      // compute slope
            float b = y0 - m*x0;
            dx = (dx < 0) ? -1 : 1;
            while (x0 != x1) {
                x0 += dx;
                //raster.setPixel(pix, x0, Math.round(m*x0 + b));
                drawPoint(x0, Math.round(m*x0 + b));	
            }
        } else
        if (dy != 0) {                              // slope >= 1
            float m = (float) dx / (float) dy;      // compute slope
            float b = x0 - m*y0;
            dy = (dy < 0) ? -1 : 1;
            while (y0 != y1) {
                y0 += dy;
                //raster.setPixel(pix, Math.round(m*y0 + b), y0);
                drawPoint(Math.round(m*y0 + b), y0);	
            }
        }
    }*/
    
    public void drawLine(int x1, int y1, int x2, int y2)
    {
    	//native Aufruf, nicht nach DDA, mit RGB-Farben
    	System.platform.drawLineRGB(x1, y1, x2, y2, color);
    }
}

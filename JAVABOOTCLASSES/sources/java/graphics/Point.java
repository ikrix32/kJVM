package java.graphics;

public class Point	
{
/*
public static double hue;		// globale Farben für Zustandsmaschine
public static double saturation;
public static double brightness;
*/
	float 	x,y,z;			// die Koordinaten
	float	h,s,b;			// 0.<= float <= 255. 
					// int Farbe als 32 Bit Wert 0000 0000 hhhh hhhh ssss ssss bbbb bbbb
	public Point()			
	{	
		this(0,0,0);	
	}	

	public Point(float x, float y, float z)		
	{
		setPoint(x,y,z);			
	}	

	public Point(float x,float y)
	{	
		this(x,y,0);	
	}

	public Point(Point p)				
	{	// nur fuer internen Gebrauch
		setPoint(p.x,p.y,p.z);
		HSBColor(p.h,p.s,p.b);		
	}

	public int getX()  	
	{ 
		return (int)(x);
	}
	
	public int getY()  	
	{ 
		return (int)(y);
	}
	
	public int getZ()  	
	{ 
		return (int)(z);
	}
	
	public int getH()  	
	{ 
		return (int)(h);
	}
	
	public int getS()	
	{ 
		return (int)(s);
	}
	
	public int getB()	
	{ 
		return (int)(b);
	}
	
	public void setPoint(float x, float y, float z)	
	{
		h=DisplayHSB.hue;
		s=DisplayHSB.saturation;
		b=DisplayHSB.brightness;
		this.x=x;
		this.y=y;
		this.z=z;		
	}
	
	public void setPoint(float x, float y )		
	{
		setPoint(x,y,0);			
	}

	public void HSBColor(float hue, float saturation, float brightness)	
	{  //nur fuer internen Gebrauch
		h	=hue;
		s	=saturation;
		b	=brightness;						
	}
/*
	public static void setHSBColor(float hue, float saturation, float brightness)	{
		MyCanvas.hue	=hue;
		MyCanvas.saturation=saturation;
		MyCanvas.brightness=brightness;						}
*/
	public String toString()		
	{				// debug
		return  (x+" "+y+" "+z+" ");	
	}	
}

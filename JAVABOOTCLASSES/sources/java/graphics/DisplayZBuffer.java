package java.graphics;

import java.util.Vector;

public class DisplayZBuffer extends DisplayHSB 
{
    public DisplayZBuffer()
    {
    	super();
    } 
	
	public void drawPoint(Point p)
    {
    	//mit zBuffer Algorithmus (Uebung 4)
		System.platform.drawPointHSBZBuffer(p.x, p.y, p.z, p.h, p.s, p.b);		
    }
	
	public void drawLine(Point pa, Point pe)
	{
		//mit ZBuffer
		System.platform.drawLineHSBZBuffer(pa.x, pa.y, pa.z, pa.h, pa.s, pa.b,
											pe.x, pe.y, pe.z, pe.h, pe.s, pe.b);		
	}
	
	public void scanFill(Polyline pl)
	{	//dda-2004
		Line l;
		Vector al	= new Vector();
		Vector loi	= new Vector();
		Point a		= new Point();
		Point b		= new Point();
		for (int i=0; i < pl.size();i++)
			if ((int)pl.pointAt(i).y != (int)pl.pointAt((i+1)%pl.size()).y)
				if (pl.pointAt(i).y > pl.pointAt((i+1)%pl.size()).y)
					al.addElement(new Line(pl.pointAt(i),pl.pointAt((i+1)%pl.size())));
				else			// eintragen der lines in Vektor, richtig rum
					al.addElement(new Line(pl.pointAt((i+1)%pl.size()),pl.pointAt(i)));
				// Parallelen zur y-Achse weglassen
		for (int j=0; j < al.size();j++)
			for (int i=1; i < al.size(); i++)	// sortieren der Lines nach y-Groesse
				if (((Line)al.elementAt(i-1)).anf.y < ((Line)al.elementAt(i)).anf.y)
				{
					l=(Line)al.elementAt(i-1);
					al.removeElementAt(i-1);
					if (i==al.size())
						al.add(l);
					else
						al.insertElementAt(l,i);
				}
				if (al.size()==0) return;
				int y=(int)((Line)al.elementAt(0)).anf.y;
				for (int i=0;i< al.size(); i++)
					if (y==(int)((Line)al.elementAt(i)).anf.y)
						loi.addElement((Line)al.elementAt(i));
					else break;  // da sie geordnet sind !!
				if ((loi.size() %2)!=0) System.out.println("hier stimmt was nicht");
		
		while (loi.size() !=0)
		{
			float [] xs= new float[loi.size()];
			float [] zs= new float[loi.size()];
			float [] hs= new float[loi.size()];
			float [] ss= new float[loi.size()];
			float [] bs= new float[loi.size()];
			
			for(int i=0; i< loi.size();i++)
				if (((int)((Line)loi.elementAt(i)).anf.y)==((int)((Line)loi.elementAt(i)).end.y))
				{
					xs[i]=((Line)loi.elementAt(i)).anf.x;
					zs[i]=((Line)loi.elementAt(i)).anf.z;
					hs[i]=((Line)loi.elementAt(i)).anf.h;
					ss[i]=((Line)loi.elementAt(i)).anf.s;
					bs[i]=((Line)loi.elementAt(i)).anf.b;
				}
				else
				{
					xs[i]=(((float)y-((Line)loi.elementAt(i)).anf.y)*
					(((Line)loi.elementAt(i)).end.x-((Line)loi.elementAt(i)).anf.x)/
					(((Line)loi.elementAt(i)).end.y-((Line)loi.elementAt(i)).anf.y)+
					((Line)loi.elementAt(i)).anf.x);
					zs[i]=(((float)y-((Line)loi.elementAt(i)).anf.y)*				
					(((Line)loi.elementAt(i)).end.z-((Line)loi.elementAt(i)).anf.z)/
					(((Line)loi.elementAt(i)).end.y-((Line)loi.elementAt(i)).anf.y)+
					((Line)loi.elementAt(i)).anf.z);
					hs[i]=(((float)y-((Line)loi.elementAt(i)).anf.y)*
					(((Line)loi.elementAt(i)).end.h-((Line)loi.elementAt(i)).anf.h)/
					(((Line)loi.elementAt(i)).end.y-((Line)loi.elementAt(i)).anf.y)+
					((Line)loi.elementAt(i)).anf.h);
					ss[i]=(((float)y-((Line)loi.elementAt(i)).anf.y)*		
					(((Line)loi.elementAt(i)).end.s-((Line)loi.elementAt(i)).anf.s)/
					(((Line)loi.elementAt(i)).end.y-((Line)loi.elementAt(i)).anf.y)+
					((Line)loi.elementAt(i)).anf.s);
					bs[i]=(((float)y-((Line)loi.elementAt(i)).anf.y)*
					(((Line)loi.elementAt(i)).end.b-((Line)loi.elementAt(i)).anf.b)/
					(((Line)loi.elementAt(i)).end.y-((Line)loi.elementAt(i)).anf.y)+
					((Line)loi.elementAt(i)).anf.b);
				}
			float help;	// zum merken
			for (int j=1;j<loi.size();j++)		//2004
				for(int i=1; i< loi.size();i++)	// sortieren
					if (xs[i]<xs[i-1])
					{
						help=xs[i];xs[i]=xs[i-1];xs[i-1]=help;
						help=zs[i];zs[i]=zs[i-1];zs[i-1]=help;
						help=hs[i];hs[i]=hs[i-1];hs[i-1]=help;
						help=ss[i];ss[i]=ss[i-1];ss[i-1]=help;
						help=bs[i];bs[i]=bs[i-1];bs[i-1]=help;
					}
			for (int i=1; i< xs.length;i+=2)
			{
				a.setPoint(xs[i-1],y,zs[i-1]);
				a.HSBColor(hs[i-1],ss[i-1],bs[i-1]);
				b.setPoint(xs[i],y,zs[i]);
				b.HSBColor(hs[i],ss[i],bs[i]);
				drawLine(a,b);
			}
			y-=1;		// weiter hoch
			for (int i=0; i < loi.size();i++)
				if ((int)((Line)loi.elementAt(i)).end.y ==y)
				{
					loi.removeElementAt(i);i--;
					if ( i < 0) i=-1;
				}// entfernen uninteressanter lines
			if (y >= yMax) System.out.println("hier stimmt was nicht");
			for (int i=0; i < al.size();i++)	// hinzufgen interessanter lines
				if ((int)((Line)al.elementAt(i)).anf.y == y)
				{
					loi.addElement((Line)al.elementAt(i)/*!!*/);
					al.removeElementAt(i);
					i--;
					if (i < 0) i=-1;
				} // aufessen
				//else break;  // hier muss irgend was in der Reihenfolgen aufsteigend absteigend //in al nicht stimmen
				//!! entfernen
				//!! hinzufgen
				//!! wegsortieren
				//System.exit(-4);
		}
	}
}

package java.graphics;

class Line		
{		// Hilfsklaasse , nur fuer internen Gebrauch
	Point anf,end;

	Line(Point anf,Point end)
    {
		this.anf=anf; 
		this.end=end;
	}		//Referenzkopie

	public String toString()
	{
		return ("anf " +anf.x+ " " + anf.y+" end " + end.x+" " +end.y);
	}
}

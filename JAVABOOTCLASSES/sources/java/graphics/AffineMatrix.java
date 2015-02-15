package java.graphics;

import java.util.*;

public class AffineMatrix
{
	float [][] m = new float[4][4];
	// 1. Indize Zeile \/
	// 2. Indize Spalte ->
	Stack st = new Stack();

	public AffineMatrix()
	{ 
		loadIdentity(); 
	}

	public AffineMatrix(AffineMatrix a)
	{ 
		load(a); 
	}

	public void loadIdentity()
	{// m=I
		for (int i=0; i < 4;i++)
			for (int j=0; j < 4;j++) 
				m[i][j]= ((i==j)?1f:0f);
	}

	public void pushMatrix()
	{
		st.push(m);
		m=new float[4][4];
		for (int i=0; i < 4;i++)
			for (int j=0; j < 4; j++)
				m[i][j]=((float[][])st.peek())[i][j];
	}

	public void popMatrix()
	{
		m=(float[][])st.pop();
	}

	public String toString()
	{// debug
		String s="";
		for (int i=0; i < 4; i++)
		{
			for (int j=0;j < 4; j++)
			{
				//s+=(new Float(m[i][j]).toString()+"\t");
				System.out.print(m[i][j]);
				System.out.print(' ');
			}
				
			System.out.print('\n');
		}
		return s;
	}

	public void load(AffineMatrix source)
	{// m=source
		for (int i=0; i < 4;i++)
			for (int j=0; j < 4;j++)
				 m[i][j]=source.m[i][j];
	}

	public void mult(AffineMatrix n)
	{// m=m*n
		AffineMatrix r =new AffineMatrix();
		r.m[0][0]=r.m[1][1]=r.m[2][2]=r.m[3][3]=0f;// alles 0
		for (int i=0; i < 4;i++)
			for (int j=0; j < 4;j++)
				for (int k=0; k < 4;k++)
					r.m[i][j]+=m[i][k]*n.m[k][j];
		m=r.m;
	}

	public Point mult(Point p)
	{// p=m*p
		float s  = m[3][0]*p.x+m[3][1]*p.y+m[3][2]*p.z+m[3][3];
		return new Point(// hier bekommt der Punkt die globale Farbe
				(m[0][0]*p.x+m[0][1]*p.y+m[0][2]*p.z+m[0][3])/s,
				(m[1][0]*p.x+m[1][1]*p.y+m[1][2]*p.z+m[1][3])/s,
				(m[2][0]*p.x+m[2][1]*p.y+m[2][2]*p.z+m[2][3])/s);
	}

	public Polyline mult(Polyline pl)
	{
		Polyline h=new Polyline();
		for(int i=0; i<pl.size();i++)
			h.add(mult(pl.pointAt(i)));
		return h;
	}

	public void translate(float x, float y, float z)
	{// m=m*t
		for (int i=0; i < 4; i++)
			m[i][3]+=m[i][0]*x+m[i][1]*y+m[i][2]*z;
	}

	public void scale(float x, float y, float z)
	{// m=m*s
		for (int i=0; i < 3; i++)
		{
			m[i][0]*=x;
			m[i][1]*=y;
			m[i][2]*=z;
	   	}
	}

	public void rotate(float x, float y, float z,float alpha)
	{//m=m*r
		AffineMatrix h=new AffineMatrix();
		float l=Math.sqrt(x*x+y*y+z*z);// Normierung
		float cow0=x/l;
		float cow1=y/l;
		float cow2=z/l;
		float emcot=1f-Math.cos(alpha*Math.PI/180f);
		float sit=Math.sin(alpha*Math.PI/180f)*-1f;
		h.m[0][0]=cow0*cow0*emcot-emcot+1f;
		h.m[1][1]=cow1*cow1*emcot-emcot+1f;
		h.m[2][2]=cow2*cow2*emcot-emcot+1f;
		h.m[0][1]=cow0*cow1*emcot+cow2*sit;
		h.m[0][2]=cow0*cow2*emcot-cow1*sit;
		h.m[1][0]=cow1*cow0*emcot-cow2*sit;
		h.m[1][2]=cow1*cow2*emcot+cow0*sit;
		h.m[2][0]=cow2*cow0*emcot+cow1*sit;
		h.m[2][1]=cow2*cow1*emcot-cow0*sit;
		mult(h);
	}

	public void transpose()
	{
		AffineMatrix transpose=new AffineMatrix();	
		for (int i =0; i < 4;i++)
			for (int j =0; j < 4;j++)
				transpose.m[i][j]=m[j][i];
		m=transpose.m;
	}

	public void invAffine()
	{
		// aff=m(tr)*m(r,s)
		// aff-1=m(r,s)-1 * m(tr)-1
		AffineMatrix inv=new AffineMatrix();
		float quot=1f;
		inv.m[0][0] = m[2][2]*m[1][1]-m[1][2]*m[2][1];
		inv.m[0][1] = m[2][1]*m[0][2]-m[0][1]*m[2][2];
		inv.m[0][2] = m[0][1]*m[1][2]-m[0][2]*m[1][1];
		inv.m[1][0] = m[2][0]*m[1][2]-m[2][2]*m[1][0];
		inv.m[1][1] = m[2][2]*m[0][0]-m[0][2]*m[2][0];
		inv.m[1][2] = m[0][2]*m[1][0]-m[0][0]*m[1][2];
		inv.m[2][0] = m[2][1]*m[1][0]-m[2][0]*m[1][1];
		inv.m[2][1] = m[0][1]*m[2][0]-m[2][1]*m[0][0];
		inv.m[2][2] = m[0][0]*m[1][1]-m[1][0]*m[0][1];
		quot=(inv.m[0][0]* m[0][0]+inv.m[0][1]*m[1][0]+inv.m[0][2]*m[2][0]);  // reicht das !!!
		// aff * aff-1 = I
		inv.m[0][0]/=quot;
		inv.m[1][0]/=quot;
		inv.m[2][0]/=quot;
		inv.m[0][1]/=quot;
		inv.m[1][1]/=quot;
		inv.m[2][1]/=quot;
		inv.m[0][2]/=quot;
		inv.m[1][2]/=quot;
		inv.m[2][2]/=quot;
		AffineMatrix tinv=new AffineMatrix();
		tinv.m[0][3] = -m[0][3];
		tinv.m[1][3] = -m[1][3];
		tinv.m[2][3] = -m[2][3];
		inv.mult(tinv);
		m=inv.m;
	}
}


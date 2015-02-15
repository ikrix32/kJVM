package java.graphics;

class ProjectionMatrix	extends AffineMatrix 
{

	public ProjectionMatrix()		
	{
		super();
	}

	public ProjectionMatrix(ProjectionMatrix p)	
	{
		super(p);			
	}
	
	public ProjectionMatrix(float d)	
	{
		super();
		prpZ(d);		
	}
	
	public void prpZ(float d)	
	{	// d < 0 PRP projection reference point
		ProjectionMatrix h=new ProjectionMatrix();
		if (d !=0)	{h.m[3][2]=-1/d;}
		mult(h);		
	}
	
	public void invProjection()	
	{					// pp=aff*p
						// p-1 ist bekannt, pp ist bekannt
						// pp*p-1=aff  -> aff-1
						//  pp-1 = p-1 * aff-1
		ProjectionMatrix i=new ProjectionMatrix(this);	// pp
		ProjectionMatrix invp=new ProjectionMatrix();
		invp.m[3][2]=-m[3][2];
		i.mult(invp);
		i.invAffine();
		invp.mult(i);
		m=invp.m;			
	}
}

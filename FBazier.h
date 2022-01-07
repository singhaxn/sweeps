
#include <math.h>

typedef class xyz XYZ,Vector;
class xyz {
public:
	double x,y,z;

	/*xyz(double X,double Y,double Z)
	{
		x=X;
		y=Y;
		z=Z;
	}
	xyz(){
		x=0.0;
		y=0.0;
		z=0.0;
	}
	*/
	//A = B + C
/*
	Vector operator +(const Vector &A) const
	{
		Vector v;

		v.x = x + A.x;
		v.y = y + A.y;
		v.z = z + A.z;

		return v;
	}

	//A = B -C
	Vector operator -(const Vector &A) const
	{
		Vector v;

		v.x = x - A.x;
		v.y = y - A.y;
		v.z = z - A.z;

		return v;
	}

	//A=B*C
	Vector operator *(const Vector &A) const
	{
		Vector v;

		v.x = y*A.z - z* A.y;
		v.y = -x*A.z + z*A.x;
		v.z = x*A.y - y*A.x;

		return v;
	}

	inline double DotVects(Vector C) const
	{
		return (x*C.x + y*C.y + z*C.z);
	}

	inline double MagnitudeVect() const
	{
		return (sqrt(x*x + y*y + z*z));
	}

	// targetA = 2*(1)B - 1.5*(0)C - 0.5(2)D
	inline Vector Tangent(Vector B, Vector D) const
	{
		Vector A;

		A.x = 2*B.x - 1.5*x - 0.5*D.x;
		A.y = 2*B.y - 1.5*y - 0.5*D.y;
		A.z = 2*B.z - 1.5*z - 0.5*D.z;

		return A;
	}
	inline Vector InitialNormal() const
	{
	
	}
*/
	// vect A = Vect B * Scalor C
	/*inline void ScalVects(Vector &A,Vector B,double C)
	{
		A.x=B.x*C;
		A.y=B.y*C;
		A.z=B.z*C;
	}

/*	Vector NegateVector(){
		p.x = -p.x;
		p.y = -p.y;
		p.z=  -p.z;
		return p;
	}

	void NegateVector(){
		x = -x;
		y = -y;
		z=  -z;
	}*/

	Vector operator -() const
	{
		Vector v;

		v.x=-x;
		v.y=-y;
		v.z=-z;

		return v;
	}

	Vector operator *(double d) const
	{
		Vector v;

		v.x = x*d;
		v.y = y*d;
		v.z = z*d;

		return v;
	}

	double Normalize()
	{
		double divide = sqrt(x*x+y*y+z*z);
		if(divide==0.0 || divide==1.0) {
			return divide;
		}
		x /= divide;
		y /= divide;
		z /= divide;

		return divide;
	}
};

/*
   Three control point Bezier interpolation
   mu ranges from 0 to 1, start to end of the curve
*/
/*XYZ Bezier3(XYZ p1,XYZ p2,XYZ p3,double mu)
{
   double mum1,mum12,mu2;
   XYZ p;

   mu2 = mu * mu;
   mum1 = 1 - mu;
   mum12 = mum1 * mum1;
   p.x = p1.x * mum12 + 2 * p2.x * mum1 * mu + p3.x * mu2;
   p.y = p1.y * mum12 + 2 * p2.y * mum1 * mu + p3.y * mu2;
   p.z = p1.z * mum12 + 2 * p2.z * mum1 * mu + p3.z * mu2;

   return(p);
}

/*
   Four control point Bezier interpolation
   mu ranges from 0 to 1, start to end of curve
*/
/*XYZ Bezier4(XYZ p1,XYZ p2,XYZ p3,XYZ p4,double mu)
{
   double mum1,mum13,mu3;
   XYZ p;

   mum1 = 1 - mu;
   mum13 = mum1 * mum1 * mum1;
   mu3 = mu * mu * mu;

   p.x = mum13*p1.x + 3*mu*mum1*mum1*p2.x + 3*mu*mu*mum1*p3.x + mu3*p4.x;
   p.y = mum13*p1.y + 3*mu*mum1*mum1*p2.y + 3*mu*mu*mum1*p3.y + mu3*p4.y;
   p.z = mum13*p1.z + 3*mu*mum1*mum1*p2.z + 3*mu*mu*mum1*p3.z + mu3*p4.z;

   return(p);
}

/*
   General Bezier curve
   Number of control points is n+1
   0 <= mu < 1    IMPORTANT, the last point is not computed
*/
/*XYZ Bezier(XYZ *p,int n,double mu)
{
   int k,kn,nn,nkn;
   double blend,muk,munk;
   XYZ b = {0.0,0.0,0.0};

   muk = 1;
   munk = pow(1-mu,(double)n);

   for (k=0;k<=n;k++) {
      nn = n;
      kn = k;
      nkn = n - k;
      blend = muk * munk;
      muk *= mu;
      munk /= (1-mu);
      while (nn >= 1) {
         blend *= nn;
         nn--;
         if (kn > 1) {
            blend /= (double)kn;
            kn--;
         }
         if (nkn > 1) {
            blend /= (double)nkn;
            nkn--;
         }
      }
      b.x += p[k].x * blend;
      b.y += p[k].y * blend;
      b.z += p[k].z * blend;
   }

   return(b);
}
*/

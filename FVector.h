using namespace std;

#include <iostream>
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
	
	char *toString(char *res) const;
	void parsePoint(const char *point);
};

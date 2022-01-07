#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include<math.h>
#define Coordinate GLdouble
#define Rational GLdouble
#define Real GLdouble
#define Integer GLint
struct RGBColor
{
	Real r,g,b;
	
	RGBColor(Real _r=0.0,Real _g=0.0,Real _b=0.0)
	{
		r = _r;
		g = _g;
		b = _b;	
	}
};
#define White RGBColor(1.0,1.0,1.0)
#define Red RGBColor(1.0,0.0,0.0)
#define Blue RGBColor(0.0,0.0,1.0)
#define Green RGBColor(0.0,1.0,0.0)
#define Yellow RGBColor(1.0,1.0,0.0)
struct XYZPoint
{
	Real x, y, z;
	XYZPoint(Real _x=0.0,Real _y=0.0,Real _z=0.0)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	
};
XYZPoint xyzpoint(Polygon & rop, Integer i)
{
	if(i<rop.dim && i>=0)
		return rop.pts[i].p;
	return rop.pts[0].p;
}
RGBColor rgbcolor(Polygon &rop, Integer i)
{
	if(i<rop.dim && i>=0)
		return rop.pts[i].color;
	return rop.pts[0].color;
}

struct Vector
{
	GLdouble i, j, k;
};

GLvoid drawPoint(XYZPoint & p, RGBColor & c)
{
	
	glBegin(GL_POINTS);
		glColor3f(c.r, c.g, c.b);
		glVertex3f(p.x, p.y, p.z);
	glEnd();
	
}

GLvoid drawLine(XYZPoint & startpt, XYZPoint & endpt, RGBColor & startcolor, RGBColor & endcolor)
{
	glBegin(GL_LINES);
		glColor3f(startcolor.r, startcolor.g, startcolor.b);
		glVertex3f(startpt.x, startpt.y, startpt.z);
		glColor3f(endcolor.r, endcolor.g, endcolor.b);
		glVertex3f(endpt.x, endpt.y, endpt.z);
	glEnd();
}
GLvoid drawEllipse(Vector major, Vector minor, RGBColor start, RGBColor end)
{

}


GLvoid drawArc(XYZPoint Center,	Real  Radius, Vector  StartAngle, Vector  SweepAngle, RGBColor start, RGBColor end)
{
}

GLvoid fillpolygon( struct XYZPoint (*x)(Integer i), struct RGBColor (*r)(Integer i), Integer dim)
{

}

const GLdouble DEG2RAD = 3.14159/180;


#include "lowlevel.h"
#include <iostream.h>
#include <fstream.h>
//Base Class of all the graphics objects
class Object;
class Point;
class Line;
class Polygon;

class Object
{
	protected :
	int type;
	public:
		Object(int t = 0){
			type = t;
		}
		virtual ~Object(){
		}
		
		friend ofstream & operator << (ofstream & out, Object &rop){
			out.write( (char*)&rop, sizeof(rop) );
			return out;
		}
		friend istream & operator >> (istream & in, Object &rop){
			in.read((char*)&rop, sizeof(rop) );
			return in;
		}
		int mytype()
		{
			return type;
		}
		void draw()
		{
			
		}
};

class Point : public Object
{
	protected :
	XYZPoint p;
	RGBColor color;
	
	public : 
		friend class Polygon;

		Point(Coordinate xpos=0.0, Coordinate ypos=0.0, Coordinate zpos=0.0, RGBColor c=White)
		:Object(0)
		{
			p.x = xpos;
			p.y = ypos;
			p.z = zpos;
			color = c;
		}
		virtual ~Point(){}
		Coordinate & xpos()
		{
			return p.x;
		}
		Coordinate & ypos()
		{
			return p.y;
		}
		Coordinate & zpos()
		{
			return p.z;
		}
		virtual void draw()
		{
			drawPoint(p, color);
		}
};
class Line : public Object
{
	protected :
	XYZPoint startpt,endpt;
	RGBColor startcolor,endcolor;
	public :
		//Two point form (x-x1)/(x2-x1) = (y-y1)/(y2-y1) = (z-z1)/(z2-z1). 
		Line(XYZPoint stpt=XYZPoint(0),XYZPoint edpt=XYZPoint(0),RGBColor stclr=RGBColor(0),RGBColor edclr=RGBColor(0))
		:Object(1)
		{
			startpt=stpt;
			endpt=edpt;
			startcolor=stclr;
			endcolor=edclr;
		}
		//Point direction form  (x-x1)/a = (y-y1)/b = (z-z1)/c.
/* 		Line(Point p,Vector v):Object(0)
		{
			
		}*/
		//Parametric form  x = x1 + t cos(alpha), y = y1 + t cos(beta), z = z1 + t cos(gamma)
		Line(XYZPoint p, Real alpha, Real beta, Real gamma, RGBColor stclr=RGBColor(0),RGBColor edclr=RGBColor(0))
		{
			startpt=p;
			endpt.x=p.x+cos(alpha);
			endpt.y=p.y+cos(beta);
			endpt.z=p.z+cos(gamma);
			
			startcolor=stclr;
			endcolor=edclr;
			
		}
		virtual ~Line(){}
		XYZPoint & startpos()
		{
			return startpt;
		}
		XYZPoint & endpos()
		{
			return endpt;
		}
		virtual void draw()
		{
			drawLine(startpt,endpt,startcolor,endcolor);
		}
};

class Polygon : public Object
{
	protected :
	Point *pts;
	Integer dim;
	bool fill;
	public:
		Polygon(Integer d):Object(2)
		{
			dim = d;
			pts = new Point[dim];
		}
		Polygon(Point *p, Integer d):Object(2)
		{
			dim = d;
			pts = new Point[dim];
			memcpy(pts, p, dim*sizeof(Point));
		}
		~Polygon()
		{
			delete []pts;
		}
		Point & operator [](Integer i)
		{
			if(i<dim && i>=0)
				return pts[i];
			return pts[0];
		}
		XYZPoint & xyzpoint(Integer i)
		{
			if(i<dim && i>=0)
				return pts[i].p;
			return pts[0].p;
		}
		RGBColor & rgbcolor(Integer i)
		{
			if(i<dim && i>=0)
				return pts[i].color;
			return pts[0].color;
		}
		//friend GLvoid fillpolygon(struct XYZPoint (*xyzpoint)(Integer i),struct RGBColor (*rgbcolor)(Integer i), Integer dim);

		virtual void draw()
		{
			if(fill == true)
				fillpolygon( &XYZPoint , &RGBColors ,dim);
			else
			{
				int i;
				for(i=0; i<dim-1; i++)
					drawLine( (pts+i)->p, (pts+i+1)->p, (pts+i)->color, (pts+i+1)->color);
				drawLine( (pts+i)->p, pts->p, (pts+i)->color, pts->color);
			}
		}
};

// class Rectangle : public Object, public Polygon
// {
// 	public:
// 		Rectangle(Integer d):Base(2), Polygon
// 		{
// 			dim = d;
// 			pts = new Point[dim];
// 		}
// 		Rectangle(Point *p, Integer d):Base(2)
// 		{
// 			dim = d;
// 			pts = new Point[dim];
// 			memcpy(pts, p, dim*sizeof(Point));
// 		}
// 		~Rectangle()
// 		{
// 			delete []pts;
// 		}
// 		Point & operator [](Integer i)
// 		{
// 			if(i<dim && i>=0)
// 				return pts[i];
// 			return pts[0];
// 		}
// 		virtual draw()
// 		{
// 			for(i=0; i<3; i++)
// 				drawline( (pts+i)->p, (pts+i+1)->p, (pts+i)->color, (pts+i+1)->color);
// 			drawline( (pts+i)->p, pts->p, (pts+i)->color, pts->color);
// 		}
// };

class Ellipse : public Object
{
	protected :
	Vector major, minor;
	RGBColor start, end;
	public :
			Ellipse(Vector &maj, Vector &min, RGBColor &s, RGBColor &e):Object(3)
			{
				
			}
			void draw()
			{
				drawEllipse(major, minor, start, end);
			}
};

class Arc : public Object
{
	protected :
	XYZPoint Center;     //circle's center
	Real  Radius;     // circle's radius
	Vector  StartAngle; // arc's start angle
	Vector  SweepAngle; // arc's sweep angle
	RGBColor start, end;

	public :
			Arc(XYZPoint &c, Real &r, Vector &strt, Vector &end):Object(4)
			{
				Center =c;
				Radius =r;
				StartAngle = strt;
				SweepAngle = end;
			}
			~Arc()
			{
			
			}
			void draw()
			{
				drawArc(Center, Radius, StartAngle, SweepAngle, start, end);
			}
};
class Curve : public Object
{

};

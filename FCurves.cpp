#include <GL/gl.h>
#include <GL/glut.h>
//#include <GL/glu.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream.h>
#include "FVector.h"
#include "parsers/Expression.cpp"

#define YES 1
#define NO 0
//#define PI 3.141592654
#define PI M_PI

#define ALLOC_ERROR(S) {printf(S);exit(0);}

/*
   Rotate a 'src' points by angle theta around an arbitrary axis vect
   Return the rotated point.
   Positive angles are anticlockwise looking down the axis
   towards the origin.
   Assume right hand coordinate system.
   Assume 'vect' as Normalized vector.
*/

int pttp=0;

void AxisRotation(XYZ *dest, XYZ *src, Vector *vect, double theta, int dim){
	GLint i;
	double costheta,sintheta,srcX,srcY,srcZ;
	vect->Normalize();
	costheta = cos(theta);
	sintheta = sin(theta);
	for(i=0; i<dim; i++){
		srcX = (src+i)->x;
		srcY = (src+i)->y;
		srcZ = (src+i)->z;
		(dest+i)->x = (costheta + (1 - costheta) * vect->x * vect->x) * srcX;
		(dest+i)->x += ((1 - costheta) * vect->x * vect->y - vect->z * sintheta) * srcY;
		(dest+i)->x += ((1 - costheta) * vect->x * vect->z + vect->y * sintheta) * srcZ;

		(dest+i)->y = ((1 - costheta) * vect->x * vect->y + vect->z * sintheta) * srcX;
		(dest+i)->y += (costheta + (1 - costheta) * vect->y * vect->y) * srcY;
		(dest+i)->y += ((1 - costheta) * vect->y * vect->z - vect->x * sintheta) * srcZ;

		(dest+i)->z = ((1 - costheta) * vect->x * vect->z - vect->y * sintheta) * srcX;
		(dest+i)->z += ((1 - costheta) * vect->y * vect->z + vect->x * sintheta) * srcY;
		(dest+i)->z += (costheta + (1 - costheta) * vect->z * vect->z) * srcZ;
	}
}

class Curve {
	private :
		bool Restored;
		int  IteratorCnt;
		XYZ  *WorkPoints;//This pointer will be used to refer to the current working set of points
		XYZ  *TempPoints;// Buffer instance along Time Axis
	protected:
		int nPoints;// Original Buffer At T=0(time). i.e. Starting one
		XYZ *points;//Always be the original Points

		virtual void EvaluatePoints() {
				int i;
				for (i = 0; i <nPoints; i++){
					points[i].x=  rand()/RAND_MAX;
					points[i].y= rand()/RAND_MAX;
					points[i].z= rand()/RAND_MAX;
				}
		}
	public:
		char *name;
		Curve *next;
		
		void saveAs(char *filename);
		virtual void writeXML(ofstream &fout, int indent)=0;
		virtual void setProperties(const char **attr)=0;
		virtual void setExtProperty(int index, const char *buffer)=0;
		
		Curve()
		{
			name=new char[10];
			strcpy(name, "curve1");
		}
		
		Curve(int n) {
			name=new char[10];
			strcpy(name, "curve1");
			Restored = YES;
			nPoints = n;
			points = new XYZ[n];
			TempPoints= new XYZ[n];
			WorkPoints = points;
			IteratorCnt = 0;
			next=NULL;
		}
		
		~Curve() {
			printf("\n\n\n Deallocating Curves\n");	
			delete []points;
			delete []TempPoints;
		}
		int DeclarenPoints(XYZ **Out=NULL){
			if(Out)
				*Out = WorkPoints;
			return nPoints;
		}
		inline void Restore(){
			Restored = YES;
			WorkPoints = points;
		}
		void FixCurve(){
			if(Restored == NO){
				memcpy(points,TempPoints,nPoints * sizeof(XYZ));
				Restore();
			}
		}
		inline void Rotate(double angle,XYZ p){
				Rotate(angle,p.x,p.y,p.z);
		}
		void Rotate(double theta,double x,double y,double z){
			if(Restored == YES){
				memcpy(TempPoints,points, nPoints * sizeof(XYZ));
				Restored = NO;
				WorkPoints = TempPoints;
			}
			//*Now do the Transformations
			XYZ p;
			p.x = x;
			p.y = y;
			p.z = z;
			AxisRotation(WorkPoints, WorkPoints, &p,theta, nPoints);
		}
		inline void Translate(XYZ p){
			Translate(p.x,p.y,p.z);
		}
		void Translate(double xDisp,double yDisp,double zDisp){
			int i;
			if(Restored == YES){
				memcpy(TempPoints,points, nPoints * sizeof(XYZ));
				Restored = NO;
				WorkPoints = TempPoints;
			}
			//*Now do the Transformations
			for(i=0;i<nPoints;i++){
				WorkPoints[i].x+= xDisp;
				WorkPoints[i].y+= yDisp;
				WorkPoints[i].z+= zDisp;
			}
		}
//*******************************************************************************************
		XYZ * Iterator(bool ResetIterator = NO){
			if(ResetIterator != NO || IteratorCnt >= nPoints){
					IteratorCnt =0;
					return NULL;
			}else
					return WorkPoints + IteratorCnt++;
		}
		virtual XYZ InitialTangent(){
			XYZ retval;
			if(nPoints < 2){
				printf("\nTangent to a point !!!!..Bye\n");
				exit(0);
			}

			if(nPoints == 2){
				retval=WorkPoints[1]-WorkPoints[0];
			}else{
				// targetA = 2*(1)B - 1.5*(0)C - 0.5(2)D
				retval=WorkPoints[0].Tangent(WorkPoints[1],WorkPoints[2]);
			}
			retval.Normalize();
			return -retval;
		}
};

class Parametric: public Curve {
	private:
		char *sf[3];// string Function
		Expression ef[3];
		double tmin, tmax;

	void EvaluatePoints(){
		double t=tmin;
		double dt=(tmax-tmin)/(nPoints-1);
		int i;

		for(i=0;i<nPoints;i++)
		{
			points[i].x= ef[0].evaluate(&t);
			points[i].y= ef[1].evaluate(&t);
			points[i].z= ef[2].evaluate(&t);
			t+=dt;
		}
	}
	public:
		void writeXML(ofstream &fout, int indent);
		void setProperties(const char **attr);
		void setExtProperty(int index, const char *buffer);
		
		Parametric():Curve(){};
		Parametric(char *sp[3],double tmin, double tmax,int n=30):Curve(n) {//String Parameter
			this->tmin=tmin;
			this->tmax=tmax;

			for(int i=0; i<3; i++) {
				sf[i]=sp[i];
				ef[i].functionType(Expression::FTYPE_3);
				ef[i].parse(sf[i]);
				//	double ef[i].evaluate(double*);
			}

			EvaluatePoints();
		}
};

class BazierCurve: public Curve{
		int nCtrlPoints;
		XYZ *cntrlPoints;//XYZ *endpt1,*endpt2,*ctrlpt1,*ctrlpt2;
		char sf[3][100];// string Function
		Expression ef[3];	
		double tmin,tmax;
		void EvaluatePoints(){
			int i;
			for (i = 0; i < nPoints; i++){
        		points[i] = Bezier4(cntrlPoints[0],cntrlPoints[1],
						cntrlPoints[2],cntrlPoints[3],
						(double)((double)i/(double)(nPoints-1.0))); 
			}
		}
	
		void EvaluatePoints(int i){
			double t=tmin;
			double dt=(tmax-tmin)/(double)(nPoints-1.0);
	
			for(i=0;i<nPoints;i++)
			{
				points[i].x= ef[0].evaluate(&t);
				points[i].y= ef[1].evaluate(&t);
				points[i].z= ef[2].evaluate(&t);
				t+=dt;
			}
		}
	
	public:
		void writeXML(ofstream &fout, int indent);
		void setProperties(const char **attr);
		void setExtProperty(int index, const char *buffer);
		
		void funparametric() {//String Parameter
			this->tmin=0.0;
			this->tmax=1.0;
	
			for(int i=0; i<3; i++) {
				ef[i].functionType(Expression::FTYPE_3);
				ef[i].parse(sf[i]);
			}
			EvaluatePoints(0);
		}


		BazierCurve():Curve()
		{
			nCtrlPoints=4;
			cntrlPoints=new XYZ[4];
		}
		
		BazierCurve(XYZ cntrlPointsIN[],int HowManyCntrlPoints,
			int NumberOfPoints=30):Curve(NumberOfPoints)
		{	
/*		
		mum1 = 1 - mu;
		mum13 = mum1 * mum1 * mum1;
		mu3 = mu * mu * mu;
		
		p.x = mum13*p1.x + 3*mu*mum1*mum1*p2.x + 3*mu*mu*mum1*p3.x + mu3*p4.x;
		p.y = mum13*p1.y + 3*mu*mum1*mum1*p2.y + 3*mu*mu*mum1*p3.y + mu3*p4.y;
		p.z = mum13*p1.z + 3*mu*mum1*mum1*p2.z + 3*mu*mu*mum1*p3.z + mu3*p4.z;
*/			
			sprintf(sf[0],"(%lf)*(1-t)^3+(%lf)*3*t*(1-t)*(1-t)+(%lf)*3*t*t*(1-t)+(%lf)* t^3",cntrlPointsIN[0].x, cntrlPointsIN[1].x, cntrlPointsIN[2].x, cntrlPointsIN[3].x);
			sprintf(sf[1],"(%lf)*(1-t)^3+(%lf)*3*t*(1-t)*(1-t)+(%lf)*3*t*t*(1-t)+(%lf)* t^3",cntrlPointsIN[0].y, cntrlPointsIN[1].y, cntrlPointsIN[2].y, cntrlPointsIN[3].y);
			sprintf(sf[2],"(%lf)*(1-t)^3+(%lf)*3*t*(1-t)*(1-t)+(%lf)*3*t*t*(1-t)+(%lf)* t^3",cntrlPointsIN[0].z, cntrlPointsIN[1].z, cntrlPointsIN[2].z, cntrlPointsIN[3].z);
			//:Curve(NumberOfPoints)
//			printf("\ncfun-fx=%s",sf[0]);
//			printf("\ncfun-fy=%s",sf[1]);
//			printf("\ncfun-fz=%s",sf[2]);
////			Wrapped(cntrlPointsIN,HowManyCntrlPoints);
			funparametric();
		}


/*				
		BazierCurve(XYZ cntrlPointsIN[],int HowManyCntrlPoints,
			int NumberOfPoints=30):Curve(NumberOfPoints)
		{	
////		
////		mum1 = 1 - mu;
////		mum13 = mum1 * mum1 * mum1;
////		mu3 = mu * mu * mu;
		
////		p.x = mum13*p1.x + 3*mu*mum1*mum1*p2.x + 3*mu*mu*mum1*p3.x + mu3*p4.x;
////		p.y = mum13*p1.y + 3*mu*mum1*mum1*p2.y + 3*mu*mu*mum1*p3.y + mu3*p4.y;
////		p.z = mum13*p1.z + 3*mu*mum1*mum1*p2.z + 3*mu*mu*mum1*p3.z + mu3*p4.z;
////			
//			char cfun[3][100];
//			sprintf(cfun[0],"(%lf)*(1-t)^3+(%lf)*3*t*(1-t)*(1-t)+(%lf)*3*t*t*(1-t)+(%lf)* t^3",cntrlPointsIN[0].x, cntrlPointsIN[1].x, cntrlPointsIN[2].x, cntrlPointsIN[3].x);
//			sprintf(cfun[1],"(%lf)*(1-t)^3+(%lf)*3*t*(1-t)*(1-t)+(%lf)*3*t*t*(1-t)+(%lf)* t^3",cntrlPointsIN[0].y, cntrlPointsIN[1].y, cntrlPointsIN[2].y, cntrlPointsIN[3].y);
//			sprintf(cfun[2],"(%lf)*(1-t)^3+(%lf)*3*t*(1-t)*(1-t)+(%lf)*3*t*t*(1-t)+(%lf)* t^3",cntrlPointsIN[0].z, cntrlPointsIN[1].z, cntrlPointsIN[2].z, cntrlPointsIN[3].z);
//			//:Curve(NumberOfPoints)
//			printf("\ncfun-fx=%s",cfun[0]);
//			printf("\ncfun-fy=%s",cfun[1]);
//			printf("\ncfun-fz=%s",cfun[2]);
////			Wrapped(cntrlPointsIN,HowManyCntrlPoints);
//			funparametric(cfun, 0.0, 1.0);
//		}
*/		
		
				
		void Wrapped(XYZ cntrlPointsIN[],int HowManyCntrlPoints){
			if(HowManyCntrlPoints != 4)
			{
			printf("\nBeziers other than 4 control points not yet implemented ... Bye \n ");
			exit(100);
			}

			nCtrlPoints =  HowManyCntrlPoints;
			cntrlPoints = new XYZ[HowManyCntrlPoints];

			memcpy(cntrlPoints,cntrlPointsIN, HowManyCntrlPoints * sizeof(XYZ));
			EvaluatePoints();
		}

		~BazierCurve() {
			delete []cntrlPoints;
		}
		void UpdateControlPoints(XYZ cntrlPointsIN[],int HowManyCntrlPoints){
			delete []cntrlPoints;
			Wrapped(cntrlPointsIN,HowManyCntrlPoints);	
		}
	
		virtual XYZ InitialTangent() {
	
			XYZ retval;
			retval=cntrlPoints[1]-cntrlPoints[0];	
			retval.Normalize();
			return retval;	
		}
		
		XYZ Bezier4(XYZ p1,XYZ p2,XYZ p3,XYZ p4,double mu)
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
};

class Lagranges: public Curve{
		int nCtrlPoints;
		XYZ *cntrlPoints;

		void CalculateF(float X, double ret[]){
			int i , j;
			double Fy = 0, Fz = 0 , termy, termz;

			for(i=0 ; i<nCtrlPoints ; i++){
				termy = cntrlPoints[i].y; // this is the i'th term
				termz = cntrlPoints[i].z;
				for(j=0 ;j<nCtrlPoints ; j++){
//					if(j != i) {
					if(cntrlPoints[i].x != cntrlPoints[j].x)
					{
						termy = termy * (X - cntrlPoints[j].x)/(cntrlPoints[i].x - cntrlPoints[j].x);
						termz = termz * (X - cntrlPoints[j].x)/(cntrlPoints[i].x - cntrlPoints[j].x);
					}
				}
				Fy = Fy + termy;
				Fz = Fz + termz;
			}

			  ret[0]=Fy;
			  ret[1]=Fz;
		}

		void EvaluatePoints(){
			int i;        
			double minX ,maxX;

			minX = maxX = cntrlPoints[0].x;

			for(i=0;i<nCtrlPoints;i++){
				if(minX>cntrlPoints[i].x)
						minX = cntrlPoints[i].x;
				if(maxX < cntrlPoints[i].x)
						maxX =cntrlPoints[i].x;
			}
			double step = fabs(maxX-minX)/ (double)nPoints;
			double x = minX;

			for (i = 0; i < nPoints; i++){
				double ans[2];
				points[i].x = x;
				CalculateF (x, ans);
				points[i].y = ans[0];
				points[i].z = ans[1];
				x+= step;
			}
		}
	
	public:
		void writeXML(ofstream &fout, int indent);
		void setProperties(const char **attr);
		void setExtProperty(int index, const char *buffer);
		Lagranges()
		{
			nCtrlPoints=0;
		}
		
		Lagranges(const XYZ cntrlPointsIN[],int HowManyCntrlPoints,
					int NumberOfPoints=3000):Curve(NumberOfPoints)
		{	
			Wrapped(cntrlPointsIN,HowManyCntrlPoints,NumberOfPoints);
		}	
		void Wrapped(const XYZ cntrlPointsIN[],int HowManyCntrlPoints,
					int NumberOfPoints=30)
		{
				if(HowManyCntrlPoints < 2){
						printf("\nLagr Error ... Bye \n ");
						exit(100);
					}
					
					nCtrlPoints =  HowManyCntrlPoints;
					cntrlPoints = new XYZ[HowManyCntrlPoints];
					
					memcpy(cntrlPoints,cntrlPointsIN, HowManyCntrlPoints * sizeof(XYZ));
					EvaluatePoints();	
		}
		~Lagranges(){
			printf("\n\n\n Deallocating Lagr\n");
			delete []cntrlPoints;
        	}
		void UpdateControlPoints(XYZ cntrlPointsIN[],int HowManyCntrlPoints,
				int NumberOfPoints=30)
		{
			delete []cntrlPoints;
			Wrapped(cntrlPointsIN,HowManyCntrlPoints,NumberOfPoints);	
		}
};

class Sweep {
	public:
		XYZ **points, *SweepPoints;
		int nPath, nSurface, nPoints;
		int iteratorPath,iteratorSurface;
		char *f[3];
		char *s;
		Expression ef[3], es;
		char *name;
		bool rotation;
		bool Restored;
		int  IteratorCnt;
		XYZ  *WorkPoints;//This pointer will be used to refer to the current working set of points
		XYZ  *TempPoints;// Buffer instance along Time Axis
		double tmin, tmax;
		struct Color
		{
			double r,g,b;
		}color;
	
	public:
		Curve *Surface,*Path;
		
		void writeXML(ofstream &fout, int indent);
		void setFunction(int index, char *fun);
		void setParametricFunction(char *buffer);
		void saveAs(char *filename);
		
		Sweep(){}
		
		void setColor(double r,double g,double b)
		{
			color.r=r;
			color.g=g;
			color.b=b;
		}
		
		// Start of Constructor
		Sweep(Curve *SurfaceIN, Curve *PathIN, char *fun[], char *sp,
			 		double tmin, double tmax, bool rot=0)
		{
			cerr<<"sweep1\n";
			color.r=0.5;
			color.g=0.5;
			color.b=1;
			
			this->tmin=tmin;
			this->tmax=tmax;			
			f[0]=fun[0];
			f[1]=fun[1];
			f[2]=fun[2];
			s=sp;
			ef[0].functionType(Expression::FTYPE_1);
			ef[1].functionType(Expression::FTYPE_1);
			ef[2].functionType(Expression::FTYPE_1);
			ef[0].parse(f[0]);
			ef[1].parse(f[1]);
			ef[2].parse(f[2]);
			es.functionType(Expression::FTYPE_3);
			es.parse(s);
			
			iteratorPath = iteratorSurface = 0;
			Surface=SurfaceIN;
			Path=PathIN;
			int i,j;
			XYZ *SurfacePts,*PathPoints;

			rotation=rot;
			nPath =Path->DeclarenPoints(&PathPoints);
			nSurface  = Surface->DeclarenPoints(&SurfacePts);

			nPoints=nPath*nSurface;
			points = new XYZ*[nPath];
			

			SweepPoints=new XYZ[nPoints];
			TempPoints= new XYZ[nPoints];
			
			WorkPoints = SweepPoints;
			
			for(i =0;i<nPath;i++) {
				points[i] =  SweepPoints+i*nSurface;
			}
//	Curve Class
			Restored = YES;
			
			
//	Curve Class			
			
			
			Vector vSrc,vDest,vRotation,pvRotation;
			Vector vPrev,vCurrent,vTranslate;

			double theta,cosTh,Stheta,sinTh,vMag;
			
			Surface->Restore();
			
			nSurface = Surface->DeclarenPoints(&SurfacePts);
			
//			memcpy(points[0],SurfacePts,nSurface*sizeof(XYZ));
			
			vPrev = -PathPoints[0]*2+PathPoints[1]*3-PathPoints[2];
			vSrc = vPrev*(PathPoints[2]-PathPoints[1]);
			vSrc = vSrc*vPrev;
			vSrc.Normalize();
			
//			printf("\n\nvSrc x=%lf y=%lf z=%lf\n\n",vSrc.x,vSrc.y,vSrc.z);	

			vCurrent =  vPrev = vSrc;
				
			Surface->Translate(-PathPoints[0]);
			Surface->FixCurve();
			
			
			
			
			double t=tmin,tincr=(tmax-tmin)/(nPath-1);			
			
			
			for(i=0;i<nPath;i++,t+=tincr)
			{
				// Some code for s.expression
				double svar[4];
				svar[3]=es.evaluate(&t);
				int j;
				
				Surface->Translate(0,0,0);
				nSurface = Surface->DeclarenPoints(&SurfacePts);
			
								
				for(j=0; j<nSurface; j++)
				{
					svar[0]=SurfacePts[j].x;
					svar[1]=SurfacePts[j].y;
					svar[2]=SurfacePts[j].z;
//					cerr<<"("<<SurfacePts[j].x<<", "<<SurfacePts[j].y<<", "<<SurfacePts[j].z<<")   ";
					SurfacePts[j].x = ef[0].evaluate(svar);
					SurfacePts[j].y = ef[1].evaluate(svar);
					SurfacePts[j].z = ef[2].evaluate(svar);
//					cerr<<"("<<SurfacePts[j].x<<", "<<SurfacePts[j].y<<", "<<SurfacePts[j].z<<")\n";
				}
					printf("(%d) t=%lf, s=%lf, x=%lf, newx=%lf",i, t, svar[3], svar[0], SurfacePts[j-1].x);
					printf(", y=%lf, newy=%lf, z=%lf, newz=%lf\n",svar[1], SurfacePts[j-1].y ,svar[2],
						SurfacePts[j-1].z);
					//Surface->Restore();
							
				// end of Some code for s.expression
				
				if(rotation) 
				{
					if(i&&i<nPath-1) 
					{
			//vCurrent=PathPoints[i+1]-PathPoints[i];
			//vCurrent=PathPoints[i].Tangent(PathPoints[i+1],PathPoints[i+2]);
						vCurrent=(PathPoints[i]-PathPoints[i-1])*
						(PathPoints[i+1]-PathPoints[i]);
						vCurrent=vCurrent*(-PathPoints[i-1]*2+
						PathPoints[i]*3-PathPoints[i+1]);
						vCurrent.Normalize();
					}
					vDest=vPrev+vCurrent;
					//vDest=vPrev*vCurrent;
					vDest.Normalize();
					cosTh = vSrc.DotVects(vDest);
	
					theta = acos(cosTh);
//					printf("\n cosTh=%f theta=%f",cosTh,theta);
					vRotation=vSrc*vDest;
	
					sinTh=vRotation.MagnitudeVect();
					Stheta = asin(sinTh);
//					printf("   Asin = %lf",Stheta);
	
					if(Stheta<0) {
						printf("\n\n\n\n\n\nHere My Manip Exiting ... \n\n\n\n\n");
						exit(0);
						vRotation = pvRotation;
					}
	
					vRotation.Normalize();
	
					if(theta!=0.0)
						Surface->Rotate(theta,vRotation);
//					printf("(%d)%d theta = %lf vRotation x=%lf y=%lf z=%lf\n",pttp++,nPath,theta,vRotation.x,vRotation.y,vRotation.z);
					/*cerr<<theta<<" =  "<<vRotation.x<<"  "<<vRotation.y<<"  "<<vRotation.z<<" "<<i<<"\n";
					*/
					vPrev = vCurrent;
					pvRotation = vRotation;
				
				}// end of if rotation
			Surface->Translate(PathPoints[0]);
			Surface->Translate(PathPoints[i]-PathPoints[0]);
			nSurface = Surface->DeclarenPoints(&SurfacePts);
			memcpy(points[i],SurfacePts,nSurface*sizeof(XYZ));
			Surface->Restore();
			}// end of for loop			
			Surface->Translate(PathPoints[0]);
			Surface->FixCurve();
			printf("\n\n\n\n\n\n");
			pttp=0;
			fflush(stdout);
		}// end of sweep
		
		
		
		
		

	
		Sweep(Curve *SurfaceIN, Curve *PathIN, bool rot=1) 
		{
			cerr<<"sweep2\n";
//			f[0]=fun[0];
//			f[1]=fun[1];
//			f[2]=fun[2];
//			s=sp;
//			ef[0].parse(f[0]);
//			ef[1].parse(f[1]);
//			ef[2].parse(f[2]);
//			es.parse(s);

			f[0]=f[1]=f[2]=s=NULL;
			color.r=0.5;
			color.g=0.5;
			color.b=1;
			
			iteratorPath = iteratorSurface = 0;
			Surface=SurfaceIN;
			Path=PathIN;
			int i,j;
			XYZ *SurfacePts,*PathPoints;

			rotation=rot;
			nPath =Path->DeclarenPoints(&PathPoints);
			nSurface  = Surface->DeclarenPoints(&SurfacePts);

			nPoints=nPath*nSurface;
			points = new XYZ*[nPath];


			SweepPoints=new XYZ[nPoints];
			
			for(i =0;i<nPath;i++) {
				points[i] =  SweepPoints+i*nSurface;
				if(!points[i])
					 ALLOC_ERROR("Sweep Contructor Allocation Failed");
			}
//	Curve Class
			Restored = YES;
			TempPoints= new XYZ[nPoints];
			WorkPoints = SweepPoints;
//	Curve Class			
			
			
			Vector vSrc,vDest,vRotation,pvRotation;
			Vector vPrev,vCurrent,vTranslate;


			double theta,cosTh,Stheta,sinTh,vMag;
			
			Surface->Restore();
			
			nSurface = Surface->DeclarenPoints(&SurfacePts);
			
//			memcpy(points[0],SurfacePts,nSurface*sizeof(XYZ));
			
			vPrev = -PathPoints[0]*2+PathPoints[1]*3-PathPoints[2];
			vSrc = vPrev*(PathPoints[2]-PathPoints[1]);
			vSrc = vSrc*vPrev;
			vSrc.Normalize();
			
//			printf("\n\nvSrc x=%lf y=%lf z=%lf\n\n",vSrc.x,vSrc.y,vSrc.z);	

			vCurrent =  vPrev = vSrc;
				
			Surface->Translate(-PathPoints[0]);
			Surface->FixCurve();
			
			
			for(i=0;i<nPath;i++)
			{
// Nilesh				
				Surface->Translate(0,0,0);
				nSurface = Surface->DeclarenPoints(&SurfacePts);
// Nilesh				
				if(rotation) 
				{
					if(i&&i<nPath-1) 
					{
			//vCurrent=PathPoints[i+1]-PathPoints[i];
			//vCurrent=PathPoints[i].Tangent(PathPoints[i+1],PathPoints[i+2]);
						vCurrent=(PathPoints[i]-PathPoints[i-1])*
						(PathPoints[i+1]-PathPoints[i]);
						vCurrent=vCurrent*(-PathPoints[i-1]*2+
						PathPoints[i]*3-PathPoints[i+1]);
						vCurrent.Normalize();
					}
					vDest=vPrev+vCurrent;
					//vDest=vPrev*vCurrent;
					vDest.Normalize();
					cosTh = vSrc.DotVects(vDest);
	
					theta = acos(cosTh);
//					printf("\n cosTh=%f theta=%f",cosTh,theta);
					vRotation=vSrc*vDest;
	
					sinTh=vRotation.MagnitudeVect();
					Stheta = asin(sinTh);
//					printf("   Asin = %lf",Stheta);
	
					if(Stheta<0) {
						printf("\n\n\n\n\n\nHere My Manip Exiting ... \n\n\n\n\n");
						exit(0);
						vRotation = pvRotation;
					}
	
					vRotation.Normalize();
	
					if(theta!=0.0)
						Surface->Rotate(theta,vRotation);
					printf("(%d/%d) theta = %lf vRotation x=%lf y=%lf z=%lf\n",++pttp,nPath,theta,vRotation.x,vRotation.y,vRotation.z);
					/*cerr<<theta<<" =  "<<vRotation.x<<"  "<<vRotation.y<<"  "<<vRotation.z<<" "<<i<<"\n";
					*/
					vPrev = vCurrent;
					pvRotation = vRotation;
				}// end of if rotation
			Surface->Translate(PathPoints[0]);
			Surface->Translate(PathPoints[i]-PathPoints[0]);
			nSurface = Surface->DeclarenPoints(&SurfacePts);
			memcpy(points[i],SurfacePts,nSurface*sizeof(XYZ));
			Surface->Restore();
			}// end of for loop
			Surface->Translate(PathPoints[0]);
			Surface->FixCurve();
			printf("\n\n\n\n\n\n");
			pttp=0;
			fflush(stdout);
		}// end of sweep
		
		// End of Constructor
		
//		void modifier()
		~Sweep(){
			int i;
			for(i =0;i<nPath;i++){
				delete points[i] ;
			}
			delete points;
			delete []SweepPoints;
			delete []TempPoints;
		}
		
	public:
		int DeclarenPoints(XYZ **Out=NULL){
			if(Out)
				*Out = WorkPoints;
			return nPoints;
		}
		inline void Restore(){
			Restored = YES;
			WorkPoints = SweepPoints;
		}
		void FixCurve(){
			if(Restored == NO){
				memcpy(SweepPoints,TempPoints,nPoints * sizeof(XYZ));
				Restore();
			}
		}
		
		
		
		//	Picked up from Curve
		
		inline void Rotate(double angle,XYZ p){
				Rotate(angle,p.x,p.y,p.z);
		}
		
		void Rotate(double theta,double x,double y,double z){
			if(Restored == YES){
				memcpy(TempPoints,SweepPoints, nPoints * sizeof(XYZ));
				Restored = NO;
				WorkPoints = TempPoints;
			}
			//*Now do the Transformations
			XYZ p;
			p.x = x;
			p.y = y;
			p.z = z;
			AxisRotation(WorkPoints, WorkPoints, &p,theta, nPoints);
		}
		
		inline void Translate(XYZ p){
			Translate(p.x,p.y,p.z);
		}
		
		void Translate(double xDisp,double yDisp,double zDisp){
			int i;
			if(Restored == YES){
				memcpy(TempPoints,SweepPoints, nPoints * sizeof(XYZ));
				Restored = NO;
				WorkPoints = TempPoints;
			}
			//*Now do the Transformations
			for(i=0;i<nPoints;i++){
				WorkPoints[i].x+= xDisp;
				WorkPoints[i].y+= yDisp;
				WorkPoints[i].z+= zDisp;
			}
		}
		//	Picked up from Curve End
		//	Picked up from Curve End
		//	Picked up from Curve End
		
		XYZ *Iterator(bool Reset = NO){
			int i=0;
			int path = iteratorPath ,surf = iteratorSurface;
			if(Reset != NO){
				iteratorPath = iteratorSurface = 0;
				return NULL;
			}
			if(iteratorPath == nPath ){
				iteratorPath = iteratorSurface = 0;
				return NULL;
			}

			iteratorSurface ++;
			if(iteratorSurface == nSurface){
				iteratorSurface = 0;
				iteratorPath++;
			}
			return &points[path][surf];
		}
		
		bool MeshPoints(XYZ p[4],bool Reset=NO){
			int path = iteratorPath ,surf = iteratorSurface;

			if(Reset != NO){
				iteratorPath = iteratorSurface = 0;
				return 0;
			}
			if(iteratorPath >= nPath-1){
				iteratorPath = iteratorSurface = 0;
				return 0;
			}

			iteratorSurface ++;

			if(iteratorSurface >= nSurface-1){
				iteratorSurface = 0;
				iteratorPath++;
			}
			p[0] = points[path][surf];
			p[1] = points[path+1][surf];
			p[2] = points[path+1][surf+1];
			p[3] = points[path][surf+1];
			return 1;
		}
};

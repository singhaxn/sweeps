#include <GL/gl.h>
#include <GL/glut.h>
//#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream.h>
#include "FBazier.h"
#include "Expression.cpp"

#define  YES 1
#define NO 0
#define PI 3.141592654

#define ALLOC_ERROR(S) {printf(S);exit(0);}



/*
   Rotate a 'src' points by angle theta around an arbitrary axis vect
   Return the rotated point.
   Positive angles are anticlockwise looking down the axis
   towards the origin.
   Assume right hand coordinate system.
   Assume 'vect' as Normalized vector.
*/

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
	private:
		bool Restored;
		int  IteratorCnt;
		XYZ  *WorkPoints;//This pointer will be used to refer to the current working set of points
		XYZ  *TempPoints;// Buffer instance along Time Axis
		//*TBuffer=TempPoints;
		//TempPoints;//This will be the work area used for transformations
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
		Curve(int n){
			Restored = YES;
			nPoints = n;
			points = new XYZ[n];
			TempPoints= new XYZ[n];
			WorkPoints = points;
			IteratorCnt = 0;
		}
		~Curve(){

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
		
		virtual Curve *copy()=0;
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
class Line: public Curve{

		XYZ start,end;
		void EvaluatePoints(){
			int i;

			for(i=0;i<nPoints;i++)
			{
				points[i].x =  (double)((double)(nPoints-1-i)*start.x + (double)i*end.x)/(double)(nPoints-1);
				points[i].y =  (double)((double)(nPoints-1-i)*start.y + (double)i*end.y)/(double)(nPoints-1);
				points[i].z =  (double)((double)(nPoints-1-i)*start.z + (double)i*end.z)/(double)(nPoints-1);
			}
		}	
		public:
			Line(XYZ startIN,XYZ endIN,int n=4):Curve(n){
				if(n <2 ){
					printf("\n Line ..ck Off \n");
					exit(0);
				}
				start = startIN;
				end = endIN;
				EvaluatePoints();
			}
			virtual XYZ InitialTangent(){

				XYZ retval;
				if(nPoints < 2){
					printf("\nTangent to a point !!!!..Bye\n");
					exit(0);
				}
				retval=points[1]-points[0];
				retval.Normalize();
				return retval;
			}
};
class Helix: public Curve{
		private:
				double radius;
				double Height,nTurns;
		void EvaluatePoints(){
			int i;
			/*double t = 0.0,dt = 2.0*PI*(double)nTurns/(double)(nPoints-1);
			double ratio = Height/2.0*PI*(double)nTurns;
			for (i = 0; i <nPoints; i++){
					points[i].x= (double)cos(t) * radius;
					points[i].y= (double)sin(t) * radius;
					points[i].z= (double) -t*ratio;
					t+=dt;
			}*/
			double z = 0.0,dz = Height/nPoints;
			double t = 0.0,dt =  2.0*PI*nTurns/(double)nPoints;


			for (i = 0; i <nPoints; i++){
					points[i].x= (double)cos(t) * radius;
					points[i].y= (double)sin(t) * radius;
					points[i].z= z;
					z+=dz;
					t+=dt;
			}
		}
		public:
			Helix(double RadiusIN,double HeightIN,int nTurnsIN,int n=30):Curve(n){
				Height = HeightIN;
				nTurns =nTurnsIN;
				radius = RadiusIN;
				EvaluatePoints();
			}
			virtual XYZ InitialTangent(){

				XYZ retval={0,0,-1};

				return retval;
			}

};


class Parametric: public Curve {
	private:
		char *sf[3];// string Function
		Expression ef[3];
		double tmin, tmax;

	void EvaluatePoints(){
		double t=tmin;
		double dt=(tmax-tmin)/nPoints;
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
		Parametric(char *sp[3],double tmin, double tmax,int n=30):Curve(n) {//String Parameter
			this->tmin=tmin;
			this->tmax=tmax;

			for(int i=0; i<3; i++) {
				sf[i]=sp[i];
				ef[i].functionType(FTYPE_3);
				ef[i].parse(sf[i]);
				//	double ef[i].evaluate(double*);
			}

			EvaluatePoints();
		}
		
		Parametric()
		{
		}
		
		Parametric *copy()
		{
			Parametric *temp=new Parametric();
			
			*temp=*this;
			temp->points=new XYZ[nPoints];//Always be the original Points
			memcpy(temp->points, points, nPoints*sizeof(XYZ));
		}
};

class HelixCone: public Curve {
		private:
				double radius;
				double Height,nTurns;
		void EvaluatePoints(){
			int i;
			double z = 0.0,dz = Height/nPoints;
			double t = 0.0,dt =  2.0*PI*nTurns/(double)nPoints;
			double r=0,dr=radius/nPoints;

			for (i = 0; i <nPoints; i++){
					points[i].x= (double)cos(t) * r;
					points[i].y= (double)sin(t) * r;
					points[i].z= z;
					z+=dz;
					t+=dt;
					r+=dr;
			}
		}
		public:
			HelixCone(double RadiusIN,double HeightIN,int nTurnsIN,int n=30):Curve(n){
				Height = HeightIN;
				nTurns =nTurnsIN;
				radius = RadiusIN;
				EvaluatePoints();
			}
/*			
			virtual XYZ InitialTangent(){
			
				XYZ retval={0,0,-1};
						
				return retval;	
			}
*/			
};

class Circle: public Curve{
		private:
				double radius;
				XYZ p;
		void EvaluatePoints(){
			int i;

			double t = 0.0,dt = 3.141592654*2.0/(nPoints-1);
			for (i = 0; i <nPoints; i++){
					points[i].x = p.x+(double)cos(t) * radius;
					points[i].y = p.y+(double)sin(t) * radius;
					points[i].z = p.z+0;
					t+=dt;
			}
/*
			points[nPoints-1].x = p.x+radius;
			points[nPoints-1].y = p.y;
			points[nPoints-1].z = p.z+0;
*/

/*
					points[i].x = p.x+(double)cos(0) * radius;
					points[i].y = p.y+(double)sin(0) * radius;
					points[i].z = p.z+0;
*/
		}
		public:
			Circle(XYZ pIN,double RadiusIN,int n=30):Curve(n+1){
				if(pIN.z != 0){
					printf("\nCircle Center Needed to be in XY Plane (z must equal 0)..Bye\n");
					exit(0);
				}
				p = pIN;
				radius = RadiusIN;
				EvaluatePoints();
			}
			/*virtual XYZ InitialTangent(){

				XYZ retval;
				if(nPoints < 2){
					printf("\nTangent to a point !!!!..Bye\n");
					exit(0);
				}
				SubVects(retval,points[0],p);
				double swap;
				swap=retval.x;
				retval.x = -retval.y;
				retval.y = swap;
				Normalize(&retval);
				return retval;
			}*/
};

class BazierCurve: public Curve{
		int nCtrlPoints;
		XYZ *cntrlPoints;//XYZ *endpt1,*endpt2,*ctrlpt1,*ctrlpt2;
		void EvaluatePoints(){
			int i;
			for (i = 0; i < nPoints; i++){
        		points[i] = Bezier4(cntrlPoints[0],cntrlPoints[1],
						cntrlPoints[2],cntrlPoints[3],
						(double)((double)i/(double)(nPoints-1.0))); 
			}
		}
	public:
		BazierCurve(XYZ cntrlPointsIN[],int HowManyCntrlPoints,
					int NumberOfPoints=30):Curve(NumberOfPoints)
		{	
			Wrapped(cntrlPointsIN,HowManyCntrlPoints);
		}
		
		void Wrapped(XYZ cntrlPointsIN[],int HowManyCntrlPoints){
		if(HowManyCntrlPoints != 4){
				printf("\nBeziers other than 4 control points not yet implemented ... Bye \n ");
				exit(100);
			}

			nCtrlPoints =  HowManyCntrlPoints;
			cntrlPoints = new XYZ[HowManyCntrlPoints];

			memcpy(cntrlPoints,cntrlPointsIN, HowManyCntrlPoints * sizeof(XYZ));
			EvaluatePoints();	
		}

		~BazierCurve(){
         	delete []cntrlPoints;
         }
	 void UpdateControlPoints(XYZ cntrlPointsIN[],int HowManyCntrlPoints){
		delete []cntrlPoints;
		Wrapped(cntrlPointsIN,HowManyCntrlPoints);	
	}
	virtual XYZ InitialTangent(){

		XYZ retval;
		retval=cntrlPoints[1]-cntrlPoints[0];	
		retval.Normalize();
		return retval;	
	}

};

class Flower: public Curve{
		private:
				double radius,ratio;
				int npetal,ncnt;
			void EvaluatePoints(){
				int i;
				XYZ tmpcentre  = {0.0,0.0,0.0};
				Circle C(tmpcentre,radius,npetal);				
				BazierCurve *B;
				XYZ *CirclePts,*BazierPts;
				
				C.DeclarenPoints(&CirclePts);
				XYZ bPts[4];
/*				
				int j;
				for(i=0;i<npetal;i++)
				{					
					j=(i==npetal-1)?0:i+1;
					bPts[0]= CirclePts[i];
					// vect A = Vect B * Scalor C
					ScalVects(bPts[1],bPts[0],ratio);
					bPts[3]= CirclePts[j];
					ScalVects(bPts[2],bPts[3],ratio);
					
					B = new BazierCurve(bPts,4,ncnt);
					B->DeclarenPoints(&BazierPts);
					memcpy(points+i*ncnt,BazierPts,ncnt*sizeof(XYZ));
				}
*/				
					bPts[0]= CirclePts[0];
					// vect A = Vect B * Scalor C
					bPts[1]=bPts[0]*ratio;
					bPts[3]= CirclePts[1];
					bPts[2]=bPts[3]*ratio;
					B = new BazierCurve(bPts,4,ncnt);
					B->DeclarenPoints(&BazierPts);
					memcpy(points,BazierPts,ncnt*sizeof(XYZ));
					double theta=0.0;
					Vector v={0.0,0.0,1.0};
					for(i=1;i<npetal;i++ )
						theta=(i*2.0*PI)/npetal,
						AxisRotation(points+i*ncnt, BazierPts, &v, theta, ncnt);
		}	
		public:
			Flower(double RadiusIN,double RatioIN, int NpetalIN,int NcntIN=30):Curve(NpetalIN*NcntIN){
			    radius=RadiusIN;
				ratio=RatioIN;
				npetal=NpetalIN;
				ncnt=NcntIN;
				EvaluatePoints();
			}
};

class Rectangle: public Curve{
		private:
				XYZ p[4];
		void EvaluatePoints(){
				for(int i=0;i<4;i++){
					points[i] = p[i];
				}
				points[4]=p[0];
		}	
		public:
			Rectangle(XYZ *pIN):Curve(5){
				for(int i=0;i<4;i++){
					p[i] = pIN[i];
				}
				EvaluatePoints();
			}
//********************  Changed  ********************
			virtual XYZ InitialTangent(){
			
				XYZ retval;
				if(nPoints < 2){
					printf("\nTangent to a point !!!!..Bye\n");
					exit(0);
				}
				retval=points[1]-points[0];
				retval.Normalize();
				return retval;
			}			
};

/*
class BmpCurve: public Curve{
		private:
		void EvaluatePoints(ifstream &file){
			int i,mult=4;
			for(i=0;i<nPoints;i+=2){
				XYZ fread;
				file >> fread.x >> fread.y >> fread.z;
				points[i].x = mult*fread.x;
				points[i].y = -mult*fread.z;
				points[i].z = 0.0;
				points[i+1].x = mult*fread.y;
				points[i+1].y = -mult*fread.z;
				points[i+1].z = 0.0;
			}
		}
		public:
			BmpCurve(char *fname,int n=0):Curve(0,bmp){

				ifstream file(fname);
				file >> n;
				nPoints = 2*n;
				points = new XYZ[nPoints];
				TempPoints= new XYZ[nPoints];
				WorkPoints = points;
				EvaluatePoints(file);
				file.close();
			}
};
*/

class Sweep{
// Curve End
	private:		
		bool Restored;
		int  IteratorCnt;
		XYZ  *WorkPoints;//This pointer will be used to refer to the current working set of points
		XYZ  *TempPoints;// Buffer instance along Time Axis
	Curve *SurfaceIN, *PathIN;
	protected:
		int nPoints;// Original Buffer At T=0(time). i.e. Starting one
		XYZ *points;//Always be the original Points
	
	public:
		Curve(int n){
			Restored = YES;
			nPoints = n;
			points = new XYZ[n];
			TempPoints= new XYZ[n];
			WorkPoints = points;
			IteratorCnt = 0;
		}
		~Curve(){

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
// Curve End	
	public:
		XYZ **points, *p1d;
		XYZ **Normals;
		int nPath, nSurface, totpts;
		int iteratorPath,iteratorSurface;
		char *name;
	public:
		Curve *Surface,*Path;
		Sweep(const Curve *SurfaceINParameter,const Curve *PathINParameter){
			nPath =Path->DeclarenPoints(&PathPoints);
			nSurface  = Surface->DeclarenPoints(&SurfacePts);			
//			SurfaceIN=
//			PathIN=
			
			Surface=SurfaceIN;
			Path=PathIN;

			iteratorPath = iteratorSurface = 0;
			Surface=SurfaceIN;
			Path=PathIN;
			int i,j;
			XYZ *SurfacePts,*PathPoints;

			nPath =Path->DeclarenPoints(&PathPoints);
			nSurface  = Surface->DeclarenPoints(&SurfacePts);

			totpts=nPath*nSurface;
			points = new XYZ*[nPath];
			Normals = new XYZ*[nPath];

			p1d=new XYZ[totpts];
			for(i =0;i<nPath;i++){
				points[i] =  p1d+i*nSurface;
				if(!points[i])
					 ALLOC_ERROR("Sweep Contructor Allocation Failed");
			}

			Vector vSrc,vDest,vRotation,pvRotation;
			Vector vPrev,vCurrent,vTranslate;


			double theta,cosTh,Stheta,sinTh,vMag;

			//vSrc = Path->InitialTangent();
			//vSrc = Path->InitialNormal();
			//vSrc=PathPoints[0].Tangent(PathPoints[1],PathPoints[2]);
			vPrev = -PathPoints[0]*2+PathPoints[1]*3-PathPoints[2];
			vSrc = vPrev*(PathPoints[2]-PathPoints[1]);
			vSrc = vSrc*vPrev;
			vSrc.Normalize();
			printf("\n\nvSrc x=%lf y=%lf z=%lf\n\n",vSrc.x,vSrc.y,vSrc.z);


			vCurrent =  vPrev = vSrc;

			Surface->Restore();

			nSurface = Surface->DeclarenPoints(&SurfacePts);
			memcpy(points[0],SurfacePts,nSurface*sizeof(XYZ));


			XYZ translate = PathPoints[0];
			XYZ ntranslate = -translate;
			XYZ ptranslate;

			Surface->Translate(ntranslate);
			Surface->FixCurve();





	//*********************  Main Evaluator
			for(i =1;i<nPath;i++){
			     if(i<nPath-1){
//	 		vCurrent=PathPoints[i+1]-PathPoints[i];
//			vCurrent=PathPoints[i].Tangent(PathPoints[i+1],PathPoints[i+2]);
					vCurrent=(PathPoints[i]-PathPoints[i-1])*(PathPoints[i+1]-PathPoints[i]);
					vCurrent=vCurrent*(-PathPoints[i-1]*2+PathPoints[i]*3-PathPoints[i+1]);
					vCurrent.Normalize();
				 }
				vDest=vPrev+vCurrent;
				//vDest=vPrev*vCurrent;
				vDest.Normalize();

				cosTh = vSrc.DotVects(vDest);

				theta = acos(cosTh);
				printf("\n cosTh=%f theta=%f",cosTh,theta);
				vRotation=vSrc*vDest;

				sinTh=vRotation.MagnitudeVect();

				/*if(sinTh==0){
    					printf("\n\n\n\n\n\nHere My Manip Exiting ... \n\n\n\n\n");
					exit(0);
				 	theta =0.0;
					vRotation = vSrc;

				}
				*/

/*
				if(sinTh==0){
					Vector tempX={1.0,0.0,0.0};
					vRotation=vSrc*tempX;

					sinTh=vRotation.MagnitudeVect();
					if(vRotation.MagnitudeVect()==0){
						Vector tempY={0.0,1.0,0.0};
						vRotation=vSrc*tempY;
						sinTh=vRotation.MagnitudeVect();
						if(vRotation.MagnitudeVect()==0){
							Vector tempZ={0.0,0.0,1.0};
							vRotation=vSrc*tempZ;
						}
					}

				}
*/

				Stheta = asin(sinTh);
				printf("   Asin = %lf",Stheta);

				if(Stheta<0) {
					printf("\n\n\n\n\n\nHere My Manip Exiting ... \n\n\n\n\n");
					exit(0);
					vRotation = pvRotation;
				}

				vRotation.Normalize();

					if(theta!=0.0)
						Surface->Rotate(theta,vRotation);

					Surface->Translate(translate);
					ptranslate=PathPoints[i]-PathPoints[0];
					Surface->Translate(ptranslate);

						nSurface = Surface->DeclarenPoints(&SurfacePts);
				 		memcpy(points[i],SurfacePts,nSurface*sizeof(XYZ));
				Surface->Restore();

				vPrev = vCurrent;
				pvRotation = vRotation;
			}
			Surface->Translate(translate);
			Surface->FixCurve();
		}
		~Sweep(){
			int i;
			for(i =0;i<nPath;i++){
				delete points[i] ;
			}
			delete points;
		}
		
		//	Picked up from Curve
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

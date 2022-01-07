#include <GL/gl.h>
#include <GL/glut.h>
//#include <GL/glu.h>
#include <iostream>
#include <math.h>
#include "FCurves.cpp"
#include "FObject.cpp"
#include "parsers/XMLHandling.cpp"
#include <pthread.h>

//#define PI 3.141592654
//#define TOTAL_SWEEPS 2 //12
#define TOTAL_SWEEPS 1
int aassdd=0;
int Auto = 0;
int lights=1;
int CurveIndex=0;
bool ShowCurve =0;
bool ShowColor = 0;
bool NormalsDir=0;
double x_Angle = 0,y_Angle = 0,z_Angle=0, AngleAuto=0.0;
double x_translate=0.0,y_translate=0.0;

double NEAR_Z=1.0, FAR_Z=100.0;

/*Bezier along Bezier Surfaces*/
/*XYZ bSurfPts[4] = {
	{ 0.0, -2.0, 0.0},
	{ 2.0, 2.0, 0.0},
    	{ -2.0, 2.0, 0.0},
	{ 0.0, -2.0, 0.0}
};

const double add = 0.0;
XYZ bPathPts[4] = {
	{ 0.0+add, 0.0, 0.0},
	{ 0.0+add, 0.0, -4.0},
	{ -4.0+add, 0.0, -4.0},
	{ -4.0+add, 0.0, 0.0}
};
BazierCurve B(bSurfPts,4,90);
BazierCurve B1(bPathPts,4,40);


/*Cofee Pot*/
/*#define CofeeRadius 2
#define CofeeHeight 4
#define DestortX 1
XYZ bCofee[4] = {
	{ CofeeRadius, 0.0, 0.0},
	{ CofeeRadius+DestortX, 1, 0},
	{ CofeeRadius-DestortX, CofeeHeight-1, 0},
	{ CofeeRadius, CofeeHeight, 0.0}
};
BazierCurve BCofee(bCofee,4,50);
Circle CCofee(origin,2,90);
*/

Object *S;

void *autoThread(void *dummy)
{
   int const cnt = 390, xcnt = 990, ycnt = 690, zcnt = 890;
   double const TwoPI = 360, arcTwoPI = 2*PI;
   double incr=1.0*TwoPI/cnt, arcincr=1.0*arcTwoPI/xcnt;   
   double Autox_Angle = 1.0*TwoPI/xcnt, Autoy_Angle = 1.0*TwoPI/ycnt , Autoz_Angle = 1.0*TwoPI/zcnt;
   double Autox_trans=1.15, Autoy_trans=-0.6, Autox_range=2.5, Autoy_range=3.0;	
   
	while(Auto) {
		AngleAuto = fmod(AngleAuto+ arcincr,arcTwoPI);
		x_translate = Autox_trans + cos(AngleAuto)*Autox_range;
		y_translate = Autoy_trans + sin(AngleAuto)*Autoy_range;
		x_Angle = fmod((x_Angle + Autox_Angle),TwoPI);
		y_Angle = fmod((y_Angle + Autoy_Angle),TwoPI);
		z_Angle = fmod((z_Angle + Autoz_Angle),TwoPI);
		glutPostRedisplay();
		usleep(100);
	}
	pthread_exit(NULL);
}

void display(void){
   	int i;
	XYZ *p;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 0.0, 1);

   	glPushMatrix ();
  			//glTranslatef(x_translate, y_translate, z_translate-5);
         glTranslatef(x_translate, y_translate, 0.0);
		glRotatef(x_Angle, 1.0, 0.0, 0.0);
		glRotatef(y_Angle, 0.0, 1.0, 0.0);
		glRotatef(z_Angle, 0.0, 0.0, 1.0);
		//glTranslatef(x_translate, y_translate, z_translate);//Axes
	glBegin(GL_LINES);


		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(-5,0,0);
		glVertex3f(0,0,0);

		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0,0,0);
		glVertex3f(5,0,0);

		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0,-5,0);
		glVertex3f(0,0,0);


		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0,0,0);
		glVertex3f(0,5,0);



		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0,0,-5);
		glVertex3f(0,0,0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0,0,0);
         	glVertex3f(0,0,5);

   glEnd();


	
	if(ShowCurve){
			
		Sweep *thisSweep;
		
		(S+CurveIndex)->Iterator(&thisSweep,1);
					
		while( (S+CurveIndex)->Iterator(&thisSweep) ){
			glColor3f(0, 0, 1.0);
			glBegin(GL_LINE_STRIP);
				while(( p = (thisSweep->Path)->Iterator() ) != NULL){
					glVertex3f(p->x,p->y,p->z);	
				}
			glEnd();
			
			glColor3f(1.0, 0 , 0);
			glBegin(GL_LINE_STRIP);
				while(( p = (thisSweep->Surface)->Iterator() ) != NULL){
					glVertex3f(p->x,p->y,p->z);
				}
			glEnd();
		}
		glPopMatrix ();
    		glutSwapBuffers();
    		glFlush();
		return;
	}

	
	   XYZ Pt[4];
	   int j=0;
	   Vector tt,n,sub1,sub2;
		if(lights)
	   		while((S+CurveIndex)->MeshPoints(Pt)){
				glBegin(GL_POLYGON);
					sub1=Pt[0]-Pt[1];
					sub2=Pt[2]-Pt[0];
					n=sub1*sub2;
					n.Normalize();
					if(NormalsDir)
						n=-n;
					for(i=0;i<4;i++){
                  //Normal Assignments
						glNormal3f(n.x,n.y,n.z);
                  //Color Calculations
						if(ShowColor) glColor3f(fabsf(n.x),fabsf(n.y),fabsf(n.z));
						glVertex3f(Pt[i].x,Pt[i].y,Pt[i].z);
					}
				glEnd();
			}
		else
	   		while((S+CurveIndex)->MeshPoints(Pt)){
				glBegin(GL_LINE_STRIP);
					for(i=0;i<3;i++){
						glVertex3f(Pt[i].x,Pt[i].y,Pt[i].z);
						glVertex3f(Pt[i+1].x,Pt[i+1].y,Pt[i+1].z);
					}
					glVertex3f(Pt[3].x,Pt[3].y,Pt[3].z);
					glVertex3f(Pt[0].x,Pt[0].y,Pt[0].z);
				glEnd();
			}
		
    glPopMatrix ();
    glutSwapBuffers();
    glFlush();
}
void mouse(int button,int state,int x,int y){
	static int whichPoint = 0;
	int i;


}

void myReshape(GLsizei w, GLsizei h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-5.0, 5.0, -5.0*(GLfloat)h/(GLfloat)w,
            5.0*(GLfloat)h/(GLfloat)w, -5.0, 5.0);
    else
        glOrtho(-5.0*(GLfloat)w/(GLfloat)h,
            5.0*(GLfloat)w/(GLfloat)h, -5.0, 5.0, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void initlights(void){

	glEnable(GL_LIGHTING);
  	glEnable( GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);


	GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
	GLfloat light0_diffuse[] =  {.6f, .6f, 1.0f, 1.0f};
	GLfloat light0_position[] = {5.0f, 5.0f, 5.0f, 0.0f};

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);



	GLfloat light1_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
	GLfloat light1_diffuse[] =  {.9f, .6f, 0.0f, 1.0f};
	GLfloat light1_position[] = {-5.0f, -5.0f, -5.0f, 0.0f};


	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT,GL_FILL);
	glPolygonMode(GL_BACK,GL_FILL);



	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_SHININESS, 60.0);
}

void setProjection(void){
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
}

void myinit(void){
	glClearColor (0.0, 0.0, 0.0, 1.0);
	setProjection();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_DEPTH_TEST);
	if(lights)
		initlights();
	glutSwapBuffers();
}

double myfmod(double y, double x)
{
	double rem=y,xa;
	xa=x<0?-x:x;
	if(y<0)
		while(1)
		{
			rem+=xa;
			if(rem>=0) break;
		}
	else
		while(1)
		{
			rem-=xa;
			if(rem<=0) 
			{
				rem+=xa;
				break;
			}
		}	
	return rem;
}

void keyboard (unsigned char key, int x, int y)
{
   int const cnt = 90;
   double const TwoPI = 360;
   double incr=1.0*TwoPI/cnt;   
   static pthread_t thread;
   
   switch (key) {
      case 'x':
         x_Angle = fmod((x_Angle + incr),TwoPI);
         glutPostRedisplay();
         break;
      case 'X':
         x_Angle = fmod((x_Angle + TwoPI - incr),TwoPI);
         glutPostRedisplay();
         break;
      case 'y':
         y_Angle = fmod((y_Angle + incr),TwoPI);
         glutPostRedisplay();
         break;
      case 'Y':
         y_Angle = fmod((y_Angle + TwoPI - incr),TwoPI);
         glutPostRedisplay();
         break;
      case 'z':
    	 z_Angle = fmod((z_Angle + incr),TwoPI);
         glutPostRedisplay();
         break;
      case 'Z':
         z_Angle = fmod((z_Angle + TwoPI - incr),TwoPI);
         glutPostRedisplay();
         break;
      case 'l': case 'L':
			lights^=1;
         if(lights){
         	initlights();
	  }
          else{
         	 glDisable(GL_LIGHTING);
		 glDisable(GL_LIGHT0);
          }
         glutPostRedisplay();

         break;
	 case 'n': case 'N':
	 		CurveIndex = (CurveIndex+1)%TOTAL_SWEEPS;
			glutPostRedisplay();
	 	;break;
	case 'p': case 'P':
	 		CurveIndex = (CurveIndex+TOTAL_SWEEPS-1)%TOTAL_SWEEPS;
			glutPostRedisplay();
	 	;break;
	case 's': case 'S':
		ShowCurve ^= 1;
		if(ShowCurve){
			lights=0;
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
		}else{
			lights=1;
			initlights();
		}
		glutPostRedisplay();
		break;
	case 'r': case 'R':
		NormalsDir^=1;
		glutPostRedisplay();
		break;
	case 'c': case 'C':
		ShowColor^=1;
		glutPostRedisplay();
		break;
	case 'a': case 'A':
		if(!Auto)
			pthread_create(&thread, NULL, autoThread, NULL);
		else
			pthread_cancel(thread);
		Auto^=1;
		break;		
   case 'j':
      x_translate = x_translate + 0.1;
      glutPostRedisplay();
      break;
   case 'J':
      x_translate = x_translate - 0.1;
      glutPostRedisplay();
       break;
   case 'k':
      y_translate = y_translate + 0.1;
      glutPostRedisplay();
      break;
   case 'K':
      y_translate = y_translate - 0.1;
      glutPostRedisplay();
      break;
	 case 27:exit(0);
   }
}

void printKeyBindings(){
//	system("clear");
	fprintf(stderr, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~KeyBindings~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("\t\t\t\t X Y Z : Rotations about respective Axes (shift+key reverses dir).\n");

	printf("\n");

	printf("\t\t\t\t L     : Toggle Lights.\n");
	printf("\t\t\t\t S     : Toggle Sweept Volume And Parent Curves.\n");
	printf("\t\t\t\t R     : Toggle Surface Normals Direction.\n");

	printf("\n");
	printf("\t\t\t\t j/J   : Translate(+,-) Across x Axis.\n");
	printf("\t\t\t\t k/K   : Translate(+,-) Across y Axis.\n");
//	printf("\t\t\t\t m/M   : Translate(*,/) Across z Axis.\n");
	printf("\n");

	printf("\t\t\t\t N     : NextSweep.\n");
	printf("\t\t\t\t P     : PrevSweep.\n");
	printf("\t\t\t\t ESC   : Quit.\n");
	printf("\n");
	
	fprintf(stderr, "\t\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
int main(int argc, char** argv)
{
	
	char *cfun[3]= {
		"2+0.3*cos(t)",
		"0",
		"0.3*sin(t)"
	};
/*	
	Parametric C(cfun, 0, 2*PI,20);	
	Parametric C1(cfun, 0, 2*PI,20);
	Parametric C2(cfun, 0, 2*PI,20);
	Parametric C3(cfun, 0, 2*PI,20);
*/	
	Parametric C4(cfun, 0, 2*PI,20);
	XYZ ptemp[]={ {0, 0, 0}, {1, 2, 3}, {2, 2.5, -1.5}, {3, -2, 1.2} };
//	Lagranges lag(ptemp, 4, 40);

	/*
		points[i].x= (double)cos(t) * radius;
		points[i].y= (double)sin(t) * radius;
		points[i].z= z;


		Helix(double RadiusIN,double HeightIN,int nTurnsIN,int n=30)
		Helix H(2,4,8,1000);




	*/
	char *FunHeliX3[3]= {
		"2*cos(t)",
		"2*sin(t)",
		"t/(3.141592654*2.0)"
	};
	char *FunHeliX4[3]= {
		"2*cos(t)",
		"2*sin(t)",
		"t"
	};
	
//	Parametric ParametricHelix3(FunHeliX3,0,6*PI,100);
//	Parametric ParametricHelix4(FunHeliX4,0,0.6+PI,15);	
	CurveFileParser cfp;
	Parametric *ParametricHelix4=(Parametric *)cfp.getCurve("phel.curve");


/*	CCofee.Rotate(-PI/2,1,0,0);
	CCofee.FixCurve();*/

/*
	C.Translate(-centre);
	C.Rotate(-PI/2,1,0,0);
	C.Translate(centre);
	C.FixCurve();
*/	
/*
	CC.Translate(-centre);
	CC.Rotate(-PI/2,1,0,0);
	CC.Translate(centre);
	CC.FixCurve();


	Cnew.Translate(-centrenew);
	Cnew.Rotate(-PI/2,1,1,1);
	Cnew.Translate(centrenew);
	Cnew.FixCurve();*/
/*		
		Sweep(Curve *SurfaceIN, Curve *PathIN, char *fun[], char *sp,
			 		double tmin, double tmax, bool rot=0)
		Sweep(Curve *SurfaceIN, Curve *PathIN, bool rot=1)
*/	
/*
	char *fun[3]={
		"x*sin(t)", "y*cos(t)", "z"
	};

	char *fun[3]={
		"x*cos(s)", "y*cos(s)", "z"
	};
*/	
	char *fun[3]={
		"x", "y", "z"
	};
	
	char *Helixfun3[3]={
		"x*(1+cos(s)/2)","y","z*(1+cos(s)/2)"
	};

	char *Helixfun4[3]={
		"x","y","z"
	};
	
	char *Helixfun2[3]={
		"x","y+1.25*tan(s)","z*1.25"
	};
	
	char sp[]="t";
	
	Sweep s[TOTAL_SWEEPS]={

//		Sweep(&lag,&C1,0),
//		Sweep(&lag,&C2,fun, sp, 0, 1, 0),
//		Sweep(&C3,&ParametricHelix3, Helixfun3, sp, 0, 68, 0),
//		Sweep(&C4,&ParametricHelix4, Helixfun4, sp, 0, 68, 1)
		Sweep(&C4,ParametricHelix4,1)


//		Sweep(&BCofee,&CCofee),
	};	
	Object obj[3];
	
/*	SweepFileParser sfp;
	Sweep *swp=sfp.getSweep("sweep0.sweep");
	obj[0].addSweep(swp);*/
	
	ObjectFileParser ofp(&obj[0]);
	ofp.parse("obj0.object");
/*	obj[0].saveAs("obj0.object");
	s[0].saveAs("sweep0.sweep");
	ParametricHelix4.saveAs("phel.curve");*/
	
	
//	obj[0].addSweep(&s[1]);	
//	obj[1].addSweep(&s[1]);
//	obj[2].addSweep(&s[0]);
//	obj[2].addSweep(&s[2]);
//	obj[3].addSweep(&s[3]);	
//	obj[2].saveAs("object2.xml");
//	ObjectFileParser prs(&obj[1]);
//	prs.parse("object2.xml");
//	fprintf(stderr, "\nam\n");
	
	S = obj;
	
//	fprintf(stderr, "\nBoy\n");
		
	/*BCofee.Translate(-2,0,0);
	BCofee.FixCurve();
	*/
	//printKeyBindings();
//	fprintf(stderr, "\nBoy1\n");

	/*B.Translate(1.5,0,0);
	B.FixCurve();
	*/

   	glutInit(&argc, argv);

	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );//(GLUT_SINGLE | GLUT_RGB);// | GLUT_DEPTH );
	glutInitWindowSize (1000,1000);
	glutInitWindowPosition (0,0);
	glutCreateWindow (argv[0]);
	myinit();
	glutReshapeFunc (myReshape);
	glutDisplayFunc (display);
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	
	fprintf(stdout,"\n\n\n\n Bye \n\n\n");
}


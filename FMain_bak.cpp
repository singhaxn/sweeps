#include <GL/gl.h>
#include <GL/glut.h>
//#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "FCurves.cpp"
#include "FObject.cpp"

#define PI 3.141592654
//#define TOTAL_SWEEPS 2 //12
#define TOTAL_SWEEPS 3
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
XYZ bSurfPts[4] = {
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

/*********************************/

/* Helix Telephone Wire */
XYZ centre  = {2,0,0};
//XYZ centre  = {2,2,2};
Circle C(centre,0.3,6);//Not at centre
Helix H(2,4,4,250);


Circle CC(centre,0.3,4);//Not at centre

XYZ centrenew  = {1,2.5,0};
Circle Cnew(centrenew,0.2,30);//Not at centre



/* Circular Cone Helix */
XYZ origin  = {0,0,0};
Circle C1(origin,0.1,30);
HelixCone HC(2,4,8,200);


/*
//Rectangular Helix
const double sub=0.1;
XYZ Rc1Pts[4] = {
	{ 2.0-sub, -sub, 0.0},
	{ 2.0-sub,  sub, 0.0},
        { 2.0+sub,  sub, 0.0},
	{ 2.0+sub, -sub, 0.0}
};
Rectangle R1(Rc1Pts);
*/


/*Cylinder And Box */

XYZ RcPts[4] = {
	{ 0.5, -0.5, 0.0},
	{ -0.5, -0.5, 0.0},
	{ -0.5, 0.5, 0.0},
	{ 0.5, 0.5, 0.0}
};

XYZ start = {0,0,0},end = {0,0,-4.0};

Circle Cyl(origin,0.4);//Origin
Rectangle R(RcPts);//Origin
Line L(start,end,2);

XYZ end2 = {-0.15,0,0.4};
Line L1(start,end2,2);

/*Cofee Pot*/
#define CofeeRadius 2
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



/**************************************Sandy********************/

Flower F(1.5,3,12,20);
XYZ L2start = {1.5,.5,1.0},L2end = {1.5,-0.5,-1.0};
Line L22(L2start*2.3,L2end*2.3,10);

XYZ Lstart = {1.5,.5,1},Lend = {1.5,-.50,-1.0};
Line L11(Lstart,Lend,10);
xyz cpt={0.0,0.0,0.5};
Line L11C(centrenew,centrenew+cpt,2);
Flower F1(0.6,8,5,20);




//Sweep *S;
Object *S;

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


/*
	//Rotation COde
	glColor3f(0, 1.0, 0);

	int cnt = 5;
	double angle=6.283/cnt;

	for(i=0;i<cnt;i++){
		//C.Rotate(angle*i,1,0,0);

		C.Translate(0.0,0.0,i);
		//C.FixCurve();
			glBegin(GL_LINE_STRIP);
		   		while((p = C.Iterator()) != NULL){
					glVertex3f(p->x,p->y,p->z);

				}
		    	glEnd();
		C.Restore();
	}
	return;

*/
/*
	if(ShowCurve){
		glColor3f(0,0,1.0);
			glBegin(GL_LINE_STRIP);
			while((p = ((S+CurveIndex)->Path)->Iterator()) != NULL){
				glVertex3f(p->x,p->y,p->z);

			}
    		glEnd();

		glColor3f(1.0,0 , 0);
			glBegin(GL_LINE_STRIP);
	     		while((p = (S+CurveIndex)->Surface->Iterator()) != NULL){
				glVertex3f(p->x,p->y,p->z);
			}
	    		glEnd();
		glPopMatrix ();
    		glutSwapBuffers();
    		glFlush();
		return;
	}
*/
	
	   XYZ Pt[4];
	   int j=0;
	   glColor3f(0.5,0.5,1.0);
	   Vector tt,n,sub1,sub2;
//	   Vector vcolr,vpos;
//		fprintf(stderr, "\nInside Display\n");
		if(lights)
	   		while((S+CurveIndex)->MeshPoints(Pt)){
//				fprintf(stderr, "\nlights c=1 CurveIndex= %d\n",CurveIndex);
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
//				fprintf(stderr, "\nlights c=0 CurveIndex= %d\n",CurveIndex);			
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
	//GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};
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
  //gluPerspective(30.0, 1.0,NEAR_Z, FAR_Z);
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
   int const cnt = 90, xcnt = 190, ycnt = 190, zcnt = 190;
   double const TwoPI = 360, arcTwoPI = 2*PI;
   double incr=1.0*TwoPI/cnt, arcincr=1.0*arcTwoPI/xcnt;
   double Autox_Angle = 1.0*TwoPI/xcnt, Autoy_Angle = 1.0*TwoPI/ycnt , Autoz_Angle = 1.0*TwoPI/zcnt;
   double Autox_trans=1.15, Autoy_trans=-0.6, Autox_range=2.5, Autoy_range=3.0;
   
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
		Auto^=1;
		AngleAuto = fmod(AngleAuto+ arcincr,arcTwoPI);
		x_translate = Autox_trans + cos(AngleAuto)*Autox_range;
		y_translate = Autoy_trans + sin(AngleAuto)*Autoy_range;
		x_Angle = fmod((x_Angle + Autox_Angle),TwoPI);
		y_Angle = fmod((y_Angle + Autoy_Angle),TwoPI);	 			
		z_Angle = fmod((z_Angle + Autoz_Angle),TwoPI);			
		printf("\n (%d) x_translate = %lf y_translate = %lf AngleAuto=%lf",aassdd++,x_translate,y_translate,AngleAuto);
      		glutPostRedisplay();		
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
/*
   case 'm':
      z_translate = z_translate * 1.5;
      glutPostRedisplay();
      break;
   case 'M':
      z_translate = z_translate / 1.5;
      glutPostRedisplay();
      break;
*/
	 case 27:exit(0);
   }
}

void printKeyBindings(){
	system("clear");
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
	char *hfun[3]= {
		"1+sin(t)",
		"1.25*(1+cos(t))",
		"t/(3.141592654)"
	};
	Parametric p(hfun, 0, PI*2);
	
	char *cfun[3]= {
		"2+0.3*cos(t)",
		"0.3*sin(t)",
		"0"
	};
	Parametric C(cfun, 0, PI*3,10);
	XYZ ptemp[]={ {0, 0, 0}, {1, 2, 3}, {2, 2.5, -1.5}, {3, -2, 1.2} };
	Lagranges lag(ptemp, 4, 40);

	/*
		points[i].x= (double)cos(t) * radius;
		points[i].y= (double)sin(t) * radius;
		points[i].z= z;


		Helix(double RadiusIN,double HeightIN,int nTurnsIN,int n=30)
		Helix H(2,4,8,1000);




	*/
	char *FunHeliX[3]= {
		"2*cos(t)",
		"2*sin(t)",
		"t/(3.141592654*4/4.0)"
	};
	Parametric ParametricHelix(FunHeliX,0,2*PI*8/4.0,40);


	CCofee.Rotate(-PI/2,1,0,0);
	CCofee.FixCurve();


	C.Translate(-centre);
	C.Rotate(-PI/2,1,0,0);
	C.Translate(centre);
	C.FixCurve();

	CC.Translate(-centre);
	CC.Rotate(-PI/2,1,0,0);
	CC.Translate(centre);
	CC.FixCurve();


	Cnew.Translate(-centrenew);
	Cnew.Rotate(-PI/2,1,1,1);
	Cnew.Translate(centrenew);
	Cnew.FixCurve();
	Sweep s[TOTAL_SWEEPS-1]={

		/*
		Sweep(&L11C,&p),
		Sweep(&p,&L11C),
		Sweep(&B,&B1),
		*/
//		Sweep(&C,&H),
		Sweep(&lag,&C),
		Sweep(&CC,&ParametricHelix)

/*
		Sweep(&C1,&HC),
		Sweep(&Cyl,&L),
		Sweep(&R,&L),
		Sweep(&BCofee,&CCofee),

		Sweep(&L11,&F),
		Sweep(&L22,&F1),
		Sweep(&F1,&L),
*/



	//		Sweep(&Cyl,&L),
	//		Sweep(&L22,&L1)
	//		Sweep(&C, &p)
	};
	
	s[0].Rotate(M_PI/4, 1, 1, 1);
	s[0].Translate(centre);
	s[0].FixCurve();
	s[1].Rotate(M_PI/4, -1, -1, -1);
	s[1].Translate(-centre);
	s[1].FixCurve();
	
	fprintf(stderr, "\nHi\n");
		
	Object obj[3];
	
	fprintf(stderr, "\ni\n");
		
	obj[0].addSweep(&s[0]);
	obj[1].addSweep(&s[1]);
	obj[2].addSweep(&s[0]);
	obj[2].addSweep(&s[1]);

	fprintf(stderr, "\nam\n");
	
	S = obj;
	
	fprintf(stderr, "\nBoy\n");
		
	/*BCofee.Translate(-2,0,0);
	BCofee.FixCurve();
	*/
	printKeyBindings();
	fprintf(stderr, "\nBoy1\n");

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
}


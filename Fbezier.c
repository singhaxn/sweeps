#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include "FBazier.h"
GLfloat ctrlpoints[4][3] = {
    { -4.0, -4.0, 0.0}, { -2.0, 4.0, 0.0},
    {2.0, -4.0, 0.0}, {4.0, 4.0, 0.0}};




/*

GLfloat ctrlpoints[4][3] = {
	    {4.0, 4.0, 0.0},
      {0.5, 4.0, 0.5},//u1v2
      {2.5, 0.5, 1.0},//u1v3
      {4.0, 4.0, -2.0}
};
*/
XYZ cPts[4];
void convert(){
	int i;

   for(i=0;i<4;i++){
   		cPts[i].x = ctrlpoints[i][0];
         cPts[i].y = ctrlpoints[i][1];
         cPts[i].z = ctrlpoints[i][2];
      }

}
void myinit(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4,&ctrlpoints[0][0]);
    glEnable(GL_MAP1_VERTEX_3);
    glShadeModel(GL_FLAT);
}

void display(void)
{
    int i;
    //printf("\nIn display\n");
    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4,&ctrlpoints[0][0]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    glBegin(GL_LINE_STRIP);
		convert();
      XYZ p;

        for (i = 0; i <= 30; i++){
         	p = Bezier4(cPts[0],cPts[1],cPts[2],cPts[3],i/30.0);
         //   glEvalCoord1f((GLfloat) i/30.0);
         		glVertex3f(p.x,p.y,p.z);
    		}
    glEnd();

   /* The following code displays the control points as dots. */
    glPointSize(5.0);
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_POINTS);
        for (i = 0; i < 4; i++)
            glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
    glFlush();
}

void mouse(int button,int state,int x,int y){
	static int whichPoint = 0;
   int i;
        switch(button){
                case GLUT_LEFT_BUTTON:
                        if(state == GLUT_DOWN){
									whichPoint = (whichPoint+ 1)%4;

									ctrlpoints[whichPoint][0] = (((float)x-250.0)/250.0)*5.0;
									ctrlpoints[whichPoint][1] = -(((float)y-250.0)/250.0)*5.0;
		 					        printf("\n(Point #%d )Pressed Left %f %f!!\n",whichPoint,(((float)x-250.0)/250.0)*5.0,-(((float)y-250.0)/250.0)*5.0);
                             display();

               			}
                break;

        }

}


void myReshape(GLsizei w, GLsizei h)
{
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


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	Vector v, w;
	
	v.x=1;
	v.y=2;
	v.z=0;
	
	w=-v;
	
	printf("(%lf, %lf, %lf)", w.x, w.y, w.z);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);// | GLUT_DEPTH );
	glutInitWindowSize (500,500);
	glutInitWindowPosition (0,0);
	glutCreateWindow (argv[0]);
	myinit();
	glutReshapeFunc (myReshape);
	glutDisplayFunc (display);
	glutMouseFunc(mouse);
	glutMainLoop();
}

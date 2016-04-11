/* recursive procedure to form Menger Sponge */
/* number of recursive steps given on command line */
/* default is 3 with no parameter */
/* maximum recursion level allowed is 5 (why?) */

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>  
#include <stdio.h>

/* initial  */
GLfloat c[8][3]={{0.0, 0.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 1.0, 1.0}, {0.0, 1.0, 0.0},
                 {1.0, 0.0, 0.0}, {1.0, 1.0, 0.0}, {1.0, 1.0, 1.0}, {1.0, 0.0, 1.0}};

static const GLfloat colors[6][3] = {{1.0, 0.0, 0.0},
                                     {0.0, 1.0, 0.0},
                                     {0.0, 0.0, 1.0},
                                     {1.0, 1.0, 0.0},
                                     {0.0, 1.0, 1.0},
                                     {1.0, 0.0, 1.0}};

int n=2;
int currX = 0;
int currY = 0;
int displaySize = 700;
bool rotate = false;
char buf[100];

GLfloat axisY = 0.0;

GLfloat rotationAngleRotateX = 0.0;
GLfloat rotationAngleRotateY = 0.0;
GLfloat rotationMatrixRotate[] = {1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0};

void init()
{
/* attributes */
   glClearColor(0.0, 0.0, 0.0, 1.0); /* black background */
   glEnable(GL_DEPTH_TEST);

/* set light */
   GLfloat position[]={-3.0, 3.0, 0.0, 1.0};
   GLfloat ambience[]={1.0, 1.0, 1.0, 1.0};
   GLfloat diffusive[]={1.0, 0.0, 0.0, 1.0};
   GLfloat specular[]={1.0, 1.0, 1.0, 1.0};
   GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
   GLfloat local_view[] = {1.0};

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glLightfv(GL_LIGHT0, GL_POSITION, position);
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambience);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusive);
   glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
   glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
   GLfloat matAmbient1[] = {0.33, 0.22, 0.03, 1.0}; 
   GLfloat matDiffuse1[] = {0.78, 0.57, 0.11, 1.0}; 
   GLfloat matSpecular1[] = {0.99, 0.91, 0.81, 1.0}; 
   GLfloat matShininess1 =  27.8;
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient1);
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse1);
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular1);
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, matShininess1);

}

// Creates a unit vector
void normalize(float v[3])  
{  
   float d = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);  
   if (d != 0.0) 
   {  
      v[0]/=d; 
      v[1]/=d;  
      v[2]/=d;  
   }  
}

//Vector cross product 
void normCrossProd(float v1[3], float v2[3], float out[3])  
{  
   out[0] = v1[1]*v2[2] - v1[2]*v2[1];  
   out[1] = v1[2]*v2[0] - v1[0]*v2[2];  
   out[2] = v1[0]*v2[1] - v1[1]*v2[0];  
   normalize(out);  
} 

/* Since GL_QUAD is called prior to this method, it simply list the 
   corners of the square */
void square(GLfloat va[3], GLfloat vb[3], GLfloat vc[3], GLfloat vd[3]) 
{
   /* The following lines of code are used to create
      a normal vector. This is mainly used to ensure
      that the object will have a normal for proper
      shading */
   float v1[3], v2[3], normal[3];
   v1[0] = vc[0] - va[0];
   v1[1] = vc[1] - va[1];
   v1[2] = vc[2] - va[2];

   v2[0] = vc[0] - vb[0];
   v2[1] = vc[1] - vb[1];
   v2[2] = vc[2] - vb[2];

   normCrossProd(v1, v2, normal);  
 
   glNormal3fv(normal);
   glVertex3fv(va);
   glVertex3fv(vb);
   glVertex3fv(vc);
   glVertex3fv(vd);
}

/* Draws a cube by creating a square at each face */
void cube(GLfloat v[8][3]) 
{
   square(v[0], v[1], v[2], v[3]);
   square(v[0], v[4], v[7], v[1]);
   square(v[0], v[3], v[5], v[4]);
   square(v[6], v[7], v[4], v[5]);
   square(v[6], v[5], v[3], v[2]);
   square(v[6], v[2], v[1], v[7]);
}

/* Build the sponge recursively */
void sponge(GLfloat v[8][3], int m) 
{
   GLfloat vv[8][3];
   static GLfloat scale = 1.0 / 3.0;
   int i, j, k, l;

   if(m > 0) 
   {
    
      for (i = 0; i < 3; i++) 
      {
         for (j = 0; j < 3; j++) 
         {
            for (k = 0; k < 3; k++) 
            {
              /* leave holes in the right places */
               if (i%2 + j%2 + k%2 < 2) 
               { 
                  for (l = 0; l < 8; l++) 
                  {
                     vv[l][0] = (v[l][0] + i) * scale;
                     vv[l][1] = (v[l][1] + j) * scale;
                     vv[l][2] = (v[l][2] + k) * scale;
                  }
                  sponge(vv, m - 1);
               }
            }
         }
      }
   }
   else
   {
     (cube(v)); /* draw cube at end of recursion */
   }
}

/* display callback */
void display() 
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glPushMatrix();
   glTranslatef(0, 0, 0);
   glRotatef(rotationAngleRotateX, 0.0, 1.0, 0.0);
   glRotatef(-rotationAngleRotateY, 1.0, 0.0, 0.0);
   glBegin(GL_QUADS);
   sponge(c, n);
   glEnd();
   glPopMatrix();
   glPushMatrix();
   glLoadIdentity();
   glDisable(GL_DEPTH_TEST);
   //char buf[100];
   //print(-3, 3, 0, buf, 300);
   glEnable(GL_DEPTH_TEST);
   glPopMatrix();
   //glutBitmapCharacter( GLUT_BITMAP_8_BY_13 , n);
   glFlush();
}


/* reshape callback, set lights and viewing parameters, redraw */
void myReshape(int w, int h) 
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 1.0, 10.0);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(3.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
   glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
   switch(key)
   {
      case '+':
         n++;
         if (n > 3) n = 3;
         glutRemoveMenuItem(5);
         sprintf(buf, "Depth of sponge: %d", n);
         glutAddMenuEntry(buf, 5);
         glutPostRedisplay();
         break;
      case '-':
         n--;
         if (n < 0) n = 0;
         glutRemoveMenuItem(5);
         sprintf(buf, "Depth of sponge: %d", n);
         glutAddMenuEntry(buf, 5);
         glutPostRedisplay();
         break;
   }
}

void mouseButton(int button, int state, int x, int y)
{
   if(state == GLUT_DOWN)
   {
      currX = x;
      currY = y;
      if(button == GLUT_LEFT_BUTTON)
      {
         rotate = true;
      }
   }
   else
   {
      rotate = false;
      glutPostRedisplay();
   }
}

void mouseMotion(int x, int y)
{
   //Generate degrees using current x/y and universalX/Y
   float degreesX = 360*(x-currX)/sqrt(displaySize*displaySize * 2);
   float degreesY = 360*(y-currY)/sqrt(displaySize*displaySize * 2);
   //Set universalX/Y to current x/y
   if(x - currX == 0)
   {	
      axisY = 0;	
   }
   else
   {
      axisY = (y-currY)/(x-currX);	
   }

   currX = x;
   currY = y;

   //if left mouse down rotate
   if(rotate)
   {
      rotationAngleRotateX += degreesX;
      rotationAngleRotateY += degreesY;
      /*if((degreesX < 90) || (270 < degreesX))
      {  
         rotationAngleRotateY = -rotationAngleRotateY;
      }   */
   }
   //recall display method
   glutPostRedisplay();
}

void myMenu(int value)
{
   if(value == 1)
   {
      n = 0;
      glutRemoveMenuItem(5);
      sprintf(buf, "Depth of sponge: %d", n);
      glutAddMenuEntry(buf, 5);
     
      glutPostRedisplay();
   }
   if(value == 2)
   {
      n = 1;
      glutRemoveMenuItem(5);
      sprintf(buf, "Depth of sponge: %d", n);
      glutAddMenuEntry(buf, 5);
      glutPostRedisplay();
   }
   if(value == 3)
   {
      n = 2;
      glutRemoveMenuItem(5);
      sprintf(buf, "Depth of sponge: %d", n);
      glutAddMenuEntry(buf, 5);
      glutPostRedisplay();
   }
   if(value == 4)
   {
      n = 3;
      glutRemoveMenuItem(5);
      sprintf(buf, "Depth of sponge: %d", n);
      glutAddMenuEntry(buf, 5);
      glutPostRedisplay();
      glutPostRedisplay();
   }
}

int getDepth()
{
   return n;
}

int main(int argc, char **argv) {
  /* enter number of subdivision steps here */
  /* limited to 5 to prevent disasters */
  //if (argc > 1) {
  //  n=atoi(argv[1]);
  //}
  

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(700, 700);
  glutCreateWindow("Zach Peppler: Project 4");
  init();
  glutReshapeFunc(myReshape);
  //init();
  glutKeyboardFunc(keyboard);
  glutDisplayFunc(display);
  glutCreateMenu(myMenu);
  glutAddMenuEntry("Change depth of sponge to 0", 1);
  glutAddMenuEntry("Change depth of sponge to 1", 2);
  glutAddMenuEntry("Change depth of sponge to 2", 3);
  glutAddMenuEntry("Change depth of sponge to 3", 4);
  sprintf(buf, "Depth: %d", n);
  glutAddMenuEntry(buf, 5);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  glutMouseFunc(mouseButton);
  glutMotionFunc(mouseMotion);

  /* flat shade the flat surfaces */
  //glShadeModel(GL_FLAT);

  /* cull back faces for speed */
  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glutMainLoop();
}

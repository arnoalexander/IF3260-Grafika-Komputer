
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>
#include <GL/gl.h>

#define MAX_PARTICLES 3000
#define WCX		640
#define WCY		480
#define RAIN	0
#define SNOW	1
#define	HAIL	2

clock_t start;
bool notDetermined = true;

float slowdown = 2.0;
float velocity = 0.0;
float zoom = -40.0;
float pan = 0.0;
float tilt = 0.0;
float flank = 0.0;
float hailsize = 0.1;

int loop;
int fall;
int frame = 0;

//floor colors
float r = 0.0;
float g = 1.0;
float b = 0.0;
float ground_points[21][21][3];
float ground_colors[21][21][4];
float accum = -10.0;

typedef struct {
  // Life
  bool alive;	// is the particle alive?
  float life;	// particle lifespan
  float fade; // decay
  // color
  float red;
  float green;
  float blue;
  // Position/direction
  float xpos;
  float ypos;
  float zpos;
  // Velocity/Direction, only goes down in y dir
  float vel;
  // Gravity
  float gravity;
}particles;

// Paticle System
particles par_sys[MAX_PARTICLES];
particles smoke_sys[MAX_PARTICLES];

void normal_keys(unsigned char key, int x, int y) {
  if (key == 'r') { // Rain
    fall = RAIN;
    glutPostRedisplay();
  }
  if (key == 'h') { // Hail
    fall = HAIL;
    glutPostRedisplay();
  }
  if (key == 's') { // Snow
    fall = SNOW;
    glutPostRedisplay();
  }
  if (key == '=') { //really '+' - make hail bigger
    hailsize += 0.01;
  }
  if (key == '-') { // make hail smaller
    if (hailsize > 0.1) hailsize -= 0.01;
  }
  if (key == ',') { // really '<' slow down
    if (slowdown > 4.0) slowdown += 0.01;
  }
  if (key == '.') { // really '>' speed up
    if (slowdown > 1.0) slowdown -= 0.01;
  }
  if (key == 'q') { // QUIT
    exit(0);
  }
}

void special_keys(int key, int x, int y) {
  if (key == GLUT_KEY_UP) {
    zoom += 1.0;
  }
  if (key == GLUT_KEY_DOWN) {
    zoom -= 1.0;
  }
  if (key == GLUT_KEY_RIGHT) {
    flank -= 1.0;
  }
  if (key == GLUT_KEY_LEFT) {
    flank += 1.0;
  }
  if (key == GLUT_KEY_PAGE_UP) {
    tilt -= 1.0;
  }
  if (key == GLUT_KEY_PAGE_DOWN) {
    tilt += 1.0;
  }
  if (key == GLUT_KEY_END) {
    pan += 1.0;
  }
  if (key == GLUT_KEY_HOME) {
    pan -= 1.0;
  }
}


// Initialize/Reset Particles - give them their attributes
void initParticles(int i) {
    par_sys[i].alive = true;
    par_sys[i].life = 1.0;
    par_sys[i].fade = float(rand()%100)/1000.0f+0.003f;

    par_sys[i].xpos = (float) (rand() % 21) - 10;
    par_sys[i].ypos = 10.0;
    par_sys[i].zpos = (float) (rand() % 21) - 10;

    par_sys[i].red = 0.5;
    par_sys[i].green = 0.5;
    par_sys[i].blue = 1.0;

    par_sys[i].vel = velocity;
    par_sys[i].gravity = -0.8;
}

// Initialize/Reset Smoke - give them their attributes
void initSmoke(int i) {
    smoke_sys[i].alive = true;
    smoke_sys[i].life = 1.0;
    smoke_sys[i].fade = float(rand()%100)/100.0f+0.003f;

    smoke_sys[i].xpos = ((float) (rand() % 21) - 10) / 20.0f;
    smoke_sys[i].ypos = (float) (rand() % 21) / 15.0f - 9.5f;
    smoke_sys[i].zpos = ((float) (rand() % 21) - 10) / 20.0f;

    smoke_sys[i].red = 0.8;
    smoke_sys[i].green = 0.6;
    smoke_sys[i].blue = 0.5;

    smoke_sys[i].vel = velocity;
    smoke_sys[i].gravity = 1.0;
}

void init( ) {
  int x, z;

    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);

  // Ground Verticies
    // Ground Colors
    for (z = 0; z < 21; z++) {
      for (x = 0; x < 21; x++) {
        ground_points[x][z][0] = x - 10.0;
        ground_points[x][z][1] = accum;
        ground_points[x][z][2] = z - 10.0;

        ground_colors[z][x][0] = r; // red value
        ground_colors[z][x][1] = g; // green value
        ground_colors[z][x][2] = b; // blue value
        ground_colors[z][x][3] = 0.0; // acummulation factor
      }
    }

    // Initialize particles
    for (loop = 0; loop < MAX_PARTICLES; loop++) {
        initParticles(loop);
        initSmoke(loop);
    }
}

// For Car
void drawCar() {
  glBegin(GL_QUADS);
    // belakang
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glVertex3f(0.5, -9, -2);
    glVertex3f(0.5, -9, 2);
    glVertex3f(0.5, -5, 2);
    glVertex3f(0.5, -5, -2);

    // depan
    glColor4f(1.0, 0.0, 0.0, 1.0);
    glVertex3f(8, -9, -2);
    glVertex3f(8, -9, 2);
    glVertex3f(8, -5, 2);
    glVertex3f(8, -5, -2);

    // bawah
    glColor4f(1.0, 0.0, 0.4, 1.0);
    glVertex3f(0.5, -9, -2);
    glVertex3f(0.5, -9, 2);
    glVertex3f(8, -9, 2);
    glVertex3f(8, -9, -2);

    // atas
    glColor4f(1.0, 0.0, 0.4, 1.0);
    glVertex3f(0.5, -5, 2);
    glVertex3f(0.5, -5, -2);
    glVertex3f(8, -5, -2);
    glVertex3f(8, -5, 2);

    // samping
    glColor4f(1.0, 0.0, 0.2, 1.0);
    glVertex3f(8, -9, 2);
    glVertex3f(0.5, -9, 2);
    glVertex3f(0.5, -5, 2);
    glVertex3f(8, -5, 2);

    glColor4f(1.0, 0.0, 0.2, 1.0);
    glVertex3f(8, -9, -2);
    glVertex3f(0.5, -9, -2);
    glVertex3f(0.5, -5, -2);
    glVertex3f(8, -5, -2);
  glEnd();
}

// For Smoke
void drawSmoke() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (smoke_sys[loop].alive == true) {
      x = smoke_sys[loop].xpos;
      y = smoke_sys[loop].ypos;
      z = smoke_sys[loop].zpos;

      // printf("%d\n", loop);

      // Draw particles
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glColor4f(.5, .5, .5, .2);
      glBegin(GL_QUADS);
        glVertex3f(x, y, z);
        glVertex3f(x, y+0.2, z);
        glVertex3f(x+0.2, y+0.2, z);
        glVertex3f(x+0.2, y, z);
      glEnd();

      // Update values
      //Move
      // Adjust slowdown for speed!
      smoke_sys[loop].ypos += smoke_sys[loop].vel / (slowdown*1000);
      smoke_sys[loop].vel += smoke_sys[loop].gravity;
      // Decay
      smoke_sys[loop].life -= smoke_sys[loop].fade*2;

      if (smoke_sys[loop].ypos <= -10) {
        smoke_sys[loop].life = -1.0;
      }
      //Revive
      if (smoke_sys[loop].life < 0.0) {
        initSmoke(loop);
      }
    }
  }
}

// For Rain
void drawRain() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos;

      // Draw particles
      glColor3f(0.5, 0.5, 1.0);
      glBegin(GL_LINES);
        glVertex3f(x, y, z);
        glVertex3f(x, y+0.5, z);
      glEnd();

      // Update values
      //Move
      // Adjust slowdown for speed!
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= -10) {
        par_sys[loop].life = -1.0;
      }
      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}

// For Hail
void drawHail() {
  float x, y, z;

  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos;

      // Draw particles
      glColor3f(0.8, 0.8, 0.9);
      glBegin(GL_QUADS);
        // Front
        glVertex3f(x-hailsize, y-hailsize, z+hailsize); // lower left
        glVertex3f(x-hailsize, y+hailsize, z+hailsize); // upper left
        glVertex3f(x+hailsize, y+hailsize, z+hailsize); // upper right
        glVertex3f(x+hailsize, y-hailsize, z+hailsize); // lower left
        //Left
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        // Back
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        //Right
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
        //Top
        glVertex3f(x-hailsize, y+hailsize, z+hailsize);
        glVertex3f(x-hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z-hailsize);
        glVertex3f(x+hailsize, y+hailsize, z+hailsize);
        //Bottom
        glVertex3f(x-hailsize, y-hailsize, z+hailsize);
        glVertex3f(x-hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z-hailsize);
        glVertex3f(x+hailsize, y-hailsize, z+hailsize);
      glEnd();

      // Update values
      //Move
      if (par_sys[loop].ypos <= -10) {
        par_sys[loop].vel = par_sys[loop].vel * -1.0;
      }
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000); // * 1000
      par_sys[loop].vel += par_sys[loop].gravity;

      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}

// For Snow
void drawSnow() {
  float x, y, z;
  for (loop = 0; loop < MAX_PARTICLES; loop=loop+2) {
    if (par_sys[loop].alive == true) {
      x = par_sys[loop].xpos;
      y = par_sys[loop].ypos;
      z = par_sys[loop].zpos;

      // Draw particles
      glColor3f(1.0, 1.0, 1.0);
      glPushMatrix();
      glTranslatef(x, y, z);
      glutSolidSphere(0.2, 16, 16);
      glPopMatrix();

      // Update values
      //Move
      par_sys[loop].ypos += par_sys[loop].vel / (slowdown*1000);
      par_sys[loop].vel += par_sys[loop].gravity;
      // Decay
      par_sys[loop].life -= par_sys[loop].fade;

      if (par_sys[loop].ypos <= -10) {
        int zi = z + 10;
        int xi = x + 10;
        ground_colors[zi][xi][0] = 1.0;
        ground_colors[zi][xi][2] = 1.0;
        ground_colors[zi][xi][3] += 1.0;
        if (ground_colors[zi][xi][3] > 1.0) {
          ground_points[xi][zi][1] += 0.1;
        }
        par_sys[loop].life = -1.0;
      }

      //Revive
      if (par_sys[loop].life < 0.0) {
        initParticles(loop);
      }
    }
  }
}

// Draw Particles
void drawScene( ) {
  int i, j;
  float x, y, z;

  frame++;
  // printf("frame = %d\n", frame);

  // printf("duration = %f\n", (float)(clock() - start) / CLOCKS_PER_SEC);
  if (10 * (clock() - start) >= CLOCKS_PER_SEC && notDetermined) {
    printf("frame per sec = %d\n", frame);
    start = clock();
    frame = 0;
  }

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();


  glRotatef(pan, 0.0, 1.0, 0.0);
  glRotatef(tilt, 1.0, 0.0, 0.0);
  glTranslatef(flank, 0.0, zoom);

  // GROUND?!
  glColor3f(r, g, b);
  glBegin(GL_QUADS);
    // along z - y const
    for (i = -10; i+1 < 11; i++) {
      // along x - y const
      for (j = -10; j+1 < 11; j++) {
        glColor3fv(ground_colors[i+10][j+10]);
        glVertex3f(ground_points[j+10][i+10][0],
              ground_points[j+10][i+10][1],
              ground_points[j+10][i+10][2]);
        glColor3fv(ground_colors[i+10][j+1+10]);
        glVertex3f(ground_points[j+1+10][i+10][0],
              ground_points[j+1+10][i+10][1],
              ground_points[j+1+10][i+10][2]);
        glColor3fv(ground_colors[i+1+10][j+1+10]);
        glVertex3f(ground_points[j+1+10][i+1+10][0],
              ground_points[j+1+10][i+1+10][1],
              ground_points[j+1+10][i+1+10][2]);
        glColor3fv(ground_colors[i+1+10][j+10]);
        glVertex3f(ground_points[j+10][i+1+10][0],
              ground_points[j+10][i+1+10][1],
              ground_points[j+10][i+1+10][2]);
      }

    }
  glEnd();
  // Which Particles
  if (fall == RAIN) {
    drawRain();
  }else if (fall == HAIL) {
    drawHail();
  }else if (fall == SNOW) {
    drawSnow();
  }
  // Draw Smoke
  drawSmoke();
  // Draw Car
  drawCar();

  glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, (float) w / (float) h, .1, 200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void idle ( ) {
  glutPostRedisplay();
}

int main (int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(WCX, WCY);
  glutCreateWindow("Partikel Mantap - Anjas and Friends");
  init();
  start = clock();
  glutDisplayFunc(drawScene);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(normal_keys);
  glutSpecialFunc(special_keys);
  glutIdleFunc(idle);
  glutMainLoop();
  return 0;
}

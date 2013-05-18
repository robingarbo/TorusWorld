//
// game.cpp for game in /home/robin/TorusWorld/game/src
// 
// Made by robin garbo
// email   <robingarbo@gmail.com>
// 
// Started on  Tue Apr 30 04:31:14 2013 robin garbo
// ##Last update Tue Apr 30 04:36:53 2013 robin garbo
//

#include "game.hh"

typedef struct {         // vertex coordinates - 3d and texture
    GLfloat x, y, z;     // 3d coords.
    GLfloat u, v;        // texture coords.
} VERTEX;

typedef struct {         // triangle
    VERTEX vertex[3];    // 3 vertices array
} TRIANGLE;

typedef struct {         // sector of a 3d environment
    int numtriangles;    // number of triangles in the sector
    TRIANGLE* triangle;  // pointer to array of triangles.
} SECTOR;

SECTOR sector1;

Game *global_game = NULL;



// helper for SetupWorld.  reads a file into a string until a nonblank, non-comment line
// is found ("/" at the start indicating a comment); assumes lines < 255 characters long.
void readstr(FILE *f, char *string)
{
    do {
	fgets(string, 255, f); // read the line
    } while ((string[0] == '/') || (string[0] == '\n'));
    return;
}

// degrees to radians...2 PI radians = 360 degrees
float Game::rad(float angle)
{
    return angle * piover180;
}


// loads the world from a text file.
void Game::SetupWorld()
{
    float x, y, z, u, v;
    int vert;
    int numtriangles;
    FILE *filein;        // file to load the world from
    char oneline[255];

    filein = fopen("data/world.txt", "rt");
    readstr(filein, oneline);
	std::cout << "readstr\n";
    sscanf(oneline, "NUMPOLLIES %d\n", &numtriangles);
	
    sector1.numtriangles = numtriangles;
    sector1.triangle = (TRIANGLE *) malloc(sizeof(TRIANGLE)*numtriangles);
	std::cout << "loop\n";
    for (loop = 0; (int)loop < numtriangles; loop++) {
	for (vert = 0; vert < 3; vert++) {
	    readstr(filein,oneline);
	    sscanf(oneline, "%f %f %f %f %f", &x, &y, &z, &u, &v);
	    sector1.triangle[loop].vertex[vert].x = x;
	    sector1.triangle[loop].vertex[vert].y = y;
	    sector1.triangle[loop].vertex[vert].z = z;
	    sector1.triangle[loop].vertex[vert].u = u;
	    sector1.triangle[loop].vertex[vert].v = v;
	}
    }

    fclose(filein);
}
// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.
int Game::ImageLoad(char *filename, Image *image)
{
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1)
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // used to convert bgr to rgb color.

    // make sure the file is there.
    if ((file = fopen(filename, "rb")) == NULL) {
      printf("File Not Found : %s\n",filename);
      return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // No 100% errorchecking anymore!!!

    // read the width
    image->sizeX = getint (file);
    printf("Width of %s: %lu\n", filename, image->sizeX);

    // read the height
    image->sizeY = getint (file);
    printf("Height of %s: %lu\n", filename, image->sizeY);

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    planes = getshort(file);
    if (planes != 1) {
	printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    // read the bpp
    bpp = getshort(file);
    if (bpp != 24) {
      printf("Bpp from %s is not 24: %u\n", filename, bpp);
      return 0;
    }

    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL) {
	printf("Error allocating memory for color-corrected image data");
	return 0;
    }

    if ((i = fread(image->data, size, 1, file)) != 1) {
	printf("Error reading image data from %s.\n", filename);
	return 0;
    }

    for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
	temp = image->data[i];
	image->data[i] = image->data[i+2];
	image->data[i+2] = temp;
    }

    // we're done.
    return 1;
}

// Load Bitmaps And Convert To Textures
void Game::LoadGLTextures()
{
    // Load Texture
    Image *image1;

    // allocate space for texture
    image1 = (Image *) malloc(sizeof(Image));
    if (image1 == NULL) {
	printf("Error allocating space for image");
	exit(0);
    }

   // if (!ImageLoad((char*)"data/mud.bmp", image1)) {
	if (!ImageLoad((char*)"data/crate.bmp", image1)) {
	exit(1);
    }

    // Create Textures
    glGenTextures(3, &texture[0]);

    // nearest filtered texture
    glBindTexture(GL_TEXTURE_2D, texture[0]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale cheaply when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale cheaply when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    // linear filtered texture
    glBindTexture(GL_TEXTURE_2D, texture[1]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image1->sizeX, image1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image1->data);

    // mipmapped texture
    glBindTexture(GL_TEXTURE_2D, texture[2]);   // 2d texture (x and y size)
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST); // scale mipmap when image smalled than texture
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image1->sizeX, image1->sizeY, GL_RGB, GL_UNSIGNED_BYTE, image1->data);
};

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void Game::InitGL(GLsizei Width, GLsizei Height)	// We call this right after our OpenGL window is created.
{
    LoadGLTextures();                           // load the textures.
    glEnable(GL_TEXTURE_2D);                    // Enable texture mapping.

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);          // Set the blending function for translucency (note off at init time)
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// This Will Clear The Background Color To Black
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);                       // type of depth test to do.
    glEnable(GL_DEPTH_TEST);                    // enables depth testing.
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

    glMatrixMode(GL_MODELVIEW);

    // set up lights.
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHT1);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(GLsizei Width, GLsizei Height)
{
    if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
	Height=1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene(void)
{
    GLfloat x_m, y_m, z_m, u_m, v_m;
    GLfloat xtrans, ztrans, ytrans;
    GLfloat sceneroty;
    int numtriangles;

    // calculate translations and rotations.
    xtrans = -global_game->xpos;
    ztrans = -global_game->zpos;
    ytrans = -global_game->walkbias-0.25f;
    sceneroty = 360.0f - global_game->yrot;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();

    glRotatef(global_game->lookupdown, 1.0f, 0, 0);
    glRotatef(sceneroty, 0, 1.0f, 0);

    glTranslatef(xtrans, ytrans, ztrans);

    glBindTexture(GL_TEXTURE_2D, global_game->texture[global_game->filter]);    // pick the texture.

    numtriangles = sector1.numtriangles;

    for (global_game->loop=0; (int)global_game->loop < numtriangles; global_game->loop++) {        // loop through all the triangles
	glBegin(GL_TRIANGLES);
	glNormal3f( 0.0f, 0.0f, 1.0f);

	x_m = sector1.triangle[global_game->loop].vertex[0].x;
	y_m = sector1.triangle[global_game->loop].vertex[0].y;
	z_m = sector1.triangle[global_game->loop].vertex[0].z;
	u_m = sector1.triangle[global_game->loop].vertex[0].u;
	v_m = sector1.triangle[global_game->loop].vertex[0].v;
	glTexCoord2f(u_m,v_m);
	glVertex3f(x_m,y_m,z_m);

	x_m = sector1.triangle[global_game->loop].vertex[1].x;
	y_m = sector1.triangle[global_game->loop].vertex[1].y;
	z_m = sector1.triangle[global_game->loop].vertex[1].z;
	u_m = sector1.triangle[global_game->loop].vertex[1].u;
	v_m = sector1.triangle[global_game->loop].vertex[1].v;
	glTexCoord2f(u_m,v_m);
	glVertex3f(x_m,y_m,z_m);

	x_m = sector1.triangle[global_game->loop].vertex[2].x;
	y_m = sector1.triangle[global_game->loop].vertex[2].y;
	z_m = sector1.triangle[global_game->loop].vertex[2].z;
	u_m = sector1.triangle[global_game->loop].vertex[2].u;
	v_m = sector1.triangle[global_game->loop].vertex[2].v;
	glTexCoord2f(u_m,v_m);
	glVertex3f(x_m,y_m,z_m);

	glEnd();
    }

    // since this is double buffered, swap the buffers to display what just got drawn.
    glutSwapBuffers();
}




Game::Game()
{
	piover180 = 0.0174532925f;
	light = 0;
	blend = 0;
	walkbiasangle = 0;
	walkbias = 0;
	lookupdown = 0.0;
	camx = 0;
	camy = 0;
	camz = 0;
	z=0.0f;
	LightAmbient[0] = 0.5f;
	LightAmbient[1] = 0.5f;
	LightAmbient[2] = 0.5f;
	LightAmbient[3] = 1.0f;
	LightDiffuse[0] = 1.0f;
	LightDiffuse[1] = 1.0f;
	LightDiffuse[2] = 1.0f;
	LightDiffuse[3] = 1.0f;
	LightPosition[0] = 0.0f;
	LightPosition[1] = 0.0f;
	LightPosition[2] = 2.0f;
	LightPosition[3] = 1.0f;
	filter = 0;
	global_game = this; 
}

Game::~Game()
{
 global_game = NULL;
}

void Game::run()
{
	InitGL(640, 480);
    std::cout << " Start Event Processing Engine "<< std::endl; 
    glutMainLoop();
}


/* The function called whenever a normal key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
    /* avoid thrashing this procedure */
    usleep(100);

    switch (key) {
    case ESCAPE: // kill everything.
	/* exit the program...normal termination. */
	exit(1);
	break; // redundant.

    case 'b':
    case 'B': // switch the blending
	printf("B/b pressed; blending is: %d\n", global_game->blend);
	global_game->blend = global_game->blend ? 0 : 1;              // switch the current value of blend, between 0 and 1.
	if (global_game->blend) {
	    glEnable(GL_BLEND);
	    glDisable(GL_DEPTH_TEST);
	} else {
	    glDisable(GL_BLEND);
	    glEnable(GL_DEPTH_TEST);
	}
	printf("Blending is now: %d\n", global_game->blend);
	break;

    case 'f':
    case 'F': // switch the filter
	printf("F/f pressed; filter is: %d\n", global_game->filter);
	global_game->filter++;                           // switch the current value of filter, between 0/1/2;
	if (global_game->filter > 2) {
	    global_game->filter = 0;
	}
	printf("Filter is now: %d\n", global_game->filter);
	break;

    case 'l':
    case 'L': // switch the lighting
	printf("L/l pressed; lighting is: %d\n", global_game->light);
	global_game->light = global_game->light ? 0 : 1;              // switch the current value of light, between 0 and 1.
	if (global_game->light) {
	    glEnable(GL_LIGHTING);
	} else {
	    glDisable(GL_LIGHTING);
	}
	printf("Lighting is now: %d\n", global_game->light);
	break;

    default:
      printf ("Key %d pressed. No action there yet.\n", key);
      break;
    }
}

/* The function called whenever a normal key is pressed. */
void specialKeyPressed(int key, int x, int y)
{
	(void)x;
	(void)y;
    /* avoid thrashing this procedure */
    usleep(100);

    switch (key) {
    case GLUT_KEY_PAGE_UP: // tilt up
	global_game->z -= 0.2f;
	global_game->lookupdown -= 0.2f;
	break;

    case GLUT_KEY_PAGE_DOWN: // tilt down
	global_game->z += 0.2f;
	global_game->lookupdown += 1.0f;
	break;

    case GLUT_KEY_UP: // walk forward (bob head)
	global_game->xpos -= (float)sin(global_game->yrot*global_game->piover180) * 0.05f;
	global_game->zpos -= (float)cos(global_game->yrot*global_game->piover180) * 0.05f;
	if (global_game->walkbiasangle >= 359.0f)
	    global_game->walkbiasangle = 0.0f;
	else
	    global_game->walkbiasangle+= 10;
	global_game->walkbias = (float)sin(global_game->walkbiasangle * global_game->piover180)/20.0f;
	break;

    case GLUT_KEY_DOWN: // walk back (bob head)
	global_game->xpos += (float)sin(global_game->yrot*global_game->piover180) * 0.05f;
	global_game->zpos += (float)cos(global_game->yrot*global_game->piover180) * 0.05f;
	if (global_game->walkbiasangle <= 1.0f)
	    global_game->walkbiasangle = 359.0f;
	else
	    global_game->walkbiasangle-= 10;
	global_game->walkbias = (float)sin(global_game->walkbiasangle * global_game->piover180)/20.0f;
	break;

    case GLUT_KEY_LEFT: // look left
	global_game->yrot += 1.5f;
	break;

    case GLUT_KEY_RIGHT: // look right
	global_game->yrot -= 1.5f;
	break;

    default:
	printf ("Special key %d pressed. No action there yet.\n", key);
	break;
    }
}


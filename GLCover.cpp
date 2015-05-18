

#include <QtGui>
#include <QtOpenGL>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <cstring>
#include "GLCover.h"
#include "mainwindow.h"

using namespace std;
// user-defined datatypes
typedef float vector3f[3];
typedef struct {
    int		width;
    int		height;
    GLuint	texId;
    char	imageFilename[512];
} Record;

// global variables
Record  *_records;		// list of records (albums)
int	 _recordCount;		// number of records
int	 _shift=0;
float	 _scrollOffset=0;	// -1<offset<1 current scroll position
int	 _scrollDir;		// current scroll velocity
float	 _scrollTime=150;	// total scrolling time (per push)
int	 _scrollUpdateInterval=5;	// scrolling time increment
int	 _bScrolling=0;		// scrolling boolean: 1=true; 0=false

int  readPPM		(char*, int&, int&, unsigned char *&);
void initRecords	();
void display		();
void drawRecords	();
void drawRecord		(Record *record, bool flip=false);
void scrollTimer    (int value);
void keyPress       (unsigned char key, int x, int y);
void specialKeyPress(int key, int x, int y);
void scrollRecords  (int dir);



GLCover::GLCover(QGLWidget *parent): 
	QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

}



void GLCover::initializeGL() {

    glEnable(GL_DEPTH_TEST);
            glClearColor(0,0,0,0);

            // load textures
            glEnable(GL_TEXTURE_2D);
            int		ww, hh;
            unsigned char  *texData;
            for(int i=0; i<_recordCount; i++) {
                readPPM(_records[i].imageFilename, ww, hh, texData);
                glGenTextures  (1, &_records[i].texId);
                glBindTexture  (GL_TEXTURE_2D,  _records[i].texId);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D   (GL_TEXTURE_2D, 0, 3, ww, hh, 0, GL_RGB,
                                GL_UNSIGNED_BYTE, texData);
            }
            glDisable(GL_TEXTURE_2D);
//Working
	glEnable(GL_POINT_SMOOTH);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glClearColor(0.0, 0.0, 0.0, 1.0);	// set background color to white



}

void GLCover::resizeGL(int w, int h) {

    glViewport(0, 0, w, h);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(90., (float) w/h, 1.0, 1000);
            glMatrixMode(GL_MODELVIEW);

}

void GLCover::paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    display();


}

void drawRecord(Record *record, bool flip)
{
    // enable texture mapping and bind image to polygon
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, record->texId);

    // draw filled polygon
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(.8, .8, .7);
    if(!flip) {
        glBegin(GL_QUADS);
        glTexCoord2f(1, 1);	glVertex3f(0,0,0);
        glTexCoord2f(0, 1);	glVertex3f(0,0,-record->width);
        glTexCoord2f(0, 0);	glVertex3f(0,record->width,-record->width);
        glTexCoord2f(1, 0);	glVertex3f(0,record->width,0);
        glEnd();
    }else {
        glBegin(GL_QUADS);
        glTexCoord2f(0, 1);	glVertex3f(0,0,0);
        glTexCoord2f(1, 1);	glVertex3f(0,0,-record->width);
        glTexCoord2f(1, 0);	glVertex3f(0,record->width,-record->width);
        glTexCoord2f(0, 0);	glVertex3f(0,record->width,0);

        glEnd();
    }
    glDisable(GL_TEXTURE_2D);

    // draw polygon outline
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_LINE_SMOOTH);
    glColor3f(.8, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(0,0,0);
    glVertex3f(0,0,-record->width);
    glVertex3f(0,record->width,-record->width);
    glVertex3f(0,record->width,0);
    glEnd();
    glDisable(GL_LINE_SMOOTH);
}

void specialKeyPress(int key, int x, int y)
{

//    switch (event->key()) {
//            case Key::Key_A: /* do something useful */; break;
//            case Key::Key_S: /* do something useful */; break;
//            case Key::Key_W: /* do something useful */; break;
//            case Key::Key_D: /* do something useful */; break;

    switch(key) {
        //case GLUT_KEY_LEFT:
   case Qt::Key_Up:
            if(_shift<_recordCount/2)
                scrollRecords(-1);
            break;
       // case GLUT_KEY_RIGHT:
    case Qt::Key_Down:
            if(_shift>-_recordCount/2)
                scrollRecords(1);
            break;
    }
}

void scrollRecords(int dir)
{
    // do nothing if we are already scrolling
    if(_bScrolling) return;

    // init scrolling params
    _bScrolling = 1;		// flag scrolling condition
    _scrollDir = dir;		// save scrolling direction

    // register a timer callback (scrollTimer) to be triggered in a
    // specified number of milliseconds
}

void keyPress(unsigned char key, int x, int y)
{
    switch(key) {
        case 'q':
            //quit(); //LOOK FOR QT EQUIVALENT
            break;
    }
}
void scrollTimer(int value)
{
    static unsigned int counter=0;

    // update velocity and position
    counter++;
    _scrollOffset = (float)_scrollDir * counter * _scrollUpdateInterval
    / _scrollTime;

    // last iteration
    if(fabs(_scrollOffset) >= 1 ||
       counter >= _scrollTime/(_scrollUpdateInterval)) {
        _shift	        -=_scrollDir;
        counter	= 0;
        _scrollDir	= 0;
        _scrollOffset	= 0;
        _bScrolling	= 0;
        display();
    } else {
        display();
    }
}



void drawRecords()
{
    int	 i, middle, incoming, outgoing;
    float	 front, center_dist, w2;
    vector3f pos;

    // center of the bounding box
    pos[0] = 0;
    pos[1] = -_records[0].height/2.0;
    pos[2] = -6;

    // center scene
    glTranslatef(pos[0], pos[1], pos[2]);
    front	    = 2.0;
    center_dist = 5.0;
    middle	    = _recordCount  / 2 + _shift;
    if(_scrollOffset > 0) {		// scroll right
        incoming = middle-1;
        outgoing = middle;
    } else {			// scroll left
        incoming = middle+1;
        outgoing = middle;
    }

    // leftmost record position
    pos[0] = -middle - center_dist + _scrollOffset - 1;
    pos[1] =  0;
    pos[2] =  0;
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);

    // draw all shifting (non-rotating) records
    for(i= 0; i<_recordCount; i++) {
        // move over records that do not purely translate
        if(i == incoming || i == outgoing) {
            glTranslatef(1 + center_dist, 0, 0);
            continue;
        }

        glTranslatef(1, 0, 0);
        bool flip = (i<middle);
        drawRecord(&_records[i], flip);
    }
    glPopMatrix();

    // draw central incoming rotating record
    w2 = _records[middle].width / 2.0;
    if(_scrollOffset <= 0) {
        pos[0] = (1+_scrollOffset) *  -w2 +
        (  _scrollOffset) * (center_dist+1);
        pos[1] =  0;
        pos[2] = (1+_scrollOffset) * front;
    } else {
        pos[0] = (1-_scrollOffset) *   w2 +
        (  _scrollOffset) * (center_dist+1);
        pos[1] =  0;
        pos[2] = (1-_scrollOffset) * front;
    }
    glPushMatrix();
    glTranslatef(pos[0], pos[1], pos[2]);

    // rotate it between 90 and 180 degrees if scrollOffset < 0
    // rotate it between 90 and  0  degrees if scrollOffset > 0
    glRotatef(90.0 *(1-_scrollOffset), 0 , 1, 0);

    // translate so that it rotates around the other edge
    if(_scrollOffset <= 0)
        glTranslatef(0, 0, _records[middle].width);

    drawRecord(&_records[middle]);
    glPopMatrix();

    // draw central outgoing rotating record
    if(_scrollOffset <= 0) {
        pos[0] = -w2*_scrollOffset + (center_dist+1)*(1+_scrollOffset);
        pos[1] =  0;
        pos[2] = _scrollOffset * -front;

        glTranslatef(pos[0], pos[1], pos[2]);
        glRotatef(-90.0*_scrollOffset, 0, 1, 0);
        drawRecord(&_records[incoming]);
    } else {
        pos[0] = -w2*_scrollOffset - (center_dist+1)*(1-_scrollOffset);
        pos[1] =  0;
        pos[2] = _scrollOffset * front;

        glTranslatef(pos[0], pos[1], pos[2]);
        glRotatef(-90.0*_scrollOffset, 0, 1, 0);
        drawRecord(&_records[outgoing-1], true);
    }
}


void display()
{
    // clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // draw all records
    drawRecords();



}

void initRecords()
{
    // init cover art info
    int   albumCount =  11;
    char  imageDir[] = "alb";
    char *imageFilename[] = {
        (char *) "beatles.ppm",
        (char *) "dylan.ppm",
        (char *) "crickets.ppm",
        (char *) "animals.ppm",
        (char *) "kinks.ppm",
        (char *) "perkins.ppm",
        (char *) "stones.ppm",
        (char *) "presley.ppm",
        (char *) "richard.ppm",
        (char *) "supremes.ppm",
        (char *) "bluejean.ppm"};

    // init global vars and create space
    _recordCount = albumCount;
    _records = (Record*) malloc(sizeof(Record)*_recordCount);

    // init data structure with dimensions of cover art polygons
    // and filenames for the polygon textures
    for(int i=0; i<_recordCount; i++) {
        // save dimensions
        _records[i].width  = 5;
        _records[i].height = 5;

        // save image filenames
        sprintf_s(_records[i].imageFilename, "%s/%s", imageDir,
                imageFilename[i % albumCount]);
    }
}

int
readPPM(char *file, int &width, int &height, unsigned char* &image)
{
    // open binary file for reading
    ifstream inFile(file, ios::binary);
    //assert(inFile);

    // verify that the image is in PPM format
    // error checking: first two bytes must be code for a raw PPM file
    char buf[80];
    inFile.getline(buf, 3);
    if(strncmp(buf, "P6", 2)) {
        cerr << "The file " << file << " is not in PPM format)\n";
        inFile.close();
        return 0;
    }

    // read width, height, and maximum gray val
    int maxGray;
    inFile >> width >> height >> maxGray;

    // skip over linefeed and carriage return
    inFile.getline(buf, 2);

    // allocate image
    image = new unsigned char[width*height*3];
    assert(image);

    // read entire image data
    inFile.read((char*) image, width*height*3);
    inFile.close();
    return 1;
}


#include "Visualizer.h"



Visualizer::Visualizer(QGLWidget *parent):
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {

}

void Visualizer::initializeGL()
{


        //Working
        glEnable(GL_POINT_SMOOTH);

        // enable depth test
        glEnable(GL_DEPTH_TEST);

        // enable blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glClearColor(0.0, 0.0, 0.0, 1.0);	// set background color to white
        //Workin

}

void Visualizer::resizeGL(int w, int h)
{

        /* initialize viewing values; setup unit view cube */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        int size = rand() % 5;
        int size2 = rand() % 5;
        glViewport(0, 0, w, h);


}


void Visualizer::paintGL()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glBegin(GL_TRIANGLES);
        GLfloat size = rand() % 2;
        GLfloat size2 = rand() % 2;
        GLfloat size3 = rand() % 2;
        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(3, size3, 3);
        glVertex3f(0, -0.5, size);
        glVertex3f(0, 0, 3);

        glEnd();
}


#ifndef GLCover_H
#define GLCover_H

#include <QGLWidget>
////#undef GL_DOUBLE
////#include <GL/GL.h>
#include <GL/GLU.h>


class GLCover : public QGLWidget {

    Q_OBJECT

public:
	GLCover(QGLWidget *parent = NULL );
 
protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:

};

#endif // GLCover_H

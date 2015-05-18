#ifndef Visualizer_H
#define Visualizer_H

#include <QtGui>
#include <QGLWidget>


#include <GL/GLU.h>


class Visualizer : public QGLWidget {

    Q_OBJECT

public:
    Visualizer(QGLWidget *QTVisualizer = NULL );
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

protected:


private:


};

#endif // GL_H

#-------------------------------------------------
#
# Project created by QtCreator 2013-03-13T11:23:12
#
#-------------------------------------------------


QT       += opengl multimedia


FORMS    += mainwindow.ui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tes4
TEMPLATE = app


INCLUDEPATH += C:\Library\tag\include\taglib
INCLUDEPATH += C:\Library\tag\include\taglib\toolkit
INCLUDEPATH += C:\Library\audiere\include

#LIBS += -LC:\Library\tag\lib
#LIBS += -ltag
LIBS += -LC:\Library\audiere\lib
LIBS	+= -laudiere

SOURCES +=	main.cpp\
		mainwindow.cpp \
		Visualizer.cpp \
		GLCover.cpp


HEADERS  +=	mainwindow.h \
		Visualizer.h \
		GLCover.h











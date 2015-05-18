#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia/QMediaPlayer>
#include <QMediaMetaData>
#include <QtWidgets>
#include "GLCover.h"
#include <audiere.h>


namespace Ui {
class MainWindow;
class QCamboBox;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *Coverflow = 0);

    ~MainWindow();
    void createMenus                ();
    int volume                      () const;
    qreal playbackRate              () const;
    QSplitter *middlearea;
    QTabWidget *mainlibrary;
    QGroupBox *visual;
    QGroupBox *cdplayer;
    QGroupBox *controls;
    int c;
    QString fileName;
    QString Path;
    QListWidget *Genre ;
    QListWidget *Artist ;
    QListWidget *Album ;
    QGridLayout *m_topLayout;

public slots:
    void OpenMusicFile          ();
    void play                   ();
    void playcd                 ();
    void stop                   ();
    void pause                  ();
    void stopcd                 ();
    void pausecd                ();
    void next                   ();
    void prev                   ();
    void nextcd                 ();
    void prevcd                 ();
    void setVolume              (int volume);
    void showVisualizer         ();
    void themechoice            ();
    void themechoicea           ();
    void themechoiceb           ();
    void themechoicec           ();
    void themechoiced           ();
    void genreUpdate            ();
    void artistUpdate           ();
    void albumUpdate            ();
    void setcurrenttrack        (int i);
    void setPlaybackRate        (float rate);




signals:
    void changeVolume           (int volume);
    void changeRate             (qreal rate);


private slots:
    void positionChanged        (qint64 position);
    void durationChanged        (qint64 duration);
    void setPosition            (int position);
    void openDir                ();
    void updateRate             ();

private:
    enum{TITLE,TRACK,TIME,ARTIST,ALBUM,GENRE,PATH};
    enum{LIST_COLS=3,COLS=PATH};
    Ui::MainWindow *ui;
    QComboBox *rateBox;
    QMenu *fileMenu;
    QAction *openAction;
    QAction *playAction;
    QAction *stopAction;
    QAction *pauseAction;
    QAction *importAction;
    QAction *m_yellowTheme;
    QAction *blueTheme;
    QAction *m_theme1;
    QAction *m_theme2;
    QAction *visualizer;
    QAction *cdAction;
    QTableWidget *m_table;
    QMediaPlayer mediaPlayer;
    QSlider *positionSlider;
    QAbstractSlider *volumeSlider;
    void populateTable                  ();
    QList<QStringList> m_songs;
    QListWidget *m_list[LIST_COLS];
    QLabel *m_label[LIST_COLS];
    void populateLists();
    void traverseDirs(QString path);
    audiere::CDDevicePtr m_audiocd;
    int m_tracknumber ;
    bool m_playing;
    void updateDurationInfo(qint64 currentInfo);
    qint64 duration;
    QLabel *labelDuration;



};

#endif // MAINWINDOW_H

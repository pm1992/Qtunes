#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QMediaMetaData>
#include <QMediaContent>
//#include <audiere.h>
//#include <tag.h>
//#include <fileref.h>
//#include <mpeg/id3v2/id3v2tag.h>
#include <algorithm>
#include "GLCover.h"
#include "Visualizer.h"
#include <QtMultimedia>


extern void initRecords ();


using namespace audiere;

MainWindow::MainWindow(QWidget *parent) :


    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    //Frame -> Box -> Buttons
    //Add spacer or q gird
    ui->setupUi(this);
   //The mainlibrary tabwidget is the main widget in the program
   QTabWidget *mainlibrary = new QTabWidget(parent);

   //This spliter widget is used for the CD tab
   QSplitter *cdplayer = new QSplitter;

   QTableWidget *m_cdtable = new QTableWidget(0, 2);

    mainlibrary->setTabEnabled(1,false);

   //QHBoxLayout *m_cdtablev = new QHBoxLayout;
   m_cdtable->setHorizontalHeaderLabels(QStringList() <<"Track" << "Duration");


   initRecords();
   //glutSwapBuffers();
  // readPPM();
   //drawRecord();
   //This QGroupBox is used for the Visualizer tab
   QGroupBox *visual = new QGroupBox("QTunes Visualizer");

   //This defines the GLWidget for the program


  GLCover *visual2 = new GLCover;

  Visualizer *converl = new Visualizer;

//coverflow = new QGLWidget;
   QHBoxLayout *vis = new QHBoxLayout;
   visual->setLayout(vis);
//visual2->
   vis->addWidget(converl);
//Coverflowe->paintGL();


//The middlearea splitter is the widget that creates the library

   middlearea = new QSplitter(); //Look here to set it vertical

   //This block adds the tabs to the mainwindow
   mainlibrary->addTab(middlearea, "Music Library");
   mainlibrary->addTab(cdplayer, "CD Player");
   mainlibrary->addTab(visual, "QTunes Vizualizer");

//Qtunes Control Box (This block has the code for the playback buttons

   controls = new QGroupBox("Qtunes Controls");
   QHBoxLayout *control = new QHBoxLayout;
//Play Button
   QPushButton *playButton = new QPushButton ("Play");
   playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
   connect(playButton, SIGNAL(clicked()),this, SLOT(play()));
//Stop Button
   QPushButton *stop = new QPushButton ("Stop");
   stop->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
   connect(stop, SIGNAL(clicked()),this, SLOT(stop()));
//Pause Button
   QPushButton *pause = new QPushButton ("Pause");
   pause->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
   connect(pause, SIGNAL(clicked()),this, SLOT(pause()));
//Next Button
   QPushButton *next = new QPushButton ("Next");
   next->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
   connect(next, SIGNAL(clicked()),this, SLOT(next()));

//Previous Button
   QPushButton *prev = new QPushButton ("Previous");
   prev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
   connect(prev, SIGNAL(clicked()),this, SLOT(prev()));


//Volume Slider
   volumeSlider = new QSlider(Qt::Horizontal, this);
   volumeSlider->setRange(0, 100);

//Reserved for volume
   connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SIGNAL(changeVolume(int)));



//This block defines the ratebox
    rateBox = new QComboBox(this);
    rateBox->addItem("0.5x", QVariant(0.5));
    rateBox->addItem("1.0x", QVariant(1.0));
    rateBox->addItem("2.0x", QVariant(2.0));
    rateBox->setCurrentIndex(1);

    connect(rateBox, SIGNAL(activated(int)), SLOT(updateRate()));

//This block controls the time from the song
    labelDuration = new QLabel(this);
   //connect(positionSlider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

//This block adds the buttons to the control widgets
    control->addWidget(playButton);
    control->addWidget(pause);
    control->addWidget(stop);
    control->addWidget(prev);
    control->addWidget(next);
    control->addWidget(volumeSlider);
    control->addWidget(rateBox);
    control->addWidget(labelDuration);
    controls->setStyleSheet(QString::fromUtf8("background-color: rgb(192, 192, 192);"));
    controls->setLayout(control);
    middlearea->addWidget(controls);


//This adds the CD Player Widget
   cdplayer->addWidget(m_cdtable);

//This block defines the playback controls for the CD Player
    QGroupBox *controlscd = new QGroupBox("Qtunes Controls");
    QHBoxLayout *controlcd = new QHBoxLayout;
//Play CD
    QPushButton *playButtoncd = new QPushButton ("Play");
    playButtoncd->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    connect(playButtoncd, SIGNAL(clicked()),this, SLOT(playcd()));
//Stop CD
    QPushButton *stopcd = new QPushButton ("Stop");
    stopcd->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    connect(stopcd, SIGNAL(clicked()),this, SLOT(stopcd()));
//Pause CD
    QPushButton *pausecd = new QPushButton ("Pause");
    pausecd->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    connect(pausecd, SIGNAL(clicked()),this, SLOT(pausecd()));
//Play the next track on the CD
    QPushButton *nextcd = new QPushButton ("Next");
    nextcd->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    connect(nextcd, SIGNAL(clicked()),this, SLOT(nextcd()));

//Play the previous track on the CD
    QPushButton *prevcd = new QPushButton ("Previous");
    prevcd->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(prevcd, SIGNAL(clicked()),this, SLOT(prevcd()));


    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);

    connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SIGNAL(changeVolume(int)));
    connect(controlcd, SIGNAL(changeVolume(int)), &mediaPlayer, SLOT(setVolume(int)));

//This block adds the playback buttons to the CD Player widget
    controlcd->addWidget(playButtoncd);
    controlcd->addWidget(pausecd);
    controlcd->addWidget(stopcd);
    controlcd->addWidget(prevcd);
    controlcd->addWidget(nextcd);
    controlcd->addWidget(volumeSlider);
    controlscd->setStyleSheet(QString::fromUtf8("background-color: rgb(192, 192, 192);"));
    controlscd->setLayout(controlcd);
    cdplayer->addWidget(m_cdtable);
    cdplayer->addWidget(controlscd);



//This block activates the CD Player functions from Audiere


    int tracks = 2;

  //  m_audiocd = audiere::OpenCDDevice("F:");

//Checks to see if CD is present

//    if (m_audiocd->containsCD() == true){
//        tracks =  m_audiocd->getTrackCount();
//    } else {
//        tracks = 2;
//    }
//Reserved for CD Table
    for (int i = 0; i<=tracks; i++){

        QTableWidgetItem *newItem = new QTableWidgetItem(tr("Track %1").arg(i));
        m_cdtable->setItem(0,i,newItem);
    }


//This block creates the track list for the CD Player
    QComboBox *tracklist = new QComboBox;

   // connect (tracklist, SIGNAL(currentIndexChanged(int)),this, SLOT(setcurrenttrack(int)));

    cdplayer->addWidget(tracklist);

    for (int i = 0; i<tracks; i++){

     tracklist->addItem(QString("Track %1").arg(i+1));

  }

//This block creates the position slider during playback
    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);
    connect(positionSlider, SIGNAL(sliderMoved(int)),this, SLOT(setPosition(int)));

//Adds the position slder to the mainwindow
    middlearea->addWidget(positionSlider);
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(durationChanged(qint64)));

//This is the Coverflow GL Widget

    middlearea->addWidget(visual2);


//This block creates the widget for the Library Columns and Lists

    QGroupBox *MusicLibrary = new QGroupBox("Music Library");
    QHBoxLayout *MusicLibraryH = new QHBoxLayout;
   // QVBoxLayout *MusicLibraryV = new QVBoxLayout;

//This defines the List Widget for the library
    Genre = new QListWidget;
    Artist = new QListWidget;
    Album = new QListWidget;

//This block makes the labels for the Library columns
    for (size_t i = 0; i < LIST_COLS; ++i){
        m_label[i] = new QLabel;
        m_label[i]->setAlignment(Qt::AlignHCenter);
        m_label[i]->setFrameStyle(QFrame::Panel);
        m_label[i]->setFrameShadow(QFrame::Sunken);
        m_list[i] = new QListWidget;

        }

    m_topLayout = new QGridLayout;

//This part labels the columns
    m_label[0]->setText("Genres");
    m_label[1]->setText("Artists");
    m_label[2]->setText("Albums");

//This adds the labels to the tables
    for (size_t i = 0; i < LIST_COLS; ++i){

        MusicLibraryH->addWidget(m_label[i]);
        middlearea->addWidget(m_label[i]);

        }


        for (size_t i = 0; i < LIST_COLS; ++i){

            m_topLayout->addWidget(m_label[i],0,i);

        }


//This Block makes sure the tables are sorted
    Genre->setSortingEnabled(true);
    Artist->setSortingEnabled(true);
    Album->setSortingEnabled(true);

    m_topLayout->addWidget(Genre);
    m_topLayout->addWidget(Artist);
    m_topLayout->addWidget(Album);
    MusicLibrary->setLayout(m_topLayout);
    middlearea->addWidget(MusicLibrary);


//This configures the main window
    resize(800,600);
    setWindowTitle(tr("Qtunes"));
    createMenus();
    
    
    
    
    
    //This block creates the table widget for the music library
        m_table = new QTableWidget(0, 7);
            QHeaderView *header = new QHeaderView(Qt::Horizontal,m_table);
            header->setSectionResizeMode(QHeaderView::Stretch);
            m_table->setHorizontalHeader(header);
            m_table->setHorizontalHeaderLabels(QStringList() <<
                                               "Song Name" << "Track" << "Time" <<"Artist" << "Album" << "Genre" << "Path");
            m_table->setAlternatingRowColors(1);
                m_table->setShowGrid(0);
                m_table->setEditTriggers (QAbstractItemView::NoEditTriggers);
                m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
         middlearea->addWidget(m_table);
                 //
    
//This block defines what happens when an item is clicked in the list widgets.
    connect(Genre,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(genreUpdate()));
    connect(Artist,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(artistUpdate()));
    connect(Album,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(albumUpdate()));
    connect(m_table,SIGNAL(itemClicked(QTableWidgetItem*)),m_table,SLOT(selectRow(int)));
    connect(m_table,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(play()));
    middlearea->setOrientation(Qt::Vertical);  //Look here to set it vertical



//This line sets the tab widget as the main widget in the program
    setCentralWidget(mainlibrary);


//Function Definintions
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createMenus()
{
    Genre->clear();
    Artist->clear();
    Album->clear();
    fileMenu = menuBar()->addMenu(tr("&Main Menu"));

    openAction = new QAction(tr("&Open Music File"), this);
    fileMenu->addAction(openAction);
    connect(openAction, SIGNAL(triggered()), this, SLOT(OpenMusicFile()));

    importAction = new QAction(tr("Import From Folder"), this);
    fileMenu->addAction(importAction);
    connect(importAction, SIGNAL(triggered()), this, SLOT(openDir()));

    cdAction = new QAction (tr("Play CD"), this);
    fileMenu->addAction(cdAction);
    connect(cdAction, SIGNAL(triggered()), this, SLOT(OpenMusicFile()));

    fileMenu = menuBar()->addMenu(tr("&Controls"));
    playAction = new QAction(tr("&Play"), this);
    fileMenu->addAction(playAction);
    connect(playAction, SIGNAL(triggered()), this, SLOT(play()));

    pauseAction = new QAction(tr("&Pause"), this);
    fileMenu->addAction(pauseAction);
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(pause()));

    stopAction = new QAction(tr("&Stop"), this);
    fileMenu->addAction(stopAction);
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));

    fileMenu = menuBar()->addMenu(tr("&QTunes Visualizer"));

    visualizer = new QAction(tr("&Show Visualizer"), this);
    fileMenu->addAction(visualizer);
    connect(visualizer, SIGNAL(triggered()), this, SLOT(showVisualizer()));


    fileMenu = menuBar()->addMenu(tr("&QTunes Theme"));

    m_yellowTheme = new QAction(tr("&Yellow"), this);
    fileMenu->addAction(m_yellowTheme);
    //qint64 c;
    connect(m_yellowTheme, SIGNAL(triggered()), this, SLOT(themechoice()));
    //c = 1;
    blueTheme = new QAction(tr("&Mixed"), this);
    fileMenu->addAction(blueTheme);

    connect(blueTheme, SIGNAL(triggered()), this, SLOT(themechoiceb()));

    m_theme1 = new QAction(tr("&Pink"), this);
    fileMenu->addAction(m_theme1);

    connect(m_theme1, SIGNAL(triggered()), this, SLOT(themechoicec()));

    m_theme2 = new QAction(tr("&Cyan"), this);
    fileMenu->addAction(m_theme2);

    connect(m_theme2, SIGNAL(triggered()), this, SLOT(themechoiced()));
}




void MainWindow::OpenMusicFile()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open Music File"),QDir::homePath());
    if (!fileName.isEmpty()){

        mediaPlayer.setMedia(QUrl::fromLocalFile(fileName));

    }
}

void MainWindow::play()
{
    QString Song = (m_table->item(m_table->currentRow(),6)->text());

    mediaPlayer.setMedia(QUrl::fromLocalFile(Song));
    mediaPlayer.play();
    m_audiocd->stop();
}

void MainWindow::stop()
{
    mediaPlayer.stop();
}

void MainWindow::pause()
{
    mediaPlayer.pause();

}

void MainWindow::next()
{
    int row = m_table->currentRow();

    QString Song = (m_table->item(row=row+1,6)->text());

    m_table->selectRow(row++);
    mediaPlayer.setMedia(QUrl::fromLocalFile(Song));
    mediaPlayer.play();

}

void MainWindow::prev()
{
    int row = m_table->currentRow();
    QString Song = (m_table->item(row=row-1,6)->text());
    m_table->selectRow(row--);
    mediaPlayer.setMedia(QUrl::fromLocalFile(Song));
    mediaPlayer.play();

}


void MainWindow::stopcd()
{
 //   m_audiocd->stop();

}


void MainWindow::playcd()
{
    //m_audiocd->play(m_tracknumber);

}


void MainWindow::pausecd()
{
  //  m_audiocd->pause();
  //  m_playing = true;

}

void MainWindow::nextcd()
{
   // m_audiocd->play(m_tracknumber++);

}
void MainWindow::prevcd()
{
   //  m_audiocd->play(m_tracknumber=m_tracknumber-1);
}

void MainWindow::setcurrenttrack(int i = 0)
{
    m_tracknumber = i;
}


void MainWindow::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
}

void MainWindow::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
}

void MainWindow::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

void MainWindow::setVolume(int volume)
{
    if (volumeSlider)
        volumeSlider->setValue(volume);
}

int MainWindow::volume() const
{
    return volumeSlider ? volumeSlider->value() : 0;
}

void MainWindow::showVisualizer()
{

    QKeyEvent *event = new QKeyEvent ( QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    QCoreApplication::postEvent (this, event);

}


void MainWindow::themechoice()
{
    controls->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 60);"));
    middlearea->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 60);"));
    m_table->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 60);"));

}

void MainWindow::themechoicea()
{
    controls->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 60);"));
    middlearea->setStyleSheet(QString::fromUtf8("background-color: rgb(107, 255, 60);"));
    m_table->setStyleSheet(QString::fromUtf8("background-color: rgb(107, 255, 60);"));


}

void MainWindow::themechoiceb()
{
    middlearea->setStyleSheet(QString::fromUtf8("background-color: rgb(131, 255, 241);"));
    m_table->setStyleSheet(QString::fromUtf8("background-color: rgb(131, 80, 60);"));

}

void MainWindow::themechoicec()
{
    controls->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 60, 255);"));
    middlearea->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 60, 255);"));
    m_table->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 60, 255);"));

}

void MainWindow::themechoiced()
{
    controls->setStyleSheet(QString::fromUtf8("background-color: rgb(60, 255, 255);"));
    middlearea->setStyleSheet(QString::fromUtf8("background-color: rgb(60, 255, 255);"));
    m_table->setStyleSheet(QString::fromUtf8("background-color: rgb(60, 255, 255);"));

}

void MainWindow::openDir()

{
    QString PATH = QFileDialog::getExistingDirectory
                (this,tr("Add Folder to Library"),"C:/Users/Peter/Desktop/aaaaaaaMusic",
                 QFileDialog::DontResolveSymlinks |
                 QFileDialog::ReadOnly |
                 QFileDialog::ShowDirsOnly);

    traverseDirs(PATH);
    populateLists();
    populateTable();

}

void MainWindow::traverseDirs(QString path)
{
    QString		key, val;
       QStringList	list;

       // init listDirs with subdirectories of path
       QDir dir(path);
       dir.setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
       QFileInfoList listDirs = dir.entryInfoList();

       // init listFiles with all *.mp3 files in path
       QDir files(path);
       files.setFilter(QDir::Files);
       files.setNameFilters(QStringList("*.mp3"));
       QFileInfoList listFiles = files.entryInfoList();

       //m_progressBar->setMaximum(listFiles.size());
       c = listFiles.size();
       for(int i=0; i < listFiles.size(); i++) {
           // adjust progress dialog to current settings
   //        m_progressBar->setLabelText(listFiles.at(i).fileName());
   //        m_progressBar->setValue(i);
           qApp->processEvents();
   //        if(m_progressBar->wasCanceled()) break;

           // init list with default values: ""
           for(int j=0; j<=COLS; j++)
               list.insert(j, "");

           // store file pathname into 0th position in list
           QFileInfo fileInfo = listFiles.at(i);
           list.replace(PATH, fileInfo.filePath());

           // convert it from QString to Ascii and store in source
           SampleSourcePtr source = OpenSampleSource(
                        fileInfo.filePath().toUtf8());

           // error checking
           if(!source || source->getTagCount() == 0) continue;

           // process all song tags
           for(int j=0; j<source->getTagCount(); j++) {
               // get tag key and value
               key =  source->getTagKey  (j);
               val =  source->getTagValue(j);

               int factor1 = source->getLength()/44100;
               if(val.isEmpty()  ) val = "Unknown";

               // store tag value in proper position in list
               if(key == "genre" ) list.replace(GENRE , val);
               if(key == "artist") list.replace(ARTIST, val);
               if(key == "album" ) list.replace(ALBUM , val);
               if(key == "title" ) list.replace(TITLE , val);
               if(key == "track" ) list.replace(TRACK , val);
//               if(tme == "time"  ) list.replace(TIME  , mil2);
               QString tme = QString::number(factor1);
                list.replace(TIME  , tme);

           }

           // append list (song data) into songlist m_listSongs;
           // uninitialized fields are empty strings
           m_songs << list;
       }

       // base case: no more subdirectories
       if(listDirs.size() == 0) return;

       // recursively descend through all subdirectories
       for(int i=0; i<listDirs.size(); ++i) {
           QFileInfo fileInfo = listDirs.at(i);
           traverseDirs( fileInfo.filePath() );
       }

       return;
}



void MainWindow::populateTable()
{
    QTableWidgetItem *tempItem;
    m_table->setRowCount(m_songs.size());
    m_table->setColumnCount(7);
    for(int i = 0; i < m_songs.size(); ++i){
        for(int j = 0; j < COLS+1; ++j){
            tempItem = new QTableWidgetItem(m_songs.at(i).at(j),0);
            m_table->setItem(i,j,tempItem);
        }
    }

   m_table->setColumnHidden(PATH,true);
   m_table->sortByColumn(TITLE ,Qt::AscendingOrder);
}

void MainWindow::populateLists()
{
    QStringList tempstrlist[LIST_COLS];

    //Get information from song list about , ARTIST, ALBUM
    for(int i = 0; i < m_songs.size(); ++i){
        tempstrlist[0] << (m_songs.at(i)).at(GENRE );
        tempstrlist[1] << (m_songs.at(i)).at(ARTIST);
        tempstrlist[2] << (m_songs.at(i)).at(ALBUM );
    }

    //Sort and remove deuplicates for input
    for(int i = 0; i < LIST_COLS; ++i){
        if(!tempstrlist[i].isEmpty()){
            tempstrlist[i].removeDuplicates();
            tempstrlist[i].sort(Qt::CaseSensitive);
        }
    }


   //Input the modified list items into the lists
   Genre->addItems(tempstrlist[0]);
   Genre->addItem("All");
   Genre->item(0)->setTextAlignment(Qt::AlignHCenter);
   Genre->setCurrentRow(0);
   Artist->addItem("All");
   Artist->addItems(tempstrlist[1]);
   Artist->item(0)->setTextAlignment(Qt::AlignHCenter);
   Artist->setCurrentRow(0);
   Album->addItem("All");
   Album->addItems(tempstrlist[2]);
   Album->item(0)->setTextAlignment(Qt::AlignHCenter);
   Album->setCurrentRow(0);


}
void MainWindow::genreUpdate()
{
    QString genreNow(Genre->currentItem()->text());

    Artist->setCurrentRow(0);
    Album->setCurrentRow(0);

    if(Genre->currentRow()==0)
        for(int i = 0; i < m_table->rowCount(); ++i)
        {
            m_table->setRowHidden(i,true);
        }
    else
        for(int i = 0; i < m_table->rowCount(); ++i)
        {
            if(m_table->item(i,GENRE)->text()==genreNow)
                m_table->setRowHidden(i,true);
            else
                m_table->setRowHidden(i,true);
        }
}

void MainWindow::artistUpdate()
{
    QString artistNow(Artist->currentItem()->text()),genreNow(Genre->currentItem()->text());

    Album->setCurrentRow(0);
     if(Genre->currentRow()==0 && Artist->currentRow()==0)
         for(int i = 0; i < m_table->rowCount(); ++i)
         {
             m_table->setRowHidden(i,false);
         }
    else if(Genre->currentRow()==0)
        for(int i = 0; i < m_table->rowCount(); ++i)
        {
            if(m_table->item(i,ARTIST)->text()==artistNow)
                m_table->setRowHidden(i,false);
            else
                m_table->setRowHidden(i,true);
        }
     else
         for(int i=0; i<m_table->rowCount(); ++i)
         {
             if(m_table->item(i,GENRE )->text()==genreNow && m_table->item(i,ARTIST)->text()==artistNow)
                 m_table->setRowHidden(i,false);
             else
                 m_table->setRowHidden(i,true);
        }
}

void MainWindow::albumUpdate()
{
    QString albumNow(Album->currentItem()->text()),
            genreNow(Genre->currentItem()->text()),
            artistNow(Artist->currentItem()->text());

    if(Genre->currentRow()==0 && Artist->currentRow()==0 && Album->currentRow()==0)
        for(int i=0; i<m_table->rowCount(); ++i)
        {
            m_table->setRowHidden(i,false);
        }

    else if(Genre->currentRow()==0 && Artist->currentRow()==0)
        for(int i=0; i<m_table->rowCount(); ++i)
        {
            if(m_table->item(i,ALBUM )->text()==albumNow)
                m_table->setRowHidden(i,false);
            else
                m_table->setRowHidden(i,true);
        }

    else if(Genre->currentRow()==0)
        for(int i=0; i<m_table->rowCount(); ++i)
        {
            if(m_table->item(i,ARTIST)->text()==artistNow && m_table->item(i,ALBUM )->text()==albumNow)
                m_table->setRowHidden(i,false);
            else
                m_table->setRowHidden(i,true);
        }

    else
        for(int i=0; i<m_table->rowCount(); ++i)
        {
            if(m_table->item(i,GENRE )->text()==genreNow && m_table->item(i,ARTIST)->text()==artistNow && m_table->item(i,ALBUM )->text()==albumNow)
                m_table->setRowHidden(i,false);
            else
                m_table->setRowHidden(i,true);
        }
}



qreal MainWindow::playbackRate() const
{
    return rateBox->itemData(rateBox->currentIndex()).toDouble();
}

void MainWindow::setPlaybackRate(float rate)
{
    for (int i = 0; i < rateBox->count(); ++i) {
        if (qFuzzyCompare(rate, float(rateBox->itemData(i).toDouble()))) {
            rateBox->setCurrentIndex(i);
            return;
        }
    }

    rateBox->addItem(QString("%1x").arg(rate), QVariant(rate));
    rateBox->setCurrentIndex(rateBox->count() - 1);
}

void MainWindow::updateRate()
{
    emit changeRate(playbackRate());
}

void MainWindow::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    labelDuration->setText(tStr);
}


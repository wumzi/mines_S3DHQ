#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtWidgets"
#include "QFormLayout"
#include "camera.h"
#include "serialconnection.h"

//#define DEBUG_NOCAMERA
//#define DEBUG_NOSERIALCO

// TODO add engines
#define VERTICAL_ENGINE_ID  1
#define LASER_ID            4

enum State {WAIT_USER, INIT, INIT_OK, POSITION, POSITION_OK, TAKEIMAGE, TAKEIMAGE_OK, DOWNLOAD, DOWNLOAD_OK};
enum Laser {LASER_ON, LASER_OFF};

class SerialConnection;
class Camera;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void angleToIndex(int vertAngle, int horizAngle, int* vertIndex, int* horizIndex);
    void indexToAngle(int vertIndex, int horizIndex, int* vertAngle, int* horizAngle);
    void setState(State st);
    State getState();
    bool nextPosition();
    void setWindow(bool enabled);
    void setTaken(bool allocate);

    // TODO erase
    QFile* file;
    QTextStream* out;

public slots:
    void init();
    void initOK();
    void position();
    void positionOK();
    void takeImage();
    void takeImageOK();
    void download();
    void downloadOK();
    void laserOn();
    void laserOff();
    void pause();
    void abort();
    void takePreview();
    void takePreviewOK();
    void calibrage();
    void sendPosition();
    void end();

    void destinationSelect();
    void refreshComboBox();

private:
    // Links to hardware
    #ifndef DEBUG_NOCAMERA
        Camera *camera;
    #endif
    #ifndef DEBUG_NOSERIALCO
        SerialConnection *control;
    #endif

    // Internal variables
    State state;
    Laser laser_state;
    bool pause_state;
    int horizIndex;
    int vertIndex;
    int vertMaxIndex;
    int horizMaxIndex;
    int imageTakenNb;
    int imageTotalNb;
    bool** photoTaken;
    bool** laserTaken;
    QString linkNewImage;

    // Graphic and configuration variables
    Ui::MainWindow *ui;
    QVBoxLayout *layout ;
    QLabel *titre ;
    QHBoxLayout *partieCentrale;
    QFormLayout *parametres;
    QComboBox *formCamera ;
    QComboBox *formControleur;
    QString *destination;
    QPushButton *destinationButton;
    QSpinBox *extremHorizontalAngle;
    QSpinBox *extremVerticalAngle;
    QSpinBox *stepHorizontalAngle;
    QSpinBox *stepVerticalAngle;
    QCheckBox *formPhoto;
    QCheckBox *formLaser;
    QVBoxLayout *photo;
    QLabel *previewZone;
    QProgressBar *progressBar;
    QLabel *remainingTime;
    QPushButton *refreshButton;
    QPushButton *previewButton;
    QHBoxLayout *partiebasse;
    QPushButton *beginButton;
    QPushButton *pauseButton;
    QPushButton *abortButton;
    QWidget *zonecentrale;
    QTime *time;
};

#endif // MAINWINDOW_H

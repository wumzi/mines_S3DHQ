#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "serialconnection.h"
#include "camera.h"
#include <QtWidgets>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Scanner 3D HQ");
        QPixmap window_pix1("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/logo.png");
        QPixmap window_pix2 = window_pix1.scaled(QSize(64,64),Qt::IgnoreAspectRatio);
    setWindowIcon(window_pix2);

    // Non graphic and internal variables
    setState(WAIT_USER);
    time = new QTime();
    laser_state = LASER_OFF;
    pause_state = false;
    imageTakenNb = 0;
    imageTotalNb = 0;
    vertIndex = 0;
    horizIndex = -1;
    vertMaxIndex = 0;
    horizMaxIndex = 0;

    // Global widgets
    zonecentrale = new QWidget(this);
    layout = new QVBoxLayout(zonecentrale);

    // Title
    titre = new QLabel(this);
        QPixmap title_pix1("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/title.png");
        QPixmap title_pix2 = title_pix1.scaled(QSize(453,109),Qt::IgnoreAspectRatio);
    titre->setPixmap(title_pix2);
    titre->setAlignment(Qt::AlignHCenter);

    // Left-hand part
    partieCentrale = new QHBoxLayout();

        parametres = new QFormLayout();

        formCamera = new QComboBox(this);
        formControleur = new QComboBox(this);
        destinationButton = new QPushButton("Choisir un dossier", this);
        destination = new QString();
        extremHorizontalAngle = new QSpinBox(this);
            extremHorizontalAngle->setRange(0, 180);
            extremHorizontalAngle->setValue(20);
        extremVerticalAngle = new QSpinBox(this);
            extremVerticalAngle->setRange(0, 90);
            extremVerticalAngle->setValue(10);
        stepHorizontalAngle = new QSpinBox(this);
            stepHorizontalAngle->setRange(3, 180);
            stepHorizontalAngle->setValue(10);
        stepVerticalAngle = new QSpinBox(this);
            stepVerticalAngle->setRange(3, 90);
            stepVerticalAngle->setValue(10);
        formPhoto = new QCheckBox(this);
            formPhoto->setChecked(true);
        formLaser = new QCheckBox(this);
        refreshButton = new QPushButton("", this);
            refreshButton->setIcon(QIcon("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/reload.png"));

        #ifndef DEBUG_NOCAMERA
            camera = new Camera(this);
            camera->getCameraList(formCamera);
        #endif
        #ifndef DEBUG_NOSERIALCO
            control = new SerialConnection(this);
            control->getSerialList(formControleur);
        #endif

        parametres->addRow("Caméra", formCamera);
        parametres->addRow("Contrôleur", formControleur);
        parametres->addRow("Dossier d'enregistrement", destinationButton);
        parametres->addRow("Angle Horizontal extrême", extremHorizontalAngle);
        parametres->addRow("Angle Vertical extrême", extremVerticalAngle);
        parametres->addRow("Pas Horizontal", stepHorizontalAngle);
        parametres->addRow("Pas Vertical", stepVerticalAngle);
        parametres->addRow("Photo colorée", formPhoto);
        parametres->addRow("Profil laser", formLaser);
        parametres->addRow("Rafraîchir les listes", refreshButton);

    // Right-hand part
    photo = new QVBoxLayout();

        previewZone = new QLabel();
        // TODO Change photo
        QPixmap preview_pix1("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/pierre.jpg");
        QPixmap preview_pix2 = preview_pix1.scaled(QSize(300,200),Qt::IgnoreAspectRatio);
        previewZone->setPixmap(preview_pix2);
        progressBar = new QProgressBar();
        remainingTime = new QLabel();
        previewButton = new QPushButton("Aperçu");
            previewButton->setIcon(QIcon("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/camera.png"));

        photo->addWidget(previewZone);
        photo->addWidget(progressBar);
        photo->addWidget(remainingTime);
        photo->addWidget(previewButton);

    partieCentrale->addLayout(parametres);
    partieCentrale->addLayout(photo);

    // Bottom part
    partiebasse = new QHBoxLayout();
        beginButton = new QPushButton("Démarrer", this);
            beginButton->setIcon(QIcon("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/player_play.png"));
        pauseButton = new QPushButton("Pause", this);
            pauseButton->setIcon(QIcon("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/player_pause.png"));
        abortButton = new QPushButton("Quitter", this);
            abortButton->setIcon(QIcon("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/player_stop.png"));
        partiebasse->addWidget(beginButton);
        partiebasse->addWidget(pauseButton);
        partiebasse->addWidget(abortButton);

    // Global
    layout->addWidget(titre);
    layout->addLayout(partieCentrale);
    layout->addLayout(partiebasse);

    zonecentrale->setLayout(layout);
    setCentralWidget(zonecentrale);

    // Connect buttons
    QObject::connect(destinationButton, SIGNAL(clicked()), this, SLOT(destinationSelect()));
    QObject::connect(refreshButton, SIGNAL(clicked()), this, SLOT(refreshComboBox()));
    QObject::connect(previewButton, SIGNAL(clicked()), this, SLOT(takePreview()));

    QObject::connect(beginButton, SIGNAL(clicked()), this, SLOT(init()));
    QObject::connect(pauseButton, SIGNAL(clicked()), this, SLOT(pause()));
    QObject::connect(abortButton, SIGNAL(clicked()), this, SLOT(abort()));

    // TODO erase
    file = new QFile("C:/Users/Romain/Desktop/tmp.txt");
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
            return;
    out = new QTextStream(file);
}

void MainWindow::destinationSelect()
{
    *destination = QFileDialog::getExistingDirectory(this);
    destinationButton->setIcon(QIcon("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/agt_action_success.png"));
}

void MainWindow::refreshComboBox()
{
   delete formCamera;
   formCamera = new QComboBox(this);
   camera->getCameraList(formCamera);
   parametres->setWidget(0, QFormLayout::FieldRole, formCamera);

   delete formControleur;
   formControleur = new QComboBox(this);
   control->getSerialList(formControleur);
   parametres->setWidget(1, QFormLayout::FieldRole, formControleur);
}

State MainWindow::getState()
{
    return state;
}

void MainWindow::setState(State st)
{
    state = st;

    switch (state)
    {
        case WAIT_USER:
            ui->statusBar->showMessage("Configurerqss l'acquisition");
            break;
        case INIT:
            ui->statusBar->showMessage("Initialisation");
            // No pause effect
            break;
        case INIT_OK:
            ui->statusBar->showMessage("Initialisation terminée");
            if (!pause_state) { initOK(); }
            break;
        case POSITION:
            ui->statusBar->showMessage("Mise en position");
            if (!pause_state) { position(); }
            break;
        case POSITION_OK:
            ui->statusBar->showMessage("Caméra en position");
            // No pause effect
            break;
        case TAKEIMAGE:
            ui->statusBar->showMessage("Capture de l'image");
            if (!pause_state) { takeImage(); }
            break;
        case TAKEIMAGE_OK:
            ui->statusBar->showMessage("Image capturée");
            // No pause effect
            takeImageOK();
            break;
        case DOWNLOAD:
            ui->statusBar->showMessage("Téléchargement de l'image");
            if (!pause_state) { download(); }
            break;
        case DOWNLOAD_OK:
            ui->statusBar->showMessage("Image téléchargée");
            // No pause effect
            break;
        default:
            ui->statusBar->showMessage("");
    }
}

MainWindow::~MainWindow()
{
    setTaken(false);

    #ifndef DEBUG_NOCAMERA
        delete camera;
    #endif
    #ifndef DEBUG_NOSERIALCO
        delete control;
    #endif

    // TODO erase
    file->close();

    delete ui;
}

void MainWindow::setWindow(bool enabled)
{
    formCamera->setEnabled(enabled);
    formControleur->setEnabled(enabled);
    destinationButton->setEnabled(enabled);
    extremHorizontalAngle->setEnabled(enabled);
    extremVerticalAngle->setEnabled(enabled);
    stepHorizontalAngle->setEnabled(enabled);
    stepVerticalAngle->setEnabled(enabled);
    formPhoto->setEnabled(enabled);
    formLaser->setEnabled(enabled);
    previewButton->setEnabled(enabled);

    beginButton->setEnabled(enabled);

    // Pause is in opposed state
    pauseButton->setEnabled(!enabled);

    // Abort label is changed
    if (enabled) abortButton->setText("Quitter");
    else         abortButton->setText("Avorter");
}

void MainWindow::setTaken(bool allocate)
{
    if (allocate)
    {
        photoTaken = new bool*[vertMaxIndex];
        laserTaken = new bool*[vertMaxIndex];

        for (int i = 0; i < vertMaxIndex; i++)
        {
            photoTaken[i] = new bool[horizMaxIndex];
            laserTaken[i] = new bool[horizMaxIndex];
        }
    }
    else
    {
        for (int i = 0; i < vertMaxIndex; i++)
        {
            delete[] photoTaken[i];
            delete[] laserTaken[i];
        }

        delete[] photoTaken;
        delete[] laserTaken;
    }
}

void MainWindow::init()
{
    // Non graphic and internal variables
    setState(INIT);
    time = new QTime();
    laser_state = LASER_OFF;
    pause_state = false;
    imageTakenNb = 0;
    vertIndex = 0;
    horizIndex = -1;
    vertMaxIndex = 0;
    horizMaxIndex = 0;

    // Verify user entries
    if (!formPhoto->isChecked() && !formLaser->isChecked())
    {
        QMessageBox::warning(this, "Problème de configuration", "Aucune acquisition n'est configurée.\nVeuillez cocher au moins 'Photo colorée' ou 'Profil laser' avant de commencer.");
    }
    else if (destination->compare("") == 0)
    {
       QMessageBox::warning(this, "Problème de configuration", "Veuillez sélectionner un dossier d'enregistrement des images acquises en cliquant sur 'Choisir un dossier'.");
    }
    else
    {
        try
        {
            #ifndef DEBUG_NOCAMERA
                camera->getCamera(formCamera->currentIndex());
            #endif
            #ifndef DEBUG_NOSERIALCO
                QStringList list = (formControleur->currentText()).split(" ");
                control->openSerialPort(list[0]);
                control->sendBegin();
            #endif

            // Create internal variables
            vertMaxIndex = (2 * extremVerticalAngle->value() / stepVerticalAngle->value() ) + 1;
            horizMaxIndex = (2 * extremHorizontalAngle->value() / stepHorizontalAngle->value() ) + 1;

            if (formPhoto->isChecked() && formLaser->isChecked()) { imageTotalNb = 2 * vertMaxIndex * horizMaxIndex; }
            else                                                  { imageTotalNb = vertMaxIndex * horizMaxIndex; }

            progressBar->setRange(0, imageTotalNb);

            // Allocate photoTaken & laserTaken
            setTaken(true);

            // Call initOK() if not paused
            setState(INIT_OK);
        }
        catch (int exception)
        {
            if (exception == 0)
            {
                QMessageBox::warning(this, "Périphérique introuvable", "Caméra injoignable. Vérifiez la connexion.");
            }
            else
            {
                QMessageBox::warning(this, "Périphérique introuvable", "Microcontrôleur injoignable. Vérifiez la connexion et le périphérique sélectionné.");
            }
        }
    }
}

void MainWindow::initOK()
{
    setWindow(false);

    // If only Laser is checked, start it now
    if (formLaser->isChecked() && !formPhoto->isChecked())
    {
        #ifndef DEBUG_NOSERIALCO
            control->sendLaserOn();
        #endif
    }

    time->start();

    // Call position() if not paused
    setState(POSITION);
}

void MainWindow::position()
{
    // Go to next position
    // If it was the last, go to end
    if (nextPosition())
    {
        #ifndef DEBUG_NOSERIALCO
            sendPosition();
        #endif
        #ifdef DEBUG_NOSERIALCO
            positionOK();
        #endif
    }
    else
    {
        QMessageBox::information(this, "Acquisition terminée", "Le scan s'est déroulé avec succès !");
        end();
    }
}

void MainWindow::positionOK()
{
    // No pause effect
    setState(POSITION_OK);

    // Call takeImage() if not paused
    setState(TAKEIMAGE);
}

void MainWindow::takeImage()
{
    #ifndef DEBUG_NOCAMERA
        camera->takeImage();
        setState(TAKEIMAGE_OK);
    #endif
    #ifdef DEBUG_NOCAMERA
        setState(TAKEIMAGE_OK);
    #endif    
}

void MainWindow::takeImageOK()
{
    // Call download if not paused
    setState(DOWNLOAD);
}

void MainWindow::download()
{
    // Destination of the new downloaded image
    linkNewImage = QString(destination->unicode());
    if (laser_state == LASER_OFF) { linkNewImage += "/photo"; }
    else                          { linkNewImage += "/laser"; }
    int vertAngle;
    int horizAngle;
    indexToAngle(vertIndex, horizIndex, &vertAngle, &horizAngle);
    linkNewImage += QString().sprintf("%d,%d", vertAngle, horizAngle);


    #ifndef DEBUG_NOCAMERA
        camera->downloadImage(linkNewImage.toStdString(), true, false);
    #endif
    #ifdef DEBUG_NOCAMERA
        //qDebug() << "Copied in: " << linkNewImage;
        downloadOK();
    #endif
}

void MainWindow::downloadOK()
{
    // No pause effect
    setState(DOWNLOAD_OK);

    // Update internal variables
    imageTakenNb++;
    delay();
    if (laser_state == LASER_ON)
    {
        laserTaken[vertIndex][horizIndex] = true;
        qDebug() << "Laser : " << vertIndex << " ; " << horizIndex;
    }
    else
    {
        photoTaken[vertIndex][horizIndex] = true;
        qDebug() << "Photo : " << vertIndex << " ; " << horizIndex;
    }

    // Update GUI
    // Show last image downloaded in previewZone
    QPixmap preview_pix1(linkNewImage);
    QPixmap preview_pix2 = preview_pix1.scaled(QSize(300,200),Qt::IgnoreAspectRatio);
    previewZone->setPixmap(preview_pix2);

    // Update progressBar & elapsedTime
    progressBar->setValue(imageTakenNb);
    QTime remaining(0, 0, 0, 0);
    remaining = remaining.addMSecs(time->elapsed() * (imageTotalNb - imageTakenNb) / imageTakenNb);
    QTime now = QTime::currentTime();
    QTime elapsed(now.hour() - time->hour(), now.minute() - time->minute(), now.second() - time->second());
    remainingTime->setText("Temps écoulé : " + elapsed.toString("hh:mm:ss") + "\n" \
                           + "Temps restant : " + remaining.toString("hh:mm:ss"));

    // Go to next operation
    // Operate in that order : Photo (if checked), Laser (if checked)
    if (formPhoto->isChecked() && formLaser->isChecked())
    {
        // Laser not already done
        if (laser_state == LASER_OFF) { laserOn(); }
        else                          { laserOff(); }
    }
    else
    {
        setState(POSITION);
    }
}

void MainWindow::laserOn()
{
    laser_state = LASER_ON;

    #ifndef DEBUG_NOSERIALCO
        control->sendLaserOn();
    #endif

    // Operate in that order : Photo (if checked), Laser (if checked)
    setState(TAKEIMAGE);
}

void MainWindow::laserOff()
{
    laser_state = LASER_OFF;

    #ifndef DEBUG_NOSERIALCO
        control->sendLaserOff();
    #endif

    // Operate in that order : Photo (if checked), Laser (if checked)
    setState(POSITION);
}

void MainWindow::pause()
{
    qDebug() << "PAUSE";

    if (!pause_state)
    {
        pause_state = true;
        pauseButton->setIcon(QIcon("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/player_play.png"));
    }
    else
    {
        pause_state = false;
        pauseButton->setIcon(QIcon("C:/Users/Romain/Programmes/S3DHQ/S3DHQ/images/player_pause.png"));

        switch (state)
        {
            case WAIT_USER:
                break;
            case INIT:
                break;
            case INIT_OK:
                initOK();
                break;
            case POSITION:
                position();
                break;
            case POSITION_OK:
                break;
            case TAKEIMAGE:
                takeImage();
                break;
            case TAKEIMAGE_OK:
                break;
            case DOWNLOAD:
                download();
                break;
            case DOWNLOAD_OK:
                break;
        }
    }
}

void MainWindow::abort()
{
    if (state == WAIT_USER) { QApplication::quit(); }
    else
    {
        pause_state = true;
        end();
    }
}

void MainWindow::takePreview()
{
    #ifndef DEBUG_NOCAMERA
        EdsError err = camera->getCamera(formCamera->currentIndex());
        if (err == EDS_ERR_OK)
        {
            camera->takeImage();

            linkNewImage = QString(destination->unicode());
            linkNewImage += "/preview";
            camera->downloadImage(linkNewImage.toStdString(), true, true);
        }
        else
        {
            QMessageBox::warning(this, "Périphérique introuvable", "Caméra injoignable. Vérifiez la connexion et rafraîchissez la liste des caméras disponibles.");
        }
    #endif
}

void MainWindow::takePreviewOK()
{
    QPixmap preview_pix1(linkNewImage);
    QPixmap preview_pix2 = preview_pix1.scaled(QSize(300,200),Qt::IgnoreAspectRatio);
    previewZone->setPixmap(preview_pix2);
}

void MainWindow::calibrage()
{
    // TODO
}

void MainWindow::sendPosition()
{
    int vertAngle;
    int horizAngle;
    indexToAngle(vertIndex, horizIndex, &vertAngle, &horizAngle);

    #ifndef DEBUG_NOSERIALCO
        control->sendPosition(vertAngle, horizAngle);
    #endif
}

void MainWindow::end()
{
    #ifndef DEBUG_NOSERIALCO
        control->sendEnd();
    #endif
    setWindow(true);
    setState(WAIT_USER);
}

void MainWindow::angleToIndex(int vertAngle, int horizAngle, int* vertIndex, int* horizIndex)
{
    *vertIndex = (vertAngle + extremVerticalAngle->value()) / (stepVerticalAngle->value());
    *horizIndex = (horizAngle + extremHorizontalAngle->value()) / (stepHorizontalAngle->value());
}

void MainWindow::indexToAngle(int vertIndex, int horizIndex, int* vertAngle, int* horizAngle)
{
    *vertAngle = - extremVerticalAngle->value() + (vertIndex * stepVerticalAngle->value());
    *horizAngle = - extremHorizontalAngle->value() + (horizIndex * stepHorizontalAngle->value());
}

bool MainWindow::nextPosition()
{
    // Horizontal then Vertical movement
    if (horizIndex < horizMaxIndex - 1)
    {
        horizIndex++;
        return true;
    }
    else
    {
        if (vertIndex < vertMaxIndex - 1)
        {
            horizIndex = 0;
            vertIndex++;
            return true;
        }
        else
        {
            return false;
        }
    }
}

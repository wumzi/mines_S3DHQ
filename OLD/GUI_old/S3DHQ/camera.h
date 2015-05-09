#ifndef CAMERA_H
#define CAMERA_H

#include "QtWidgets"
#include "EDSDK.h"
#include "EDSDKErrors.h"
#include "EDSDKTypes.h"
#include <stdio.h>
#include "mainwindow.h"

class MainWindow;

class Camera//: public QWidget
{
    //Q_OBJECT

    public:
        Camera(MainWindow* parent);
        ~Camera();

        static MainWindow* lastParent;

        void getCameraList(QComboBox *list);
        EdsError getCamera(int i);
        EdsError getVolume();
        EdsError getFolder();
        EdsError takeImage();
        EdsError getFolder(const char* name, EdsVolumeRef volume, EdsDirectoryItemRef* dirItem);
        EdsError getLastImage();
        EdsError downloadImage(std::string destination, bool del, bool preview);

    private:
        bool isSDKLoaded;
        bool isCameraLoaded;
        EdsCameraRef camRef;
        EdsVolumeRef volume;
        EdsDirectoryItemRef dirItem;
        EdsDirectoryItemRef lastImageRef;

    signals:
        void downloadDone();
};

void applicationRun();
EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context);
EdsError EDSCALLBACK handlePropertyEvent (EdsPropertyEvent event, EdsPropertyID property, EdsUInt32 parameter, EdsVoid * context);
EdsError EDSCALLBACK handleStateEvent (EdsStateEvent event, EdsUInt32 parameter, EdsVoid * context);

EdsError EDSCALLBACK progressFunction (EdsUInt32 inPercent, EdsVoid* inContext, EdsBool* outCancel);
EdsError EDSCALLBACK progressFunctionDone (EdsUInt32 inPercent, EdsVoid* inContext, EdsBool* outCancel);
EdsError EDSCALLBACK progressFunctionPreviewDone (EdsUInt32 inPercent, EdsVoid* inContext, EdsBool* outCancel);

/*
EdsError BulbStart(EdsCameraRef camera);
EdsError BulbStop(EdsCameraRef camera);

EdsError startLiveview(EdsCameraRef camera);
EdsError downloadEvfData(EdsCameraRef camera);
EdsError endLiveview(EdsCameraRef camera);

EdsError get100CANONFolder(EdsVolumeRef volume, EdsDirectoryItemRef* directoryItem);
EdsError getLastImage(EdsDirectoryItemRef dirItem, EdsDirectoryItemRef* dirItemRef_Image);
*/

#endif // CAMERA_H

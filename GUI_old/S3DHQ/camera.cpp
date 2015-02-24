#include "camera.h"
#include <string>
#include <iostream>
#include <QDebug>

MainWindow* Camera::lastParent = NULL;

Camera::Camera(MainWindow* parent)
    //QWidget()
{
    lastParent = parent;
    isSDKLoaded = false;
    isCameraLoaded = false;
    camRef = NULL;
    volume = NULL;
    dirItem = NULL;
    lastImageRef = NULL;

    EdsError err = EDS_ERR_OK;
    err = EdsInitializeSDK();

    QMessageBox msgBox;
    if(err == EDS_ERR_OK)
    {
        isSDKLoaded = true;
    }
    else
    {
        switch(err) {
            case EDS_ERR_UNIMPLEMENTED:
                msgBox.setText("Unimplemented");
                break;
            case EDS_ERR_INTERNAL_ERROR:
                msgBox.setText("Internal error");
                break;
            case EDS_ERR_MEM_ALLOC_FAILED:
                msgBox.setText("Memory allocation error");
                break;
            case EDS_ERR_MEM_FREE_FAILED:
                msgBox.setText("Memory free error");
                break;
            case EDS_ERR_OPERATION_CANCELLED:
                msgBox.setText("Operation cancelled");
                break;
            case EDS_ERR_INCOMPATIBLE_VERSION:
                msgBox.setText("Version error");
                break;
            case EDS_ERR_NOT_SUPPORTED:
                msgBox.setText("No supported");
                break;
            case EDS_ERR_UNEXPECTED_EXCEPTION:
                msgBox.setText("Unexpected exception");
                break;
            case EDS_ERR_PROTECTION_VIOLATION:
                msgBox.setText("Protection violation");
                break;
            case EDS_ERR_MISSING_SUBCOMPONENT:
                msgBox.setText("Missing subcomponent");
                break;
            case EDS_ERR_SELECTION_UNAVAILABLE:
                msgBox.setText("Selection unavailable");
                break;
             default:
                msgBox.setText("Unknown");
        }
        msgBox.exec();
        throw 0;
    }
}

Camera::~Camera() {
    //lastInstance = NULL;

    // Close session with camera
    if(isCameraLoaded)
    {
        EdsCloseSession(camRef);
    }

    // Release camera
    if(camRef != NULL)
    {
        EdsRelease(camRef);
    }
    // Terminate SDK
    if(isSDKLoaded)
    {
        EdsTerminateSDK();
    }
}

EdsError EDSCALLBACK handleObjectEvent(EdsObjectEvent event, EdsBaseRef object, EdsVoid * context)
{
    // do something
    /*
    switch(event)
    {
        case kEdsObjectEvent_DirItemRequestTransfer:
            downloadImage(object);
            break;
        default:
            break;
    }
    */
    // Object must be released
    if(object)
    {
        EdsRelease(object);
    }
}

EdsError EDSCALLBACK handlePropertyEvent (EdsPropertyEvent event, EdsPropertyID property, EdsUInt32 parameter, EdsVoid * context)
{
    // do something
}

EdsError EDSCALLBACK handleStateEvent (EdsStateEvent event, EdsUInt32 parameter, EdsVoid * context)
{
    // do something
}

void Camera::getCameraList(QComboBox* list) {
    EdsError err = EDS_ERR_OK;
    EdsCameraListRef cameraList = NULL;
    EdsUInt32 count = 0;
    EdsCameraRef camRefTmp;

    // Get camera list
    err = EdsGetCameraList(&cameraList);

    // Get number of cameras
    if(err == EDS_ERR_OK)
    {
        err = EdsGetChildCount(cameraList, &count);
        if(count == 0)
        {
            err = EDS_ERR_DEVICE_NOT_FOUND;
        }
    }

    // Get first camera retrieved
    if(err == EDS_ERR_OK)
    {
        for (EdsUInt32 i = 0; i < count; i++) {
            err = EdsGetChildAtIndex(cameraList, i, &camRefTmp);
            // TODO convert i to string
            list->addItem("Caméra détectée");
        }
    }

    // Release camera list
    if(cameraList != NULL)
    {
        isCameraLoaded = true;
        EdsRelease(cameraList);
        cameraList = NULL;
    }

    if (camRefTmp != NULL) {
        EdsRelease(camRefTmp);
        camRefTmp = NULL;
    }
}

EdsError Camera::getCamera(int i)
{
    EdsError err = EDS_ERR_OK;
    EdsCameraListRef cameraList = NULL;
    EdsUInt32 count = 0;

    // Get camera list
    err = EdsGetCameraList(&cameraList);

    // Get number of cameras
    if(err == EDS_ERR_OK)
    {
        err = EdsGetChildCount(cameraList, &count);
        if(count == 0)
        {
            err = EDS_ERR_DEVICE_NOT_FOUND;
        }
    }

    // Get first camera retrieved
    if(err == EDS_ERR_OK)
    {
        err = EdsGetChildAtIndex(cameraList, i, &camRef);
    }

    // Release camera list
    if(cameraList != NULL)
    {
        EdsRelease(cameraList);
        cameraList = NULL;
    }

    ////////////////////
    // Set event handler
    if(err == EDS_ERR_OK)
    {
        err = EdsSetObjectEventHandler(camRef, kEdsObjectEvent_All, handleObjectEvent, NULL);
    }

    // Set event handler
    if(err == EDS_ERR_OK)
    {
        err = EdsSetPropertyEventHandler(camRef, kEdsPropertyEvent_All, handlePropertyEvent, NULL);
    }

    // Set event handler
    if(err == EDS_ERR_OK)
    {
        err = EdsSetCameraStateEventHandler(camRef, kEdsStateEvent_All, handleStateEvent, NULL);
    }

    // Open session with camera
    if(err == EDS_ERR_OK)
    {
        err = EdsOpenSession(camRef);
    }

    if(err == EDS_ERR_OK)
    {
        isCameraLoaded = true;
    }

    return err;
}

EdsError Camera::getVolume()
{
    EdsError err = EDS_ERR_OK;
    EdsUInt32 count = 0;

    // Get the number of camera volumes
    err = EdsGetChildCount(camRef, &count);
    if(err == EDS_ERR_OK && count == 0)
    {
        err = EDS_ERR_DIR_NOT_FOUND;
    }
    // Get initial volume
    if(err == EDS_ERR_OK)
    {
        err = EdsGetChildAtIndex(camRef, 0, &volume);
    }

    return err;
}

EdsError Camera::getFolder(const char* name, EdsVolumeRef volume, EdsDirectoryItemRef* dirItem)
{
    EdsError err = EDS_ERR_OK;
    //EdsDirectoryItemRef dirItem = NULL;
    EdsDirectoryItemInfo dirItemInfo;
    EdsUInt32 count = 0;

    // Get number of items under the volume
    err = EdsGetChildCount(volume, &count);
    if(err == EDS_ERR_OK && count == 0)
    {
        err = EDS_ERR_DIR_NOT_FOUND;
    }

    // Get DCIM folder
    for(EdsUInt32 i = 0; i < count && err == EDS_ERR_OK; i++)
    {
        // Get the ith item under the specifed volume
        if(err == EDS_ERR_OK)
        {
            err = EdsGetChildAtIndex(volume, i, dirItem);
        }
        // Get retrieved item information
        if(err == EDS_ERR_OK)
        {
            err = EdsGetDirectoryItemInfo(*dirItem, &dirItemInfo);
        }
        // Indicates whether or not the retrieved item is a DCIM folder.
        if(err == EDS_ERR_OK)
        {
            if(stricmp(dirItemInfo.szFileName, name) == 0 && dirItemInfo.isFolder == true)
            {
                //qDebug() << "DCIM FOUND " << dirItemInfo.szFileName;
                break;
            }
        }
    }
    EdsDirectoryItemInfo toto;
    EdsGetDirectoryItemInfo(*dirItem, &toto);
    //qDebug() << "BEFORE RETURN " << toto.szFileName;

    return err;
}

EdsError Camera::takeImage()
{
    return EdsSendCommand(camRef, kEdsCameraCommand_TakePicture , 0);
}

EdsError EDSCALLBACK progressFunction (EdsUInt32 inPercent, EdsVoid* inContext, EdsBool* outCancel) {
    qDebug() << "Completed " << inPercent << "%\n";
    return EDS_ERR_OK;
}

EdsError EDSCALLBACK progressFunctionDone (EdsUInt32 inPercent, EdsVoid* inContext, EdsBool* outCancel) {
    if (Camera::lastParent != NULL)
        Camera::lastParent->downloadOK();
    return EDS_ERR_OK;
}

EdsError EDSCALLBACK progressFunctionPreviewDone (EdsUInt32 inPercent, EdsVoid* inContext, EdsBool* outCancel) {
    if (Camera::lastParent != NULL)
        Camera::lastParent->takePreviewOK();
    return EDS_ERR_OK;
}

EdsError Camera::downloadImage(std::string destination, bool del, bool preview)
{
    EdsError err = EDS_ERR_OK;
    EdsStreamRef stream = NULL;

    getVolume();
    getFolder();
    err = getLastImage();

    if (err == EDS_ERR_FILE_NOT_FOUND)
    {
        qDebug() << "No JPEG found";
        delay();
        lastParent->setState(TAKEIMAGE);
        return err;
    }

    // Get directory item information
    EdsDirectoryItemInfo lastImageInfo;
    err = EdsGetDirectoryItemInfo(lastImageRef, &lastImageInfo);

    // Set destination file
    std::string imageName = lastImageInfo.szFileName;
    std::string extension = imageName.substr(imageName.find_last_of("."), std::string::npos);
    std::string imagePath = destination + extension;

    // Create file stream for transfer destination
    if(err == EDS_ERR_OK)
    {
        err = EdsCreateFileStream(imagePath.c_str(), kEdsFileCreateDisposition_CreateAlways, kEdsAccess_ReadWrite, &stream);
    }

    // Progress Notification
    //err = EdsSetProgressCallback(stream, progressFunction, kEdsProgressOption_Periodically, NULL);
    if (!preview)
    {
        err = EdsSetProgressCallback(stream, progressFunctionDone, kEdsProgressOption_Done, NULL);
    }
    else
    {
        err = EdsSetProgressCallback(stream, progressFunctionPreviewDone, kEdsProgressOption_Done, NULL);
    }

    // Download image
    if(err == EDS_ERR_OK)
    {
        err = EdsDownload(lastImageRef, lastImageInfo.size, stream);
    }

    // Issue notification that download is complete
    if(err == EDS_ERR_OK)
    {
        err = EdsDownloadComplete(lastImageRef);
    }

    // Release stream
    if(stream != NULL)
    {
        EdsRelease(stream);
        stream = NULL;
    }

    if(err == EDS_ERR_OK && del)
    {
        err = EdsDeleteDirectoryItem(lastImageRef);
    }

    lastImageRef = NULL;

    return err;
}

EdsError Camera::getFolder() {
    EdsError err;
    EdsDirectoryItemRef dcimFolder = NULL;
    err = getFolder("DCIM", volume, &dcimFolder);
    EdsDirectoryItemInfo dirItemInfo;
    EdsGetDirectoryItemInfo(dcimFolder, &dirItemInfo);
    //qDebug() << "Level up " << dirItemInfo.szFileName << " end";

    err = getFolder("100CANON", dcimFolder, &dirItem);
    EdsGetDirectoryItemInfo(dirItem, &dirItemInfo);
    //qDebug() << "Level up " << dirItemInfo.szFileName << " end";

    return err;
}

EdsError Camera::getLastImage()
{
    EdsDirectoryItemInfo lastImageInfo;
    EdsError err    = EDS_ERR_OK;

    EdsUInt32 jpgCount = 0;
    err = EdsGetChildCount(dirItem, &jpgCount);

    if(jpgCount > 0)
    {
        int latestJpg = jpgCount-1;

        err = EdsGetChildAtIndex(dirItem, latestJpg, &lastImageRef) ;
        err = EdsGetDirectoryItemInfo(lastImageRef, &lastImageInfo);

        //qDebug() << "Latest image is " << lastImageInfo.szFileName << "\n";
    }
    else
    {
        err = EDS_ERR_FILE_NOT_FOUND;
        //qDebug() << "No JPEG inside " << lastImageInfo.szFileName << "\n";
    }

    return err;
}

void applicationRun()
{
    /*
    Camera *cam = new Camera();

    EdsError err;
    QMessageBox msgBox;

    err = cam->getCamera(0);

    // Get first camera
    if(err != EDS_ERR_OK)
    {
        msgBox.setText("Error getting camera");
        msgBox.exec();
    }


    ////////////////////////////////////////
    // do something
    ////////////////////////////////////////
    cam->takeImage();

    err = cam->getVolume();
    if (err != EDS_ERR_OK)
    {
        msgBox.setText("Error getting volume");
        msgBox.exec();
    }

    // FONCTIONNE CORRECTEMENT //
    err = cam->getFolder();
    if (err != EDS_ERR_OK)
    {
        msgBox.setText("Error getting DCIM");
        msgBox.exec();
    }

    if (err != EDS_ERR_OK)
    {
        msgBox.setText("Error getting last image");
        msgBox.exec();
    }
    err = cam->downloadImage("", false);
    if (err != EDS_ERR_OK)
    {
        msgBox.setText("Error downloading image");
        msgBox.exec();
    }

    switch(err) {
        case EDS_ERR_TAKE_PICTURE_AF_NG:
            msgBox.setText("Focus failed");
            break;
        case EDS_ERR_TAKE_PICTURE_RESERVED:
            msgBox.setText("Reserved");
            break;
        case EDS_ERR_TAKE_PICTURE_MIRROR_UP_NG:
            msgBox.setText("Mirror Up");
            break;
        case EDS_ERR_TAKE_PICTURE_SENSOR_CLEANING_NG:
            msgBox.setText("Mirror Cleaning");
            break;
        case EDS_ERR_TAKE_PICTURE_SILENCE_NG:
            msgBox.setText("Silence");
            break;
        case EDS_ERR_TAKE_PICTURE_NO_CARD_NG:
            msgBox.setText("No card");
            break;
        case EDS_ERR_TAKE_PICTURE_CARD_NG:
            msgBox.setText("Error writing");
            break;
        case EDS_ERR_TAKE_PICTURE_CARD_PROTECT_NG:
            msgBox.setText("Card protected");
            break;

         default:
            msgBox.setText("Unknown");
    }
    if (err != EDS_ERR_OK)
    {
       msgBox.exec();
    }

    delete cam;
    */
}

/*
EdsError get100CANONFolder(EdsVolumeRef volume, EdsDirectoryItemRef* directoryItem)
{
    EdsError err = EDS_ERR_OK;
    EdsDirectoryItemRef dirItem = NULL;
    EdsDirectoryItemInfo dirItemInfo;
    EdsUInt32 count = 0;

    // Get number of items under the volume
    err = EdsGetChildCount(volume, &count);
    if(err == EDS_ERR_OK && count == 0)
    {
        err = EDS_ERR_DIR_NOT_FOUND;
    }

    // Get DCIM folder
    for(EdsUInt32 i = 0; i < count && err == EDS_ERR_OK; i++)
    {
        // Get the ith item under the specifed volume
        if(err == EDS_ERR_OK)
        {
            err = EdsGetChildAtIndex(volume, i, &dirItem);
        }
        // Get retrieved item information
        if(err == EDS_ERR_OK)
        {
            err = EdsGetDirectoryItemInfo(dirItem, &dirItemInfo);
        }
        // Indicates whether or not the retrieved item is a DCIM folder.
        if(err == EDS_ERR_OK)
        {
            if(stricmp(dirItemInfo.szFileName, "DCIM") == 0 && dirItemInfo.isFolder == true)
            {
                qDebug() << "DCIM FOUND " << dirItemInfo.szFileName;
                //directoryItem = &dirItem;
                break;
            }
        }
    }

    err = EdsGetChildCount(dirItem, &count);
    if(err == EDS_ERR_OK && count == 0)
    {
        err = EDS_ERR_DIR_NOT_FOUND;
    }

    // Get DCIM folder
    for(int i = 0; i < count && err == EDS_ERR_OK; i++)
    {
        // Get the ith item under the specifed volume
        if(err == EDS_ERR_OK)
        {
            err = EdsGetChildAtIndex(dirItem, i, directoryItem);
        }
        // Get retrieved item information
        if(err == EDS_ERR_OK)
        {
            err = EdsGetDirectoryItemInfo(*directoryItem, &dirItemInfo);
        }
        // Indicates whether or not the retrieved item is a DCIM folder.
        if(err == EDS_ERR_OK)
        {
            if(stricmp(dirItemInfo.szFileName, "100CANON") == 0 && dirItemInfo.isFolder == true)
            {
                qDebug() << "100CANON FOUND " << dirItemInfo.szFileName;
                //directoryItem = &dirItem;
                break;
            }
        }
        // Release retrieved item
        if(dirItem)
        {
            EdsRelease(dirItem);
            dirItem = NULL;
        }
    }
    EdsDirectoryItemInfo toto;
    EdsGetDirectoryItemInfo(*directoryItem, &toto);
    qDebug() << "BEFORE RETURN " << toto.szFileName;
    return err;
}
*/

/*
EdsError BulbStart(EdsCameraRef camera)
{
    EdsError err;
    bool locked = false;
    err = EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UILock, 0);

    if(err == EDS_ERR_OK)
    {
        locked = true;
    }

    if(err == EDS_ERR_OK)
    {
        err = EdsSendCommand(camera, kEdsCameraCommand_BulbStart, 0);
    }

    if(err != EDS_ERR_OK && locked)
    {
        err = EdsSendStatusCommand (camera, kEdsCameraStatusCommand_UIUnLock, 0);
    }

    return err;
}


EdsError BulbStop(EdsCameraRef camera)
{
    EdsError err;
    err = EdsSendCommand(camera ,kEdsCameraCommand_BulbEnd, 0);
    EdsSendStatusCommand(camera, kEdsCameraStatusCommand_UIUnLock, 0);
    return err;
}

EdsError startLiveview(EdsCameraRef camera)
{
    EdsError err = EDS_ERR_OK;

    // Get the output device for the live view image
    EdsUInt32 device;
    err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);

    // PC live view starts by setting the PC as the output device for the live view image.
    if(err == EDS_ERR_OK)
    {
        device |= kEdsEvfOutputDevice_PC;
        err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);
    }

    // A property change event notification is issued from the camera if property settings are made successfully.
    // Start downloading of the live view image once the property change notification arrives.
}

EdsError downloadEvfData(EdsCameraRef camera)
{
    EdsError err = EDS_ERR_OK;
    EdsStreamRef stream = NULL;
    EdsEvfImageRef evfImage = NULL;

    // Create memory stream.
    err = EdsCreateMemoryStream( 0, &stream);

    // Create EvfImageRef.
    if(err == EDS_ERR_OK)
    {
        err = EdsCreateEvfImageRef(stream, &evfImage);
    }
    // Download live view image data.
    if(err == EDS_ERR_OK)
    {
        err = EdsDownloadEvfImage(camera, evfImage);
    }
    // Get the incidental data of the image.
    if(err == EDS_ERR_OK)
    {
        // Get the zoom ratio
        EdsUInt32 zoom;
        EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0 , sizeof(zoom), &zoom);
        // Get the focus and zoom border position
        EdsPoint point;
        EdsGetPropertyData(evfImage, kEdsPropID_Evf_ZoomPosition, 0 , sizeof(point), &point);
    }

    //
    // Display image
    //

    // Release stream
    if(stream != NULL)
    {
        EdsRelease(stream);
        stream = NULL;
    }
    // Release evfImage
    if(evfImage != NULL)
    {
        EdsRelease(evfImage);
        evfImage = NULL;
    }
}

EdsError endLiveview(EdsCameraRef camera)
{
    EdsError err = EDS_ERR_OK;
    // Get the output device for the live view image
    EdsUInt32 device;

    err = EdsGetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0, sizeof(device), &device);
    // PC live view ends if the PC is disconnected from the live view image output device.

    if(err == EDS_ERR_OK)
    {
        device &= ~kEdsEvfOutputDevice_PC;
        err = EdsSetPropertyData(camera, kEdsPropID_Evf_OutputDevice, 0 , sizeof(device), &device);
    }
}
*/

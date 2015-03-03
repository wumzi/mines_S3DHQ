// commandeCamera.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include "EDSDK.h"

using namespace std;



EdsError getFirstCamera(EdsCameraRef *camera){
	EdsError err = EDS_ERR_OK;
	EdsCameraListRef cameraList = NULL;
	EdsUInt32 count = 0;
	// Get camera list
	err = EdsGetCameraList(&cameraList);
	// Get number of cameras
	if(err == EDS_ERR_OK){
		err = EdsGetChildCount(cameraList, &count);
		if(count == 0){
			err = EDS_ERR_DEVICE_NOT_FOUND;
		}
	}
	// Get first camera retrieved
	if(err == EDS_ERR_OK){
		err = EdsGetChildAtIndex(cameraList , 0 , camera);
	}
	// Release camera list
	if(cameraList != NULL){
		EdsRelease(cameraList);
		cameraList = NULL;
	}

	return err;
}


int _tmain(int argc, _TCHAR* argv[])
{
    printf("Appuyez sur entrée pour continuer..."); 
	cout << "Hello World" << endl;

	EdsError err = EDS_ERR_OK;
	EdsCameraRef camera = NULL;

	//Init SDK
	err = EdsInitializeSDK();

	switch(err) {
            case EDS_ERR_UNIMPLEMENTED:
                printf("Unimplemented !");
                break;
            case EDS_ERR_INTERNAL_ERROR:
                printf("Internal error");
                break;
            case EDS_ERR_MEM_ALLOC_FAILED:
                printf("Memory allocation error");
                break;
            case EDS_ERR_MEM_FREE_FAILED:
                printf("Memory free error");
                break;
            case EDS_ERR_OPERATION_CANCELLED:
                printf("Operation cancelled");
                break;
            case EDS_ERR_INCOMPATIBLE_VERSION:
                printf("Version error");
                break;
            case EDS_ERR_NOT_SUPPORTED:
                printf("No supported");
                break;
            case EDS_ERR_UNEXPECTED_EXCEPTION:
                printf("Unexpected exception");
                break;
            case EDS_ERR_PROTECTION_VIOLATION:
                printf("Protection violation");
                break;
            case EDS_ERR_MISSING_SUBCOMPONENT:
                printf("Missing subcomponent");
                break;
            case EDS_ERR_SELECTION_UNAVAILABLE:
                printf("Selection unavailable");
                break;
             default:
                printf("Unknown");
        }


	printf("\n");
	printf(err == EDS_ERR_OK ? "1" : "0");
	printf("\n");
	//Get first camera
	if (err == EDS_ERR_OK){
		err = getFirstCamera(&camera);
			printf("\n");
			printf(err == EDS_ERR_OK ? "1" : "0");
			printf("\n");
	}

	EdsOpenSession(camera);


    //EdsCapacity newCapacity = {0x7FFFFFFF, 0x1000, 1};
    //err = EdsSetCapacity(camera, newCapacity);
	printf("\n");
	printf(err == EDS_ERR_OK ? "1" : "0");
	printf("\n");

	printf("\n");
	printf(err == EDS_ERR_OK ? "1" : "0");
	printf("\n");

    EdsSendCommand(camera, kEdsCameraCommand_TakePicture, 0);

	printf("\n");
	printf(err == EDS_ERR_OK ? "1" : "0");
	printf("\n");

    EdsCloseSession(camera);
	
    EdsTerminateSDK();

    system("pause");
	return 0;

}

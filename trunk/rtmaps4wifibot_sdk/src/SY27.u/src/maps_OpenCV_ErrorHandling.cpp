#include "maps_OpenCV_ErrorHandling.h"

bool OpenCV_Static::gOpenCVErrReset = false;
MAPSMutex OpenCV_Static::gOpenCVErrMutex;
void OpenCV_ResetErrors() {
	if (OpenCV_Static::gOpenCVErrReset==false) {
		OpenCV_Static::gOpenCVErrMutex.Lock();
		int status = cvGetErrStatus();
		if (status != CV_StsOk) {
			OpenCV_Static::gOpenCVErrReset = true;
			cvSetErrStatus(CV_StsOk);
		}
		OpenCV_Static::gOpenCVErrMutex.Release();
	}
}

void OpenCV_ReleaseErrors()
{
	OpenCV_Static::gOpenCVErrReset = false;
}

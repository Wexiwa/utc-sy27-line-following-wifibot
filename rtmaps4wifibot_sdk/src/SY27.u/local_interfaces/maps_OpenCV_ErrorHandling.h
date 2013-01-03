#ifndef __MAPS_OPENCV_ERR__
#define __MAPS_OPENCV_ERR__

// Author: Intempora S.A.
// Date: 2007

#include <opencv/cv.h>
#define __IPL_H__
#include "maps.hpp"

class OpenCV_Static
{
public:
static MAPSMutex gOpenCVErrMutex;
static bool gOpenCVErrReset;
};

void OpenCV_ResetErrors();
void OpenCV_ReleaseErrors();

#endif

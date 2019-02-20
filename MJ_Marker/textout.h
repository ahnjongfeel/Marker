#pragma once
#include "stdafx.h"
#include <opencv/cv.hpp>
#include <opencv/cxcore.hpp>
#include <opencv/highgui.h>

#include <opencv2/opencv.hpp>


#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>




void setPutText(IplImage * src_image,		/* 원영상 */
				IplImage *dst_image,  /* 저장할 영상 */
				char * text,		/* 텍스트 */
				CvPoint textPos,		/* 텍스트 위치 */
				CvScalar color,			/* 텍스트 색상 */
				CvScalar edge_color,			/* 텍스트테두리 색상 */
				int fontSize = 10);

							
	
HBITMAP setTextToBitmap(char* text, CvScalar color, int fontSize);	// Text To Bitmap	
IplImage * setBitmapToIplImage(HBITMAP hBitmap);		// Bitmap to IplImage


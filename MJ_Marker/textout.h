#pragma once
#include "stdafx.h"
#include <opencv/cv.hpp>
#include <opencv/cxcore.hpp>
#include <opencv/highgui.h>

#include <opencv2/opencv.hpp>


#include <opencv2/core/utility.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio.hpp>




void setPutText(IplImage * src_image,		/* ������ */
				IplImage *dst_image,  /* ������ ���� */
				char * text,		/* �ؽ�Ʈ */
				CvPoint textPos,		/* �ؽ�Ʈ ��ġ */
				CvScalar color,			/* �ؽ�Ʈ ���� */
				CvScalar edge_color,			/* �ؽ�Ʈ�׵θ� ���� */
				int fontSize = 10);

							
	
HBITMAP setTextToBitmap(char* text, CvScalar color, int fontSize);	// Text To Bitmap	
IplImage * setBitmapToIplImage(HBITMAP hBitmap);		// Bitmap to IplImage


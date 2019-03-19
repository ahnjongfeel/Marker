#pragma once
#include <opencv2\opencv.hpp>
#include <vector>
#include <opencv2\core.hpp>
#include <opencv2\videoio\videoio.hpp>

class CTagRect
{
public:
	int id = -1;
	int trackingID = 0;
	float left = 0;
	float right = 0;
	float top = 0;
	float bottom = 0;

	float rateX = 0;
	float rateY = 0;
	float rateW = 0;
	float rateH = 0;

	float fimgW = 0;
	float fimgH = 0;



	float getWidth()
	{
		return (right - left);
	}
	float getHeight()
	{
		return (bottom - top);
	}

	float getCenterX()
	{
		return (left + right) / 2.0;
	}
	float getCenterY()
	{
		return (top + bottom) / 2.0;
	}
};

class CFileInfo
{
public:
	float rateX = 0;
	float rateY = 0;
	float rateW = 0;
	float rateH = 0;

	float fimgW = 0;
	float fimgH = 0;


	CString strPathImg;
	CString strPathTxt;
	cv::Mat cvMat;	
	CTagRect rect;
	std::vector<CTagRect> vRect;
	std::vector<CTagRect> vRectAdd;
	int iTagNameIndex=-1;
	int iTrackingID = 0;
	bool bVideoGrabMode = 0;
	bool bFullSizeClassify = 0;
	
	int iFrameNumber = 0;
	bool bLoadAnnotation = 0;

};

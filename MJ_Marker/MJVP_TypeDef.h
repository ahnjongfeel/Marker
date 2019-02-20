#ifndef __MJVT_TYPE_DEF_H__
#define __MJVT_TYPE_DEF_H__
#pragma once
#include <string>
#include <vector>
#define MVAP_MAX_LEN	260
struct ObjRect{
	int x;
	int y;
	int width;
	int height;
};
struct ObjectInfo {
	ObjRect         rect;		// object potition.
	float			prob;		// credit(0.0~1.0).
	unsigned int	clsIdx;		// object class(0~n).
	std::string		clsLabel;	// object name.
	unsigned int	trackId;	// object id that will be used in the tracker.
	//cv::Mat         cvCrop;
	//float           fRateBestObj;
	//int				iFrameCount;
	//bool            bObjInsidePos;
	//std::vector<std::string> vPlateChar;
};



#endif // __MJVT_TYPE_DEF_H__
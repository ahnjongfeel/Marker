//////////////////////////////////////////////////////////////////////////////
//
// FILE INFORMATION
//
//   Project: implement of multiple objects detection and anpr.
//      Unit: 
// $Workfile: MJVT_security.dll $
// $Revision: 0.1 $
//    $ Date: 03/05/2018 $
//
// DESCRIPTION :
//
//
//////////////////////////////////////////////////////////////////////////////
//
// MODIFICATION HISTORY
//    $ Date :  $
// Note :
// 
//////////////////////////////////////////////////////////////////////////////

#ifndef __MJVT_SECURITY_H__
#define __MJVT_SECURITY_H__


#ifdef MJVT_SECURITY_EXPORTS
#define MJVT_SECURITY_API __declspec(dllexport)
#else
#define MJVT_SECURITY_API __declspec(dllimport)
#endif
#include "MJVP_TypeDef.h"
#include "MJVT_Tracker.h"
typedef enum _MSVP_STATUS_{
	MSVP_SUCCESS = 0x00,	// success.
	MSVP_CFG_NOT_EXIST,		// config file not exist.
	MSVP_LOCK_FAIL,			// key-lock fail.
	MSVP_MEMORY_FAIL,		// memory allocate to object fail.
	MSVP_CORE_INIT_FAIL,	// initialize core fail.
	MSVP_MODE_INVALID		// MVAP_MODE is not valid.
}MSVP_STATUS;

typedef enum _MSVP_MODE_{
	MSVP_OBJECT_DETECT = 0x00,	// detect multiple objects.
	MSVP_CAR_DETECT,			// detect car and number plate.
	MSVP_CAR_ANPR,				// recognize number from number plate.
	MSVP_COLOR_DETECT,			// detect color
	MSVP_MODEL_DETECT			// detect model
}MSVP_MODE;

class MultiDet;
class MJVT_SECURITY_API MSVP_DeepLearning {
public:
	MSVP_DeepLearning(void);
	~MSVP_DeepLearning(void);

	MSVP_STATUS init(MSVP_MODE msvp_mode, const char *p_sz_path, int gpu_id = 0);
	MSVP_STATUS uninit();
	std::vector<ObjectInfo> detect(unsigned char *p_src, int col, int row, int chn);
	std::vector<ObjectInfo> detect(cv::Mat src);
	std::vector<ObjectInfo> tracker(std::vector<ObjectInfo> result, cv::Mat cur_frame, bool bDraw = 0);
	const std::string get_version();
	
	int getGpuCount();

private:
	MultiDet *m_det;
	MSVP_MODE m_msvp_mode;
	char m_sz_path[MVAP_MAX_LEN];

	
	std::vector<ObjectInfo> bb_to_obj(std::vector<bbox_t> bb);
	std::vector<bbox_t>     obj_to_bb(std::vector<ObjectInfo> obj);
};

#endif//__MJVT_SECURITY_H__
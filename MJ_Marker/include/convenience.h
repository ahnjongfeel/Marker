#pragma once
//#include "stdafx.h"
//#include <window.h>
//warning disable
#pragma warning(disable : 4819)
#pragma warning(disable : 4996)
#pragma warning(disable : 4267)
#pragma warning(disable : 4018)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#pragma warning(disable : 4288)

//folder name read

void GetCurTime(char *dst);
void GetIniFolderName(char* dst);
void GetIniFolderFileName(char* ini_file_name, char* dst);
void GetIniFolderFileName(char* folder_path, char* ini_file_name, char* dst);
void GetIniAttribute_int(char* ini_file_name, char* section, char* name, int* iDst);
void GetIniAttribute_str(char* ini_file_name, char* section, char* name, char* dst);
void GetIniAttribute_str(char* folder_name, char* ini_file_name, char* section, char* name, char* dst);
bool SetIniAttribute_str(char* ini_file_name, char* section, char* name, char* src);
bool SetIniAttribute_str(char* folder_path, char* ini_file_name, char* section, char* name, char* src);
VOID UBEYE_LOG(char *p_msg, ...);
VOID UBEYE_LOG2(char *p_msg, ...);
void  GetParsing_str(char* src, char* token, int section_index, char* dst);
void GetParsing_int(char* src, char* token, int section_index, int* iDst);
void openFolder(char* path);
CString ConvertMultibyteToUnicode(char* pMultibyte);


#include <time.h>
//folder name read
#define LOG_FILE_NAME       (_T("log.txt"))
void ProcessWindowMessage();

VOID di_trace(char *p_msg, ...);

#define DI_TRACE di_trace
#include "stdafx.h"
#include "convenience.h"

void GetCurTime(char * dst)
{
	CString tmp;
	SYSTEMTIME st;
	
	GetLocalTime(&st);
	tmp.Format("[%02d:%02d:%02d]-", st.wHour, st.wMinute, st.wSecond);
	sprintf(dst, "%s", tmp.GetBuffer());
}


void GetIniFolderName(char* dst)
{
	char *pSearchPath, sz_folder_name[2048];
		
	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) -1 );
	pSearchPath = strrchr(sz_folder_name, '\\');

	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	int last_index =strlen(sz_folder_name);
	char last_char;
	if(last_index -1 > 0) last_char = sz_folder_name[last_index-1];
	if(last_char != '\\') 	strcat(sz_folder_name, "\\");
		
	sprintf(dst, "%s", sz_folder_name);

}


void GetIniFolderFileName(char* ini_file_name, char* dst)
{
	char *pSearchPath, sz_folder_name[2048];
		
	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) -1 );
	pSearchPath = strrchr(sz_folder_name, '\\');


	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	strcat(sz_folder_name, ini_file_name);

	sprintf(dst, "%s", sz_folder_name);
}
void GetIniFolderFileName(char* folder_path, char* ini_file_name, char* dst)
{
	char *pSearchPath, sz_folder_name[2048];

	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) - 1);

	sprintf(sz_folder_name, "%s", folder_path);



	pSearchPath = strrchr(sz_folder_name, '\\');


	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	strcat(sz_folder_name, ini_file_name);

	sprintf(dst, "%s", sz_folder_name);
}


     
void GetIniAttribute_int(char* ini_file_name, char* section, char* name, int* iDst)
{
	char *pSearchPath, sz_folder_name[2048];

	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) - 1);
	pSearchPath = strrchr(sz_folder_name, '\\');
	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	strcat(sz_folder_name, ini_file_name);

	int ireturn;
	ireturn = GetPrivateProfileInt(section, name, 0, sz_folder_name);

	*iDst = ireturn;
}

void GetIniAttribute_str(char* ini_file_name, char* section, char* name, char* dst)
{
	char *pSearchPath, sz_folder_name[2048];

	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) - 1);
	pSearchPath = strrchr(sz_folder_name, '\\');
	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	strcat(sz_folder_name, ini_file_name);

	char sz_buf[2048];
	memset(sz_buf, 0x00, sizeof(sz_buf));
	GetPrivateProfileString(section, name, "ERROR", sz_buf, sizeof(sz_buf), sz_folder_name);

	sprintf(dst, "%s", sz_buf);
}

void GetIniAttribute_str(char* folder_name, char* ini_file_name, char* section, char* name, char* dst)
{
	char *pSearchPath, sz_folder_name[2048];

	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) - 1);
	sprintf(sz_folder_name, "%s", folder_name);
	pSearchPath = strrchr(sz_folder_name, '\\');
	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	strcat(sz_folder_name, ini_file_name);

	char sz_buf[2048];
	memset(sz_buf, 0x00, sizeof(sz_buf));
	GetPrivateProfileString(section, name, "ERROR", sz_buf, sizeof(sz_buf), sz_folder_name);

	sprintf(dst, "%s", sz_buf);
}

bool SetIniAttribute_str(char * ini_file_name, char * section, char * name, char * src)
{
	char *pSearchPath, sz_folder_name[2048];

	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) - 1);
	pSearchPath = strrchr(sz_folder_name, '\\');
	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	strcat(sz_folder_name, ini_file_name);

	char sz_buf[2048];
	memset(sz_buf, 0x00, sizeof(sz_buf));
	
	return WritePrivateProfileString(section, name, src, sz_folder_name);
}

bool SetIniAttribute_str(char* folder_path, char * ini_file_name, char * section, char * name, char * src)
{
	char *pSearchPath, sz_folder_name[2048];

	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) - 1);
	sprintf(sz_folder_name, "%s", folder_path);
	pSearchPath = strrchr(sz_folder_name, '\\');
	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	strcat(sz_folder_name, ini_file_name);

	char sz_buf[2048];
	memset(sz_buf, 0x00, sizeof(sz_buf));

	return WritePrivateProfileString(section, name, src, sz_folder_name);
}


//»ç¿ë¿¹ : UBEYE_LOG("%s", "IrisControlThr\r\n");


VOID UBEYE_LOG(char *p_msg, ...)
{
	char *pSearchPath, sz_folder_name[2048];
		
	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) -1 );
	pSearchPath = strrchr(sz_folder_name, '\\');
	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	int last_index =strlen(sz_folder_name);
	char last_char;
	if(last_index -1 > 0) last_char = sz_folder_name[last_index-1];
	if(last_char != '\\') 	strcat(sz_folder_name, "\\");

	
	FILE *fp;
	TCHAR file_path[2048];
	SYSTEMTIME st;
	char tmp[512];
	char buf[25];	
	char context[1024];
	va_list vl;

	GetLocalTime(&st);
	//strcpy(file_path, gStr_root);
	//strcat(file_path, "\\");
	//strcat(file_path, LOG_FILE_NAME);
#define LOG_FILE_NAME "logfile.log"
	sprintf(file_path, "%s\\%04d%02d%02d_%s", sz_folder_name, st.wYear, st.wMonth, st.wDay, LOG_FILE_NAME);

	fp = fopen(file_path, "a+t");
	if(!fp)	{	fp = fopen(file_path, "w+t");	if(!fp)	return;	}

	va_start(vl, p_msg);
	vsprintf_s(tmp, p_msg, vl);
	va_end(vl);

	sprintf(buf, "[%02d:%02d:%02d]-", st.wHour, st.wMinute, st.wSecond);
	strcpy(context, buf);
	strcat(context, tmp);
	fputs(context, fp);

	fclose(fp);
}


VOID UBEYE_LOG2(char *p_msg, ...)
{
	char *pSearchPath, sz_folder_name[2048];

	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) - 1);
	pSearchPath = strrchr(sz_folder_name, '\\');
	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	int last_index = strlen(sz_folder_name);
	char last_char;
	if (last_index - 1 > 0) last_char = sz_folder_name[last_index - 1];
	if (last_char != '\\') 	strcat(sz_folder_name, "\\");


	FILE *fp;
	TCHAR file_path[2048];
	SYSTEMTIME st;
	char tmp[512];
	char buf[512];
	char context[1024];
	va_list vl;

	GetLocalTime(&st);
	//strcpy(file_path, gStr_root);
	//strcat(file_path, "\\");
	//strcat(file_path, LOG_FILE_NAME);
#define LOG_FILE_NAME2 "Result_Log_File.log"
	sprintf(file_path, "%s\\%04d%02d%02d_%s", sz_folder_name, st.wYear, st.wMonth, st.wDay, LOG_FILE_NAME2);

	fp = fopen(file_path, "a+t");
	if (!fp) { fp = fopen(file_path, "w+t");	if (!fp)	return; }

	va_start(vl, p_msg);
	vsprintf_s(tmp, p_msg, vl);
	va_end(vl);

	strcpy(context, tmp);
	//strcat(context, tmp);
	fputs(context, fp);

	fclose(fp);
}







void GetParsing_int(char* src, char* token, int section_index, int* iDst)
{
	char sz_src[2048];
	char* sz_dst;
	char sz_token[2048];
	strcpy(sz_src, src);
	strcpy(sz_token, token);
	int i;
	sz_dst = strtok(sz_src, sz_token);

	for (i = 0; i<section_index; i++)
	{
		sz_dst = strtok(NULL, sz_token);

	}
	*iDst = atoi(sz_dst);
}

void openFolder(char * path)
{
	ShellExecute(NULL, "open", "explorer.exe", path, NULL, SW_SHOW);
}


void  GetParsing_str(char* src, char* token, int section_index, char* dst)
{
	char sz_src[2048];
	char* sz_dst;
	char sz_token[2048];
	strcpy(sz_src, src);
	strcpy(sz_token, token);
	int i;
	sz_dst = strtok(sz_src, sz_token);

	for (i = 0; i<section_index; i++)
	{
		sz_dst = strtok(NULL, sz_token);
		if (!sz_dst)
		{
			sprintf(dst, "%s", "\0");
			return;
		}


	}
	sprintf(dst, "%s", sz_dst);
}




VOID di_trace(char *p_msg, ...)
{
	char *pSearchPath, sz_folder_name[2048];
		
	GetModuleFileName(NULL, sz_folder_name, sizeof(sz_folder_name) -1 );
	pSearchPath = strrchr(sz_folder_name, '\\');
	sz_folder_name[strlen(sz_folder_name) - strlen(pSearchPath)] = '\0';
	strcat(sz_folder_name, "\\");
	int last_index =strlen(sz_folder_name);
	char last_char;
	if(last_index -1 > 0) last_char = sz_folder_name[last_index-1];
	if(last_char != '\\') 	strcat(sz_folder_name, "\\");

	
	FILE *fp;
	TCHAR file_path[2048];
	SYSTEMTIME st;
	char tmp[512];
	char buf[25];	
	char context[1024];
	va_list vl;

	GetLocalTime(&st);
	//strcpy(file_path, gStr_root);
	//strcat(file_path, "\\");
	//strcat(file_path, LOG_FILE_NAME);
	sprintf(file_path, "%s\\%04d%02d%02d_%s", sz_folder_name, st.wYear, st.wMonth, st.wDay, LOG_FILE_NAME);

	fp = fopen(file_path, "a+t");
	if(!fp)	{	fp = fopen(file_path, "w+t");	if(!fp)	return;	}

	va_start(vl, p_msg);
	vsprintf_s(tmp, p_msg, vl);
	va_end(vl);

	sprintf(buf, "[%02d:%02d:%02d]-", st.wHour, st.wMinute, st.wSecond);
	strcpy(context, buf);
	strcat(context, tmp);
	fputs(context, fp);

	fclose(fp);
	TRACE(buf);
}



bool di_time_getTimeStrCurrent(char *pszDst)
{
	time_t t;
	struct tm * tm;

	time(&t);

	tm = localtime(&t);

	sprintf(pszDst, "%04d-%02d-%02d %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);

	return true;
}



CString ConvertMultibyteToUnicode(char* pMultibyte)
{
	int nLen = strlen(pMultibyte);

	WCHAR *pWideChar = new WCHAR[nLen];
	memset(pWideChar, 0x00, (nLen) * sizeof(WCHAR));

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pMultibyte, -1, pWideChar, nLen);

	CString strUnicode;
	strUnicode.Format(_T("%s"), pWideChar);

	delete[] pWideChar;

	return strUnicode;
}


void ProcessWindowMessage()

{

	MSG msg;

	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))

	{

		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);

	}

}

// јвторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueFileSystem.cpp: implementation of the CValueFileSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueFileSystem.h"

#include "OutToDebug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "..\DirDialog\DirDialog.h"      // Our own header file

CString GetGetLastStringError();

CMethods CValueFileSystem::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueFileSystem,CValue);


CValueFileSystem::CValueFileSystem()
{
	nType=100;
	hFindFile=0;
}

CValueFileSystem::~CValueFileSystem()
{
	if(hFindFile)
		FindClose(hFindFile);
	hFindFile=0;

	CloseAllHandle();
}

void CValueFileSystem::CloseAllHandle()
{
	HANDLE hFile;
	POSITION pos;
	CString csKey;
	for( pos = aListHandle.GetStartPosition(); pos != NULL; )
	{
		aListHandle.GetNextAssoc( pos, csKey, (void*&)hFile );
		if(hFile)
			CloseHandle (hFile);
	}
	aListHandle.RemoveAll();

}


CString GetStringFromTime(FILETIME &FileTime)
{
	SYSTEMTIME time;
	FileTimeToSystemTime(&FileTime,&time);
	CString csRes;
	csRes.Format("%d.%d.%d %d:%d:%d",time.wDay,time.wMonth,time.wYear,time.wHour,time.wMinute,time.wSecond);
	return csRes;
}
//******************************
//–абота как агрегатного объекта
//******************************
void CValueFileSystem::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"SelectFile","¬ыбрать‘айл","¬ыбрать‘айл(“ипƒиалога,»м€‘айла,»м€Ќач аталога,«аголовокќкна,‘ильтр,–асширение,“аймаут)"},
		{"SelectDirectory","¬ыбрать аталог","¬ыбрать аталог(»м€ аталога,«аголовокќкна,“аймаут)"},
		{"ExistFile","—уществует‘айл","—уществует‘айл(»м€‘айла) - провер€ет существование файла"},
		{"CreateDirectory","—оздать аталог","—оздать аталог(»м€‘айла)"},
		{"RemoveDirectory","”далить аталог","”далить аталог(»м€‘айла)"},
		{"SetCurrentDirectory","”ст“ек аталог","”ст“ек аталог(»м€‘айла) - устанавливает текущий каталог файлов"},
		{"GetCurrentDirectory","“ек аталог","“ек аталог() - возвращает строку с именем текущего каталога файлов"},
		{"DeleteFile","”далить‘айл","”далить‘айл(»м€‘айла)"},
		{"FindFirstFile","Ќайтиѕервый‘айл","Ќайтиѕервый‘айл(ћаска»мени)"},
		{"FindNextFile","Ќайти—ледующий‘айл","Ќайти—ледующий‘айл() - находит следующий файл по открытой выборке"},
		{"GetFileAttr","јтрибуты‘айла","јтрибуты‘айла(»м€‘айла,–азмер‘айла,јтрибуты‘айла,¬рем€—оздани€,¬рем€ѕоследнегоƒоступа,¬рем€ѕоследней«аписи,–асширенное»м€‘айла)"},
		{"FileCopy"," опировать‘айл"," опировать‘айл(»м€‘айла»сточника,»м€‘айлаѕриемника,‘лагѕерезаписи)"},
		{"GetWindowsDirectory","Windows аталог","Windows аталог() - возвращает строку с именем Windows директории"},
		{"GetDiskFreeSpace","—вободноећестоЌаƒиске","—вободноећестоЌаƒиске(»м€ƒиска)"},
		{"MoveFile","ѕереименовать‘айл","ѕереименовать‘айл(»м€‘айла»сточника,»м€‘айлаѕриемника,‘лагѕерезаписи)"},

		{"LockFile ","«аблокировать‘айл","«аблокировать‘айл(»м€‘айла,ѕозици€=0,ƒлина=1) /блокировка участка файла/"},
		{"UnLockFile ","–азблокировать‘айл","–азблокировать‘айл(»м€‘айла,ѕозици€=0,ƒлина=1) /разблокировка участка файла/"},
		{"UnLockAllFile ","–азблокировать¬се‘айлы","–азблокировать¬се‘айлы() /разблокировка всех ранее заблокированных файлов/"},
		{"WriteData ","«аписатьƒанные","«аписатьƒанные(»м€‘айла,—трƒанные,ѕозици€) /записать данные в файл с указанной позиции"},
		{"ReadData ","ѕрочитатьƒанные","ѕрочитатьƒанные(»м€‘айла,ѕозици€,ƒлина) /прочитать данные из файла с указанной позиции"},
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}
enum
{
	enSelectFile=0,
	enSelectDirectory,
	enExistFile,
	enCreateDirectory,
	enRemoveDirectory,
	enSetCurrentDirectory,
	enGetCurrentDirectory,
	enDeleteFile,
	enFindFirstFile,
	enFindNextFile,
	enGetFileAttr,
	enFileCopy,
	enGetWindowsDirectory,
	enGetDiskFreeSpace,
	enMoveFile,
	enLockFile,
	enUnLockFile,
	enUnLockAllFile,
	enWriteData,
	enReadData,
};


CValue CValueFileSystem::Method(int iName,CValue **p)
{
	CValue Ret;
	switch(iName)
	{
		case enSelectFile:
			{
				CDirDialog dlg(p[0]->GetString(),p[3]->GetString(),0);

				CString csFile=p[1]->GetString();
				
				CString csFilter=p[4]->GetString();
				char *sFilter=csFilter.GetBuffer(MAX_PATH);
				for(int i=0;i<MAX_PATH;i++)
				{
					if(sFilter[i]=='|'||i>=csFilter.GetLength())
						sFilter[i]=0;
				}

				OPENFILENAME ofn;
				ofn.lStructSize       = sizeof(OPENFILENAME);
				ofn.hwndOwner         = NULL;
				ofn.lpstrFilter       = sFilter;
				ofn.lpstrCustomFilter = NULL;//p[5]->GetString();
				ofn.nFilterIndex      = 1;
				ofn.lpstrFile         = csFile.GetBuffer(MAX_PATH);
				ofn.nMaxFile          = MAX_PATH;
				ofn.nMaxFileTitle	  = MAX_PATH;
				ofn.lpstrInitialDir   = p[2]->GetString();
				ofn.lpstrTitle        = p[3]->GetString();
				ofn.lpstrFileTitle    = NULL;
				ofn.lpstrInitialDir	  = NULL; 
				ofn.lCustData		  = 0;
				ofn.lpfnHook		  = NULL;
				ofn.lpTemplateName	  = NULL; 
				ofn.lpstrDefExt       = (LPCTSTR)"TXT";
				ofn.Flags             = 0;//OFN_PATHMUSTEXIST; 

				Ret=0;
				if(p[0]->GetNumber())
				{
					if (GetSaveFileName(&ofn))         
						Ret=1;
				}
				else
				{
					if (GetOpenFileName(&ofn))         
						Ret=1;
				}

				csFilter.ReleaseBuffer();
				csFile.ReleaseBuffer();
				*p[1]=String(csFile);
				break;
			}
		case enSelectDirectory:
			{
				CDirDialog dlg(p[0]->GetString(),p[3]->GetString(),0);
				dlg.m_ofn.lpstrTitle = p[1]->GetString();
				if (dlg.DoModal() == IDOK)
				{
					Ret=1;
					*p[0]=String(dlg.GetPath());
				}
				else
				{
					Ret=0;
				}
				break;
			}
		case enGetCurrentDirectory:
			{
				CString csCurrentDir;
				GetCurrentDirectory(MAX_PATH,csCurrentDir.GetBuffer(MAX_PATH));
				csCurrentDir.ReleaseBuffer();
				Ret=String(csCurrentDir);
				break;
			}
		case enSetCurrentDirectory:
			{
				SetCurrentDirectory(p[0]->GetString());
				break;
			}
		case enCreateDirectory:
			{
				CreateDirectory(p[0]->GetString(),0);
				break;
			}
		case enDeleteFile:
			{
				DeleteFile(p[0]->GetString());
				break;
			}
		case enFindFirstFile:
			{
				if(hFindFile)
					FindClose(hFindFile);
				hFindFile=0;
				WIN32_FIND_DATA FindFileData;
				hFindFile = FindFirstFile(p[0]->GetString(), &FindFileData);
				if (hFindFile != INVALID_HANDLE_VALUE)
					Ret=String(FindFileData.cFileName);
				break;
			}
		case enFindNextFile:
			{
				WIN32_FIND_DATA FindFileData;
				if(FindNextFile(hFindFile,&FindFileData))
					Ret=String(FindFileData.cFileName);
				else
				{
					if(hFindFile)
						FindClose(hFindFile);
					hFindFile=0;
				}
				break;
			}
		case enGetFileAttr:
			{
				HANDLE hFindFile;
				WIN32_FIND_DATA FindFileData;
				hFindFile = FindFirstFile(p[0]->GetString(), &FindFileData);
				if (hFindFile != INVALID_HANDLE_VALUE)
				{
					*p[0]=String(FindFileData.cFileName);
					*p[1]=double(FindFileData.nFileSizeHigh)*0x100000000+double(FindFileData.nFileSizeLow);
					CString csAttr;
					int nMask=1;
					for(int i=0;i<9;i++)
					{
						if((FindFileData.dwFileAttributes&nMask)==nMask)
							csAttr+="1";
						else
							csAttr+="0";
						nMask=nMask*2;
					}
					*p[2]=String(csAttr);

					*p[3]=String(GetStringFromTime(FindFileData.ftCreationTime));
					*p[4]=String(GetStringFromTime(FindFileData.ftLastAccessTime));
					*p[5]=String(GetStringFromTime(FindFileData.ftLastWriteTime));


					*p[6]=String(FindFileData.cAlternateFileName);
				}

				break;
			}
		case enFileCopy:
			{
				Ret=CopyFile(p[0]->GetString(),p[1]->GetString(),p[2]->GetNumber());
				break;
			}

		case enExistFile:
			{
				WIN32_FIND_DATA data;
				HANDLE hFile=FindFirstFile(p[0]->GetString(),&data);
				if(hFile==INVALID_HANDLE_VALUE||hFile==0)
				{
					Ret=0;
				}
				else
				{
					Ret=1;
					FindClose(hFile);
				}
				break;
			}

		case enRemoveDirectory:
			{
				Ret=RemoveDirectory(p[0]->GetString());
				break;
			}

		case enGetWindowsDirectory:
			{
				CString csStr;
				GetWindowsDirectory(csStr.GetBuffer(MAX_PATH),MAX_PATH);
				csStr.ReleaseBuffer();
				Ret=String(csStr);
				break;
			}
			
			
		case enGetDiskFreeSpace:
			{
                ULARGE_INTEGER i64FreeBytesToCaller;
                ULARGE_INTEGER i64TotalBytes;
                ULARGE_INTEGER i64FreeBytes;

				GetDiskFreeSpaceEx (p[0]->GetString(),	&i64FreeBytesToCaller,&i64TotalBytes,&i64FreeBytes);
				Ret=double(i64FreeBytes.HighPart)*0x100000000+double(i64FreeBytes.LowPart);
				break;
			}
		case enMoveFile:
			{
				Ret=MoveFile(p[0]->GetString(),p[1]->GetString());
				break;
			}
		case enLockFile:
		case enUnLockFile:
			{
				CString csFileName=p[0]->GetString();
				int nStartPos=0;
				if(p[1]->nType)
					nStartPos=p[1]->GetNumber();
				int nLength=1;
				if(p[2]->nType)
					nLength=p[2]->GetNumber();

				HANDLE hFile=0;
				hFile=aListHandle[mUpper(csFileName)];
				if(!hFile)
				{
					hFile=CreateFile(csFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,NULL,NULL);
					aListHandle[mUpper(csFileName)]=hFile;
				}

				Ret=0;
				if(hFile==INVALID_HANDLE_VALUE||hFile==0)
				{
					Error("‘айл %s не найден",csFileName);
				}
				else
				{
					BOOL bRes;
					if(enLockFile==iName)
					{
						bRes=LockFile(hFile,nStartPos,0,nLength,0);//лочим файлы до 4√...
					}
					else
					{
						bRes=UnlockFile(hFile,nStartPos,0,nLength,0);
					}
					Ret=1;//(int)hFile;
					//CloseHandle (hFile);

					if(!bRes)
					{
						//CloseHandle (hFile);
						Error("‘айлова€ ошибка: %s",GetGetLastStringError());
					}
				}

				break;
			}
		case enUnLockAllFile:
			{
				CloseAllHandle();
				Ret=1;
				break;
			}
		case enWriteData:
			{
				CString sFileName=p[0]->GetString();
				CString Str=p[1]->GetString();
				int nPos=p[2]->GetNumber();
				HANDLE hFile=CreateFile(sFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,NULL,NULL);
				if(hFile)
				{
					DWORD dwWrite=Str.GetLength();
					SetFilePointer(hFile,nPos,NULL,FILE_BEGIN);
					WriteFile(hFile,Str.GetBuffer(0),dwWrite,&dwWrite,NULL);
					CloseHandle (hFile);
				}
				break;
			}
		case enReadData:
			{
				CString sFileName=p[0]->GetString();
				int nPos=p[1]->GetNumber();
				int nLenght=p[2]->GetNumber();
				HANDLE hFile=CreateFile(sFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,NULL,NULL);
				if(hFile)
				{
					SetFilePointer(hFile,nPos,NULL,FILE_BEGIN);
					DWORD dwRead;
					char *pBuf=new char [nLenght+1];
					pBuf[nLenght]=0;
					if(!ReadFile(hFile,pBuf,nLenght,&dwRead,NULL))
					{
						CloseHandle (hFile);
						Error(ERROR_FILE_READ,sFileName.GetBuffer(0));
					}
					CloseHandle (hFile);
					return pBuf;
				}
				break;
			}
	}
	return Ret;
}

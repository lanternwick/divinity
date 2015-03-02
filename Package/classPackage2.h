#ifndef _PACKAGE_H
#define _PACKAGE_H

#include <result.h>
#include <stdio.h>
template<int n>
class readBuffer
{
public:
	readBuffer()
	{
	}

	BYTE ReadByte(FILE* f)
	{
		BYTE b;
		fread(&b, 1, 1, f);
		for(INT i = 1; i < n; ++i)
		{
			buffer[i-1] = buffer[i];
		}
		buffer[n-1] = b;
		return b;
	}

	BOOL CheckSequence(BYTE b, BYTE numCheck)
	{
		INT cnt = 0;
		for(INT i = 0; i < n; ++i)
		{
			if(buffer[i] == b)
			{
				++cnt;
				if(cnt == numCheck)
					return TRUE;
			}
		}
		return FALSE;
	}

	BYTE buffer[n];
};


union DWORDBBUFF
{
	DWORD d;
	BYTE buff[4];
};

struct PACKDESC
{
	BYTE encrypted;
	BYTE compressed;
	DWORD strSize;
	UINT numFiles;
	char* filenames;
	DWORD* filesizes;
};


class Package
{
public:
	Package();
	~Package();

	RESULT Open(char* packfile);
	RESULT Close();
	RESULT GetDesc(PACKDESC* pd);
	RESULT Extract(char* filename, UINT identifier);
	RESULT Delete(UINT identifier);
	RESULT AddFile(char* file, char* dir);
	RESULT New(char* file);
	VOID SetUI(BOOL on);
	BOOL GetUI();
	BOOL Open();
private:
	PACKDESC myPD;
	FILE* f;
	BOOL pOpen;
	readBuffer<3> buffer;
	BYTE* PackageBuffer;
	BOOL AllowUI;
	DWORD filestart;

	RESULT ExtractByOffset(char* filename, DWORD offset, DWORD size);
	RESULT DeleteByOffset(DWORD offset, DWORD size);
};

Package::Package()
{
	f = NULL;
	pOpen = FALSE;
	ZeroMemory(&myPD, sizeof(myPD));
	PackageBuffer = NULL;
	AllowUI = FALSE;
}

Package::~Package()
{

}

RESULT Package::Open(char* packfile)
{
	if(packfile == NULL)
		return FAILED_OPEN;
	if(strcmp("", packfile) == 0)
		return FAILED_OPEN;
	f = fopen(packfile, "r+b");
	BYTE* files = NULL;
	DWORD* fsizes = NULL;
	if(!f)
		return FAILED_OPEN;
	
	fread(&myPD.encrypted, 1, 1, f);
	fread(&myPD.compressed, 1, 1, f);
	fread(&myPD.strSize, 1, 4, f);
	fread(&myPD.numFiles, 1, 2, f);
	myPD.filenames = new char[myPD.numFiles*128];
	myPD.filesizes = new DWORD[myPD.numFiles];
	files = (BYTE*)myPD.filenames;
	fsizes = myPD.filesizes;
	BYTE tmp;
	DWORD dtmp;
	UINT num = 0;
	while(num < myPD.numFiles)
	{
		tmp = buffer.ReadByte(f);
		*files = tmp;
		if(tmp == 0)
		{
			*files = tmp;
			fread(&dtmp, 1, 4, f);
			*fsizes = dtmp;
			fsizes+=4;
			fread(&tmp, 1, 1, f);
			num++;
		}
		files++;
	}
	filestart = ftell(f)+3;
	pOpen = TRUE;
	return OK;
}

RESULT Package::Close()
{
	if(pOpen)
	{
		fclose(f);
		pOpen = FALSE;
		return OK;
	}
	return PACK_NOT_OPEN;
}

RESULT Package::GetDesc(PACKDESC* pd)
{
	char* files = myPD.filenames;
	DWORD* fsizes = myPD.filesizes;
	UINT num = 0;
	BYTE tmp;
	DWORD dtmp;
	fseek(f, 8, SEEK_SET);
	while(num < myPD.numFiles)
	{
		tmp = buffer.ReadByte(f);
		*files = tmp;
		if(tmp == 0)
		{
			*files = tmp;
			fread(&dtmp, 1, 4, f);
			*fsizes = dtmp;
			fsizes+=4;
			fread(&tmp, 1, 1, f);
			num++;
		}
		files++;
	}
	*pd = myPD;
	return OK;
}

RESULT Package::ExtractByOffset(char* filename, DWORD offset, DWORD size)
{
	BYTE* buff = NULL;
	FILE* eFile;
	eFile = fopen(filename, "w+b");
	fseek(f, filestart+offset, SEEK_SET);
	buff = (BYTE*)malloc(size);
	fread(buff, 1, size, f);
	fwrite(buff, 1, size, eFile);
	fclose(eFile);
	free(buff);
	return OK;
}

RESULT Package::DeleteByOffset(DWORD offset, DWORD size)
{
	BYTE* buff = NULL;
	DWORD len = 0;
	DWORD off = 0;
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, offset+size, SEEK_SET);
	off = offset+size;
	len-=off;
	buff = (BYTE*)malloc(len);
	fread(buff, 1, len, f);
	fseek(f, offset, SEEK_SET);
	fwrite(buff, 1, len, f);
	myPD.numFiles--;
	fseek(f, 6, SEEK_SET);
	fwrite(&myPD.numFiles, 2, 1, f);
	return OK;
}



RESULT Package::Extract(char* filename, UINT identifier)
{
	if(!pOpen)
		return PACK_NOT_OPEN;

	if(myPD.numFiles == 0)
		return NO_FILES_TO_EXTRACT;
	
	UINT num = 0;
	BYTE b;
	DWORD s;
	DWORD off = 0;
	
	fseek(f, 8, SEEK_SET);
	
	while(num < identifier)
	{
		fread(&b, 1, 1, f);
		if(b == 0)
		{
			//fseek(f, 1, SEEK_CUR);
			fread(&s, 4, 1, f);
			off += s;
			fseek(f, 1, SEEK_CUR);
			++num;
		}
	}
	return ExtractByOffset(filename, off, s);
}

RESULT Package::Delete(UINT identifier)
{
	if(!pOpen)
		return PACK_NOT_OPEN;

	if(myPD.numFiles == 0)
		return NO_FILES_TO_EXTRACT;
	
	UINT num = 0;
	BYTE b;
	DWORD s;
	DWORD off = 0;
	DWORD fsize = 0;
	fseek(f, 8, SEEK_SET);
	
	while(num < identifier)
	{
		fread(&b, 1, 1, f);
		fsize++;
		if(b == 0)
		{
			//fseek(f, 1, SEEK_CUR);
			fread(&s, 4, 1, f);
			off += s;
			fseek(f, 1, SEEK_CUR);
			++num;
			if(num < identifier)
				fsize = 0;
		}
		
	}
	fsize+=6;
	DeleteByOffset(ftell(f)-fsize, fsize);
	return DeleteByOffset(filestart+off-s-fsize, s);
}

RESULT Package::AddFile(char* file, char* dir)
{
	if(!pOpen)
		return PACK_NOT_OPEN;
	if(file == NULL)
		return FAILED_OPEN;
	if(strcmp(file, "")==0)
		return FAILED_OPEN;
	if(dir == NULL)
		return FAILED_OPEN;
	if(strcmp(dir, "")==0)
		return FAILED_OPEN;

	FILE* fileToAdd;
	BYTE* mbuffer = NULL;
	BYTE pbuff = NULL;
	DWORD fLen = 0;
	DWORD plen;
	DWORD bslash = 0;
	char* title = NULL;

	fileToAdd = fopen(file, "r+b");
	if(!fileToAdd)
		return FAILED_OPEN;
	
	for(INT i = strlen(file)-1; i >= 0; --i)
	{
		if(file[i] == '\\')
		{
			bslash = i;
			break;
		}
	}
	title = new char[strlen(file)-bslash+strlen(dir)];
	for(i = bslash+1; i < (INT)strlen(file); ++i)
	{
		title[i-bslash-1] = file[i];
	}
	strcat(dir, title);
	strcpy(title, dir);
	fseek(fileToAdd, 0, SEEK_END);
	fLen = ftell(fileToAdd);
	mbuffer = new BYTE[fLen];
	rewind(fileToAdd);
	fread(mbuffer, 1, fLen, fileToAdd);
	fclose(fileToAdd);
	fseek(f, 0, SEEK_END);
	fwrite(mbuffer, 1, fLen, f);
	plen = ftell(f);
	fseek(f, filestart, SEEK_SET);
	plen-=ftell(f);
	realloc(mbuffer, plen);
	fread(mbuffer, 1, plen, f);
	fseek(f, filestart-3, SEEK_SET);
		
	pbuff = 0;
	fwrite(&title[0], 1, strlen(file) - bslash-1, f);
	fwrite(&pbuff, 1, 1, f);
	/*x.d = fLen;
	for(i = 0; i < 4; ++i)
	{
		fwrite(&(x.buff[i]), 1, 1, f);
	}*/
	fwrite(&fLen, 4, 1, f);
	fwrite(&pbuff, 1, 1, f);

	fwrite(&pbuff, 1, 1, f);
	fwrite(&pbuff, 1, 1, f);
	fwrite(&pbuff, 1, 1, f);

	myPD.numFiles++;
	filestart = ftell(f);
	fwrite(mbuffer, 1, plen, f);
	fseek(f, 6, SEEK_SET);
	fwrite(&myPD.numFiles, 2, 1, f);
	return OK;
}

RESULT Package::New(char* file)
{
	BYTE t;
	Close();
	f = fopen(file, "w+b");
	t = 0;
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	filestart = ftell(f);
	pOpen = TRUE;
	return OK;
}

VOID Package::SetUI(BOOL on)
{
	AllowUI = on;
}

BOOL Package::GetUI()
{
	return AllowUI;
}

BOOL Package::Open()
{
	return pOpen;
}

#endif
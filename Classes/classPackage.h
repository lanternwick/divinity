#ifndef _PACKAGE_H
#define _PACKAGE_H

#define HEADER_END 8

#include <result.h>
#include <windows.h>
#include <stdio.h>

/*enum RESULT
{
	OK,
	FAILED_OPEN,
	PACK_NOT_OPEN,
	NO_FILES_TO_EXTRACT,

};*/

typedef BYTE EncDecFunc(BYTE x, DWORD key);

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
	RESULT Encrypt(EncDecFunc* encrypt, DWORD key);
	RESULT Decrypt(EncDecFunc* decrypt, DWORD key);
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
	VOID ReadHeader();
	char file[128];
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

VOID Package::ReadHeader()
{
	if(!f)  // file not open...
		return;
	
	fseek(f, 0, SEEK_SET);
	if(myPD.filenames) // delete data?
	{
		delete myPD.filenames;
	}
	if(myPD.filesizes) // delete data?
	{
		delete myPD.filesizes;
	}
	BYTE* files = NULL;
	DWORD* fsizes = NULL;
	
	fread(&myPD.encrypted, 1, 1, f); // get the encrypted byte
	fread(&myPD.compressed, 1, 1, f); // get the compressed byte
	fread(&myPD.strSize, 1, 4, f); // get extra data (unused)
	fread(&myPD.numFiles, 1, 2, f); // get number of files in package
	myPD.filenames = new char[myPD.numFiles*128];
	myPD.filesizes = new DWORD[myPD.numFiles];
	files = (BYTE*)myPD.filenames;
	fsizes = myPD.filesizes;
	BYTE tmp;
	DWORD dtmp;
	UINT num = 0;
	while(num < myPD.numFiles) // parse file names and sizes
	{
		tmp = buffer.ReadByte(f);
		*files = tmp;
		if(tmp == 0) // end of a filename, get the filesize0
		{
			*files = tmp;
			fread(&dtmp, 1, 4, f); // get file size
			*fsizes = dtmp; 
			fsizes++;
			fread(&tmp, 1, 1, f); // get next data (will be 0)
			num++;
		}
		files++;
	}
	filestart = ftell(f)+3; // get the start position of file data
	return;
}

RESULT Package::Open(char* packfile)
// Open existing package file.
{
	
	f = fopen(packfile, "r+b");
	if(!f)
		return FAILED_OPEN; 
	
	ReadHeader(); // process header
	pOpen = TRUE;
	strcpy(file, packfile);
	return OK;
}

RESULT Package::Close()
// close an open package.  if not open, return
{
	if(!pOpen)
		return PACK_NOT_OPEN;

	fclose(f);
	pOpen = FALSE;
	delete myPD.filenames; // free memory
	delete myPD.filesizes; //   ...
	return OK;
	
	
}

RESULT Package::GetDesc(PACKDESC* pd)
// gets an updated copy of the header.
{
	ReadHeader(); // process header
	*pd = myPD;
	return OK;
}

RESULT Package::ExtractByOffset(char* filename, DWORD offset, DWORD size)
// extracts a file into filename, based on a start position in file
// data, and a size to read.
{
	BYTE* buff = NULL;
	FILE* eFile;
	eFile = fopen(filename, "w+b"); // create file.

	// seek to offset in data
	fseek(f, filestart+offset-size, SEEK_SET); 
	buff = new BYTE[size]; // alloc enough memory
	fread(buff, 1, size, f); // read data from package
	fwrite(buff, 1, size, eFile); // stick it into the new file
	fclose(eFile); // close new file
	delete buff; // done, free me mem, me 'earties!
	return OK;
}

RESULT Package::DeleteByOffset(DWORD offset, DWORD size)
// deletes a file from the package, based on an offset and
// size
{
	BYTE* buff = NULL;
	DWORD len = 0;
	DWORD off = 0;
	fseek(f, 0, SEEK_END); 
	len = ftell(f); // get file size

	// seek to end of file to delete
	fseek(f, offset+size, SEEK_SET); 
	// get size of data from end of to be deleted to end of file
	off = offset+size; 
	len-=off;

	buff = new BYTE[len];
	// grab data from end of file to delete to end of file
	fread(buff, 1, len, f);
	//seek to start of file to delete
	fseek(f, offset, SEEK_SET);
	//write data over file to delete, deleting file
	fwrite(buff, 1, len, f);
	
	delete buff; // free mem
	return OK;
}



RESULT Package::Extract(char* filename, UINT identifier)
// extracts a file to filename based on an ID number.
{
	if(!pOpen)
		return PACK_NOT_OPEN;
	
	if(myPD.numFiles == 0)
		return NO_FILES_TO_EXTRACT;
	
	UINT num = 0;
	BYTE b;
	DWORD s;
	DWORD off = 0;
	
	// seek to start of file name list in header
	fseek(f, HEADER_END, SEEK_SET); 
	
	// loop to find the file's location in file data.
	// simply adds file sizes.
	while(num < identifier) 
	{
		fread(&b, 1, 1, f);
		if(b == 0)
		{
			fread(&s, 4, 1, f);
			off += s;
			fseek(f, 1, SEEK_CUR);
			++num;
		}
	}
	return ExtractByOffset(filename, off, s); // get the file  =)
}

RESULT Package::Delete(UINT identifier)
// deletes a file from the package
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
	// seek to start of filenames in header
	fseek(f, HEADER_END, SEEK_SET);
	
	// find file position in package, 
	// by adding file sizes as it loops
	while(num < identifier)
	{
		fread(&b, 1, 1, f);
		fsize++;
		if(b == 0)
		{
			fread(&s, 4, 1, f);
			off += s;
			fseek(f, 1, SEEK_CUR);
			++num;
			if(num < identifier)
				fsize = 0;
		}
		
	}
	fsize+=5;
	DeleteByOffset(ftell(f)-fsize, fsize); // delete the filename
	// delete the file
	DeleteByOffset(filestart+off-s-fsize, s);
	fclose(f);
	HANDLE hf;
	BOOL bR;
	long filesize, ldw, hdw;
	filestart-=fsize;
	hf = CreateFile(file, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ldw = GetFileSize(hf, (DWORD*)&hdw);
	filesize = SetFilePointer((HANDLE)hf, ldw - s - fsize, 0, FILE_BEGIN);
	bR = SetEndOfFile((HANDLE)hf);
	CloseHandle(hf);
	f = fopen(file, "r+b");
	myPD.numFiles--; // one less file...
	fseek(f, 6, SEEK_SET); // write number of files into package
	fwrite(&myPD.numFiles, 2, 1, f);
	
	return OK;
}

RESULT Package::AddFile(char* file, char* dir)
// adds a file to the package
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
	LONG bslash = -1; // location of a backslash in the filename
	char* title = NULL;
	
	fileToAdd = fopen(file, "r+b"); // open the file
	if(!fileToAdd)
		return FAILED_OPEN;

	// find the last backslash...
	for(INT i = strlen(file)-1; i >= 0; --i) 
	{
		if(file[i] == '\\')
		{
			bslash = i;
			break;
		}
	}
	// .. give me mem for new title
	title = new char[128];
	
	// put new file title (no backslash) into title
	if(bslash == -1)
	{
		strcpy(title, file);
	}
	else
	{
		for(i = bslash+1; i < (INT)strlen(file); ++i)
		{
			title[i-bslash-1] = file[i];
		}
		title[i-bslash-1] = 0; 
	}
	strcat(dir, title);
	strcpy(title, dir);
	fseek(fileToAdd, 0, SEEK_END); 
	fLen = ftell(fileToAdd); // get the len of the new file
	mbuffer = new BYTE[fLen]; // alloc memory for file
	rewind(fileToAdd); // start of new file
	fread(mbuffer, 1, fLen, fileToAdd);  //read the file..
	fclose(fileToAdd); // close the file
	fseek(f, 0, SEEK_END); // seek to end of package
	fwrite(mbuffer, 1, fLen, f); // write the contents of the new file
	plen = ftell(f); // new package length
	fseek(f, filestart, SEEK_SET); // seek to start of file data
	plen-=ftell(f); // plen == size of file data in package
	BYTE* nbuffer;
	nbuffer = new BYTE[plen]; // alloc memory for entire package

	fread(nbuffer, 1, plen, f); // read entire file data from package
	fseek(f, filestart-3, SEEK_SET); // end of file name data
	
	pbuff = 0;
	// add title and file size data into header
	fwrite(&title[0], 1, strlen(title), f);
	fwrite(&pbuff, 1, 1, f);
	
	fwrite(&fLen, 4, 1, f); 
	fwrite(&pbuff, 1, 1, f);
	
	// 3 null chars to specify end of header, and start of file data
	fwrite(&pbuff, 1, 1, f);
	fwrite(&pbuff, 1, 1, f);
	fwrite(&pbuff, 1, 1, f);
	
	myPD.numFiles++; // increase files
	filestart = ftell(f);
	// write out package data
	fwrite(nbuffer, 1, plen, f);
	fseek(f, 6, SEEK_SET);
	// write new num files into header
	fwrite(&myPD.numFiles, 2, 1, f); 
	// free memory
	delete mbuffer; 
	delete nbuffer;
	delete title;
	return OK;
}

RESULT Package::New(char* packfile)
{
	BYTE t;
	Close();
	f = fopen(packfile, "w+b");
	t = 0;
	//Encrypted and compressed
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	//numfiles
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	//strsize (reserved)
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	//3 NULLs
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	fwrite(&t, 1, 1, f);
	filestart = ftell(f);
	pOpen = TRUE;
	strcpy(file, packfile);
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

RESULT Package::Encrypt(EncDecFunc* encrypt, DWORD key)
// Encrypt the file data
{
	if(!pOpen) 
		return PACK_NOT_OPEN;
	
	BYTE b;
	LONG pos;
	// seek to end of file
	fseek(f, 0, SEEK_END);
	// get length of file
	pos = ftell(f);
	// seek to start of file data
	fseek(f, filestart, SEEK_SET);
	while(ftell(f) < pos)
	{
		fread(&b, 1, 1, f);
		fseek(f, -1, SEEK_CUR);
		// encrypt a single byte
		b = encrypt(b, key);
		// write it into the package
		fwrite(&b, 1, 1, f);
		fseek(f, 1, SEEK_CUR);
	}
	// set encrypted byte
	myPD.encrypted = TRUE;
	// ... and write it out
	fseek(f, 2, SEEK_SET);
	fwrite(&myPD.encrypted, 1, 1, f);
	
	return OK;
}

RESULT Package::Decrypt(EncDecFunc* decrypt, DWORD key)
// Decrypt the package data
{
	if(!pOpen) 
		return PACK_NOT_OPEN;
	
	BYTE b;
	LONG pos;
	// seek to the start of the file
	fseek(f, 0, SEEK_END);
	// get the file length
	pos = ftell(f);
	// seek to start of file data
	fseek(f, filestart, SEEK_SET);
	while(ftell(f) < pos)
	{
		fread(&b, 1, 1, f);
		fseek(f, -1, SEEK_CUR);
		// decrypt a single byte
		b = decrypt(b, key);
		// ... and write it out
		fwrite(&b, 1, 1, f);
		fseek(f, 1, SEEK_CUR);
	}
	// set encrypted byte
	myPD.encrypted = FALSE;
	fseek(f, 2, SEEK_SET);
	// ... and write it out
	fwrite(&myPD.encrypted, 1, 1, f);
	
	return OK;
}

#endif

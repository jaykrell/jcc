#include "jos.h"

#if _WIN32
#include "windows.h"
#else
#include "unistd.h"
#endif

/* TODO: Test on old Windows, and possibly improve the code there. */
#define jVistaOrNewer 1

static int jos_open_file(const char* file_path, int read, int* file_handle)
{
#if _WIN32
	HANDLE f;
	f = CreateFileA(file_path, GENERIC_READ | (read ? 0 : GENERIC_WRITE), read ? FILE_SHARE_READ : 0, 0, read ? OPEN_EXISTING : OPEN_ALWAYS, 0, 0);
	if (h == INVALID_HANDLE_VALUE) {
		*file_handle = -1;
		return GetLastError();
	}
#else
	int f;
	f = open(file_path, read ? 0 : 0); /* todo */
	if (f < 0) {
		*file_handle = -1;
		return errno;
	}
#endif
	/* Windows handles are actually only 29 or 30 bits depending on how you count. */
	*file_handle = (int)(intptr_t)f;
	return 0;
}

int jos_open_file_read(const char* file_path, int* file_handle)
{
	return jos_open_file(file_path, 1, file_handle);
}

int jos_open_file_write(const char* file_path, int* file_handle)
{
	return jos_open_file(file_path, 0, file_handle);
}

int jos_close_file(int file_handle)
{
#if _WIN32
	CloseHandle((HANDLE)(intptr_t)file_handle);
#else
	close(file_handle);
#endif
}

int jos_set_file_size(int file_handle, int64_t file_size)
{
#if _WIN32
	LARGE_INTEGER li;
	DWORD result;
	if (jVistaOrNewer)
	{
		return SetFileInformationByHandle(file_handle, FileEndOfFileInfo, &file_size, sizeof(file_size)) ? 0 : GetLastError();
	}
	li.QuadPart = file_size - 1;
	result = SetFilePointer(file_handle, li.LowPart, &li.HighPart, 0);
	if (result == INVALID_SET_FILE_POINTER && GetLastError())
		return GetLastError();
	return SetEndOfFile(file_handle) ? 0 : GetLastError();
#else
	return ftruncate(file_handle, file_size) ? errno : 0;
#endif
}

int jos_get_file_size(int file_handle, int64_t* file_size)
{
#if _WIN32
	LARGE_INTEGER li;
	BY_HANDLE_FILE_INFORMATION info;
	JMEMSET0_VALUE(info);
	if (!GetFileInformationByHandle(file_handle, &info))
		return GetLastError();
	li.HighPart = info.nFileSizeHigh;
	li.LowPart = info.nFileSizeLow;
	*file_size = li.QuadPart;
	return 0;
#else
#endif
}

int jos_mmap(int file_handle, int64_t size, int read)
{
#if _WIN32
	HANDLE fileMappingObject;
	CreateFileMapping(file_handle, 0, 0);
#else
	mmap(0, file_handle, size);
#endif
}

int jos_mmap_read(int file_handle, int64_t size)
{
	return jos_mmap_read(file_handle, size, 1);
}

int jos_mmap_write(int file_handle, int64_t size)
{
	return jos_mmap_read(file_handle, size, 0);
}

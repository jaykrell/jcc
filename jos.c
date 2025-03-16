#include "jos.h"

#if _WIN32
#include "windows.h"
#else
#include "sys/types.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/mman.h"
#include <errno.h>
#endif

#pragma warning(disable:4100) /* unused parameter */

/* TODO: Test on old Windows, and possibly improve the code there. */
#if _WIN32
jbool jos_vista_or_newer = 1;
#endif

void jos_set_vista_or_newer(jbool value)
{
#if _WIN32
	jos_vista_or_newer = value;
#endif
}

int jos_open_file(const char* file_path, int read, int* file_handle)
/* open a file using relatively low level operating system functionality.
Implemented is provided for Win32 and Posix (missing environments include NT kernel and EFI).
File can be opened for read or write.
This is an internal function.
Public functions are: jos_open_file_read and jos_open_file_write.
*/
{
#if _WIN32
	HANDLE f = CreateFileA(file_path, GENERIC_READ | (read ? 0 : GENERIC_WRITE), FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 0, read ? OPEN_EXISTING : OPEN_ALWAYS, 0, 0);
	if (f == INVALID_HANDLE_VALUE) {
		*file_handle = -1;
		return GetLastError();
	}
#else
	int f = open(file_path, O_CLOEXEC
#ifdef O_CLOFORK
		| O_CLOFORK
#endif
		| (read ? O_RDONLY : (O_RDWR | O_CREAT)), 0);
	if (f < 0) {
		*file_handle = -1;
		return errno;
	}
#endif
	/* Windows handles are actually only 29 or 30 bits depending on how you count.
	Handles are interchangable between 32bit and 64 processes.
	The lower 2 bits are reserved for user
	kernel handles have upper bit set
	user mode handlers have upper bit clear
	*/
	*file_handle = (int)(intptr_t)f;
	return 0;
}

int jos_open_file_read(const char* file_path, int* file_handle)
/* open a file using relatively low level operating system functionality. For read.
Return the handle through an out parameter and return 0 for success, else error.
*/
{
	return jos_open_file(file_path, 1, file_handle);
}

int jos_open_file_write(const char* file_path, int* file_handle)
/* open a file using relatively low level operating system functionality. For write.
Return the handle through an out parameter and return 0 for success, else error.
*/
{
	return jos_open_file(file_path, 0, file_handle);
}

void jos_close_file(int file_handle)
/* Close a file.
*/
{
#if _WIN32
	HANDLE hfile = (HANDLE)(intptr_t)file_handle;
	CloseHandle(hfile);
#else
	close(file_handle);
#endif
}

int jos_set_file_size(int file_handle, int64_t file_size)
/* Set the size of an open file.
*/
{
#if _WIN32
	LARGE_INTEGER li;
	int err;
	HANDLE hfile = (HANDLE)(intptr_t)file_handle;
	if (jos_vista_or_newer)
	{
		return SetFileInformationByHandle(hfile, FileEndOfFileInfo, &file_size, sizeof(file_size)) ? 0 : GetLastError();
	}
	li.QuadPart = file_size - 1;
	li.LowPart = SetFilePointer(hfile, li.LowPart, &li.HighPart, 0);
	if (li.LowPart == INVALID_SET_FILE_POINTER)
	{
		err = GetLastError();
		if (err) return err;
	}
	return SetEndOfFile(hfile) ? 0 : GetLastError();
#else
	return ftruncate(file_handle, file_size) ? errno : 0;
#endif
}

int jos_get_file_size(int file_handle, int64_t* file_size)
/* Get the size of an open file.
*/
{
#if _WIN32
	LARGE_INTEGER li;
	BY_HANDLE_FILE_INFORMATION info={0};
	HANDLE hfile = (HANDLE)(intptr_t)file_handle;
	if (!GetFileInformationByHandle(hfile, &info))
		return GetLastError();
	li.HighPart = info.nFileSizeHigh;
	li.LowPart = info.nFileSizeLow;
	*file_size = li.QuadPart;
#else
	struct stat st={0};
	if (fstat(file_handle, &st))
		return errno;
	*file_size = st.st_size;
#endif
	return 0;
}

int jos_mmap(int file_handle, int64_t size, int read, void** q)
{
	void* p;
#if _WIN32
	int err = 0;
	HANDLE hfile = (HANDLE)(intptr_t)file_handle;
	LARGE_INTEGER li={0};;
	HANDLE fileMapping={0};;
	li.QuadPart = size;
	fileMapping = CreateFileMappingA(hfile, 0, read ? PAGE_READONLY : PAGE_READWRITE, li.HighPart, li.LowPart, 0);
	if (!fileMapping) return GetLastError();
	p = MapViewOfFile(fileMapping, FILE_MAP_READ | (read ? 0 : FILE_MAP_WRITE), 0, 0, size);
	if (p) *q = p;
	else err = GetLastError();
	CloseHandle(fileMapping);
	return err;
#else
	p = mmap(0/*addr*/, file_handle, size, PROT_READ | (read ? 0 : PROT_WRITE), read ? 0 : MAP_SHARED, 0/*offset*/);
	if (p == MAP_FAILED)
		return errno;
	*q = p;
#endif
	return 0;
}

//int jos_read(int file_handle, void* buffer, size_t bytes, size_t *actual);
//int jos_write(int file_handle, void* buffer, size_t bytes, size_t *actual);

int jos_mmap_read(int file_handle, int64_t size, void** q)
{
	return jos_mmap(file_handle, size, 1, q);
}

int jos_mmap_write(int file_handle, int64_t size, void** q)
{
	return jos_mmap(file_handle, size, 0, q);
}

int jos_munmap(void* p, size_t size)
{
	if (!p) return 0;
#if _WIN32
	UNREFERENCED_PARAMETER(size);
	if (!UnmapViewOfFile(p))
		return GetLastError();
#else
	if (!munmap(p, size)) return errno;
#endif
	return 0;
}

#include "jos.h"

int jos_delete_file(const char* file_path)
{
}

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
#else
#endif
}

int jos_get_file_size(int file_handle, int64_t*)
{
#if _WIN32
#else
#endif
}

int jos_mmap_read(int file_handle, int64_t size)
{
#if _WIN32
#else
#endif
}

int jos_mmap_write(int file_handle, int64_t size)
{
#if _WIN32
#else
#endif
}

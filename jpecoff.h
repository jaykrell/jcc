#ifndef JPECOFF
#define JPECOFF

/*
pecoff_dos_header
pecoff_nt_headers32
pecoff_nt_headers64
pecoff_file_header
pecoff_optional_header32
pecoff_optional_header64
pecoff_data_directory
*/

typedef struct jpecoff_dos_header_t jpecoff_dos_header_t;
typedef struct jpecoff_nt_headers32_t jpecoff_nt_headers32v
typedef struct jpecoff_nt_headers64_t jpecoff_nt_headers64_t;
typedef struct jpecoff_file_header_t jpecoff_file_header_t;
typedef struct jpecoff_optional_header32_t jpecoff_optional_header32_t;
typedef struct jpecoff_optional_header64_t jpecoff_optional_header64_t;
typedef struct jpecoff_data_directory_t jpecoff_data_directory_t;
typedef struct jpecoff_section_header_t jpecoff_section_header_t;

#define JPECOFF_FILE_TYPE_OBJ 1
#define JPECOFF_FILE_TYPE_EXE 2 /* or dll */

/* obj or exe */
typedef struct jpecoff_file_t jpecoff_file_t;
typedef struct jpecoff_lib_t jpecoff_lib_t;
typedef struct jpecoff_section_t jpecoff_section_t;

extern char jpecoff_dos_signature[2];
extern char jpecoff_pe_signature[4];
extern char jpecoff_pe_signature;

typedef struct jepecoff_payload_t {
	char* bytes;
	int   size;


} jepecoff_payload_t;

typedef struct jepecoff_section_t {
	const char* name;
	int ordinal;
	int no_name : 1;
	jpecoff_section_t* section;
	int rva;
} jepecoff_section_t;

typedef struct jepecoff_export_t {
	const char* name;
	int ordinal;
	int no_name : 1;
	jpecoff_section_t* section;
	int rva;
} jepecoff_export_t;

int jepecoff_dos_header_get_serialized_size();
int jepecoff_dos_header_serialized();

int jepecoff_lib_new(jpecoff_lib_t** self);
int jepecoff_file_new(jpecoff_file_t** self, int type);

int jepecoff_file_section_new(jepecoff_file_t*, jpecoff_section_t**);
int jepecoff_finish();

#endif

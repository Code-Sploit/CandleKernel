#ifndef STDLIB_H
#define STDLIB_H

#define NULL 0

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef signed char sint8;
typedef signed short sint16;
typedef signed int sint32;

typedef unsigned long long uint64;
typedef long long int64;

typedef uint32 size_t;
typedef uint32 fd_t;

typedef uint16 word;
typedef uint32 dword;
typedef int64 qword;

typedef uint8 byte;

typedef enum {
    FALSE,
    TRUE
} BOOL;

int kstd_isupper(char __c);
int kstd_islower(char __c);

char kstd_toupper(char __c);
char kstd_tolower(char __c);

int kstd_cmatch(char __c1, char __c2);

int __kstd_strcmp(char __s1[], char __s2[]);

int __kstd_strlen(const char *__sptr);

void __kstd_append_char_to_string(char *__sptr, char c);

BOOL __kstd_is_backspace_valid(char *__sptr);

#endif

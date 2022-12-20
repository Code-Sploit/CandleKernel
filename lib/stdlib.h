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

typedef struct memory_block {
    struct {
        uint32 size;  // memory size
        uint8 is_free; // block is free or not
    } meta;

    struct memory_block *__next;

    void *__data;  // data pointer
} __attribute__((packed)) MEMORY_BLOCK;

/* Stdlib memory function */

BOOL kstd_mem_isfree(MEMORY_BLOCK *__BLOCK);

MEMORY_BLOCK *kstd_mem_worst(int __SIZE);

int kstd_mem_init(void *__START_ADDR, void *__END_ADDR);

void *kstd_mem_brk(int __SIZE);
void *kstd_mem_malloc(int __SIZE);
void *kstd_mem_calloc(int __N, int __SIZE);
void *kstd_mem_realloc(void *__PTR, int __SIZE);

void kstd_mem_free(void *__ADDR);

#define SCREEN_SIZE_COLS 80
#define SCREEN_SIZE_ROWS 25
#define SCREEN_SIZE_BLEN 2

#define SCREEN_SIZE (SCREEN_SIZE_COLS * SCREEN_SIZE_ROWS * SCREEN_SIZE_BLEN)

#define ATTR_BYTE_LG_ON_BLK 0x07

static char *VIDEO_MEMORY_POINTER = (char*)0xb8000;

void kstd_write(const char *__sptr);

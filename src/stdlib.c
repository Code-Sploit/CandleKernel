#include "../lib/stdlib.h"
#include "../lib/panic.h"

/* Memory definitions */

/* Conversion functions */
void kstd_itoa(char *__BUF, int __BASE, int __D)
{
    char *__p = __BUF;
    char *__p1, *__p2;

    unsigned long __u = __D;
    
	int __DIV = 10;

    if (__BASE == 'd' && __D < 0) {
        *__p++ = '-';
        __BUF++;
        __u = -__D;
    } else if (__BASE == 'x')
        __DIV = 16;

    do {
        int __REMAIN = __u % __DIV;
        *__p++ = (__REMAIN < 10) ? __REMAIN + '0' : __REMAIN + 'a' - 10;
    } while (__u /= __DIV);

    /* Terminate BUF. */
    *__p = 0;

    /* Reverse BUF. */
    __p1 = __BUF;
    __p2 = __p - 1;

    while (__p1 < __p2) {
        char __TEMP = *__p1;

        *__p1 = *__p2;
        *__p2 = __TEMP;

        __p1++;
        __p2--;
    }
}

/* Assert string functions */

int kstd_isupper(char __c)
{
	if (__c >= 'A' && __c <= 'Z')
	{
		return TRUE;
	}

	return FALSE;
}

int kstd_islower(char __c)
{
	if (__c >= 'a' && __c <= 'z')
	{
		return TRUE;
	}

	return FALSE;
}

char kstd_toupper(char __c)
{
	if (__c >= 'a' && __c <= 'z')
	{
		return (__c - 32);
	}

	return __c;
}

char kstd_tolower(char __c)
{
	if (__c >= 'A' && __c <= 'Z')
	{
		return (__c + 32);
	}

	return __c;
}

int kstd_cmatch(char __c1, char __c2)
{
	if (__c1 == __c2)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

int __kstd_strcmp(char __s1[], char __s2[])
{
    int i;

    for (i = 0; __s1[i] == __s2[i]; i++) {
        if (__s1[i] == '\0') return 0;
    }
    
	return __s1[i] - __s2[i];
}

int __kstd_strlen(const char *__sptr)
{
	int len = 0;

	while (__sptr[len] != '\0')
	{
		len++;
	}

	return len;
}

void __kstd_append_char_to_string(char *__sptr, char c)
{
	int __slen = __kstd_strlen(__sptr);

	__sptr[__slen] = c;
	__sptr[__slen + 1] = '\0';
}

BOOL __kstd_is_backspace_valid(char *__sptr)
{
    int __slen = __kstd_strlen(__sptr);

    if (__slen > 0)
	{
        __sptr[__slen - 1] = '\0';

        return TRUE;
    }
	else
	{
        return FALSE;
    }

	return FALSE;
}

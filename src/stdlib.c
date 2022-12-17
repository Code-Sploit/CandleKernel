int kstd_strlen(const char *__sptr)
{
	unsigned int __len = 0;

	while (__sptr[__len] != '\0')
	{
		__len++;
	}

	return __len;
}

#define tolower(c) ( (c) >= 'A' && (c) <= 'Z' ? (c) - ('A' - 'a') : (c) )

char *strcasechr(char *str, char c)
{
	while (*str) {
		if (tolower(*str) == tolower(c))
			return str;

		str++;
	}

	return (void *)0;
}

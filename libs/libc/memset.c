#include <string.h>

void *memset(void *s, int c, size_t n)
{
	char *p = s;

	for (size_t i = 0; i < n; ++i)
		p[i] = c;

	return s;
}


void myMemSet(void* str, char ch, size_t n){
	int i;
	//type cast the str from void* to char*
	char *s = (char*) str;
	//fill "n" elements/blocks with ch
	for(i=0; i<n; i++)
		s[i]=ch;
}
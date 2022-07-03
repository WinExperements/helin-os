#include<mstring.h>
void memcpy(void *vd, const void *vs, unsigned length) {
	char *d = vd;
	const char *s = vs;
	while(length) {
		*d = *s;
		d++;
		s++;
		length--;
	}
}

#include <stdlib.h>
#include "mi_ctype8.h"
#include <string.h>
#include "html.h"

void html_parse(int (*getch)(void *), unsigned char *iv,
		void (*putword)(void *, unsigned char *, int), void *ov) {
	unsigned char Xbuffer[128];
	unsigned char *buffer = Xbuffer;
	int m = sizeof(Xbuffer) / sizeof(Xbuffer[0]);
	int len = 1;
	int c = iv[0];
	while (c!='\0') {
		if (mi_isspace8 (c)) {
			do {
				c = iv[len++];
			} while (mi_isspace8 (c));
		} else if (c == '<') {
			while (c != '>' && c != -1) {
				c = iv[len++];
			}
			if (c == '>') {
				c = iv[len++];
			}
		} else if (c == '&') {
			while (c != ';' && c != -1) {
				c = iv[len++];
			}
			if (c == ';') {
				c = iv[len++];
			}
		} else if (mi_isalnum8 (c)) {
			int i = 0;
			do {
				if (i + 1 == m) {
					m *= 2;
					if (buffer == Xbuffer) {
						buffer = (unsigned char *) (malloc(m));
						memcpy(buffer, Xbuffer, i);
					} else {
						buffer = (unsigned char *) realloc(buffer, m);
					}
				}
				buffer[i++] = c;
				c = iv[len++];
				if (c == '<') {
					unsigned char sbuf[10];
					unsigned char *s = sbuf;
					int j = 0;
					int intagname = 1;
					while ((c = iv[len++]) != -1 && c != '>') {
						if (j != (sizeof(sbuf) / sizeof(sbuf[0])) - 1 &&
						mi_isalnum8 (c) && intagname) {
							sbuf[j++] = mi_tolower8 (c);
						} else {
							intagname = 0;
						}
					}
					sbuf[j] = 0;
					if (s[0] == '/') {
						s++;
					}
					c = iv[len++];
					if (strlen((const char*) s) == 1
							&& strchr("abisu", s[0]) != 0) {
					} else if (strcmp((const char*) s, "basefont") == 0) {
					} else if (strcmp((const char*) s, "big") == 0) {
					} else if (strcmp((const char*) s, "blink") == 0) {
					} else if (strcmp((const char*) s, "cite") == 0) {
					} else if (strcmp((const char*) s, "code") == 0) {
					} else if (strcmp((const char*) s, "em") == 0) {
					} else if (strcmp((const char*) s, "font") == 0) {
					} else if (strcmp((const char*) s, "kbd") == 0) {
					} else if (strcmp((const char*) s, "plaintext") == 0) {
					} else if (strcmp((const char*) s, "small") == 0) {
					} else if (strcmp((const char*) s, "strike") == 0) {
					} else if (strcmp((const char*) s, "strong") == 0) {
					} else if (strcmp((const char*) s, "sub") == 0) {
					} else if (strcmp((const char*) s, "sup") == 0) {
					} else if (strcmp((const char*) s, "tt") == 0) {
					} else if (strcmp((const char*) s, "var") == 0) {
					} else {
						break;
					}
				}
			} while (mi_isalnum8 (c));
			buffer[i] = 0;
			(*putword)(ov, buffer, i);
		} else {
			c = iv[len++];
		}
	}
	if (buffer != Xbuffer) {
		free(buffer);
	}
}

#define  HTML_TEST 1
#if defined(HTML_TEST)
#include <stdio.h>

/*int getch (void *iv) {
 return (getc ((FILE *)iv));
 }*/

int getch(unsigned char* iv) {
	return (iv[1]);
}
void putword(void *ov, unsigned char *buffer, int len) {
	printf("--%s--%d--\n", buffer, len);
}

/*
 int main (int argc, char *argv[]) {
 html_parse (getch, stdin, putword, stdout);
 return (0);
 }
 */
#endif

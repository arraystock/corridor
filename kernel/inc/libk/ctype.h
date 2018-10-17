#ifndef CTYPE_H
#define CTYPE_H

int isalnum(int c);
int isalpha(int c);
#define isascii(c)	((c) < 128)
int isblank(int c);
int isdigit(int c);
int isupper(int c);
int islower(int c);
int isprint(int c);
int isspace(int c);

int toupper(int c);
int tolower(int c);

#endif	

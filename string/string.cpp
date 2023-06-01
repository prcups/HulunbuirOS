#include <types.h>

void*
memset(void *dst, int c, uint n)
{
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}

int
memcmp(const void *v1, const void *v2, uint n)
{
  const uchar *s1, *s2;

  s1 = (const unsigned char*)v1;
  s2 = (const unsigned char*)v2;
  while(n-- > 0){
    if(*s1 != *s2)
      return *s1 - *s2;
    s1++, s2++;
  }

  return 0;
}

void*
memmove(void *dst, const void *src, uint n)
{
  const char *s;
  char *d;

  if(n == 0)
    return dst;
  
  s = (const char*)src;
  d = (char *)dst;
  if(s < d && s + n > d){
    s += n;
    d += n;
    while(n-- > 0)
      *--d = *--s;
  } else
    while(n-- > 0)
      *d++ = *s++;

  return dst;
}

// memcpy exists to placate GCC.  Use memmove.
void*
memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
}

int
strncmp(const char *p, const char *q, uint n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}

char*
strncpy(char *s, const char *t, int n)
{
  char *os;

  os = s;
  while(n-- > 0 && (*s++ = *t++) != 0)
    ;
  while(n-- > 0)
    *s++ = 0;
  return os;
}

// Like strncpy but guaranteed to NUL-terminate.
char*
safestrcpy(char *s, const char *t, int n)
{
  char *os;

  os = s;
  if(n <= 0)
    return os;
  while(--n > 0 && (*s++ = *t++) != 0)
    ;
  *s = 0;
  return os;
}

int
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

char* my_strchr(const char* str, char ch) {
    while (*str != '\0') {
        if (*str == ch) {
            return (char*)str;
        }
        str++;
    }
    if (*str == ch) {
        return (char*)str;
    }
    return (char *)0;
}

char toupper(char c) {
  /* 判断是否为小写字母，如果是，转换成大写字母 */
  if(c >= 'a' && c <= 'z') {
    return c - 'a' + 'A';
  }
  /* 如果不是小写字母，则原样返回 */
  return c;
}

int islower(int c) {
  /* 如果是小写字母，返回非0值，否则返回0 */
  if(c >= 'a' && c <= 'z') {
    return 1;
  }
  return 0;
}
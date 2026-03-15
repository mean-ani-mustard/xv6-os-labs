#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define PGSIZE 4096
#define NPAGES 32
#define SECRET_LEN 8

int
isAlphaNum(char c)
{
  if(c >= 'a' && c <= 'z')
    return 1;
  if(c >= 'A' && c <= 'Z')
    return 1;
  if(c >= '0' && c <= '9')
    return 1;
  return 0;
}

int
main(void)
{
  char *mem;
  int i, j;
  int best = -1;

  mem = sbrk(NPAGES * PGSIZE);
  if(mem == (char *)-1)
    exit(1);

  for(i = 0; i + SECRET_LEN <= NPAGES * PGSIZE; i++){
    if(i > 0 && isAlphaNum(mem[i - 1]))
      continue;

    for(j = 0; j < SECRET_LEN; j++){
      if(!isAlphaNum(mem[i + j]))
        break;
    }

    if(j != SECRET_LEN)
      continue;

    if(i + SECRET_LEN < NPAGES * PGSIZE && isAlphaNum(mem[i + SECRET_LEN]))
      continue;

    best = i;
  }

  if(best >= 0){
    write(1, mem + best, SECRET_LEN);
    write(1, "\n", 1);
  }

  exit(0);
}
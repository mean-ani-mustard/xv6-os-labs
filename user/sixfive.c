#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char *separators = " -\r\t\n./,";

void
readFile(int fd)
{
  char currChar;
  int currNum = 0;
  int startedWord = 0;
  int onlyDigits = 1;

  while(read(fd, &currChar, 1) > 0){
    if(strchr(separators, currChar)){
      if(startedWord && onlyDigits && (currNum % 5 == 0 || currNum % 6 == 0)){
        printf("%d\n", currNum);
      }

      currNum = 0;
      startedWord = 0;
      onlyDigits = 1;
    } else {
      startedWord = 1;
      if(currChar >= '0' && currChar <= '9'){
        if(onlyDigits)
          currNum = currNum * 10 + (currChar - '0');
      } else {
        onlyDigits = 0;
      }
    }
  }

  if(startedWord && onlyDigits && (currNum % 5 == 0 || currNum % 6 == 0))
    printf("%d\n", currNum);
}

int
main(int argc, char *argv[])
{
  int i; int fd;

  if(argc < 2){
    fprintf(2, "usage: sixfive file...\n");
    exit(1);
  }
  for(i = 1; i < argc; i++){
    fd = open(argv[i], O_RDONLY);
    if(fd < 0){
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      continue;
    }
    readFile(fd);
    close(fd);
  }
  exit(0);
}
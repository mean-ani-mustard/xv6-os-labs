#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data);

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int a[2] = { 61810, 2025 };
    memdump("ii", (char*) a);
    
    printf("Example 2:\n");
    memdump("S", "a string");
    
    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *) &s);

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;
    
    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");
    
    printf("Example 4:\n");
    memdump("pihcS", (char*) &example);
    
    printf("Example 5:\n");
    memdump("sccccc", (char*) &example);
  } else if(argc == 2){
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while(n < sizeof(data)){
      int nn = read(0, data + n, sizeof(data) - n);
      if(nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
printHexNum(uint64 num)
{
  char hexDigits[] = "0123456789abcdef";
  char buf[16];
  int i = 0;
  if(num == 0){
    printf("0\n");
    return;
  }
  while(num > 0){
    buf[i] = hexDigits[num % 16];
    num /= 16;
    i++;
  }
  while(i > 0){
    i--;
    printf("%c", buf[i]);
  }
  printf("\n");
}

void printInt(char **data)
{
  int num;
  memmove(&num, *data, sizeof(num));
  printf("%d\n", num);
  *data += sizeof(num);
}

void printPtr(char **data)
{
  uint64 addr;
  memmove(&addr, *data, sizeof(addr));
  printHexNum((uint)addr);
  *data += sizeof(addr);
}

void printShort(char **data)
{
  ushort num;
  memmove(&num, *data, sizeof(num));
  printf("%d\n", num);
  *data += sizeof(num);
}

void printChar(char **data)
{
  printf("%c\n", **data);
  (*data)++;
}

void printStringPtr(char **data)
{
  uint64 strAddr;
  memmove(&strAddr, *data, sizeof(strAddr));
  printf("%s\n", (char *)strAddr);
  *data += sizeof(strAddr);
}

void printString(char **data)
{
  printf("%s\n", *data);
  while(**data != '\0')
    (*data)++;
  (*data)++;
}

void memdump(char *fmt, char *data)
{

  for(int i = 0; fmt[i] != '\0'; i++){
    if(fmt[i] == 'i')
      printInt(&data);
    else if(fmt[i] == 'p')
      printPtr(&data);
    else if(fmt[i] == 'h')
      printShort(&data);
    else if(fmt[i] == 'c')
      printChar(&data);
    else if(fmt[i] == 's')
      printStringPtr(&data);
    else if(fmt[i] == 'S')
      printString(&data);
  }
}

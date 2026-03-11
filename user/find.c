#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/param.h"
#include "user/user.h"

char* getName(char *path) {
  char *last;
  int i;

  last = path;
  for(i = 0; path[i] != '\0'; i++){
    if(path[i] == '/')
      last = path + i + 1;
  }
  return last;
}

int isDotOrDotDot(char *name) {
  return strcmp(name, ".") == 0 || strcmp(name, "..") == 0;
}

void runExec(char *filePath, char **cmd, int cmdCount) {
  char *newArgv[MAXARG];
  int i;
  int pid;

  if(cmdCount + 2 > MAXARG){
    fprintf(2, "find: too many exec arguments\n");
    return;
  }

  for(i = 0; i < cmdCount; i++) {
    newArgv[i] = cmd[i];
  }

  newArgv[cmdCount] = filePath;
  newArgv[cmdCount + 1] = 0;

  pid = fork();
  if(pid < 0){
    fprintf(2, "find: fork failed\n");
    return;
  }

  if(pid == 0){
    exec(newArgv[0], newArgv);
    fprintf(2, "find: exec %s failed\n", newArgv[0]);
    exit(1);
  }

  wait(0);
}

void printOrExec(char *path, char **cmd, int cmdCount) {
  if(cmdCount > 0){
    runExec(path, cmd, cmdCount);
  } else {
    printf("%s\n", path);
  } 
}

void findFiles(char *path, char *target, char **cmd, int cmdCount) {
  int fd;
  struct stat st;
  struct dirent de;
  char buf[512];
  char *p;

  fd = open(path, O_RDONLY);
  if(fd < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if(strcmp(getName(path), target) == 0) {
    printOrExec(path, cmd, cmdCount);
  }

  if(st.type != T_DIR){
    close(fd);
    return;
  }

  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
    fprintf(2, "find: path too long\n");
    close(fd);
    return;
  }

  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';

  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0) continue;

    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = '\0';

    if(isDotOrDotDot(p)) continue;

    findFiles(buf, target, cmd, cmdCount);
  }

  close(fd);
}

int main(int argc, char *argv[])
{
  char **cmd;
  int cmdCount;

  cmd = 0;
  cmdCount = 0;

  if(argc < 3){
    fprintf(2, "usage: find path name [-exec cmd ...]\n");
    exit(1);
  }

  if(argc > 3){
    if(strcmp(argv[3], "-exec") != 0){
      fprintf(2, "usage: find path name [-exec cmd ...]\n");
      exit(1);
    }
    if(argc < 5){
      fprintf(2, "find: missing command after -exec\n");
      exit(1);
    }

    cmd = &argv[4];
    cmdCount = argc - 4;
  }

  findFiles(argv[1], argv[2], cmd, cmdCount);
  exit(0);
}
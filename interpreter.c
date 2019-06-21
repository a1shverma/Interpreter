#include <stdio.h>
#include <stdlib.h>
#define MAXARRSIZE 20000
#define MAXCODESIZE 20000

int stack[MAXCODESIZE], sp; // stack to save `[` and sp is stack pointer, one ahead of tos
int codeptr, codelen; // pointer to tape and length of input in tape
short int array[MAXARRSIZE], memptr; //mem used by bf
int sqlocs[MAXCODESIZE];
int c;
char code[MAXCODESIZE]; //save data from file to local string
FILE *bfprogrammeFP; // read bf file

int main(int argc, char **argv){
  // if parameter is not mentioned, or more than one parameter is specified

  // if(argc != 2){
  //   printf("Exactly one .bf file should be added as the parameter.\n");
  //   return 0;
  // } /* dumb code dumped the code*/
  if(argc > 2){
    printf("Pass only one file.\n");
    return 0;
  }
  if(argc < 2){
    printf("Pass a file as argument.\n");
    return 0;
  }

  //failed to open in read mode
  if(!(bfprogrammeFP = fopen(argv[1], "r"))){
    printf("Failed to open %s.\n", argv[1]);
    return 0;
  }

  //from `man fread`  size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
  /*
    The function fread() reads `nmemb` items of data, each `size` bytes long,
    from the stream pointed to by `stream`, storing them at the location given by `ptr`.
  */
  /*
    On success, fread() return the number of items read. This number equals the
    number of bytes transferred only when size is 1. If an error occurs, or the
    end of the file is reached, the return value is a short item count (or zero).
    fread() does not distinguish between end-of-file and error, and callers must
     use feof(3) and ferror(3) to determine which occurred.
  */

  codelen = fread(code, 1, MAXCODESIZE, bfprogrammeFP); // copy file content to `code` string
  fclose(bfprogrammeFP);

  for(codeptr=0; codeptr < codelen; codeptr++){
    if(code[codeptr] == '[')
      stack[sp++]=codeptr; //push locations of '[' on stack
    if(code[codeptr] == ']'){
      if(sp == 0){
        printf("Errornous ']' at %d.", codeptr);
        return 0;
      }
      else {
        --sp; //if there is one, we take the matching '[' from the stack top,
        sqlocs[codeptr] = stack[sp]; //save it as the match for the current ']',
        sqlocs[stack[sp]] = codeptr; //and save the current ']' as the match for it.
      }
    }
  }
  if(sp > 0){ // `[` more than `]`
    printf("Errornous '[' at %d.", stack[--sp]);
    return 0;
  }
  for(codeptr=0;codeptr<codelen;codeptr++){
   switch(code[codeptr]){
    case '+': array[memptr]++;
              break;
    case '-': array[memptr]--;
              break;
    case '<': if(memptr != 0){
                memptr--;
              } else {
                  printf("\nError: Went to negative index.\n");
                  return 0;
              }
              break;
    case '>': if(memptr < MAXCODESIZE - 1){
                memptr++;
              } else {
                  printf("\nError: Went out of bound.\n");
                  return 0;
              }
              break;
    case ',': if((c=getchar())!=EOF)
              array[memptr]=c=='\n' ? 10 : c;
              break;
    case '.': putchar(array[memptr]==10 ? '\n' : array[memptr]);
              break;
    case '[': if(!array[memptr])
                codeptr=sqlocs[codeptr]; // skips while loop
              break;
    case ']': if(array[memptr])
                codeptr=sqlocs[codeptr];
              break;
    }
  }
  return 0;
}

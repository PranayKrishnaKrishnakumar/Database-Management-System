#include <stdlib.h>
#include <string.h>

#include "storage_mgr.h"
#include "mgmt_info.h"

#define PAGE_NUMBER_2_BYTE(X) X * PAGE_SIZE

#define FILE_PERMISSIONS "r+"

// Internal function for calculating the size of the file to determine total pages.
int __getFileSize(FILE *pageFilePointer){
  int fileSize;
  fseek(pageFilePointer, 0L, SEEK_END);
  fileSize = ftell(pageFilePointer);
  fseek(pageFilePointer, 0L, SEEK_SET);

  // printf("---> fileSize : %d\n", fileSize);

  return fileSize;
}

extern void initStorageManager (void){
  // Couldn't think of anything to do here yet.
  return;
}

extern RC createPageFile (char *fileName){
  FILE *pageFile;
  pageFile = fopen (fileName, "wb");

  if(!pageFile){
    RC_message = "Unable to create a page file.";
    printError(RC_FILE_NOT_FOUND);
    return RC_FILE_NOT_FOUND;
  }

  fclose(pageFile);

  return RC_OK;
}

extern RC openPageFile (char *fileName, SM_FileHandle *fHandle){
  // if(access(fileName, F_OK)) {
  //   RC_message = "File does not exists.";
  //   printError(RC_FILE_NOT_FOUND);
  //   return RC_FILE_NOT_FOUND;
  // }

  FILE *pageFilePointer;
  pageFilePointer = fopen(fileName, FILE_PERMISSIONS);

  if(!pageFilePointer)
    return RC_FILE_HANDLE_NOT_INIT;

  mgmtInfo* mi = (mgmtInfo*)malloc(sizeof(mgmtInfo));

  fHandle->fileName = fileName;
  int numberOfPages = __getFileSize(pageFilePointer) / PAGE_SIZE;
  fHandle->totalNumPages = numberOfPages == 0 ? 1 : numberOfPages;
  fHandle->curPagePos = 0;
  fHandle->mgmtInfo = mi;

  int a = 0;
  for(int i = 0; i < PAGE_SIZE; i++){
    fwrite(&a, 1, 1, pageFilePointer); // Writing 0 to the first page block as required
  }

  ((mgmtInfo*)(fHandle->mgmtInfo))->fp = pageFilePointer;

  return RC_OK;
}

extern RC closePageFile (SM_FileHandle *fHandle){
  if(fclose(((mgmtInfo*)(fHandle->mgmtInfo))->fp)){
    RC_message = "Unable to close the page file.";
    printError(RC_FILE_NOT_FOUND);
    return RC_FILE_NOT_FOUND;
  }

  // free memory for allocated objects
  free(fHandle->mgmtInfo);

  return RC_OK;
}

extern RC destroyPageFile (char *fileName){
  if(remove(fileName)){
    RC_message = "Unable to destroy page file.";
    printError(RC_FILE_NOT_FOUND);
    return RC_FILE_NOT_FOUND;
  }

  return RC_OK;
}

extern RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(pageNum), SEEK_SET);

  for(int i = 0; i < PAGE_SIZE; i++){
    fread(memPage + i, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  }

  fHandle->curPagePos++;

  return RC_OK;
}

extern int getBlockPos (SM_FileHandle *fHandle){
  return fHandle->curPagePos;
}

extern RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  fHandle->curPagePos = 0;
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, 0, SEEK_SET);

  for(int i = 0; i < PAGE_SIZE; i++)
    fread(memPage + i, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);

  return RC_OK;
}

extern RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  // cannot read before the first page
  if(fHandle->curPagePos == 0){
    return RC_READ_NON_EXISTING_PAGE;
  }

  fHandle->curPagePos--;
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(fHandle->curPagePos), SEEK_SET);

  for(int i = 0; i < PAGE_SIZE; i++){
    fread(memPage + i, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  }

  return RC_OK;
}

extern RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(fHandle->curPagePos), SEEK_SET);

  for(int i = 0; i < PAGE_SIZE; i++){
    fread(memPage + i, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  }

  return RC_OK;
}

extern RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  // cannot read beyond the last page
  if(fHandle->curPagePos >= fHandle->totalNumPages - 1){
    return RC_READ_NON_EXISTING_PAGE;
  }

  fHandle->curPagePos++;
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(fHandle->curPagePos + 1), SEEK_SET);

  for(int i = 0; i < PAGE_SIZE; i++){
    fread(memPage + i, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  }

  return RC_OK;
}

extern RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  fHandle->curPagePos = fHandle->totalNumPages - 1;
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(fHandle->curPagePos), SEEK_SET);

  for(int i = 0; i < PAGE_SIZE; i++){
    fread(memPage + i, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  }

  return RC_OK;
}

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage){
  if(pageNum >= fHandle->totalNumPages)
    return RC_READ_NON_EXISTING_PAGE;

  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(pageNum), SEEK_SET);
  // fwrite(memPage, 0, PAGE_SIZE, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  fwrite(memPage, PAGE_SIZE, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  __getFileSize(((mgmtInfo*)(fHandle->mgmtInfo))->fp);
// printf("--------> writeBlock:fHandle->curPagePos %d\n", fHandle->curPagePos);
  // for(int i = 0; i < PAGE_SIZE; i++){
  //   fwrite(memPage + i, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  // }
  __getFileSize(((mgmtInfo*)(fHandle->mgmtInfo))->fp);

  return RC_OK;
}

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage){
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(fHandle->curPagePos), SEEK_SET);
  // fwrite(memPage, 0, PAGE_SIZE, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);
  fwrite(memPage, PAGE_SIZE, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp);

  return RC_OK;
}

extern RC appendEmptyBlock (SM_FileHandle *fHandle){
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(fHandle->totalNumPages), SEEK_SET);
  int a = 0;
  for(int i = 0; i < PAGE_SIZE; i++){
    fwrite(&a, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp); // Writing 0 to the first page block as required
  }

  fHandle->totalNumPages++;

  return RC_OK;
}

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle){
  fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, PAGE_NUMBER_2_BYTE(fHandle->totalNumPages - 1), SEEK_SET);

  int a = 0;
  for(int i = fHandle->totalNumPages; i < numberOfPages; i++){
    fseek(((mgmtInfo*)(fHandle->mgmtInfo))->fp, i, SEEK_SET);
    for(int j = 0; j < PAGE_SIZE; j++){
      fwrite(&a, 1, 1, ((mgmtInfo*)(fHandle->mgmtInfo))->fp); // Writing 0 to the first page block as required
    }
  }

  fHandle->totalNumPages = numberOfPages;

  return RC_OK;
}
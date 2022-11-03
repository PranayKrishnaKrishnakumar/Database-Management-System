#include "storage_mgr.h"
#include "buffer_mgr.h"

// RC _pin_page_RS_FIFO(BM_BufferPool *const, BM_PageHandle *const, const PageNumber){
//   printf("pin called FIFO\n");
// }
// RC _pin_page_RS_LRU(BM_BufferPool *const, BM_PageHandle *const, const PageNumber){
//   printf("pin called LRU\n");
// }
// RC _pin_page_RS_CLOCK(BM_BufferPool *const, BM_PageHandle *const, const PageNumber){
//   printf("pin called CLOCK\n");
// }
// RC _pin_page_RS_LFU(BM_BufferPool *const, BM_PageHandle *const, const PageNumber){
//   printf("pin called LFU\n");
// }
// RC _pin_page_RS_LRU_K(BM_BufferPool *const, BM_PageHandle *const, const PageNumber){
//   printf("pin called LRU K\n");
// }

// RC (*pinPageReplacementStrategy[5])(BM_BufferPool *const, BM_PageHandle *const, const PageNumber) = {
//   [RS_FIFO] = _pin_page_RS_FIFO,
//   [RS_LRU] = _pin_page_RS_LRU,
//   [RS_CLOCK] = _pin_page_RS_CLOCK,
//   [RS_LFU] = _pin_page_RS_LFU,
//   [RS_LRU_K] = _pin_page_RS_LRU_K,
// }


// Creat a page file using the storage manager and initialize the buffer pool object
RC initBufferPool(BM_BufferPool *const bm, const char *const pageFileName,
    const int numPages, ReplacementStrategy strategy,
    void *stratData){

  // SM_FileHandle fh;
  // openPageFile(pageFileName, &fh)

  bm->pageFile = pageFileName;
  bm->numPages = numPages;
  bm->strategy = strategy;

  SM_FileHandle *fHandle = (SM_FileHandle*)malloc(sizeof(SM_FileHandle)); // creating a page file

  openPageFile (pageFileName, fHandle);

  bm->mgmtData = (void*)fHandle; // adding the handle to the page to mgmt data to use later
}

RC pinPage (BM_BufferPool *const bm, BM_PageHandle *const page,
    const PageNumber pageNum){

  if(!bm){
    printf("Buffer Pool not initialized\n");
    return RC_FILE_NOT_FOUND;
  }

  // RC _rc = pinPageReplacementStrategy[bm->strategy(bm, page, pageNum)];

  return 0;

}


RC shutdownBufferPool(BM_BufferPool *const bm){
  return 0;
}

RC forceFlushPool(BM_BufferPool *const bm){
  return 0;
}

RC markDirty (BM_BufferPool *const bm, BM_PageHandle *const page){
  return 0;
}

RC unpinPage (BM_BufferPool *const bm, BM_PageHandle *const page){
  return 0;
}

RC forcePage (BM_BufferPool *const bm, BM_PageHandle *const page){
  return 0;
}


// Statistics Interface
PageNumber *getFrameContents (BM_BufferPool *const bm){
  printf("foo\n");
  return NULL;
}

bool *getDirtyFlags (BM_BufferPool *const bm){
  printf("foo\n");
  return true;
}

int *getFixCounts (BM_BufferPool *const bm){
  printf("foo\n");
  return NULL;
}

int getNumReadIO (BM_BufferPool *const bm){
  printf("foo\n");
  return 0;
}

int getNumWriteIO (BM_BufferPool *const bm){
  printf("foo\n");
  return 0;
}

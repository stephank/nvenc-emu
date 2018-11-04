#include <stdio.h>
#include "cuda.h"
#include "util.h"

// FIXME: This simply stubs the subset of calls Steam looks for. Some are
// unimplemented / always return a generic error, because they haven't been
// seen yet. (Only referenced through `dlsym()`.)

CUresult CUDAAPI cuInit(
  unsigned int Flags
) {
  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(%d) -> %d", Flags, ret);
  return ret;
}

CUresult CUDAAPI cuDeviceGetCount(
  int* count
) {
  *count = 1;

  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(_) -> %d [%d]", ret, *count);
  return ret;
}

CUresult CUDAAPI cuDeviceGet(
  CUdevice* device,
  int ordinal
) {
  *device = 0;

  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(_, %d) -> %d [%d]", ordinal, ret, *device);
  return ret;
}

CUresult CUDAAPI cuDeviceComputeCapability(
  int* major,
  int* minor,
  CUdevice dev
) {
  *major = 3;
  *minor = 0;

  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(_, _, %d) -> %d [%d, %d]", dev, ret, *major, *minor);
  return ret;
}

CUresult CUDAAPI cuCtxCreate(
  CUcontext* pctx,
  unsigned int flags,
  CUdevice dev
) {
  *pctx = NULL;

  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(_, %d, %d) -> %d", flags, dev, ret);
  return ret;
}

CUresult CUDAAPI cuCtxPopCurrent(
  CUcontext* pctx
) {
  *pctx = NULL;

  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(_) -> %d", ret);
  return ret;
}

CUresult CUDAAPI cuCtxSetCurrent(
  CUcontext ctx
) {
  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(%p) -> %d", (void*)ctx, ret);
  return ret;
}

CUresult CUDAAPI cuCtxGetCurrent(
  CUcontext* pctx
) {
  *pctx = NULL;

  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(_) -> %d", ret);
  return ret;
}

CUresult CUDAAPI cuCtxDestroy(
  CUcontext ctx
) {
  CUresult ret = CUDA_SUCCESS;
  NEE_TRACE_FUNC("(%p) -> %d", (void*)ctx, ret);
  return ret;
}

CUresult CUDAAPI cuMemAllocPitch(
  CUdeviceptr* dptr,
  unsigned int* pPitch,
  unsigned int WidthInBytes,
  unsigned int Height,
  unsigned int ElementSizeBytes
) {
  CUresult ret = CUDA_ERROR_UNKNOWN;
  NEE_TRACE_FUNC("(%p, _, %d, %d, %d) -> %d", (void*)dptr, WidthInBytes, Height, ElementSizeBytes, ret);
  return ret;
}

CUresult CUDAAPI cuMemFree(
  CUdeviceptr dptr
) {
  CUresult ret = CUDA_ERROR_UNKNOWN;
  NEE_TRACE_FUNC("(%p) -> %d", (void*)dptr, ret);
  return ret;
}

CUresult CUDAAPI cuMemcpy2D(
  const CUDA_MEMCPY2D* pCopy
) {
  CUresult ret = CUDA_ERROR_UNKNOWN;
  NEE_TRACE_FUNC("(%p) -> %d", (void*)pCopy, ret);
  return ret;
}


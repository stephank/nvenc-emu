#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <X11/X.h>
#include <va/va.h>
#include <va/va_x11.h>
#include "nvEncodeAPI.h"
#include "util.h"

#define NEE_GUID_EQ(x, y) (memcmp(&(x), &(y), sizeof(GUID)) == 0)

#define NEE_TRACE_GUID(guid, fmt, ...) \
  NEE_TRACE("{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}" fmt, \
    (guid).Data1, \
    (guid).Data2, \
    (guid).Data3, \
    (guid).Data4[0], \
    (guid).Data4[1], \
    (guid).Data4[2], \
    (guid).Data4[3], \
    (guid).Data4[4], \
    (guid).Data4[5], \
    (guid).Data4[6], \
    (guid).Data4[7], \
    ##__VA_ARGS__)

typedef struct {
  Display* x11Display;
  VADisplay* vaDisplay;
  VAContextID vaContext;
} NeeEncoder;

typedef struct {
  VASurfaceID vaSurface;
  // FIXME: Take just the fields we use
  VAImage vaImage;
  bool derived;
} NeeInputBuffer;

typedef struct {
  VABufferID vaBuffer;
} NeeBitstreamBuffer;

NVENCSTATUS NVENCAPI NvEncodeAPIGetMaxSupportedVersion(
  uint32_t* version
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(_) -> %d [%d]", ret, *version);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncodeAPICreateInstance(
  NV_ENCODE_API_FUNCTION_LIST* functionList
) {
  memset(functionList, 0, sizeof(NV_ENCODE_API_FUNCTION_LIST));

  functionList->nvEncOpenEncodeSession = NvEncOpenEncodeSession;
  functionList->nvEncGetEncodeGUIDCount = NvEncGetEncodeGUIDCount;
  functionList->nvEncGetEncodeProfileGUIDCount = NvEncGetEncodeProfileGUIDCount;
  functionList->nvEncGetEncodeProfileGUIDs = NvEncGetEncodeProfileGUIDs;
  functionList->nvEncGetEncodeGUIDs = NvEncGetEncodeGUIDs;
  functionList->nvEncGetInputFormatCount = NvEncGetInputFormatCount;
  functionList->nvEncGetInputFormats = NvEncGetInputFormats;
  functionList->nvEncGetEncodeCaps = NvEncGetEncodeCaps;
  functionList->nvEncGetEncodePresetCount = NvEncGetEncodePresetCount;
  functionList->nvEncGetEncodePresetGUIDs = NvEncGetEncodePresetGUIDs;
  functionList->nvEncGetEncodePresetConfig = NvEncGetEncodePresetConfig;
  functionList->nvEncInitializeEncoder = NvEncInitializeEncoder;
  functionList->nvEncCreateInputBuffer = NvEncCreateInputBuffer;
  functionList->nvEncDestroyInputBuffer = NvEncDestroyInputBuffer;
  functionList->nvEncCreateBitstreamBuffer = NvEncCreateBitstreamBuffer;
  functionList->nvEncDestroyBitstreamBuffer = NvEncDestroyBitstreamBuffer;
  functionList->nvEncEncodePicture = NvEncEncodePicture;
  functionList->nvEncLockBitstream = NvEncLockBitstream;
  functionList->nvEncUnlockBitstream = NvEncUnlockBitstream;
  functionList->nvEncLockInputBuffer = NvEncLockInputBuffer;
  functionList->nvEncUnlockInputBuffer = NvEncUnlockInputBuffer;
  functionList->nvEncGetEncodeStats = NvEncGetEncodeStats;
  functionList->nvEncGetSequenceParams = NvEncGetSequenceParams;
  functionList->nvEncRegisterAsyncEvent = NvEncRegisterAsyncEvent;
  functionList->nvEncUnregisterAsyncEvent = NvEncUnregisterAsyncEvent;
  functionList->nvEncMapInputResource = NvEncMapInputResource;
  functionList->nvEncUnmapInputResource = NvEncUnmapInputResource;
  functionList->nvEncDestroyEncoder = NvEncDestroyEncoder;
  functionList->nvEncInvalidateRefFrames = NvEncInvalidateRefFrames;
  functionList->nvEncOpenEncodeSessionEx = NvEncOpenEncodeSessionEx;
  functionList->nvEncRegisterResource = NvEncRegisterResource;
  functionList->nvEncUnregisterResource = NvEncUnregisterResource;
  functionList->nvEncReconfigureEncoder = NvEncReconfigureEncoder;
  functionList->nvEncCreateMVBuffer = NvEncCreateMVBuffer;
  functionList->nvEncDestroyMVBuffer = NvEncDestroyMVBuffer;
  functionList->nvEncRunMotionEstimationOnly = NvEncRunMotionEstimationOnly;

  return NV_ENC_SUCCESS;
}

NVENCSTATUS NVENCAPI NvEncOpenEncodeSession(
  void* device,
  uint32_t deviceType,
  void** encoder
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %d, _) -> %d [%p]", device, deviceType, ret, *encoder);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodeGUIDCount(
  void* encoderPtr,
  uint32_t* count
) {
  *count = 1;

  NVENCSTATUS ret = NV_ENC_SUCCESS;
  NEE_TRACE_FUNC("(%p, _) -> %d [%d]", encoderPtr, ret, *count);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodeGUIDs(
  void* encoderPtr,
  GUID* GUIDs,
  uint32_t maxLen,
  uint32_t* len
) {
  *len = 0;
  if (maxLen >= 1) {
    *len = 1;
    GUIDs[0] = NV_ENC_CODEC_H264_GUID;
  }

  NVENCSTATUS ret = NV_ENC_SUCCESS;
  NEE_TRACE_FUNC("(%p, _, %d, _) -> %d [_, %d]", encoderPtr, maxLen, ret, *len);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodeProfileGUIDCount(
  void* encoderPtr,
  GUID encodeGUID,
  uint32_t* count
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, _, _) -> %d [%d]", encoderPtr, ret, *count);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodeProfileGUIDs(
  void* encoderPtr,
  GUID encodeGUID,
  GUID* profileGUIDs,
  uint32_t maxLen,
  uint32_t* len
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, _, _, %d, _) -> %d [_, %d]", encoderPtr, maxLen, ret, *len);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetInputFormatCount(
  void* encoderPtr,
  GUID encodeGUID,
  uint32_t* count
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, _, _) -> %d [%d]", encoderPtr, ret, *count);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetInputFormats(
  void* encoderPtr,
  GUID encodeGUID,
  NV_ENC_BUFFER_FORMAT* inputFmts,
  uint32_t maxLen,
  uint32_t* len
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, _, _, %d, _) -> %d [_, %d]", encoderPtr, maxLen, ret, *len);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodeCaps(
  void* encoderPtr,
  GUID encodeGUID,
  NV_ENC_CAPS_PARAM* params,
  int* capsVal
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;

  NEE_TRACE_GUID(encodeGUID, " = encodeGUID");
  if (!NEE_GUID_EQ(encodeGUID, NV_ENC_CODEC_H264_GUID)) {
    ret = NV_ENC_ERR_INVALID_PARAM;
  } else {
    switch (params->capsToQuery) {
      case NV_ENC_CAPS_SUPPORT_DYN_RES_CHANGE:
        *capsVal = 0;
        ret = NV_ENC_SUCCESS;
        break;
      default:
        ret = NV_ENC_ERR_UNIMPLEMENTED;
        break;
    }
  }

  NEE_TRACE_FUNC("(%p, _, _[%d], _) -> %d [%d]", encoderPtr, params->capsToQuery, ret, *capsVal);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodePresetCount(
  void* encoderPtr,
  GUID encodeGUID,
  uint32_t* count
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, _, _) -> %d [%d]", encoderPtr, ret, *count);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodePresetGUIDs(
  void* encoderPtr,
  GUID encodeGUID,
  GUID* presetGUIDs,
  uint32_t maxLen,
  uint32_t* len
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, _, _, %d, _) -> %d [_, %d]", encoderPtr, maxLen, ret, *len);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodePresetConfig(
  void* encoderPtr,
  GUID encodeGUID,
  GUID presetGUID,
  NV_ENC_PRESET_CONFIG* preset
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;

  if (preset->version != NV_ENC_PRESET_CONFIG_VER) {
    ret = NV_ENC_ERR_INVALID_VERSION;
  } else {
    NEE_TRACE_GUID(encodeGUID, " = encodeGUID");
    if (!NEE_GUID_EQ(encodeGUID, NV_ENC_CODEC_H264_GUID)) {
      ret = NV_ENC_ERR_INVALID_PARAM;
    } else {
      NV_ENC_CONFIG* config = &preset->presetCfg;
      memset(config, 0, sizeof(NV_ENC_CONFIG));
      config->version = NV_ENC_CONFIG_VER;

      NEE_TRACE_GUID(presetGUID, " = presetGUID");
      if (!NEE_GUID_EQ(presetGUID, NV_ENC_PRESET_LOW_LATENCY_DEFAULT_GUID)) {
        ret = NV_ENC_ERR_UNIMPLEMENTED;
      } else {
        config->profileGUID = NV_ENC_H264_PROFILE_MAIN_GUID;
        config->gopLength = NVENC_INFINITE_GOPLENGTH;
        config->frameIntervalP = 1;
        config->monoChromeEncoding = 0;
        config->frameFieldMode = NV_ENC_PARAMS_FRAME_FIELD_MODE_FRAME;
        config->mvPrecision = NV_ENC_MV_PRECISION_QUARTER_PEL;

        NV_ENC_RC_PARAMS* rc = &config->rcParams;
        rc->rateControlMode = NV_ENC_PARAMS_RC_CBR_LOWDELAY_HQ;
        // FIXME: Check other params

        NV_ENC_CONFIG_H264* codec = &config->encodeCodecConfig.h264Config;
        codec->level = NV_ENC_LEVEL_H264_4;
        // FIXME: Check other params

        ret = NV_ENC_SUCCESS;
      }
    }
  }

  NEE_TRACE_FUNC("(%p, _, _, _) -> %d [_]", encoderPtr, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncInitializeEncoder(
  void* encoderPtr,
  NV_ENC_INITIALIZE_PARAMS* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;
  NeeEncoder* encoder = encoderPtr;
  VAStatus vaStatus;

  if (params->version != NV_ENC_INITIALIZE_PARAMS_VER) {
    ret = NV_ENC_ERR_INVALID_VERSION;
  } else {
    NEE_TRACE_GUID(params->encodeGUID, " = encodeGUID");
    NEE_TRACE_GUID(params->presetGUID, " = presetGUID");
    NEE_TRACE("%p = cfg, %dx%d [%d:%d] @ %d/%d, max:%dx%d, flags:%s%s%s%s%s%s",
      (void*)params->encodeConfig,
      params->encodeWidth,
      params->encodeHeight,
      params->darWidth,
      params->darHeight,
      params->frameRateNum,
      params->frameRateDen,
      params->maxEncodeWidth,
      params->maxEncodeHeight,
      NEE_STR_FLAG(params, enableEncodeAsync),
      NEE_STR_FLAG(params, enablePTD),
      NEE_STR_FLAG(params, reportSliceOffsets),
      NEE_STR_FLAG(params, enableSubFrameWrite),
      NEE_STR_FLAG(params, enableExternalMEHints),
      NEE_STR_FLAG(params, enableWeightedPrediction)
    );
    if (
      !NEE_GUID_EQ(params->encodeGUID, NV_ENC_CODEC_H264_GUID) ||
      !NEE_GUID_EQ(params->presetGUID, NV_ENC_PRESET_LOW_LATENCY_DEFAULT_GUID) ||
      params->encodeWidth == 0 ||
      params->encodeHeight == 0 ||
      params->enableEncodeAsync != 0 ||
      params->reportSliceOffsets != 0 ||
      params->enableSubFrameWrite != 0 ||
      params->enableExternalMEHints != 0 ||
      params->enableWeightedPrediction != 0 ||
      params->encodeConfig == NULL ||
      params->maxEncodeWidth != 0 ||
      params->maxEncodeHeight != 0
    ) {
      ret = NV_ENC_ERR_INVALID_PARAM;
    } else {
      // FIXME: Fall back to VAEntrypointEncSlice
      VAConfigID vaConfig = 0;
      vaStatus = vaCreateConfig(
        encoder->vaDisplay,
        VAProfileH264Main, // FIXME: Set from config.
        VAEntrypointEncSliceLP,
        // FIXME: Check which other params to set based on NVENC config.
        NULL,
        0,
        &vaConfig
      );
      NEE_TRACE("vaCreateConfig() -> %d [%d]", vaStatus, vaConfig);
      if (vaStatus != VA_STATUS_SUCCESS) {
        ret = NV_ENC_ERR_GENERIC;
      } else {
        VAContextID vaContext = 0;
        vaStatus = vaCreateContext(
          encoder->vaDisplay,
          vaConfig,
          params->encodeWidth,
          params->encodeHeight,
          VA_PROGRESSIVE,
          // FIXME: Check which other params to set based on NVENC config.
          NULL,
          0,
          &vaContext
        );
        NEE_TRACE("vaCreateContext() -> %d [%d]", vaStatus, vaContext);
        if (vaStatus != VA_STATUS_SUCCESS) {
          ret = NV_ENC_ERR_GENERIC;
        } else {
          encoder->vaContext = vaContext;
          ret = NV_ENC_SUCCESS;
        }

        vaStatus = vaDestroyConfig(encoder->vaDisplay, vaConfig);
        NEE_TRACE("vaDestroyConfig() -> %d", vaStatus);
      }
    }
  }

  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncCreateInputBuffer(
  void* encoderPtr,
  NV_ENC_CREATE_INPUT_BUFFER* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;
  NeeEncoder* encoder = encoderPtr;
  VAStatus vaStatus;

  if (params->version != NV_ENC_CREATE_INPUT_BUFFER_VER) {
    ret = NV_ENC_ERR_INVALID_VERSION;
  } else {
    NEE_TRACE("%dx%d, fmt:%d, sysmem:%p",
      params->width,
      params->height,
      params->bufferFmt,
      params->pSysMemBuffer
    );
    if (
      params->width == 0 ||
      params->height == 0 ||
      params->bufferFmt != NV_ENC_BUFFER_FORMAT_NV12 ||
      params->pSysMemBuffer != NULL
    ) {
      ret = NV_ENC_ERR_INVALID_PARAM;
    } else {
      VASurfaceID vaSurface = 0;
      vaStatus = vaCreateSurfaces(
        encoder->vaDisplay,
        VA_RT_FORMAT_YUV420,
        params->width,
        params->height,
        &vaSurface,
        1,
        // FIXME: Check if we need to set anything here.
        NULL,
        0
      );
      NEE_TRACE("vaCreateSurfaces() -> %d [%d]", vaStatus, vaSurface);
      if (vaStatus != VA_STATUS_SUCCESS) {
        ret = NV_ENC_ERR_GENERIC;
      } else {
        // Try to get a derived image if possible.
        bool derived = true;
        VAImage vaImage = { 0 };
        vaStatus = vaDeriveImage(
          encoder->vaDisplay,
          vaSurface,
          &vaImage
        );
        NEE_TRACE("vaDeriveImage() -> %d [%d]", vaStatus, vaImage.image_id);

        // Must be NV12, otherwise fake failure.
        if (
          vaStatus == VA_STATUS_SUCCESS && (
            vaImage.format.fourcc != VA_FOURCC_NV12 ||
            vaImage.format.byte_order != VA_LSB_FIRST ||
            vaImage.format.bits_per_pixel != 8
          )
        ){
          vaStatus = vaDestroyImage(encoder->vaDisplay, vaImage.image_id);
          NEE_TRACE("vaDestroyImage() -> %d", vaStatus);

          vaStatus = VA_STATUS_ERROR_OPERATION_FAILED;
        }

        // Fallback to creating our own image.
        // FIXME: Should just check for 'operation failed',
        // but sometimes returns 'invalid surface'.
        if (vaStatus != VA_STATUS_SUCCESS) {
          VAImageFormat format = {
            .fourcc = VA_FOURCC_NV12,
            .byte_order = VA_LSB_FIRST,
            .bits_per_pixel = 8
          };
          vaStatus = vaCreateImage(
            encoder->vaDisplay,
            &format,
            params->width,
            params->height,
            &vaImage
          );
          NEE_TRACE("vaCreateImage() -> %d [%d]", vaStatus, vaImage.image_id);
          derived = false;
        }

        if (vaStatus != VA_STATUS_SUCCESS) {
          vaStatus = vaDestroySurfaces(encoder->vaDisplay, &vaSurface, 1);
          NEE_TRACE("vaDestroySurfaces() -> %d", vaStatus);
          ret = NV_ENC_ERR_GENERIC;
        } else {
          NeeInputBuffer* inputBuffer = calloc(1, sizeof(NeeInputBuffer));
          inputBuffer->vaSurface = vaSurface;
          inputBuffer->vaImage = vaImage;
          inputBuffer->derived = derived;
          params->inputBuffer = inputBuffer;
          ret = NV_ENC_SUCCESS;
        }
      }
    }
  }

  NEE_TRACE_FUNC("(%p, %p) -> %d [%p]", encoderPtr, (void*)params, ret, params->inputBuffer);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncDestroyInputBuffer(
  void* encoderPtr,
  NV_ENC_INPUT_PTR inputBufferPtr
) {
  NVENCSTATUS ret = NV_ENC_SUCCESS;
  NeeEncoder* encoder = encoderPtr;
  NeeInputBuffer* inputBuffer = inputBufferPtr;
  VAStatus vaStatus;

  vaStatus = vaDestroyImage(encoder->vaDisplay, inputBuffer->vaImage.image_id);
  NEE_TRACE("vaDestroyImage() -> %d", vaStatus);

  vaStatus = vaDestroySurfaces(encoder->vaDisplay, &inputBuffer->vaSurface, 1);
  NEE_TRACE("vaDestroySurfaces() -> %d", vaStatus);

  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, inputBufferPtr, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncCreateBitstreamBuffer(
  void* encoderPtr,
  NV_ENC_CREATE_BITSTREAM_BUFFER* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;
  NeeEncoder* encoder = encoderPtr;
  VAStatus vaStatus;

  if (params->version != NV_ENC_CREATE_BITSTREAM_BUFFER_VER) {
    ret = NV_ENC_ERR_INVALID_VERSION;
  } else {
    VABufferID vaBuffer = 0;
    vaStatus = vaCreateBuffer(
      encoder->vaDisplay,
      encoder->vaContext,
      VAEncCodedBufferType,
      1024 * 1024, // FIXME: Check what the correct size is.
      1,
      NULL,
      &vaBuffer
    );
    NEE_TRACE("vaCreateBuffer() -> %d [%d]", vaStatus, vaBuffer);
    if (vaStatus != VA_STATUS_SUCCESS) {
      ret = NV_ENC_ERR_GENERIC;
    } else {
      NeeBitstreamBuffer* bitstreamBuffer = calloc(1, sizeof(bitstreamBuffer));
      bitstreamBuffer->vaBuffer = vaBuffer;
      params->bitstreamBuffer = bitstreamBuffer;
      ret = NV_ENC_SUCCESS;
    }
  }

  NEE_TRACE_FUNC("(%p, _) -> %d [%p]", encoderPtr, ret, params->bitstreamBuffer);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncDestroyBitstreamBuffer(
  void* encoderPtr,
  NV_ENC_OUTPUT_PTR bitstreamBufferPtr
) {
  NVENCSTATUS ret = NV_ENC_SUCCESS;
  NeeEncoder* encoder = encoderPtr;
  NeeBitstreamBuffer* bitstreamBuffer = bitstreamBufferPtr;
  VAStatus vaStatus;

  vaStatus = vaDestroyBuffer(encoder->vaDisplay, bitstreamBuffer->vaBuffer);
  NEE_TRACE("vaDestroyBuffer() -> %d", vaStatus);

  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, bitstreamBufferPtr, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncEncodePicture(
  void* encoderPtr,
  NV_ENC_PIC_PARAMS* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;

  /* FIXME: Roughly the following: (but params contains more fields)
  if (!derived) {
    vaPutImage(vaDisplay, vaSurface, vaImage, 0, 0, width, height, 0, 0, width, height);
  }
  vaBeginPicture(vaDisplay, vaContext, vaSurface);
  vaRenderPicture(vaDisplay, vaContext, &vaBuffer, 1);
  vaEndPicture(vaDisplay, vaContext);
   */

  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncLockBitstream(
  void* encoderPtr,
  NV_ENC_LOCK_BITSTREAM* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;

  /* FIXME: Roughly the following: (but params contains more fields)
  vaMapBuffer(vaDisplay, vaBuffer, &params->outputBitstream);
  */

  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncUnlockBitstream(
  void* encoderPtr,
  NV_ENC_OUTPUT_PTR bitstreamBufferPtr
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;
  NeeEncoder* encoder = encoderPtr;
  NeeBitstreamBuffer* bitstreamBuffer = bitstreamBufferPtr;
  VAStatus vaStatus;

  vaStatus = vaUnmapBuffer(
    encoder->vaDisplay,
    bitstreamBuffer->vaBuffer
  );
  NEE_TRACE("vaUnmapBuffer() -> %d", vaStatus);
  if (vaStatus != VA_STATUS_SUCCESS) {
    ret = NV_ENC_ERR_GENERIC;
  } else {
    ret = NV_ENC_SUCCESS;
  }

  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, bitstreamBufferPtr, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncLockInputBuffer(
  void* encoderPtr,
  NV_ENC_LOCK_INPUT_BUFFER* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;
  NeeEncoder* encoder = encoderPtr;
  VAStatus vaStatus;

  if (params->version != NV_ENC_LOCK_INPUT_BUFFER_VER) {
    ret = NV_ENC_ERR_INVALID_VERSION;
  } else {
    NeeInputBuffer* inputBuffer = params->inputBuffer;
    vaStatus = vaMapBuffer(
      encoder->vaDisplay,
      inputBuffer->vaImage.buf,
      &params->bufferDataPtr
    );
    NEE_TRACE("vaMapBuffer() -> %d", vaStatus);
    if (vaStatus != VA_STATUS_SUCCESS) {
      ret = NV_ENC_ERR_GENERIC;
    } else {
      params->pitch = inputBuffer->vaImage.pitches[0];
      ret = NV_ENC_SUCCESS;
    }
  }

  NEE_TRACE_FUNC("(%p, _[%p]) -> %d [%p, %d]", encoderPtr, params->inputBuffer, ret, params->bufferDataPtr, params->pitch);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncUnlockInputBuffer(
  void* encoderPtr,
  NV_ENC_INPUT_PTR inputBufferPtr
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;
  NeeEncoder* encoder = encoderPtr;
  NeeInputBuffer* inputBuffer = inputBufferPtr;
  VAStatus vaStatus;

  vaStatus = vaUnmapBuffer(
    encoder->vaDisplay,
    inputBuffer->vaImage.buf
  );
  NEE_TRACE("vaUnmapBuffer() -> %d", vaStatus);
  if (vaStatus != VA_STATUS_SUCCESS) {
    ret = NV_ENC_ERR_GENERIC;
  } else {
    ret = NV_ENC_SUCCESS;
  }

  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, inputBufferPtr, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetEncodeStats(
  void* encoderPtr,
  NV_ENC_STAT* encodeStats
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, _) -> %d [_]", encoderPtr, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncGetSequenceParams(
  void* encoderPtr,
  NV_ENC_SEQUENCE_PARAM_PAYLOAD* sequenceParamPayload
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, _) -> %d [_]", encoderPtr, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncRegisterAsyncEvent(
  void* encoderPtr,
  NV_ENC_EVENT_PARAMS* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncUnregisterAsyncEvent(
  void* encoderPtr,
  NV_ENC_EVENT_PARAMS* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncMapInputResource(
  void* encoderPtr,
  NV_ENC_MAP_INPUT_RESOURCE* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncUnmapInputResource(
  void* encoderPtr,
  NV_ENC_INPUT_PTR buffer
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, buffer, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncDestroyEncoder(
  void* encoderPtr
) {
  NVENCSTATUS ret = NV_ENC_SUCCESS;
  NeeEncoder* encoder = encoderPtr;
  VAStatus vaStatus;

  if (encoder->vaContext) {
    vaStatus = vaDestroyContext(encoder->vaDisplay, encoder->vaContext);
    NEE_TRACE("vaDestroyContext() -> %d", vaStatus);
  }

  vaTerminate(encoder->vaDisplay);
  NEE_TRACE("vaTerminate() -> %d", vaStatus);

  XCloseDisplay(encoder->x11Display);
  free(encoderPtr);

  NEE_TRACE_FUNC("(%p) -> %d", encoderPtr, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncInvalidateRefFrames(
  void* encoderPtr,
  uint64_t timestamp
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %lld) -> %d", encoderPtr, timestamp, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncOpenEncodeSessionEx(
  NV_ENC_OPEN_ENCODE_SESSION_EX_PARAMS* params,
  void** encoderPtr
) {
  NVENCSTATUS ret = NV_ENC_ERR_GENERIC;

  if (params->version != NV_ENC_OPEN_ENCODE_SESSION_EX_PARAMS_VER) {
    ret = NV_ENC_ERR_INVALID_VERSION;
  } else {
    // FIXME: Check apiVersion? Steam requests 8.0
    if (
      params->deviceType != NV_ENC_DEVICE_TYPE_CUDA ||
      params->device != NULL
    ) {
      ret = NV_ENC_ERR_INVALID_DEVICE;
    } else {
      Display* x11Display = XOpenDisplay(NULL);
      NEE_TRACE("XOpenDisplay((nil)) -> %p", (void*)x11Display);
      if (x11Display == NULL ){
        ret = NV_ENC_ERR_NO_ENCODE_DEVICE;
      } else {
        VADisplay vaDisplay = vaGetDisplay(x11Display);

        int major, minor;
        VAStatus vaStatus = vaInitialize(vaDisplay, &major, &minor);
        NEE_TRACE("vaInitialize(%p, _, _) -> %d [%d, %d]", (void*)x11Display, vaStatus, major, minor);
        if (vaStatus != VA_STATUS_SUCCESS) {
          XCloseDisplay(x11Display);
          ret = NV_ENC_ERR_GENERIC;
        } else {
          NEE_LOG("Initialized VA-API %d.%d, vendor: %s", major, minor, vaQueryVendorString(vaDisplay));
          NeeEncoder *encoder = calloc(1, sizeof(NeeEncoder));
          encoder->x11Display = x11Display;
          encoder->vaDisplay = vaDisplay;
          *encoderPtr = (void*) encoder;
          ret = NV_ENC_SUCCESS;
        }
      }
    }
  }

  NEE_TRACE_FUNC("(%p, _) -> %d [%p]", (void*)params, ret, *encoderPtr);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncRegisterResource(
  void* encoderPtr,
  NV_ENC_REGISTER_RESOURCE* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncUnregisterResource(
  void* encoderPtr,
  NV_ENC_REGISTERED_PTR resource
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)resource, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncReconfigureEncoder(
  void* encoderPtr,
  NV_ENC_RECONFIGURE_PARAMS* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncCreateMVBuffer(
  void* encoderPtr,
  NV_ENC_CREATE_MV_BUFFER* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncDestroyMVBuffer(
  void* encoderPtr,
  NV_ENC_OUTPUT_PTR buffer
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, buffer, ret);
  return ret;
}

NVENCSTATUS NVENCAPI NvEncRunMotionEstimationOnly(
  void* encoderPtr,
  NV_ENC_MEONLY_PARAMS* params
) {
  NVENCSTATUS ret = NV_ENC_ERR_UNIMPLEMENTED;
  NEE_TRACE_FUNC("(%p, %p) -> %d", encoderPtr, (void*)params, ret);
  return ret;
}

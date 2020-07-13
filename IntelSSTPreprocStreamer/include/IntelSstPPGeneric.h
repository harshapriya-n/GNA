/******************************************************************************
INTEL CONFIDENTIAL
Copyright 2020 Intel Corporation.

This software and the related documents are Intel copyrighted materials, and
your use of them is governed by the express license under which they were
provided to you ("License"). Unless the License provides otherwise, you may not
use, modify, copy, publish, distribute, disclose or transmit this software or
the related documents without Intel's prior written permission.

This software and the related documents are provided as is, with no express or
implied warranties, other than those that are expressly stated in the License.
******************************************************************************/

#ifndef INTEL_SST_PRE_PROC_CROSS_PLATFORM_H_
#define INTEL_SST_PRE_PROC_CROSS_PLATFORM_H_

#ifdef __cplusplus
extern "C"{
#endif

#ifdef __cplusplus
#include <cstdint>      /* int32_t, ... */
#else
#include <stdint.h>     /* int32_t, ... */
#endif

#if defined _WIN32 || defined __CYGWIN__
#define INTEL_SST_HELPER_SHARED_IMPORT __declspec(dllimport)
#define INTEL_SST_HELPER_SHARED_EXPORT __declspec(dllexport)
#define INTEL_SST_HELPER_SHARED_LOCAL
#else
#if __GNUC__ >= 4
#define INTEL_SST_HELPER_SHARED_IMPORT __attribute__ ((visibility ("default")))
#define INTEL_SST_HELPER_SHARED_EXPORT __attribute__ ((visibility ("default")))
#define INTEL_SST_HELPER_SHARED_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#define INTEL_SST_HELPER_SHARED_IMPORT
#define INTEL_SST_HELPER_SHARED_EXPORT
#define INTEL_SST_HELPER_SHARED_LOCAL
#endif
#endif

#ifdef INTEL_SST_SHARED /* define when compiling or using DLL */
#ifdef INTEL_SST_SHARED_BUILD /* define when compiling DLL */
#define INTEL_SST_API INTEL_SST_HELPER_SHARED_EXPORT
#else
#define INTEL_SST_API INTEL_SST_HELPER_SHARED_IMPORT
#endif
#define INTEL_SST_LOCAL INTEL_SST_HELPER_SHARED_LOCAL
#else
#define INTEL_SST_API
#define INTEL_SST_LOCAL
#endif

/**
\name ApiVersion
API version definition.
*/
/**@{*/
#define INTEL_SST_API_VERSION_DEFINED 6
const int32_t INTEL_SST_API_VERSION = INTEL_SST_API_VERSION_DEFINED;
/**@}*/

#if (INTEL_SST_API_VERSION_DEFINED >= 4)
#include "ILogger/ILogger.h"
#endif

/**
\name ErrorCodes
Error codes definition.
*/
/**@{*/
const int32_t INTEL_SST_SUCCESS = 0;
const int32_t INTEL_SST_FAILURE = 1;
const int32_t INTEL_SST_INVALID_PARAMETER = 2;
const int32_t INTEL_SST_UNSUPPORTED_FORMAT = 3;
const int32_t INTEL_SST_NOT_ENOUGHT_RESOURCES = 4;
const int32_t INTEL_SST_OPERATION_NOT_SUPPORTED = 5;
const int32_t INTEL_SST_BLOB_PARSING_FAILED = 6;
const int32_t INTEL_SST_FORMAT_MISMATCH = 7;
const int32_t INTEL_SST_LOCK_TAKEN = 8;
/**@}*/

/**
\name ErrorCodes
Error codes definition.
*/
/**@{*/
const uint16_t INTEL_SST_SAMPLE_NONE  = 0;
const uint16_t INTEL_SST_SAMPLE_PCM   = 1;
const uint16_t INTEL_SST_SAMPLE_FLOAT = 3;
/**@}*/


/**
\struct AudioFormat
\brief Describes AudioFormat inputs and output.
*/
typedef struct IntelSstAudioFormat_
{
    uint32_t frame_rate_;               //!< sampling rate of the signal
    uint16_t sample_bit_depth_;         //!< actual bit depth of single sample, e.g.: 16, 24, 32
    uint16_t sample_container_bits_;    //!< sample container size in bits (valid values: 16, 32)
    uint16_t channel_count_;            //!< number of channels
    uint16_t sample_type_;              //!< PCM, FLOAT, etc.
} IntelSstAudioFormat;


/**
\struct IntelSstBasicConfiguration
\brief Describes data formats for all inputs and output. It also defines size
of the buffers that will be passed for processing.
*/
typedef struct IntelSstBasicConfiguration_
{
    /**
    should be PCM 16bit
    */
    const IntelSstAudioFormat* input_format_;
    /**
    should be PCM 16bit
    */
    const IntelSstAudioFormat* loopback_format_;
    /**
    should be PCM 16bit
    */
    const IntelSstAudioFormat* multiplexed_format_;
    /**
    should be PCM 16bit, 16kHz
    */
    const IntelSstAudioFormat* output_format_;
    /**
    input channel count in multiplexed data
    */
    int32_t input_channel_count_;
    /**
    defines processing chunk size in 10ms increments
    */
    int32_t processing_chunk_10ms_;
    /**
    if 0 then only format conversion should be performed
    */
    int32_t processing_enabled_;
} IntelSstBasicConfiguration;


/**
\struct IntelSstInput
\brief Structure for passing input data for processing.
Pointer loopback_data can be nullptr at any time.
Pointer input_data can only be nullptr if multiplexed data is non nullptr.
*/
typedef struct IntelSstInput_
{
    /**
    pointer to input stream (capture), if not null then format of the data
    must be the same as input_format passed in IntelSstPreProcInitialize
    */
    const void* input_data_;
    /**
    pointer to loopback stream for AEC, if not null then format of the data
    must be the same as loopback_format passed in IntelSstPreProcInitialize
    */
    const void* loopback_data_;
    /**
    pointer to mutiplexed data from input and loopback streams to provide
    synchronized capture and reference data format ot this stream will be
    */
    const void* multiplexed_data_;
} IntelSstInput;


#if (INTEL_SST_API_VERSION_DEFINED >= 2)
/**
\struct IntelSstBurstInput
\brief Structure for passing burst input data for processing.
*/
typedef struct IntelSstBurstInput_
{
    IntelSstInput input_data_;
    /**
    defines burst chunk size in 10ms increments, i.e. number of 10ms chunks
    */
    int32_t burst_size_in_10ms_;

    /**
    defines position of the detected keyphrase, -1 in any of those values -> no keyphrase
    */
    int32_t keyphrase_begin_in_10ms_; //!< begining of the detected KP, 0 - KP starts in first 10ms of the buffer, 1 - KP starts between 10th and 20th ms in the buffer ...
    int32_t keyphrase_end_in_10ms_; //!< end of the detected KP, 0 - KP ends in first 10ms of the buffer, 1 - KP ends between 10th and 20th ms in the buffer ...
} IntelSstBurstInput;
#endif


/**
\brief Returns API version number the processing DLL implements.
\return API version.
*/
INTEL_SST_API int32_t IntelSstPreProcApiVersion();


/**
\brief Returns version number of the processing DLL.
\return Version number of the DLL.
*/
INTEL_SST_API int32_t IntelSstPreProcGetVersion();

#if (INTEL_SST_API_VERSION_DEFINED >= 2)
/**
\brief Returns version string of the processing DLL.
\return Pointer to const character string with version of the DLL.
*/
INTEL_SST_API const char* IntelSstPreProcGetVersionString();
#endif

/**
\brief Get memory size required for given formats.
\param [in] configuration - descriptor of formats, processing chunk size, etc.
this should determine memory size required for object
\param [out] memory_size - size of memory that needs to be allocated for object
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcGetSize(
    const IntelSstBasicConfiguration* configuration,
    int32_t* memory_size);


/**
\brief Initializes object for given configuration of inputs and outputs.
\param [inout] instance - if *instance is nullptr, dll should allocate memory
on its own and enable freeing it in IntelSstPreProcRelease(), otherwise dll
should only use passed memory block and return non success error code from
IntelSstPreProcRelease() if called. Size of the memory block is defined for
basic configuration by IntelSstPreProcGetSize() function. After this call
instance should be ready for processing with default configuration.
\param [in] configuration - description of inputs and outputs, ...
\sa IntelSstBasicConfiguration
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcInitialize(
    void** instance,
    const IntelSstBasicConfiguration* configuration);

#if (INTEL_SST_API_VERSION_DEFINED >= 4)
INTEL_SST_API int32_t IntelSstPreProcInitializeWithLog(
    void** instance,
    const IntelSstBasicConfiguration* configuration,
    const ICLogger* logger);
#endif


/**
\brief Can be used to set vendor configuration blob.
\param [in] instance - pointer to object memory
\param [in] data - pointer do memory with configuration blob
\param [in] size - size of the configuration blob
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcSetConfig(
    void* instance,
    const void* data,
    int32_t size);

/**
\brief Can be used to get vendor configuration blob.
\param [in] instance - pointer to object memory
\param [in] data - pointer do memory for storing blob, when nullptr then only
                   size should be updated.
\param [inout] size - in size of the memory block (data),
                      out size of the configuration
\return Error code.
- SUCCESS if configuration was stored,
- NOT_ENOUGHT_RESOURCES if not enough memory given - size must be updated
*/
INTEL_SST_API int32_t IntelSstPreProcGetConfig(
    void* instance,
    void* data,
    int32_t *size);


/**
\brief Main processing function.
This function should be Real Time friendly, i.e memory allocations shouldn't be
done here, etc.
\param [in] instance - pointer to object memory
\param [in] input - input buffers descriptor
\param [out] output_data - pointer to buffer for output samples
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcProcess(
    void* instance,
    const IntelSstInput* input,
    void* output_data);


#if (INTEL_SST_API_VERSION_DEFINED >= 2)
/**
\brief Burst processing function.
This function should be Real Time friendly, i.e memory allocations shouldn't be
done here, etc.
\param [in] instance - pointer to object memory
\param [in] input - input buffers descriptor
\param [out] output_data - pointer to buffer for output samples, ensure sufficient space
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcBurstProcess(
    void* instance,
    const IntelSstBurstInput* input,
    void* output_data);
#endif


#if (INTEL_SST_API_VERSION_DEFINED >= 2)
/**
\brief Query pipeline latency.
This function returns current latency of the preprocessing pipeline in ms.
\param [in] instance - pointer to object memory
\param [out] latency_in_ms - pointer to int32_t for latency value in ms
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcGetLatency(
    void* instance,
    int32_t* latency_in_ms);
#endif


#if (INTEL_SST_API_VERSION_DEFINED >= 3)
/**
\brief Reset internal state of the pipelines.
This function resets internal state of the preprocessing.
\param [in] instance - pointer to object memory
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcReset(
    void* instance);
#endif

#if (INTEL_SST_API_VERSION_DEFINED >= 5)
const int32_t BURST_MIN_PRECEEDING_NOISE_IN_MS = 0;
const int32_t BURST_OPT_PRECEEDING_NOISE_IN_MS = 1;
const int32_t BURST_MIN_KEY_PHRASE_LENGTH_IN_MS = 2;
const int32_t BURST_MAX_BUFFER_LENGHT_IN_MS = 3;

/**
\brief Query limit values for burst processing.
This function returns queried limit value for burst processing
\param [in] instance - pointer to object memory
\param [in] parameter_id - id of parameter to query
\param [out] parameter_value -pointer to int32_t for parameter value
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcGetInt32Value(
    void* instance,
    int32_t parameter_id,
    int32_t* parameter_value);
#endif

#if (INTEL_SST_API_VERSION_DEFINED >= 6)
/**
\brief Obtain residual data.
This function should be Real Time friendly, i.e memory allocations shouldn't be
done here, etc.
\param [in] instance - pointer to object memory
\param [out] output_data - pointer to buffer for output samples
\param [inout] output_size_in_10ms - in size in 10ms increments of the memory block (output_data), i.e. number of 10ms chunks
                                     out size of the returned data in 10ms increments
\return Error code.
- SUCCESS if residual data was obtained,
- NOT_ENOUGHT_RESOURCES if not enough memory given - size must be updated
*/
INTEL_SST_API int32_t IntelSstPreProcGetResidualData(
    void* instance,
    void* output_data,
    int32_t* output_size_in_10ms);
#endif

/**
\brief Function to release memory, has is been allocated by the library.
\param [in] instance - pointer to object memory
\return Error code.
*/
INTEL_SST_API int32_t IntelSstPreProcRelease(
    void* instance);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTEL_SST_PRE_PROC_CROSS_PLATFORM_H_ */

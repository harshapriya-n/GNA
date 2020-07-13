/******************************************************************************
INTEL CONFIDENTIAL
Copyright 2017-2018 Intel Corporation

The source code contained or described herein and all documents related to the
source code ("Material") are owned by Intel Corporation or its suppliers or
licensors. Title to the Material remains with Intel Corporation or its
suppliers and licensors. The Material contains trade secrets and proprietary
and confidential information of Intel or its suppliers and licensors.
The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced,
modified, published, uploaded, posted, transmitted, distributed, or disclosed
in any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual
property right is granted to or conferred upon you by disclosure or delivery of
the Materials, either expressly, by implication, inducement, estoppel or
otherwise. Any license under such intellectual property rights must be express
and approved by Intel in writing.
******************************************************************************/

#ifndef AVSLOGGER_LOGLEVEL_H_
#define AVSLOGGER_LOGLEVEL_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum AvsLoggerLogLevel
{
    AVSLOGGER_LEVEL_NONE,         //!< No logging.
    AVSLOGGER_LEVEL_ERROR,        //!< Only error level messages will be logged.
    AVSLOGGER_LEVEL_WARNING,      //!< Only warning and error level messages will be logged.
    AVSLOGGER_LEVEL_INFO,         //!< Logging including info level messages
    AVSLOGGER_LEVEL_DEBUG,        //!< Most verbose logging for debugging purposes, not intended for production.

    //
    AVSLOGGER_LEVEL_MAX_LEVEL = AVSLOGGER_LEVEL_DEBUG, //!< Alias for most verbose logging level.
} AvsLoggerLogLevel;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#ifdef __cplusplus

namespace AvsLogger
{
    enum class LogLevel
    {
        None    = AVSLOGGER_LEVEL_NONE,         //!< No logging.
        Error   = AVSLOGGER_LEVEL_ERROR,        //!< Only error level messages will be logged.
        Warning = AVSLOGGER_LEVEL_WARNING,      //!< Only warning and error level messages will be logged.
        Info    = AVSLOGGER_LEVEL_INFO,         //!< Logging including info level messages
        Debug   = AVSLOGGER_LEVEL_DEBUG,        //!< Most verbose logging for debugging purposes, not intended for production.

        //
        MAX_LEVEL = AVSLOGGER_LEVEL_MAX_LEVEL,   //!< Alias for most verbose logging level.
    };
}

#endif /* __cplusplus */

#endif /* AVSLOGGER_LOGLEVEL_H_ */

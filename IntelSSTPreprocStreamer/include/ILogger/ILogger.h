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

#ifndef AVSLOGGER_ILOGGER_H_
#define AVSLOGGER_ILOGGER_H_

#include "cstdarg"

#include "LogLevel.h"

#define LOG_C_ERROR(...) if (nullptr != this->logger_) { this->logger_->WriteMessage(this->logger_->handle_, AVSLOGGER_LEVEL_ERROR, __VA_ARGS__); };
#define LOG_C_WARNING(...) if (nullptr != this->logger_) { this->logger_->WriteMessage(this->logger_->handle_, AVSLOGGER_LEVEL_WARNING, __VA_ARGS__); };
#define LOG_C_INFO(...) if (nullptr != this->logger_) { this->logger_->WriteMessage(this->logger_->handle_, AVSLOGGER_LEVEL_INFO, __VA_ARGS__); };
#define LOG_C_DEBUG(...) if (nullptr != this->logger_) { this->logger_->WriteMessage(this->logger_->handle_, AVSLOGGER_LEVEL_DEBUG, __VA_ARGS__); };

#ifdef __cplusplus

#ifdef __GNUC__
#define ENTRY      AvsLogger::EntryLoggerObject obj ## __LINE__ ((this->logger_), __PRETTY_FUNCTION__)
#define ENTRY_INFO AvsLogger::EntryLoggerObject obj ## __LINE__ ((this->logger_), __PRETTY_FUNCTION__, AvsLogger::LogLevel::Info)
#else
#define ENTRY      AvsLogger::EntryLoggerObject obj ## __LINE__ ((this->logger_), __FUNCTION__)
#define ENTRY_INFO AvsLogger::EntryLoggerObject obj ## __LINE__ ((this->logger_), __FUNCTION__, AvsLogger::LogLevel::Info)
#endif

#define LOG_ERROR(...) this->LogMessage(AvsLogger::LogLevel::Error, __VA_ARGS__);
#define LOG_WARNING(...) this->LogMessage(AvsLogger::LogLevel::Warning, __VA_ARGS__);
#define LOG_INFO(...) this->LogMessage(AvsLogger::LogLevel::Info, __VA_ARGS__);
#define LOG_DEBUG(...) this->LogMessage(AvsLogger::LogLevel::Debug, __VA_ARGS__);
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void* ILoggerHandle;
typedef void(*ICLoggerWriteMessage)(const ILoggerHandle logger, AvsLoggerLogLevel level, const char* format, ...);
typedef void(*ICLoggerWriteMessageVA)(const ILoggerHandle logger, AvsLoggerLogLevel level, const char* format, va_list args);

typedef struct ICLogger
{
    ILoggerHandle handle_;
    ICLoggerWriteMessage WriteMessage;
    ICLoggerWriteMessageVA WriteMessageVA;
} ICLogger;

#ifdef __cplusplus
}
#endif /* __cplusplus */


/* C++ interface if we are compiled as C++ code. */
#ifdef __cplusplus
namespace AvsLogger
{

class ILogger
{
public:
    virtual ~ILogger() {}

    /**
    * The method to store log message.
    * @param[in] level      Log level as defined in @LogLevel.
    * @param[in] format     Description of log message formatting.
    */
    virtual void WriteMessage(LogLevel level, const char* format, ...) const
    {
        va_list args;
        va_start(args, format);
        WriteMessageVA(level, format, args);
        va_end(args);
    }

    virtual void WriteMessageVA(LogLevel level, const char* format, va_list args) const = 0;
};

class Loggable
{
public:
    Loggable(ILogger* logger)
        : logger_(logger)
    {
    }

    virtual ~Loggable()
    {
        logger_ = nullptr;
    }

    virtual void LogMessage(LogLevel level, const char* format, ...) const
    {
        if(logger_)
        {
            va_list args;
            va_start(args, format);
            logger_->WriteMessageVA(level, format, args);
            va_end(args);
        }
    }

protected:
    ILogger* logger_;
};


struct EntryLoggerObject
{
    EntryLoggerObject(ILogger *obj, const char *f, LogLevel level = LogLevel::Debug) :
        f_(f),
        logger_(obj),
        level_(level)
    {
        if (logger_)
        {
            logger_->WriteMessage(level_, "[%s] Entry.", f_);
        }
    }

    ~EntryLoggerObject()
    {
        if (logger_)
        {
            logger_->WriteMessage(level_, "[%s] Exit.", f_);
        }
    }

private:
    const char *f_;
    ILogger* logger_;
    const LogLevel level_;
};

}
#endif /* __cplusplus */

#endif /* AVSLOGGER_ILOGGER_H_ */

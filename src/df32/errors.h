//-------------------------------------------------------------------------
// errors.h
//      This file contains pre-defined error messages for df32func logic.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
//
// df32func/errors.h
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Error codes
//-------------------------------------------------------------------------
enum_list
    Define ERROR_CODE_DUPLICATE_HASH_KEY$ for 8000
    Define ERROR_CODE_URLDECODE$
    Define ERROR_CODE_URLENCODE$
    Define ERROR_CODE_UNKNOWN_ALGORITHM$
    Define ERROR_CODE_INCOMPATIBLE_ALGORITHM$
    Define ERROR_CODE_INVALID_BLOCKSIZE$
    Define ERROR_CODE_UNRECOGNISED_MODE$
    Define ERROR_CODE_NO_CONTEXT$
    Define ERROR_CODE_INVALID_ADDRESS$
    Define ERROR_CODE_ADDRESS_TAKEN$
    Define ERROR_CODE_UNKNOWN_FORMAT$
    Define ERROR_CODE_INVALID_TIMESTAMP$
    Define ERROR_CODE_INVALID_POSIX_NUMBER$
    Define ERROR_CODE_INVALID_SYSTEM_TIMEZONE$
    Define ERROR_CODE_COMPARISON_OPERATOR$
    Define ERROR_CODE_INVALID_BOOLEAN$
    Define ERROR_CODE_REGEX_BUFFER_OVERFLOW$
    Define ERROR_CODE_REGEX_COMPILE_FAILURE$
    Define ERROR_CODE_FILE_NOT_FOUND$
end_enum_list

//-------------------------------------------------------------------------
// Error messages
//
// Maxsize = 40 chars   Width guide         |----------------------------------------|
//-------------------------------------------------------------------------

#REPLACE ERROR_MSG_DUPLICATE_HASH_KEY       "Duplicate key in unique hashed column"
#REPLACE ERROR_MSG_URLDECODE                "Urldecode failed"
#REPLACE ERROR_MSG_URLENCODE                "Urlencode failed"
#REPLACE ERROR_MSG_UNKNOWN_ALGORITHM        "Unknown crypt algorithm: ??"
#REPLACE ERROR_MSG_INOMPATIBLE_ALGORITHM    "Incompatible algorithm: ??"
#REPLACE ERROR_MSG_INVALID_BLOCKSIZE        "Invalid blocksize: ??"
#REPLACE ERROR_MSG_UNRECOGNISED_MODE        "Unrecognised mode: ??"
#REPLACE ERROR_MSG_NO_CONTEXT               "No crypt context exists"
#REPLACE ERROR_MSG_INVALID_ADDRESS          "Invalid address: ??"
#REPLACE ERROR_MSG_ADDRESS_TAKEN            "Address already taken: ??"
#REPLACE ERROR_MSG_UNKNOWN_FORMAT           "Unknown format: ?? not HEX|BASE64|BIN"
#REPLACE ERROR_MSG_INVALID_TIMESTAMP        "Invalid timestmap"
#REPLACE ERROR_MSG_INVALID_POSIX_NUMBER     "Invalid posix number: ??"
#REPLACE ERROR_MSG_INVALID_SYSTEM_TIMEZONE  "Invalid system timezone"
#REPLACE ERROR_MSG_INVALID_BOOLEAN          "Value does not evaluate to boolean"
#REPLACE ERROR_MSG_COMPARISON_OPERATOR      "Not a valid operator: ??"
#REPLACE ERROR_MSG_REGEX_BUFFER_OVERFLOW	"Regex output buffer too small: ??"
#REPLACE ERROR_MSG_REGEX_COMPILE_FAILURE    "Regex compilation failed"
#REPLACE ERROR_MSG_FILE_NOT_FOUND           "File not found or permission denied"

//-------------------------------------------------------------------------
// Error message detail
//
// Maxsize = 32 chars   Width guide         |--------------------------------|
//-------------------------------------------------------------------------
#REPLACE ERROR_DETAIL_DUPLICATE_HASH_KEY    "Duplicate key at index: ??"
#REPLACE ERROR_DETAIL_GETLASTERROR          "GetLastError = ??"
#REPLACE ERROR_DETAIL_INVALID_TIMESTAMP     "Format: ??"
#REPLACE ERROR_DETAIL_INVALID_BOOLEAN       "Value ?? != true/false"
#REPLACE ERROR_DETAIL_FILE_NOT_FOUND		"File: ??"

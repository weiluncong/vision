/*-----------------------------------------------------------------------------
Module: MDF4Lib
Interfaces:
-------------------------------------------------------------------------------
MDF4Lib public unmanaged C++ interface: structure definitions
-------------------------------------------------------------------------------
Copyright (c) Vector Informatik GmbH. All rights reserved.
-----------------------------------------------------------------------------*/

#pragma once

#include "GeneralDataTypes.h"
#include "MDF4LibForwardDecl.h"

#ifndef LINUX
#include <pshpack8.h>
#else
#pragma pack(push,8)
#endif


///////////////////////////////////////////////////
//! \file
//! \brief structure definitions

interface IMDF4LibStructs
{
  typedef struct Value2Value_t
  {
    REAL key;
    REAL result;

    Value2Value_t(REAL key = 0.0, REAL result = 0.0)
      : key(key)
      , result(result)
    {}

  } Value2Value;

  typedef struct Value2Text_t
  {
    REAL key;
    LPCTSTR result;

    Value2Text_t(REAL key = 0.0, LPCTSTR result = NULL)
      : key(key)
      , result(result)
    {}

  } Value2Text;

  typedef struct Text2Value_t
  {
    LPCTSTR key;
    REAL result;

    Text2Value_t(LPCTSTR key = NULL, REAL result = 0.0)
      : key(key)
      , result(result)
    {}

  } Text2Value;

  typedef struct Text2Text_t
  {
    LPCTSTR key;
    LPCTSTR result;

    Text2Text_t(LPCTSTR key = NULL, LPCTSTR result = NULL)
      : key(key)
      , result(result)
    {}

  } Text2Text;

  typedef struct ValueRange_t
  {
    REAL min;
    REAL max;

    ValueRange_t(REAL min = 0.0, REAL max = 0.0)
    {
      this->min = min;
      this->max = max;
    }

  } ValueRange;

  typedef struct ValueRange2Value_t
  {
    ValueRange keyRange;
    REAL result;

    ValueRange2Value_t(ValueRange keyRange, REAL result)
      : keyRange(keyRange)
      , result(result)
    {}

    ValueRange2Value_t(REAL min = 0.0, REAL max = 0.0, REAL result = 0.0)
      : keyRange(min, max)
      , result(result)
    {}

  } ValueRange2Value;

  typedef struct ValueRange2Text_t
  {
    ValueRange keyRange;
    LPCTSTR result;

    ValueRange2Text_t(ValueRange keyRange, LPCTSTR result)
      : keyRange(keyRange)
      , result(result)
    {}
    ValueRange2Text_t(REAL min = 0.0, REAL max = 0.0, LPCTSTR result = NULL)
      : keyRange(min, max)
      , result(result)
    {}

  } ValueRange2Text;

  typedef struct TextOrConversion_t
  {
    union
    {
      LPCTSTR text;
      IConversion* pConversion;
    } pointerTo;
    bool isText;

    TextOrConversion_t(LPCTSTR text)
      : isText(true)
    {
      pointerTo.text = text;
    }

    TextOrConversion_t(IConversion* pConversion = NULL)
      : isText(false)
    {
      pointerTo.pConversion = pConversion;
    }
  } TextOrConversion;

  typedef struct Value2TextOrConversion_t
  {
    REAL key;
    TextOrConversion result;

    Value2TextOrConversion_t(REAL key, TextOrConversion result)
      : key(key)
      , result(result)
    {}

    Value2TextOrConversion_t(REAL key, LPCTSTR text)
      : key(key)
      , result(text)
    {}

    Value2TextOrConversion_t(REAL key = 0.0, IConversion* pConversion = NULL)
      : key(key)
      , result(pConversion)
    {}

  } Value2TextOrConversion;

  typedef struct ValueRange2TextOrConversion_t
  {
    ValueRange keyRange;
    TextOrConversion result;

    ValueRange2TextOrConversion_t(ValueRange keyRange, TextOrConversion result)
      : keyRange(keyRange)
      , result(result)
    {}

    ValueRange2TextOrConversion_t(REAL min, REAL max, TextOrConversion result)
      : keyRange(min, max)
      , result(result)
    {}

    ValueRange2TextOrConversion_t(ValueRange keyRange, LPCTSTR text)
      : keyRange(keyRange)
      , result(text)
    {}

    ValueRange2TextOrConversion_t(REAL min, REAL max, LPCTSTR text)
      : keyRange(min, max)
      , result(text)
    {}

    ValueRange2TextOrConversion_t(ValueRange keyRange, IConversion* pConversion)
      : keyRange(keyRange)
      , result(pConversion)
    {}

    ValueRange2TextOrConversion_t(REAL min = 0.0, REAL max = 0.0, IConversion* pConversion = NULL)
      : keyRange(min, max)
      , result(pConversion)
    {}

  } ValueRange2TextOrConversion;


  ///////////////////////////////////////////////////
  //! properties of a comment field as structure
  typedef struct FieldProperties_t
  {
    LPCTSTR name;       //!< name of field
    LPCTSTR value;      //!< value of field as string. For valueCount > 1 this is a list of strings separated by separator.
    LPCTSTR unit;       //!< string to be displayed as unit (optional), NULL if not specified
    LPCTSTR desc;       //!< description text (optional), NULL if not specified
    LPCTSTR lang;       //!< language of element content as defined in XML standard (optional), NULL if not specified. Example: "en-US" = English as used in the United States.
    LPCTSTR separator;  //!< if valueCount >= 0, then this string is used to separate the value in the value string.
                        //!< If NULL or empty string, the strings are separated by a NULL ('\0') character. Please be careful to avoid buffer overruns!
    int valueCount;     //!< if >= 0 then the field contains a list of values (&lt;elist&gt; tag in MDF 4.1 or higher),
                        //!< otherwise it is a single value (&lt;e&gt; tag)
    FieldDataType type; //!< data type of element content (value field). See FieldDataTypeEnum
    bool ro;            //!< read-only flag for field


    //! Get type of field.
    //! \return field type, see ::CommentFieldTypeEnum, ::eCommentFieldTypeUnspec on error
    CommentFieldType GetFieldType()
    {
      if (name == NULL) return eCommentFieldTypeUnspec;
      return static_cast<CommentFieldType>(valueCount < 0 ? eSingleValueField : eMultiValueField);
    }

    //! constructor. Attention: strings must be kept "alive" as long as the structure is in use (only pointers are stored!)
    FieldProperties_t(
      LPCTSTR name = NULL,
      LPCTSTR value = NULL,
      LPCTSTR unit = NULL,
      LPCTSTR desc = NULL,
      LPCTSTR lang = NULL,
      FieldDataType type = eFieldDataTypeString,
      bool ro = false,
      int valueCount = -1,
      LPCTSTR separator = NULL)
      : name(name)
      , value(value)
      , unit(unit)
      , desc(desc)
      , lang(lang)
      , separator(separator)
      , valueCount(valueCount)
      , type(type)
      , ro(ro)
    {}

  } FieldProperties;

}; // ns IMDF4LibStructs


#ifndef LINUX
#include <poppack.h>
#else
#pragma pack(pop)
#endif

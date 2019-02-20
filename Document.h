//
// Created by fl on 11/22/18.
//

#ifndef JFSON_DOCUMENT_H
#define JFSON_DOCUMENT_H

#include <string>
#include <src/JsonValue.h>
#include "JsonValue.h"


namespace jfson
{


class Document: public JsonValue
{
public:
    JsonError parse(const char* json, size_t len)
    {

    }
};

}

#endif //JFSON_DOCUMENT_H

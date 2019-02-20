//
// Created by fl on 10/2/18.
//

#include "JsonValue.h"

using namespace jfson;

JsonValue::JsonValue(const jfson::JsonValue& rhs)
        : type_(rhs.type_)
        , arr_(rhs.arr_)
{
    switch (type_) {
        case JSON_NULL:
        case JSON_BOOL:
        case JSON_INT32:
        case JSON_INT64:
        case JSON_DOUBLE: break;
        case JSON_STRING:
        case JSON_ARRAY:
        case JSON_OBJECT:
        default: assert(false);
    }
}

JsonValue::JsonValue(JsonValue &&rhs)
            : type_(rhs.type_), arr_(rhs.arr_)
{
    rhs.type_ = JSON_NULL;
    rhs.arr_ = nullptr;
}

JsonValue& JsonValue::operator=(const jfson::JsonValue& rhs)
{
    assert(this != &rhs);
    this->~JsonValue();
}

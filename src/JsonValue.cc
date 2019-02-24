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
        case JSON_STRING: break;
        case JSON_ARRAY: break;
        case JSON_OBJECT: break;
        default: assert(false);
    }
}

JsonValue::JsonValue(JsonValue&& rhs)
            : type_(rhs.type_), arr_(rhs.arr_)
{
    rhs.type_ = JSON_NULL;
    rhs.arr_.clear();
}

JsonValue& JsonValue::operator=(const jfson::JsonValue& rhs)
{
    assert(this != &rhs);
    this->~JsonValue();
    type_ = rhs.type_;
    arr_ = rhs.arr_;
    switch (type_) {
        case JSON_NULL:
        case JSON_BOOL:
        case JSON_INT32:
        case JSON_INT64:
        case JSON_DOUBLE: break;
        case JSON_STRING: break;
        case JSON_ARRAY: break;
        case JSON_OBJECT: break;
        default: assert(false);
    }
    return *this;
}

JsonValue& JsonValue::operator=(JsonValue &&rhs) {
    assert(this != &rhs);
    this->~JsonValue();
    type_ = rhs.type_;
    arr_ = rhs.arr_;
    rhs.type_ = JSON_NULL;
    rhs.arr_.clear();
    return *this;
}


JsonValue::JsonValue(ValueType type)
        : type_(type)
        , str_(nullptr)
{
    switch (type_) {
        case JSON_NULL:
        case JSON_BOOL:
        case JSON_INT32:
        case JSON_INT64:
        case JSON_DOUBLE:                                break;
        case JSON_STRING:                                break;
        case JSON_ARRAY:                                 break;
        case JSON_OBJECT:                                break;
        default: assert(false && "bad value type");
    }
}


JsonValue::~JsonValue()
{
    switch (type_) {
        case JSON_NULL:
        case JSON_BOOL:
        case JSON_INT32:
        case JSON_INT64:
        case JSON_DOUBLE: break;
        case JSON_STRING:
            break;
        case JSON_ARRAY:
            break;
        case JSON_OBJECT:
            break;
        default: assert(false && "bad value type");
    }
}


JsonValue& JsonValue::operator[] (std::string_view key)
{
    assert(type_ == JSON_OBJECT);

    if (obj_.find(key) != obj_.end()) {
        return obj_[key];
    }

    assert(false); // unlike std::map
    static JsonValue fake(JSON_NULL);
    return fake;
}

size_t JsonValue::getSize() const
{
    if (type_ == JSON_ARRAY)
        return arr_.size();
    else if (type_ == JSON_OBJECT)
        return obj_.size();
    return 1;
}

const JsonValue&  JsonValue::operator[] (std::string_view key) const
{
    return const_cast<JsonValue&>(*this)[key];
}


JsonValue& JsonValue::addMember(JsonValue&& key, JsonValue&& value)
{
    assert(type_ == JSON_OBJECT);
    assert(key.type_ == JSON_STRING);
    assert(obj_.find(key.str_) == obj_.end());
    obj_[key.str_] = std::move(value);
    return obj_[key.str_];
}

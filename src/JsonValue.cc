//
// Created by fl on 10/2/18.
//

#include <algorithm>
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
            str_->incrAndGet(); break;
        case JSON_ARRAY:
            arr_->incrAndGet(); break;
        case JSON_OBJECT:
            obj_->incrAndGet(); break;
        default: assert(false && "Bad value type");
    }
}

JsonValue::JsonValue(JsonValue&& rhs)
            : type_(rhs.type_), arr_(rhs.arr_)
{
    rhs.type_ = JSON_NULL;
    rhs.arr_ = nullptr;
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
        case JSON_STRING:
            str_->incrAndGet(); break;
        case JSON_ARRAY:
            arr_->incrAndGet(); break;
        case JSON_OBJECT:
            obj_->incrAndGet(); break;
        default: assert(false && "Bad value type");
    }
    return *this;
}

JsonValue& JsonValue::operator=(JsonValue &&rhs) {
    assert(this != &rhs);
    this->~JsonValue();
    type_ = rhs.type_;
    arr_ = rhs.arr_;
    rhs.type_ = JSON_NULL;
    rhs.arr_ = nullptr;
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
        case JSON_DOUBLE:                                   break;
        case JSON_STRING: str_ = new StringWithRefCount();  break;
        case JSON_ARRAY:  arr_ = new ArrayWithRefCount();   break;
        case JSON_OBJECT: obj_ = new ObjectWithRefCount();  break;
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
            if (str_->decrAndGet() == 0)
                delete str_;
            break;
        case JSON_ARRAY:
            if (arr_->decrAndGet() == 0)
                delete arr_;
            break;
        case JSON_OBJECT:
            if (obj_->decrAndGet() == 0)
                delete obj_;
            break;
        default: assert(false && "Bad value type");
    }
}


JsonValue& JsonValue::operator[] (std::string_view key)
{
    assert(type_ == JSON_OBJECT);

    auto it = findMember(key);
    if (it != obj_->data.end())
        return it->value;

    assert(false);
    static JsonValue fake(JSON_NULL);
    return fake;
}

size_t JsonValue::getSize() const
{
    if (type_ == JSON_ARRAY)
        return arr_->data.size();
    else if (type_ == JSON_OBJECT)
        return obj_->data.size();
    return 1;
}

const JsonValue&  JsonValue::operator[] (std::string_view key) const
{
    return const_cast<JsonValue&>(*this)[key];
}


JsonValue::MemberIterator JsonValue::findMember(std::string_view key)
{
    assert(type_ == JSON_OBJECT);
    return std::find_if(obj_->data.begin(), obj_->data.end(), [key](const Member& m)->bool {
        return m.key.getStringView() == key;
    });
}

JsonValue::ConstMemberIterator JsonValue::findMember(std::string_view key) const
{
    return const_cast<JsonValue&>(*this).findMember(key);
}


JsonValue& JsonValue::addMember(JsonValue&& key, JsonValue&& value)
{
    assert(type_ == JSON_OBJECT);
    assert(key.type_ == JSON_STRING);
    assert(findMember(key.getStringView()) == memberEnd());
    obj_->data.emplace_back(std::move(key), std::move(value));
    return obj_->data.back().value;
}

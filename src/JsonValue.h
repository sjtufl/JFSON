//
// Created by fl on 10/2/18.
//

#ifndef JFSON_JSONVALUE_H
#define JFSON_JSONVALUE_H

#include <cassert>
#include <cstring>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <memory>
#include <atomic>

#include <src/noncopyable.h>

namespace jfson
{

using namespace std::string_view_literals;

enum ValueType {
    JSON_NULL,
    JSON_BOOL,
    JSON_INT32,
    JSON_INT64,
    JSON_DOUBLE,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT,
};

class Document;

class JsonValue
{
    friend class Document;

public:
    explicit JsonValue(ValueType type = JSON_NULL);

    explicit JsonValue(bool bo):
            type_(JSON_BOOL),
            bo_(bo)
    {}

    explicit JsonValue(int32_t int32):
            type_(JSON_INT32),
            int32_(int32)
    {}

    explicit JsonValue(int64_t int64):
            type_(JSON_INT64),
            int64_(int64)
    {}

    explicit JsonValue(double doub):
            type_(JSON_DOUBLE),
            doub_(doub)
    {}

    explicit JsonValue(std::string_view s):
            type_(JSON_STRING),
            str_(s)
    {}

    explicit JsonValue(const char* s):
            type_(JSON_STRING),
            str_(std::string_view(s, strlen(s)))
    {}

    JsonValue(const JsonValue& rhs);
    JsonValue(JsonValue&& rhs);

    JsonValue& operator=(const JsonValue& rhs);
    JsonValue& operator=(JsonValue&& rhs);

    ~JsonValue();

    ValueType getType() const { return type_; }
    size_t getSize() const;

    bool isNull()   const { return type_ == JSON_NULL; }
    bool isBool()   const { return type_ == JSON_BOOL; }
    bool isInt32()  const { return type_ == JSON_INT32; }
    bool isInt64()  const { return type_ == JSON_INT64 || type_ == JSON_INT32; }
    bool isDouble() const { return type_ == JSON_DOUBLE; }
    bool isString() const { return type_ == JSON_STRING; }
    bool isArray()  const { return type_ == JSON_ARRAY; }
    bool isObject() const { return type_ == JSON_OBJECT; }

    // GET OPERATION
    bool getBool() const
    {
        assert(type_ == JSON_BOOL);
        return bo_;
    }
    int32_t getInt32() const
    {
        assert(type_ == JSON_INT32);
        return int32_;
    }
    int64_t getInt64() const
    {
        assert(type_ == JSON_INT64);
        return int64_;
    }
    double getDouble() const
    {
        assert(type_ == JSON_DOUBLE);
        return doub_;
    }
//    std::string_view getStringView() const
//    {
//        assert(type_ == JSON_STRING);
//        return std::string_view()
//    }
    std::string getString() const
    {
        assert(type_ == JSON_STRING);
        return str_;
    }
    const auto& getArray() const
    {
        assert(type_ == JSON_ARRAY);
        return arr_;
    }
    const auto& getObject() const
    {
        assert(type_ == JSON_OBJECT);
        return obj_;
    }
    // SET OPERATION
    JsonValue& setNull()
    {
        this->~JsonValue();
        return *new (this) JsonValue(JSON_NULL);
    }

    JsonValue& setBool(bool b)
    {
        this->~JsonValue();
        return *new (this) JsonValue(b);
    }

    JsonValue& setInt32(int32_t i32)
    {
        this->~JsonValue();
        return *new (this) JsonValue(i32);
    }

    JsonValue& setInt64(int64_t i64)
    {
        this->~JsonValue();
        return *new (this) JsonValue(i64);
    }

    JsonValue& setDouble(double d)
    {
        this->~JsonValue();
        return *new (this) JsonValue(d);
    }

    JsonValue& setString(std::string_view s)
    {
        this->~JsonValue();
        return *new (this) JsonValue(s);
    }

    JsonValue& setArray()
    {
        this->~JsonValue();
        return *new (this) JsonValue(JSON_ARRAY);
    }

    JsonValue& setObject()
    {
        this->~JsonValue();
        return *new (this) JsonValue(JSON_OBJECT);
    }

    JsonValue& operator[] (std::string_view key);
    const JsonValue& operator[] (std::string_view key) const;

    template <typename T>
    JsonValue& addValue(T&& value)
    {
        assert(type_ == JSON_ARRAY);
        arr_.emplace_back(std::forward<T>(value));
        return arr_.back();
    }

    const JsonValue& operator[] (size_t i) const
    {
        assert(type_ == JSON_ARRAY);
        return arr_[i];
    }

    JsonValue& operator[] (size_t i)
    {
        assert(type_ == JSON_ARRAY);
        return arr_[i];
    }

    template <typename V>
    JsonValue& addMember(const char* key, V&& value)
    {
        return addMember(JsonValue(key),
                         Value(std::forward<V>(value)));
    };

    JsonValue& addMember(JsonValue&& key, JsonValue&& value);

private:
    ValueType type_;

//    typedef std::vector<char> String;
    typedef std::vector<JsonValue> Array;
    // Fixme: Replace map with struct Member
    typedef std::map<std::string_view, JsonValue> Object;

    union {
        bool bo_;
        int32_t int32_;
        int64_t int64_;
        double doub_;
        std::string str_;
        Array arr_;
        Object obj_;
    };

};


}

#endif //JFSON_JSONVALUE_H

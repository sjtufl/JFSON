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
struct Member;

class JsonValue
{
    friend class Document;

public:
    typedef std::vector<Member>::iterator MemberIterator;
    typedef std::vector<Member>::const_iterator ConstMemberIterator;

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
            str_(new StringWithRefCount(s.begin(), s.end()))
    {}

    explicit JsonValue(const char* s):
            type_(JSON_STRING),
            str_(new StringWithRefCount(s, s+strlen(s)))
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
    std::string_view getStringView() const
    {
        assert(type_ == JSON_STRING);
        return std::string_view(&*str_->data.begin(), str_->data.size());
    }
    std::string getString() const
    {
        assert(type_ == JSON_STRING);
        return std::string(getStringView());
    }
    const auto& getArray() const
    {
        assert(type_ == JSON_ARRAY);
        return arr_->data;
    }
    const auto& getObject() const
    {
        assert(type_ == JSON_OBJECT);
        return obj_->data;
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

    MemberIterator memberBegin()
    {
        assert(type_ == JSON_OBJECT);
        return obj_->data.begin();
    }

    ConstMemberIterator memberBegin() const
    {
        return const_cast<JsonValue&>(*this).memberBegin();
    }

    MemberIterator memberEnd()
    {
        assert(type_ == JSON_OBJECT);
        return obj_->data.end();
    }

    ConstMemberIterator memberEnd() const
    {
        return const_cast<JsonValue&>(*this).memberEnd();
    }

    MemberIterator findMember(std::string_view key);

    ConstMemberIterator findMember(std::string_view key) const;

    template <typename V>
    JsonValue& addMember(const char* key, V&& value)
    {
        return addMember(JsonValue(key),
                         JsonValue(std::forward<V>(value)));
    };

    JsonValue& addMember(JsonValue&& key, JsonValue&& value);

    template <typename T>
    JsonValue& addValue(T&& value)
    {
        assert(type_ == JSON_ARRAY);
        arr_->data.emplace_back(std::forward<T>(value));
        return arr_->data.back();
    }

    const JsonValue& operator[] (size_t i) const
    {
        assert(type_ == JSON_ARRAY);
        return arr_->data[i];
    }

    JsonValue& operator[] (size_t i)
    {
        assert(type_ == JSON_ARRAY);
        return arr_->data[i];
    }

    template <typename Handler>
    bool writeTo(Handler& handler) const;

private:
    ValueType type_;

    template <typename T>
    struct RefCount
    {
        template <typename... Args>
        RefCount(Args&&... args)
        : refCount(1)
        , data(std::forward<Args>(args)...)
        {}
        ~RefCount()
        { assert(refCount == 0); }

        int incrAndGet()
        {
            assert(refCount > 0);
            return ++refCount;
        }

        int decrAndGet()
        {
            assert(refCount > 0);
            return --refCount;
        }

        std::atomic_int refCount;
        T data;
    };

//    typedef std::vector<char> String;
    typedef RefCount<std::vector<JsonValue>> ArrayWithRefCount;
    // Fixme: Replace map with struct Member
//    typedef std::map<std::string_view, JsonValue> Object;
    typedef RefCount<std::vector<Member>> ObjectWithRefCount;
    typedef RefCount<std::vector<char>> StringWithRefCount;

    union {
        bool bo_;
        int32_t int32_;
        int64_t int64_;
        double doub_;
        StringWithRefCount* str_;
        ArrayWithRefCount* arr_;
        ObjectWithRefCount* obj_;
    };

};

struct Member
{
    Member(JsonValue&& key_, JsonValue&& value_):
            key(std::move(key_)),
            value(std::move(value_))
    {}

    Member(std::string_view key_, JsonValue&& value_):
            key(key_),
            value(std::move(value_))
    {}

    JsonValue key;
    JsonValue value;
};

#define CALL(expr) do { if (!(expr)) return false; } while(false)

template <typename Handler>
inline bool JsonValue::writeTo(Handler& handler) const
{
    switch (type_)
    {
        case JSON_NULL:
            CALL(handler.Null());
            break;
        case JSON_BOOL:
            CALL(handler.Bool(bo_));
            break;
        case JSON_INT32:
            CALL(handler.Int32(int32_));
            break;
        case JSON_INT64:
            CALL(handler.Int64(int64_));
            break;
        case JSON_DOUBLE:
            CALL(handler.Double(doub_));
            break;
        case JSON_STRING:
            CALL(handler.String(getStringView()));
            break;
        case JSON_ARRAY:
            CALL(handler.StartArray());
            for (auto& val: getArray()) {
                CALL(val.writeTo(handler));
            }
            CALL(handler.EndArray());
            break;
        case JSON_OBJECT:
            // fixme : use struct member
//            CALL(handler.StartObject());
//            for(auto& member : getObject()) {
//                handler.Key(member.first);
//                CALL(member.second.writeTo(handler));
//            }
//            CALL(handler.EndObject());
            CALL(handler.StartObject());
            for (auto& member: getObject()) {
                handler.Key(member.key.getStringView());
                CALL(member.value.writeTo(handler));
            }
            CALL(handler.EndObject());
            break;
        default:
            assert(false && "bad type");
    }
    return true;
}

#undef CALL



}

#endif //JFSON_JSONVALUE_H

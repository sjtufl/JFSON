//
// Created by fl on 10/20/18.
//
#ifndef JFSON_DOCUMENT_H
#define JFSON_DOCUMENT_H

#include <string>

#include <src/JsonValue.h>
#include <src/JsonReader.h>
#include <src/StringReadStream.h>


namespace jfson
{


class Document: public JsonValue
{
public:
    ParseError parse(const char* json, size_t len)
    {
        return parse(std::string_view(json, len));
    }

    ParseError parse(std::string_view json)
    {
        StringReadStream is(json);
        return parseStream(is);
    }

    template <typename ReadStream>
    ParseError parseStream(ReadStream& is)
    {
        return JsonReader::parse(is, *this);
    }

public: // handler
    bool Null()
    {
        addValue(JsonValue(JSON_NULL));
        return true;
    }
    bool Bool(bool b)
    {
        addValue(JsonValue(b));
        return true;
    }
    bool Int32(int32_t i32)
    {
        addValue(JsonValue(i32));
        return true;
    }
    bool Int64(int64_t i64)
    {
        addValue(JsonValue(i64));
        return true;
    }
    bool Double(double d)
    {
        addValue(JsonValue(d));
        return true;
    }
    bool String(std::string_view s)
    {
        addValue(JsonValue(s));
        return true;
    }
    bool StartObject()
    {
        auto value = addValue(JsonValue(JSON_OBJECT));
        stack_.emplace_back(value);
        return true;
    }
    bool Key(std::string_view s)
    {
        addValue(JsonValue(s));
        return true;
    }
    bool EndObject()
    {
        assert(!stack_.empty());
        assert(stack_.back().type() == JSON_OBJECT);
        stack_.pop_back();
        return true;
    }
    bool StartArray()
    {
        auto value = addValue(JsonValue(JSON_ARRAY));
        stack_.emplace_back(value);
        return true;
    }
    bool EndArray()
    {
        assert(!stack_.empty());
        assert(stack_.back().type() == JSON_ARRAY);
        stack_.pop_back();
        return true;
    }

private:
    JsonValue* addValue(JsonValue&& value)
    {
        ValueType type = value.getType();
        (void)type;
        if (seeValue_)
            assert(!stack_.empty() && "root not singular");
        else {
            assert(type_ == JSON_NULL);
            seeValue_ = true;
            type_ = value.type_;
            arr_ = value.arr_;
            value.type_ = JSON_NULL;
            value.arr_.clear(); // fixme
            return this;
        }


        auto& top = stack_.back();
        if (top.type() == JSON_ARRAY) {
            top.value->addValue(std::move(value));
            top.valueCount++;
            return top.lastValue();
        }
        else {
            assert(top.type() == JSON_OBJECT);

            if (top.valueCount % 2 == 0) {
                assert(type == JSON_STRING && "miss quotation mark");
                key_ = std::move(value);
                top.valueCount++;
                return &key_;
            }
            else {
                top.value->addMember(std::move(key_), std::move(value));
                top.valueCount++;
                return top.lastValue();
            }
        }
    }

private:
    struct Level
    {
        explicit Level(JsonValue* value_):
                value(value_), valueCount(0)
        {}

        ValueType type() const
        {
            return value->getType();
        }

        JsonValue* lastValue()
        {
            if (type() == JSON_ARRAY) {
                return &value->arr_.back();
            } else {
//                return &value->obj_->data.back().value;
                return  nullptr;  // fixme
            }
        }

        JsonValue* value;
        int valueCount;
    };

private:
    std::vector<Level> stack_;
    JsonValue key_;
    bool seeValue_ = false;
};


}



#endif //JFSON_DOCUMENT_H

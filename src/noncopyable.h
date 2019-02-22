
#ifndef JFSON_NONCOPYABLE_H
#define JFSON_NONCOPYABLE_H

namespace jfson
{

class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

}

#endif //JFSON_NONCOPYABLE_H

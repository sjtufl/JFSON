//
// Created by fl on 11/22/18.
//

#ifndef JFSON_EXCEPTION_H
#define JFSON_EXCEPTION_H

#include <exception>
#include <cassert>

namespace jfson
{

#define ERROR_MAP(XX) \
    XX(OK, "ok") \
    XX(ROOT_NOT_SINGULAR, "root not singular") \
    XX(BAD_VALUE, "bad value") \
    XX(EXPECT_VALUE, "expect value")

enum JsonError {
#define GEN_ERRNO(e, s) PARSE_##e,
    ERROR_MAP(GEN_ERRNO)
#undef GEN_ERRNO
};

inline const char* parseErrorStr(JsonError err)
{
    const static char* tab[] = {
#define GEN_STRERR(e, n) n,
            ERROR_MAP(GEN_STRERR)
#undef
    };
    assert(err >= 0 && err < sizeof(tab) / sizeof(tab[0]));
    return tab[err];
}

class Exception: public std::exception
{
public:
    explicit Exception(JsonError err):
            err_(err) {}

    const char* what() const noexcept { return parseErrorStr(err_); }
    JsonError err() const { return err_; }

private:
    JsonError err_;
};

#undef ERROR_MAP

}

#endif //JFSON_EXCEPTION_H

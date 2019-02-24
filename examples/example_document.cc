//
// Created by fl on 1/24/19.
//
#include <src/Document.h>
#include <src/JsonWriter.h>
#include <src/FileWriteStream.h>

#include "jsons.h"

using namespace jfson;

int main()
{
    Document document;
    ParseError err = document.parse(sample[0]);
    if (err != PARSE_OK) {
        puts(parseErrorStr(err));
        return 1;
    }

    FileWriteStream os(stdout);
    JsonWriter writer(os);
    document.writeTo(writer);
}

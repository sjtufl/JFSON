//
// Created by fl on 1/24/19.
//

#include <iostream>
#include <src/JsonReader.h>
#include <src/JsonWriter.h>
#include <src/FileReadStream.h>
#include <src/FileWriteStream.h>

#include "jsons.h"

using namespace jfson;

int main()
{
    FileReadStream is(stdin);
    FileWriteStream os(stdout);
    JsonWriter writer(os);

    ParseError err = JsonReader::parse(is, writer);
    if (err != PARSE_OK) {
        printf("%s\n", parseErrorStr(err));
    }
}





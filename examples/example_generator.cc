//
// Created by fl on 1/23/19.
//
#include <iostream>
#include <cstdio>

#include <src/JsonWriter.h>
#include <src/FileWriteStream.h>
//#include <src/PrettyWriter.h>

using namespace jfson;

int main()
{
    FileWriteStream ws(stdout);
    JsonWriter writer(ws);

    writer.StartArray();
    writer.Bool(false);
    writer.String("\0流弊\0"sv);
    writer.Bool(true);

    writer.StartObject();
    writer.Key("sjtu");
    writer.String("\0蛤elder蛤\0"sv);
    writer.EndObject();

    writer.Int32(2048);
    writer.Double(NAN);
    writer.EndArray();
}

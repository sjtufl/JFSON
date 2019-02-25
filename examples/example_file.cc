//
// Created by fl on 2/25/19.
//
#include <src/Document.h>
#include <src/FileReadStream.h>
#include <src/FileWriteStream.h>
#include <src/StringWriteStream.h>
#include <src/JsonWriter.h>

#include <iostream>

int main()
{
    FILE *is = fopen("canada.json", "r");
    if (is == nullptr)
        exit(1);
    jfson::Document doc;
    jfson::FileReadStream frs(is);
    fclose(is);

    if (doc.parseStream(frs) != jfson::PARSE_OK) {
        std::cout << "PARSE ERROR!" << std::endl;
        exit(1);
    }

    jfson::FileWriteStream fws(stdout);
    jfson::JsonWriter writer(fws);
    doc.writeTo(writer);

}

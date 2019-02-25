//
// Created by fl on 1/24/19.
//
#include <src/Document.h>
#include <src/JsonWriter.h>
#include <src/FileWriteStream.h>

#include <cstring>
#include <iostream>

#include "jsons.h"

using namespace jfson;

int main()
{

    Document document;
    ParseError err;
    err = document.parse(sample[0]);
    if (err != PARSE_OK) {
        puts(parseErrorStr(err));
        return 1;
    }

    FileWriteStream os(stdout);
    JsonWriter writer(os);
    document.writeTo(writer);

    Document docagain;
    ParseError err_docagain = docagain.parse(R"(
{
    "precision": "zip",
    "Latitude": 37.766800000000003,
    "Longitude": -122.3959,
    "Address": "Mountain View City",
    "City": "SAN FRANCISCO",
    "State": "CA",
    "Zip": "94107",
    "Country": "US",
    "Number": 0i32,
    "Number2": 0i64
}
)");

    if (err_docagain != PARSE_OK) {
        puts(parseErrorStr(err));
        exit(1);
    }


    JsonValue& state = docagain["State"];
    std::cout << "\nState: " << state.getStringView() << '\n';

    JsonValue& zip = docagain["Zip"];
    std::cout << "Zip: " << zip.getStringView() << "\n";

    zip.setInt32(9527);
    std::cout << "Zip: " << zip.getInt32() << "\n";

    docagain.addMember("123", "456");
    JsonValue& test = docagain["Address"];
    std::cout << "test: addMember: " << test.getStringView() << std::endl;
}

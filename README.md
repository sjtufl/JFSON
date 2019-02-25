## JFSON

JFSON is a light-weight JSON parser and generator, which support DOM (Document Object Model) as well as SAX (Simple API for XML) style API.

### Design

#### Core Concepts

Inspired by [RapidJSON](https://github.com/Tencent/rapidjson), JFSON defines several core concepts as follows:

* `Value`

  As the core concept for DOM API, `Value` is of variant type. An instance of `Value` could be any JSON data type.   

* `ReadStream`

  `ReadStream` is used for reading char stream as the input of JSON parser.`StringReadStream` and `FileReadStream` read chars from memory and file respectively.

* `WriteStream`

  Similar to `ReadStream`, `WriteStream`  outputs byte stream to memory (`StringWriteStream`) or file on disk (`FileWriteStream`).

* `Handler`

  `Handler` is critical for implementing SAX API. In nature, it's an events handler when parsing JSON stream. Class `Document` and Class `Writer` are implemented `Handler` in JFSON. However, users can build their own `Handler` according to their needs.

#### Parsing and Generating

* JFSON adopts recursive parser by default
  * Recursive parser if faster than iterative parser
* Support comprehensive error tips if parsing failed
* Support JSON string generation
* Reasonable memory overheads

#### Miscellaneous

* Make use of some new features of modern C++. 
  * std::string_view
  * rvalue reference
  * template
  * range-based loop

### Usage

#### Parsing JSON

For small sized JSON string or file, we could use DOM style API. Modifying the variable in a parsed JSON object can also be done via DOM API.

```c++
int main()
{
    Document doc;
    ParseError err = docagain.parse(R"(
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
    if (err != PARSE_OK) {
        puts(parseErrorStr(err));
        exit(1);
    }
    JsonValue& state = docagain["State"];
    std::cout << "State: " << state.getStringView() << '\n';

    JsonValue& zip = docagain["Zip"];
    std::cout << "Zip: " << zip.getStringView() << "\n";

    zip.setInt32(9527);
    std::cout << "Zip: " << zip.getInt32() << "\n";

    docagain.addMember("123", "456");
    JsonValue& test = docagain["Address"];
    std::cout << "test: addMember: " << test.getStringView() << std::endl;
}
```

Corresponding output:

```shell
State: CA
Zip: 94107
Zip: 9527
test: addMember: Mountain View City

Process finished with exit code 0
```

#### Generating JSON

We could leverage `Writer` to generate JSON string like this:

```c++
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
```

The corresponding output:

```shell
[false,"\u0000流弊\u0000",true,{"sjtu":"\u0000蛤elder蛤\u0000"},2048,NaN]
Process finished with exit code 0
```

### References

* [JSON tutorial](https://github.com/miloyip/json-tutorial) 从零开始的JSON库教程


add_library(jfson STATIC
        Exception.h
        FileReadStream.h FileWriteStream.h
        StringReadStream.h StringWriteStream.h
        JsonReader.cc JsonReader.h
        JsonWriter.cc JsonWriter.h
        JsonValue.cc JsonValue.h
        Document.h
        noncopyable.h
#        PrettyWriter.h
        )
install(TARGETS jfson DESTINATION lib)

set(HEADERS
        Document.h
        Exception.h
        FileReadStream.h
        FileWriteStream.h
        noncopyable.h
        PrettyWriter.h
        JsonReader.h
        StringReadStream.h
        StringWriteStream.h
        JsonValue.h
        JsonWriter.h
        )
install(FILES ${HEADERS} DESTINATION include)
add_executable(eg_parse example_parse.cc)
target_link_libraries(eg_parse jfson)

add_executable(eg_document example_document.cc)
target_link_libraries(eg_document jfson)

add_executable(eg_generate example_generator.cc)
target_link_libraries(eg_generate jfson)

add_executable(eg_file example_file.cc)
target_link_libraries(eg_file jfson)
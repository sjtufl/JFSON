## JFSON

JFSON is a light-weight JSON parser and generator, which support comments mode as well as standard mode. 

TODO:

* Gtest (basic functions & performance);


* API re-design;
* Support file i\o stream;
* Classes hierarchy re-design;
* Detailed documentation.



我想实现的JSON解析器应该有这些特点：

* 轻量级，性能良好，内存使用合理（value union，原位解析，短字符串优化, String_view优化）
* 遵守JSON RFC全部标准
* 支持内存字符流以及文件两种输入方式的解析
* 设计友好的API
* Writter支持prettyfy
* 尽量使用C++新特性

针对以上目标，设计如下：

Value类：存储JSON值

* Union存储
* Flag标识类型

InputStream类：抽象输入，统一为input stream

* next() 下一个字符

Reader类: 解析InputStream为DOM树 `Parsor`

* 递归下降的LL1

Writer类: 将DOM树输出为JSON字符串 `generator`
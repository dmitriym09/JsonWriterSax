# JsonWriterSax
This is module for write JSON document to QTextStream in SAX style

## Installation

- Copy folder jsonwritesax to you qt project
- Add to .pro file
```
include(./jsonwritersax/src/jsonwritersax.pri)
```
- Include header
```
#include "jsonwritersax.h"
using namespace md::json::writersax;
```

## Basic Usage
Library write JSON to QTextStream. All write function return boolean result of success. Сontrol of the uniqueness of object keys is not implemented now. Сode is documented with doxygen.

Best manual for programmist is an example of source code =)

### Create array
```
QByteArray ba;
QTextStream stream(&ba);
stream.setCodec("utf-8");
JsonWriterSax writer(stream);
writer.writeStartArray();
for(auto i = 0; i < 10; ++i) {
    writer.write(i);
}
writer.writeEndArray();
if(writer.end()) {
    stream.flush();
} else {
    qWarning() << "Error json";
}
```

Result
```
[0,1,2,3,4,5,6,7,8,9]
```

### Create object
```
QByteArray ba;
QTextStream stream(&ba);
stream.setCodec("utf-8");
JsonWriterSax writer(stream);
writer.writeStartObject();
for(auto i = 0; i < 5; ++i) {
    writer.write(QString::number(i), i);
}
for(auto i = 5; i < 10; ++i) {
    writer.write(QString::number(i), QString::number(i));
}
writer.writeKey("arr");
writer.writeStartArray();
writer.writeEndArray();
writer.writeKey("o");
writer.writeStartObject();
writer.writeEndObject();
writer.writeKey("n");
writer.writeNull();
writer.write(QString::number(11), QVariant(11));
writer.write("dt", QVariant(QDateTime::fromMSecsSinceEpoch(10)));
writer.writeEndObject();
if(writer.end()) {
    stream.flush();
} else {
    qWarning() << "Error json";
}
```
Result
```
{"0":0,"1":1,"2":2,"3":3,"4":4,"5":"5","6":"6","7":"7","8":"8","9":"9","arr":[],"o":{},"n":null,"11":11,"dt":"1970-01-01T03:00:00.010"}
```

## QJsonDocument limitations

For large JSON documemt Qt write error message 

`
QJson: Document too large to store in data structure
`

As example last element not append to array

```
auto max = std::numeric_limits<int>::max();
QJsonArray ja;
for(auto i = 0; i < max; ++i) {
    ja.append(i);
    if(ja.size() - 1 != i) {
        break;
    }
}
qDebug() << ja.size() << max;
```

out:
`
33554424 2147483647
`

I think the design of error checking is not very comfortable. For array I must check size, for object - check last key.

## Benchmarks

Using QBENCHMARK in release. You can see tests JsonWriterSaxTest.

### elementary OS 5.0 Juno, kernel 4.15.0-38-generic, cpu Intel(R) Core(TM)2 Quad CPU 9550 @ 2.83GHz, 4G RAM, Qt 5.11.2 GCC 5.3.1

#### Long number array

- QJsonDocument: 42 msecs per iteration (total: 85, iterations: 2)
- JsonWriterSax: 23 msecs per iteration (total: 93, iterations: 4)

#### Big one-level object 

- QJsonDocument: 1,170 msecs per iteration (total: 1,170, iterations: 1)
- JsonWriterSax: 53 msecs per iteration (total: 53, iterations: 1)

#### Big complex document

- QJsonDocument: 1,369 msecs per iteration (total: 1,369, iterations: 1)
- JsonWriterSax: 463 msecs per iteration (total: 463, iterations: 1)

### elementary OS 5.0 Juno, kernel 4.15.0-38-generic, cpu Intel(R) Core(TM) i7-7500U CPU @ 2.70GHz, 8G RAM, Qt 5.11.2 GCC 5.3.1

#### Long number array

- QJsonDocument: 29.5 msecs per iteration (total: 118, iterations: 4)
- JsonWriterSax: 13 msecs per iteration (total: 52, iterations: 4)

#### Big one-level object 

- QJsonDocument: 485 msecs per iteration (total: 485, iterations: 1)
- JsonWriterSax: 31 msecs per iteration (total: 62, iterations: 2)

#### Big complex document

- QJsonDocument: 734 msecs per iteration (total: 734, iterations: 1)
- JsonWriterSax: 271 msecs per iteration (total: 271, iterations: 1)

### MS Windows 7 SP1, cpu Intel(R) Core(TM) i7-4770 CPU @ 3.40GHz, 8G RAM, Qt 5.11.0 GCC 5.3.0

#### Long number array

- QJsonDocument: 669 msecs per iteration (total: 669, iterations: 1)
- JsonWriterSax: 20 msecs per iteration (total: 81, iterations: 4)

#### Big one-level object 

- QJsonDocument: 1,568 msecs per iteration (total: 1,568, iterations: 1)
- JsonWriterSax: 44 msecs per iteration (total: 88, iterations: 2)

#### Big complex document

- QJsonDocument: 1,167 msecs per iteration (total: 1,167, iterations: 1)
- JsonWriterSax: 375 msecs per iteration (total: 375, iterations: 1)

### MS Windows 7 SP1, cpu Intel(R) Core(TM) i3-3220 CPU @ 3.30GHz, 8G RAM, Qt 5.11.0 GCC 5.3.0

#### Long number array

- QJsonDocument: 772 msecs per iteration (total: 772, iterations: 1)
- JsonWriterSax: 26 msecs per iteration (total: 52, iterations: 2)

#### Big one-level object 

- QJsonDocument: 2,029 msecs per iteration (total: 2,029, iterations: 1)
- JsonWriterSax: 59 msecs per iteration (total: 59, iterations: 1)

#### Big complex document

- QJsonDocument: 1,530 msecs per iteration (total: 1,530, iterations: 1)
- JsonWriterSax: 495 msecs per iteration (total: 495, iterations: 1)

## Examples

### Create сomplex JSON document

```
QByteArray ba;
QTextStream stream(&ba);
stream.setCodec("utf-8");
JsonWriterSax writer(stream);
writer.writeStartArray();
for(auto i = 0; i < 1000; ++i) {
    writer.writeStartObject();
    writer.writeKey("key");
    writer.writeStartObject();
    for(auto j = 0; j < 1000; ++j) {
        writer.write(QString::number(j), j);
    }
    writer.writeEndObject();
    writer.writeEndObject();
}
writer.writeEndArray();
if(writer.end()) {
    stream.flush();
} else {
    qWarning() << "Error json";
}
```


## Change Log
### 0.0.1

- Published for community

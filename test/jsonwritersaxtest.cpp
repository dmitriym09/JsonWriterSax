#include "jsonwritersaxtest.h"

#include <qjsondocument.h>
#include <QtTest/QtTest>
#include <limits>

using namespace md::json::writersax;

bool md::json::writersax::testJSON(const QByteArray& ba) {
    QJsonParseError jpe;
    QJsonDocument jd = QJsonDocument::fromJson(ba, &jpe);
    if(jpe.error != QJsonParseError::NoError) {
        qWarning() << ba << jpe.errorString();
    }
    return jpe.error == QJsonParseError::NoError;
}

/*void JsonWriterSaxTest::errorQtTest() {
    auto max = std::numeric_limits<int>::max();
    QJsonArray ja;
    for(auto i = 0; i < max; ++i) {
        ja.append(i);

        if(ja.size() - 1 != i) {
            break;
        }
    }
    qDebug() << ja.size() << max;
    QVERIFY2(max != ja.size(), QString::number(ja.size()).toLatin1());
}*/

void JsonWriterSaxTest::simpleTest() {
    qDebug() << QSysInfo::prettyProductName()
             << QSysInfo::kernelVersion();

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);

        QVERIFY(writer.writeStartObject());

        QVERIFY(writer.write("key", "value"));
        QVERIFY(writer.write("key1", "value1"));

        {
            QVERIFY(writer.writeKey("o"));

            QVERIFY(writer.writeStartObject());
            QVERIFY(writer.write("key11", "value11"));

            QVERIFY(writer.writeKey("o1"));
            QVERIFY(writer.writeStartObject());
            QVERIFY(writer.write("key111", "value111"));
            QVERIFY(writer.write("key1112", "value1112"));
            QVERIFY(writer.write("key1113", "value1113"));
            QVERIFY(writer.write("key1114", "value1114"));
            QVERIFY(writer.writeKey("n"));
            QVERIFY(writer.writeNull());
            QVERIFY(writer.writeEndObject());

            QVERIFY(writer.writeEndObject());
        }

        {
            QVERIFY(writer.writeKey("arr"));
            QVERIFY(writer.writeStartArray());
            QVERIFY(writer.write("arrVal"));
            QVERIFY(writer.write("arrVal1"));
            QVERIFY(writer.write("arrVal2"));
            QVERIFY(writer.write(L"Привет"));
            QVERIFY(writer.write(1));
            QVERIFY(writer.write(1.1));
            QVERIFY(writer.write(-1));
            QVERIFY(writer.write(-1.1));
            QVERIFY(writer.write(QVariant()));
            QVERIFY(writer.write(QVariant(true)));
            QVERIFY(writer.write(QVariant(false)));
            QVERIFY(writer.write(true));
            QVERIFY(writer.write(false));

            {
                QVERIFY(writer.writeStartArray());
                QVERIFY(writer.writeEndArray());
            }

            {
                QVERIFY(writer.writeStartObject());
                QVERIFY(writer.writeEndObject());
            }

            {
                QVERIFY(writer.writeStartArray());
                QVERIFY(writer.write("arrVal"));
                QVERIFY(writer.write("arrVal1"));
                QVERIFY(writer.write("arrVal2"));
                QVERIFY(writer.writeEndArray());
            }

            {
                QVERIFY(writer.writeStartObject());
                QVERIFY(writer.writeKey("key111"));
                QVERIFY(writer.write("value111"));

                QVERIFY(writer.writeKey("v"));
                QVERIFY(writer.writeStartArray());
                QVERIFY(writer.write("arrVal"));
                QVERIFY(writer.writeNull());
                QVERIFY(writer.write("arrVal1"));
                QVERIFY(writer.write("arrVal2"));
                QVERIFY(writer.writeEndArray());

                QVERIFY(writer.writeEndObject());
            }

            QVERIFY(writer.writeEndArray());
        }

        QVERIFY(writer.writeEndObject());

        QVERIFY(writer.end());

        stream.flush();

        QVERIFY2(testJSON(ba), ba);

        qDebug() << ba;
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);

        QVERIFY(writer.write("hi"));

        QVERIFY(writer.end());

        stream.flush();

        qDebug() << ba;
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);

        QVERIFY(writer.writeStartArray());

        {
            for(auto i = 0; i < 10; ++i) {
                QVERIFY(writer.writeStartObject());
                writer.writeKey("key");
                writer.writeStartObject();
                for(auto j = 0; j < 2; ++j) {
                    writer.write(QString::number(j), j * 10);
                }
                writer.writeEndObject();

                QVERIFY(writer.writeEndObject());
            }
        }

        {
            QVERIFY(writer.writeStartArray());
            for(auto i = 0; i < 10; ++i) {
                QVERIFY(writer.writeStartArray());
                for(auto j = 0; j < 10; ++j) {
                    QVERIFY(writer.writeStartArray());
                    QVERIFY(writer.writeEndArray());
                }
                QVERIFY(writer.writeEndArray());
            }
            QVERIFY(writer.writeEndArray());

            for(auto i = 0; i < 10; ++i) {
                QVERIFY(writer.writeStartObject());
                QVERIFY(writer.writeEndObject());
            }

            QVERIFY(writer.writeStartObject());

            {
                QVERIFY(writer.writeKey("key"));
                QVERIFY(writer.writeStartObject());
                QVERIFY(writer.writeEndObject());
            }

            {
                QVERIFY(writer.writeKey("key1"));
                QVERIFY(writer.writeStartArray());
                QVERIFY(writer.writeEndArray());
            }

            {
                QVERIFY(writer.writeKey("key1"));
                QVERIFY(writer.writeStartArray());
                {
                    QVERIFY(writer.writeStartArray());
                    QVERIFY(writer.writeEndArray());
                }
                {
                    QVERIFY(writer.writeStartObject());
                    QVERIFY(writer.writeEndObject());
                }
                QVERIFY(writer.writeNull());
                QVERIFY(writer.writeEndArray());
            }

            {
                QVERIFY(writer.writeKey("key1"));
                {
                    QVERIFY(writer.writeStartObject());
                    QVERIFY(writer.writeEndObject());
                }
                QVERIFY(writer.writeKey("key2"));
                {
                    QVERIFY(writer.writeStartArray());
                    QVERIFY(writer.writeEndArray());
                }
                QVERIFY(writer.writeKey("key3"));
                QVERIFY(writer.writeNull());
            }

            QVERIFY(writer.writeEndObject());
        }

        QVERIFY(writer.writeEndArray());

        QVERIFY(writer.end());

        stream.flush();

        QVERIFY2(testJSON(ba), ba);

        qDebug() << ba;
    }
}

/*void JsonWriterSaxTest::longArray() {
    auto max = std::numeric_limits<int>::max();

    QByteArray ba;
    QTextStream stream(&ba);
    stream.setCodec("utf-8");
    JsonWriterSax writer(stream);

    QVERIFY(writer.writeStartArray());
    for(auto i = 0; i < max; ++i) {
        QVERIFY(writer.write(i));

        if((i % 1000000) == 0) {
            qDebug() << i << ba.size();
        }
    }
    QVERIFY(writer.writeEndArray());

    QVERIFY(writer.end());

    stream.flush();
}*/

void JsonWriterSaxTest::examples() {
    //array
    {
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

        qInfo() << ba;
        QVERIFY(ba == "[0,1,2,3,4,5,6,7,8,9]");
    }

    //object
    {
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

        qInfo() << ba;
        QVERIFY(ba == "{\"0\":0,\"1\":1,\"2\":2,\"3\":3,\"4\":4,\"5\":\"5\",\"6\":\"6\",\"7\":\"7\",\"8\":\"8\",\"9\":\"9\",\"arr\":[],\"o\":{},\"n\":null,\"11\":11,\"dt\":\"1970-01-01T03:00:00.010\"}");
    }
}

void JsonWriterSaxTest::notAdmissibleStepsTest() {

    //objects
    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(!writer.writeEndObject());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartObject());
        QVERIFY(!writer.writeStartObject());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartObject());
        QVERIFY(!writer.write(""));
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartObject());
        QVERIFY(!writer.end());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartObject());
        QVERIFY(writer.writeKey("k"));
        QVERIFY(writer.write("v"));
        QVERIFY(!writer.end());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartObject());
        QVERIFY(writer.writeKey("k"));
        QVERIFY(!writer.end());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartObject());
        QVERIFY(writer.writeKey("k"));
        QVERIFY(!writer.writeEndObject());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(!writer.writeKey("key"));
    }

    //array
    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(!writer.writeEndArray());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartArray());
        QVERIFY(writer.writeEndArray());
        QVERIFY(!writer.writeEndArray());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartArray());
        QVERIFY(!writer.writeKey(""));
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(writer.writeStartArray());
        QVERIFY(writer.writeEndArray());

        QVERIFY(!writer.writeStartObject());
    }

    {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);
        QVERIFY(!writer.end());
    }
}

void JsonWriterSaxTest::benchmarkLongArray() {
    QBENCHMARK {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);

        writer.writeStartArray();
        for(auto i = 0; i < 100000; ++i) {
            writer.write(i);
        }
        writer.writeEndArray();

        QVERIFY(writer.end());

        stream.flush();
    }
}

void JsonWriterSaxTest::benchmarkLongArrayQT() {
    QBENCHMARK {
        QJsonDocument jd;

        QJsonArray ja;

        for(auto i = 0; i < 100000; ++i) {
            ja.append(i);
        }
        jd.setArray(ja);

        auto t = jd.toJson();
    }
}

void JsonWriterSaxTest::benchmarkObject() {
    QBENCHMARK {
        QByteArray ba;
        QTextStream stream(&ba);
        stream.setCodec("utf-8");
        JsonWriterSax writer(stream);

        writer.writeStartObject();
        for(auto i = 0; i < 100000; ++i) {
            writer.write(QString::number(i), i);
        }
        writer.writeEndObject();

        QVERIFY(writer.end());

        stream.flush();
    }
}

void JsonWriterSaxTest::benchmarkObjectQT() {
    QBENCHMARK {
        QJsonDocument jd;

        QJsonObject jo;

        for(auto i = 0; i < 100000; ++i) {
            jo.insert(QString::number(i), i);
        }
        jd.setObject(jo);

        auto t = jd.toJson();
    }
}

void JsonWriterSaxTest::benchmarkComplexJson() {
    QBENCHMARK {
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

        QVERIFY(writer.end());

        stream.flush();
    }
}

void JsonWriterSaxTest::benchmarkComplexJsonQT() {
    QBENCHMARK {
        QJsonDocument jd;

        QJsonArray ja;

        for(auto i = 0; i < 1000; ++i) {
            QJsonObject jo;
            QJsonObject jo1;

            for(auto j = 0; j < 1000; ++j) {
                jo1.insert(QString::number(j), j);
            }

            jo.insert("key", jo1);

            ja.append(jo);
        }

        jd.setArray(ja);

        auto t = jd.toJson();
    }
}

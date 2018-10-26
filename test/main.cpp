#include <QtCore>
#include <QtTest/QtTest>

#include "jsonwritersaxtest.h"

using namespace md::json::writersax;

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    return QTest::qExec(new JsonWriterSaxTest, argc, argv);
}

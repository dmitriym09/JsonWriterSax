#ifndef JSONWRITERSAXTEST_H
#define JSONWRITERSAXTEST_H

#include <QObject>

#include "jsonwritersax.h"

namespace md {
namespace json {
namespace writersax {

bool testJSON(const QByteArray& ba);

/*!
 * \brief This is test for write JsonWriterSaxTest
 * \author dmitriym09
 * \version 0.0.1
 * \date 15.06.2018
 * \copyright MIT
 */
class JsonWriterSaxTest : public QObject {
    Q_OBJECT

private slots:
    //void errorQtTest();
    void simpleTest();
    //void longArray();
    void examples();
    void notAdmissibleStepsTest();
    void benchmarkLongArray();
    void benchmarkLongArrayQT();
    void benchmarkObject();
    void benchmarkObjectQT();
    void benchmarkComplexJson();
    void benchmarkComplexJsonQT();
};

}
}
}

#endif // JSONWRITERSAXTEST_H

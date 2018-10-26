#ifndef JSONWRITERSAX_H
#define JSONWRITERSAX_H

#include <QtCore>
#include <functional>
#include <map>
#include <set>

namespace md {
namespace json {
namespace writersax {

enum State {
    start,
    startArray,
    endArray,
    startObject,
    endObject,
    key,
    arrayValue,
    objectValue,
    value,
    end
};

QDebug operator<<(QDebug debug, State state);

static std::map<State, std::set<State>> admissibleSteps = {
    {
        State::start, {
            State::startArray,
            State::startObject,
            State::value,
            State::end
        }
    },
    {
        State::startObject, {
            State::key,
            State::endObject
        }
    },
    {
        State::key, {
            State::startArray,
            State::startObject,
            State::objectValue
        }
    },
    {
        State::objectValue, {
            State::key,
            State::endObject
        }
    },
    {
        State::startArray, {
            State::startObject,
            State::startArray,
            State::endArray,
            State::arrayValue
        }
    },
    {
        State::arrayValue, {
            State::arrayValue,
            State::endArray,
            State::startArray,
            State::startObject
        }
    },
    {
        State::value, {
            State::end
        }
    }
};

/*!
 * \brief This is class for write JSON document to QTextStream in SAX style.
 * \author dmitriym09
 * \version 0.0.1
 * \date 26.10.2018
 * \copyright MIT
 */
class JsonWriterSax {
    Q_DISABLE_COPY(JsonWriterSax)

    bool m_hasError;
    bool m_isStart;

    QTextStream& m_stream;
    QStack<State> m_stackStatus;

private:
    void normalizeAndWriteString(const QString& str);
    bool checkState(State state);
    bool beforeValue();

public:

    /*!
     * \brief JsonWriterSax constructor
     * \param [in] stream - Text stream for writing json data.
     */
    JsonWriterSax(QTextStream& stream);

    /*!
     * \brief JsonWriterSax destuctor
     */
    ~JsonWriterSax();

    /*!
      * \brief This method return Text stream for writing json data.
      * \return Text stream
      */
    QTextStream &stream();

    /*!
      * \brief Start JSON object - write "{"
      * \return Success
      */
    bool writeStartObject();

    /*!
      * \brief End JSON object - write "}"
      * \return SuccessIN
      */
    bool writeEndObject();

    /*!
      * \brief Write key in JSON object. Must be call after writeStartObject()
      * \param [in] key - key
      * \return Success
      */
    bool writeKey(const QString& key);

    /*!
      * \brief Start JSON array - write "["
      * \return Success
      */
    bool writeStartArray();

    /*!
      * \brief End JSON array - write "]"
      * \return Success
      */
    bool writeEndArray();

    /*!
      * \brief Write value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. If variant type is null - write null, if variant is a QString or
      * number (int, double, etc) or bool - write according to specification, else - call QVariant::toString() and
      * write result
      * \return Success
      */
    bool write(const QVariant& value);

    /*!
      * \brief Write bool value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const bool value);

    /*!
      * \brief Write null value in JSON document or in JSON object after key or in JSON array
      * \return Success
      */
    bool writeNull();

    /*!
      * \brief Write string value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& value);

    /*!
      * \brief Write string value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QLatin1String& value);

    /*!
      * \brief Write string value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const char *value);

    /*!
      * \brief Write string value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const wchar_t* value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const qint8 value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const qint16 value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const qint32 value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const qint64& value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const quint8 value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const quint16 value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const quint32 value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const quint64& value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const float& value);

    /*!
      * \brief Write number value in JSON document or in JSON object after key or in JSON array
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const double& value);

    /*!
      * \brief Write key and string value in JSON object. Equals call writeKey(const QString&) and write(const QString&)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const QString& value);

    /*!
      * \brief Write key and string value in JSON object. Equals call writeKey(const QString&) and write(const char*)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const char *value);

    /*!
      * \brief Write key and string value in JSON object. Equals call writeKey(const QString&) and write(const wchar_t*)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const wchar_t* value);

    /*!
      * \brief Write key and number value in JSON object. Equals call writeKey(const QString&) and write(const qint8)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const qint8 value);

    /*!
      * \brief Write key and number value in JSON object. Equals call writeKey(const QString&) and write(const qint16)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const qint16 value);

    /*!
      * \brief Write key and number value in JSON object. Equals call writeKey(const QString&) and write(const qint32)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const qint32 value);

    /*!
      * \brief Write key and number value in JSON object. Equals call writeKey(const QString&) and write(const qint64&)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const qint64& value);

    /*!
      * \brief Write key and number value in JSON object Equals call writeKey(const QString&) and write(const quint16)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const quint16 value);

    /*!
      * \brief Write key and number value in JSON object. Equals call writeKey(const QString&) and write(const quint32)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const quint32 value);

    /*!
      * \brief Write key and number value in JSON object. Equals call writeKey(const QString&) and write(const quint64&)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const quint64& value);

    /*!
      * \brief Write key and number value in JSON object. Equals call writeKey(const QString&) and write(const float&)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const float& value);

    /*!
      * \brief Write key and number value in JSON object. Equals call writeKey(const QString&) and write(const double&)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const double& value);

    /*!
      * \brief Write key and bool value in JSON object. Equals call writeKey(const QString&) and write(const bool)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const bool value);

    /*!
      * \brief Write key and variant value in JSON object. Equals call writeKey(const QString&) and write(const QVariant&)
      * \param [in] key - key. Write according to specification
      * \param [in] value - value. Write according to specification
      * \return Success
      */
    bool write(const QString& key, const QVariant& value);

    /*!
      * \brief Return an error
      * \return Return an error
      */
    bool hasError() const;

    /*!
      * \brief Finished write document
      * \return Success
      */
    bool end();
};

}
}
}

#endif // JSONWRITERSAX_H

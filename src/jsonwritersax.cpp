#include "jsonwritersax.h"

using namespace md::json::writersax;

QDebug md::json::writersax::operator<<(QDebug debug, State state) {
    switch (state) {
    case State::start:
        debug << "State::start";
        break;

    case State::startArray:
        debug << "State::startArray";
        break;

    case State::endArray:
        debug << "State::endArray";
        break;

    case State::startObject:
        debug << "State::startObject";
        break;

    case State::endObject:
        debug << "State::endObject";
        break;

    case State::key:
        debug << "State::key";
        break;

    case State::arrayValue:
        debug << "State::arrayValue";
        break;

    case State::objectValue:
        debug << "State::objectValue";
        break;

    case State::value:
        debug << "State::value";
        break;

    case State::end:
        debug << "State::end";
        break;
    }

    return debug;
}

JsonWriterSax::JsonWriterSax(QTextStream &stream) :
    m_hasError(false),
    m_isStart(true),
    m_stream(stream)
  //m_serializers(serializers),
{}

JsonWriterSax::~JsonWriterSax() {}

void JsonWriterSax::normalizeAndWriteString(const QString& str) {
    if(this->hasError()) {
        return;
    }

    for(auto ch: str) {
        if(ch == '\\'){
            this->m_stream << R"(\\)";
        }
        else if(ch == '"') {
            this->m_stream << R"(\")";
        }
        else if(ch == '\t') {
            this->m_stream << R"(\t)";
        }
        else if(ch == '\n') {
            this->m_stream << R"(\n)";
        }
        else if(ch == '\r') {
            this->m_stream << R"(\r)";
        }
        else if(ch == '\b') {
            this->m_stream << R"(\b)";
        }
        else if(ch == '\f') {
            this->m_stream << R"(\f)";
        }
        else {
            this->m_stream << ch;
        }
    }
}

QTextStream& JsonWriterSax::stream() {
    return this->m_stream;
}

bool JsonWriterSax::writeStartObject() {
    if(this->hasError()) {
        return false;
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::arrayValue) {
        this->m_stream << ",";
    }

    if(!this->checkState(State::startObject)) {
        this->m_hasError = true;
        return false;
    }

    this->m_stream << "{";

    return true;
}

bool JsonWriterSax::writeKey(const QString& key) {
    //TODO: add uniq key

    if(this->hasError()) {
        return false;
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::objectValue) {
        this->m_stream << ",";
    }

    if(!this->checkState(State::key)) {
        this->m_hasError = true;
        return false;
    }

    this->m_stream << R"(")";
    this->normalizeAndWriteString(key);
    this->m_stream << R"(":)";

    return true;
}

bool JsonWriterSax::writeEndObject() {
    if(this->hasError()) {
        return false;
    }

    if(!this->checkState(State::endObject)) {
        this->m_hasError = true;
        return false;
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::endObject) {
        this->m_stackStatus.pop();
    }

    while(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() != State::startObject) {
        this->m_stackStatus.pop();
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::startObject) {
        this->m_stackStatus.pop();
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::key) {
        this->m_stackStatus.pop();
        this->m_stackStatus.push(State::objectValue);
    }

    this->m_stream << "}";

    return true;
}

bool JsonWriterSax::writeStartArray() {
    if(this->hasError()) {
        return false;
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::arrayValue) {
        this->m_stream << ",";
    }

    if(!this->checkState(State::startArray)) {
        this->m_hasError = true;
        return false;
    }

    this->m_stream << "[";

    return true;
}

bool JsonWriterSax::writeEndArray() {
    if(this->hasError()) {
        return false;
    }

    if(!this->checkState(State::endArray)) {
        this->m_hasError = true;
        return false;
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::endArray) {
        this->m_stackStatus.pop();
    }

    while(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() != State::startArray) {
        this->m_stackStatus.pop();
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::startArray) {
        this->m_stackStatus.pop();
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::key) {
        this->m_stackStatus.pop();
    }

    this->m_stream << "]";

    return true;
}

bool JsonWriterSax::beforeValue() {
    if(this->hasError()) {
        return false;
    }

    if(!this->m_stackStatus.isEmpty() && this->m_stackStatus.top() == State::arrayValue) {
        this->m_stream << ",";
    }

    if(!this->checkState(State::value)) {
        this->m_hasError = true;
        return false;
    }

    return true;
}

bool JsonWriterSax::write(const QVariant& value) {
    if(value.isNull()){
        return this->writeNull();
    }

    //TODO: impl
    switch (value.type()) {

    case QVariant::Invalid:
        return this->writeNull();

    case QVariant::Bool:
        return this->write(value.toBool());

    case QVariant::Int:
        return this->write(value.toInt());

    case QVariant::Double:
        return this->write(value.toDouble());

    case QVariant::String:
        return this->write(value.toString());

    case QVariant::UInt:
        return this->write(value.toUInt());

    case QVariant::ULongLong:
        return this->write(value.toULongLong());


    default:
        break;

    }

    if(false) {
        //TODO: impl
    }

    return this->write(value.toString());
}

bool JsonWriterSax::hasError() const {
    return this->m_hasError;
}

bool JsonWriterSax::checkState(State state) {
    State lastState = this->m_stackStatus.isEmpty() ? State::start : this->m_stackStatus.top();

    if(lastState == State::start) {
        if(this->m_isStart) {
            if(state == State::end) {
                qWarning() << "empty json";
                return false;
            }
            this->m_isStart = false;
        }
        else if(state != State::end) {
            qWarning() << lastState << "yet starting";
            return false;
        }
    }

    auto lastStateIter = admissibleSteps.find(lastState);
    if(lastStateIter == admissibleSteps.end()) {
        qWarning() << lastState << "not allow";
        return false;
    }
    State nextState = state;
    if(lastState == State::key && nextState == State::value) {
        nextState = State::objectValue;
    }
    else if((lastState == State::startArray || lastState == State::arrayValue) && nextState == State::value) {
        nextState = State::arrayValue;
    }

    else if(lastState == State::startArray && (nextState == State::startObject || nextState == State::startArray)) {
        this->m_stackStatus.push(State::arrayValue);
    }

    if(state == State::key && this->m_stackStatus.size() > 2 && this->m_stackStatus.top() == State::value) {
        this->m_stackStatus.pop();
        this->m_stackStatus.pop();
    }

    if(lastStateIter->second.find(nextState) == lastStateIter->second.end()) {
        qWarning() << lastState << nextState << "not allow";
        return false;
    }

    if(!(nextState == lastState
         && (nextState == State::arrayValue || nextState == State::objectValue || nextState == State::value)
         )
            ){
        this->m_stackStatus.push(nextState);
    }

    return true;
}

bool JsonWriterSax::end() {
    if(this->hasError()) {
        return false;
    }

    return this->checkState(State::end);
}

bool JsonWriterSax::write(const QString& value) {
    if(!this->beforeValue()) {
        return false;
    }
    this->m_stream << "\"";
    this->normalizeAndWriteString(value);
    this->m_stream << "\"";

    return true;
}

bool JsonWriterSax::write(const char* value) {
    return this->write(QString::fromUtf8(value));
}

bool JsonWriterSax::write(const wchar_t* value) {
    //TODO: test
    return this->write(QString::fromWCharArray(value));
}

bool JsonWriterSax::write(const qint8 value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const qint16 value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const qint32 value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const qint64 &value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const quint8 value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const quint16 value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const quint32 value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const quint64& value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const double& value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const float& value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << value;

    return true;
}

bool JsonWriterSax::write(const bool value) {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << (value ? "true" : "false");

    return true;
}

bool JsonWriterSax::writeNull() {
    if(!this->beforeValue()) {
        return false;
    }

    this->m_stream << "null";

    return true;
}

bool JsonWriterSax::write(const QString& key, const QString& value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const char *value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const wchar_t* value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const qint8 value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const qint16 value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const qint32 value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const qint64& value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const quint16 value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const quint32 value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const quint64& value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const float& value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const double& value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const bool value) {
    return this->writeKey(key) && this->write(value);
}

bool JsonWriterSax::write(const QString& key, const QVariant& value) {
    return this->writeKey(key) && this->write(value);
}

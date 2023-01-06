#pragma once

#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "FormatCompat.h"
#include "Error.h"

namespace Config {


namespace JSON {

// www.json.org/json-en.html


enum class JSONTokenType
{
    BEGIN_OBJECT,
    END_OBJECT,

    VALUE_SEPRATOR,
    NAME_SEPRATOR,

    VALUE_STRING,
    VALUE_NUMBER,

    LITERAL_TRUE,
    LITERAL_FALSE,
    LITERAL_NIL,

    BEGIN_ARRAY,
    END_ARRAY,

    END_OF_SOURCE,
};


class JSONElement;
using Char = wchar_t;
using String = std::wstring;
using SStream = std::wstringstream;
using OStream = std::wostream;
using Object = std::map<String, std::unique_ptr<JSONElement>, std::less<>>;
using Array = std::vector<std::unique_ptr<JSONElement>>;


class Scanner
{
public:
    explicit Scanner(String&& source)
        : m_source(std::move(source))
        , m_current() {
    }
    explicit Scanner(const String& source)
        : m_source(source)
        , m_current() {
    }

    JSONTokenType Scan() {
        using enum JSONTokenType;

        if (IsAtEnd())
            return END_OF_SOURCE;

        m_prev_pos = m_current;

        switch (auto c = Advance()) {
        case '{': return BEGIN_OBJECT;
        case '}': return END_OBJECT;
        case '[': return BEGIN_ARRAY;
        case ']': return END_ARRAY;
        case ':': return NAME_SEPRATOR;
        case ',': return VALUE_SEPRATOR;
        case 't':
            ScanTrue();
            return LITERAL_TRUE;
        case 'f':
            ScanFalse();
            return LITERAL_FALSE;
        case 'n':
            ScanNIL();
            return LITERAL_NIL;
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            return Scan();
        case '\"':
            ScanString();
            return VALUE_STRING;
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            ScanNumber();
            return VALUE_NUMBER;
        default:
            Error(Format(L"Unsupported Token: {}", c));
        }
        Error(L"Unreachable!");
        return Scan();
    }

    void Rollback() { m_current = m_prev_pos; }

    String GetString() const { return m_string; }

    double GetNumber() const { return m_number; }

private:
    bool IsAtEnd(size_t x = 0) const { return m_current + x >= m_source.size(); }

    Char Advance() { return m_source[m_current++]; }

    void ScanString() {
        size_t pos = m_current;
        while (Peek() != '\"' && !IsAtEnd())
            Advance();
        if (IsAtEnd())
            Error(L"Invalid string: missing closing quote!");
        Advance();
        m_string = m_source.substr(pos, m_current - pos - 1);
    }

    void ScanNumber() {
        size_t pos = m_current - 1;
        while (::isdigit(Peek()))
            Advance();
        if (IsAtEnd())
            Error(L"Invalid string: missing closing quote!");
        if (Peek() == '.' && ::isdigit(Peek(1))) {
            Advance();
            while (::isdigit(Peek()))
                Advance();
        }
        m_number = std::stod(m_source.substr(pos, m_current - pos));
    }

    void ScanTrue() {
        if (m_source.compare(m_current, 3, L"rue") == 0)
            m_current += 3;
        else
            Error(L"Scan `true` error");
    }

    void ScanFalse() {
        if (m_source.compare(m_current, 4, L"alse") == 0)
            m_current += 4;
        else
            Error(L"Scan `false` error");
    }

    void ScanNIL() {
        if (m_source.compare(m_current, 3, L"ull") == 0)
            m_current += 3;
        else
            Error(L"Scan `null` error");
    }

    Char Peek(size_t x = 0) {
        if (IsAtEnd(x))
            return '\0';
        return m_source[m_current + x];
    }

    String m_source;
    size_t m_current;
    size_t m_prev_pos;
    String m_string;
    double m_number;
};


class JSONElement
{
public:
    JSONElement()
        : m_type(Type::NIL), m_number(0) {
    }
    /*explicit JSONElement(std::unique_ptr<Object> value) {
    SetValue(std::move(value)); } explicit JSONElement(std::unique_ptr<Array>
    value) { SetValue(std::move(value)); } explicit
    JSONElement(std::unique_ptr<String> value) { SetValue(std::move(value)); }
    explicit JSONElement(double value) { SetValue(value); }
    explicit JSONElement(bool value) { SetValue(value); }*/
    ~JSONElement() {};

    void SetValue(std::unique_ptr<Object> value) {
        m_type = Type::OBJECT;
        m_object = std::move(value);
    }
    void SetValue(std::unique_ptr<Array> value) {
        m_type = Type::ARRAY;
        m_array = std::move(value);
    }
    void SetValue(std::unique_ptr<String> value) {
        m_type = Type::STRING;
        m_string = std::move(value);
    }
    void SetValue(double value) {
        m_type = Type::NUMBER;
        m_number = value;
    }
    void SetValue(bool value) {
        m_type = Type::BOOL;
        m_boolean = value;
    }

    std::unique_ptr<Object> AsObject() {
        if (m_type == Type::OBJECT)
            return std::move(m_object);
        Error(L"Type of JSONElement is not OBJECT!");
        return nullptr;
    }

    std::unique_ptr<Array> AsArray() {
        if (m_type == Type::ARRAY)
            return std::move(m_array);
        Error(L"Type of JSONElement is not ARRAY!");
        return nullptr;
    }

    std::unique_ptr<String> AsString() {
        if (m_type == Type::STRING)
            return std::move(m_string);
        Error(L"Type of JSONElement is not STRING!");
        return nullptr;
    }

    double AsNumber() const {
        if (m_type == Type::NUMBER) return m_number;
        Error(L"Type of JSONElement is not NUMBER!");
        return 0.0;
    }

    bool AsBool() const {
        if (m_type == Type::BOOL) return m_boolean;
        Error(L"Type of JSONElement is not BOOL!");
        return false;
    }

    friend OStream& operator<<(OStream& os, const Object& object) {
        os << "{";
        for (auto it = object.begin(); it != object.end(); it++) {
            os << Format(LR"("{}": {})", it->first, it->second->Dumps());
            if (it != --object.end()) os << L", ";
        }
        os << "}";
        return os;
    }

    friend OStream& operator<<(OStream& os, const Array& array) {
        os << "[";
        for (auto it = array.begin(); it != array.end(); it++) {
            os << (*it)->Dumps();
            if (it != --array.end()) os << L", ";
        }
        os << "]";
        return os;
    }

    String Dumps() const {
        SStream ss;
        switch (m_type) {
            using enum Type;
        case OBJECT:
            ss << *m_object;
            break;
        case ARRAY:
            ss << *m_array;
            break;
        case STRING:
            ss << Format(LR"("{}")", *m_string);
            break;
        case NUMBER:
            ss << m_number;
            break;
        case BOOL:
            ss << (m_boolean ? L"true" : L"false");
            break;
        default:
            break;
        }
        return ss.str();
    }

private:
    enum class Type
    {
        OBJECT,
        ARRAY,
        STRING,
        NUMBER,
        BOOL,
        NIL,
    };
    Type m_type;
    union
    {
        std::unique_ptr<Object> m_object;
        std::unique_ptr<Array> m_array;
        std::unique_ptr<String> m_string;
        double m_number;
        bool m_boolean;
    };
};


class Parser
{
public:
    explicit Parser(const Scanner& scanner)
        : m_scanner(scanner) {
    }
    explicit Parser(Scanner&& scanner)
        : m_scanner(std::move(scanner)) {
    }

    std::unique_ptr<JSONElement> Parse() {
        auto element = std::make_unique<JSONElement>();

        switch (JSONTokenType token_type = m_scanner.Scan()) {
            using enum JSONTokenType;
        case END_OF_SOURCE:
            break;
        case BEGIN_OBJECT:
            element->SetValue(ParseObject());
            break;
        case BEGIN_ARRAY:
            element->SetValue(ParseArray());
            break;
        case VALUE_STRING:
            element->SetValue(std::make_unique<String>(m_scanner.GetString()));
            break;
        case VALUE_NUMBER:
            element->SetValue(m_scanner.GetNumber());
            break;
        case LITERAL_TRUE:
            element->SetValue(true);
            break;
        case LITERAL_FALSE:
            element->SetValue(false);
            break;
        default: break;
        }

        return element;
    }

private:
    std::unique_ptr<Object> ParseObject() {
        using enum JSONTokenType;
        auto res = std::make_unique<Object>();
        auto next = m_scanner.Scan();
        if (next == END_OBJECT)
            return res;
        m_scanner.Rollback();
        while (true) {
            next = m_scanner.Scan();
            if (next != VALUE_STRING)
                Error(L"Key must be string!");
            auto key = m_scanner.GetString();
            next = m_scanner.Scan();
            if (next != NAME_SEPRATOR)
                Error(L"Expected ':'!");
            (*res)[key] = Parse();
            next = m_scanner.Scan();
            if (next == END_OBJECT)
                break;
            if (next != VALUE_SEPRATOR)
                Error(L"Expected ','!");
        }
        return res;
    }

    std::unique_ptr<Array> ParseArray() {
        using enum JSONTokenType;
        auto res = std::make_unique<Array>();
        auto next = m_scanner.Scan();
        if (next == END_ARRAY)
            return std::move(res);
        m_scanner.Rollback();
        while (true) {
            res->emplace_back(Parse());
            next = m_scanner.Scan();
            if (next == END_ARRAY)
                break;
            if (next != VALUE_SEPRATOR)
                Error(L"Expected ','!");
        }
        return res;
    }

    Scanner m_scanner;
};
// JSON
}
// Y
}

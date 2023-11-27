

#include "Utilities.h"
#include <iostream>

namespace sdds {
    
    void Utilities::setFieldWidth(size_t newWidth) {
        m_widthField = newWidth;
    }

    size_t Utilities::getFieldWidth() const {
        return m_widthField;
    }

    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        std::string extractedToken{};
        size_t endPos = str.find(m_delimiter, next_pos);
        if (endPos != std::string::npos) {
            extractedToken = str.substr(next_pos, endPos - next_pos);
            next_pos = endPos + 1;
            more = true;
        }
        else {
            extractedToken = str.substr(next_pos);
            more = false;
        }
        extractedToken = trim(extractedToken);
        (extractedToken.length() > m_widthField) ? m_widthField = extractedToken.length() : m_widthField;
        if (extractedToken.empty()) {
            more = false;
            throw "ERROR: Token is empty!";
        }
        return extractedToken;
    }

    std::string Utilities::trim(const std::string& str) {
        size_t start = str.find_first_not_of(' ');
        size_t end = str.find_last_not_of(' ');

        if (start == std::string::npos || end == std::string::npos) {
            return "";
        }

        return str.substr(start, end - start + 1);
    }

    void Utilities::setDelimiter(char newDelimiter) {
        m_delimiter = newDelimiter;
    }

    char Utilities::getDelimiter() {
        return m_delimiter;
    }

}
// Declare escape_json_string helper function. In separate file to avoid circular includes.
#pragma once

// Helper function to escape JSON strings
std::string escape_json_string(const std::string& str) {
    std::stringstream ss;
    for (char c : str) {
        switch (c) {
            case '\"': ss << "\\\""; break;
            case '\\': ss << "\\\\"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                } else {
                    ss << c;
                }
        }
    }
    return ss.str();
}

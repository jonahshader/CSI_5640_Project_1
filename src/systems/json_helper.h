// Declare escape_json_string helper function. In separate file to avoid circular includes.
#pragma once

#include <string>

// Helper function to escape JSON strings
std::string escape_json_string(const std::string& str);

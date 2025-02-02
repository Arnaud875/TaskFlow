#ifndef STATIC_H_
#define STATIC_H_

// Check if DATABASE_FILE_NAME and DATABASE_FILE_PATH are defined
#if !defined(DATABASE_FILE_NAME)
#error "DATABASE_FILE_NAME must be defined."
#endif

#if !defined(DATABASE_SCHEMA_FILE_NAME)
#error "DATABASE_SCHEMA_FILE_NAME must be defined."
#endif

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "Utils/Logger/Logger.hpp"

#endif
#ifndef UTILS_LOGGER_SOURCE_LOCATION_H_
#define UTILS_LOGGER_SOURCE_LOCATION_H_

#include <string_view>

namespace Utils {
    struct SourceLocation {
        constexpr SourceLocation() noexcept
            : fileName_(nullptr), functionName_(nullptr), line_(0), column_(0){};

        static constexpr SourceLocation current(const char *fileName = __builtin_FILE(),
                                                const char *functionName = __builtin_FUNCTION(),
                                                unsigned int line = __builtin_LINE(),
                                                unsigned int column = 0) noexcept {
            SourceLocation loc;
            loc.fileName_ = getFileName(fileName);
            loc.functionName_ = functionName;
            loc.line_ = line;
            loc.column_ = column;
            return loc;
        }

        constexpr std::string_view fileName() const noexcept {
            return fileName_;
        }

        constexpr std::string_view functionName() const noexcept {
            return functionName_;
        }

        constexpr unsigned int line() const noexcept {
            return line_;
        }

        constexpr unsigned int column() const noexcept {
            return column_;
        }

    private:
        static constexpr const char *getFileName(const char *filePath) {
            const char *path = filePath;

            while (*filePath) {
                if (*filePath == '/')
                    path = filePath + 1;
                filePath++;
            }

            return path;
        }

        const char *fileName_;
        const char *functionName_;
        unsigned int line_;
        unsigned int column_;
    };
}  // namespace Utils

#endif
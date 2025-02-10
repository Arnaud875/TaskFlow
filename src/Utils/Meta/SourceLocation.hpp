#ifndef UTILS_META_SOURCE_LOCATION_H_
#define UTILS_META_SOURCE_LOCATION_H_

#include <string_view>

namespace Utils::Meta {
    /**
     * @brief The source location of the log
     * @details This class is used to get the source location of the log
     */
    struct SourceLocation {
        constexpr SourceLocation() noexcept
            : fileName_(nullptr), functionName_(nullptr), line_(0), column_(0){};

        /**
         * @brief Get the current source location
         * @param fileName The file name
         * @param functionName The function name
         * @param line The line number
         * @param column The column number
         * @return The source location
         */
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

        /**
         * @brief Get the file name of the source location
         */
        constexpr std::string_view fileName() const noexcept {
            return fileName_;
        }

        /**
         * @brief Get the function name of the source location
         */
        constexpr std::string_view functionName() const noexcept {
            return functionName_;
        }

        /**
         * @brief Get the line number of the source location
         */
        constexpr unsigned int line() const noexcept {
            return line_;
        }

        /**
         * @brief Get the column number of the source location
         */
        constexpr unsigned int column() const noexcept {
            return column_;
        }

    private:
        /**
         * @brief Get the file name from the file path
         * @param filePath The file path
         * @return The file name
         */
        static constexpr const char *getFileName(const char *filePath) {
            const char *path = filePath;

            while (*filePath) {
                if (*filePath == '/' || *filePath == '\\')
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
}  // namespace Utils::Meta

#endif
#ifndef UTILS_SINGLETON_H_
#define UTILS_SINGLETON_H_

#include <cassert>
#include <memory>
#include <mutex>

namespace Utils {
    /*
     * @brief Class that implements the Singleton pattern
     * @tparam T Type of the singleton class
     */
    template<typename T>
    class Singleton {
    protected:
        Singleton() = default;
        virtual ~Singleton() = default;

    public:
        // Remove copy operator
        Singleton(const Singleton &) = delete;
        Singleton &operator=(const Singleton &) = delete;

        // Remove move operator
        Singleton(Singleton &&) = delete;
        Singleton &operator=(Singleton &&) = delete;

        /*
         * @brief Create a new instance of the singleton class
         * @tparam Args Types of the arguments to pass to the constructor
         * @param args Arguments to pass to the constructor
         *
         * @note This method is thread-safe and is called only once
         *
         * @return Reference to the singleton instance
         */
        template<typename... Args>
        static T &CreateInstance(Args &&...args) {
            assert(instance == nullptr && "Singleton instance should not exist "
                                          "before calling CreateInstance");

            std::call_once(
                init_flag,
                [&](Args &&...params) { instance.reset(new T{std::forward<Args>(args)...}); },
                std::forward<Args>(args)...);

            if (!instance) {
                throw std::runtime_error("Failed to create singleton instance");
            }

            return *instance;
        }

        /*
         * @brief Get the singleton instance
         * @return Reference to the singleton instance
         */
        static T &GetInstance() {
            assert(instance != nullptr && "Singleton instance must be created "
                                          "before calling GetInstance");
            if (!instance) {
                throw std::runtime_error("Singleton instance does not exist");
            }

            return *instance;
        }

        /**
         * @brief Check if the singleton instance exists
         * @return True if the singleton instance exists, false otherwise
         */
        static bool HasInstance() {
            return instance != nullptr;
        }

        /**
         * @brief Destroy the singleton instance
         */
        static void DestroyInstance() {
            instance.reset(nullptr);
        }

        /**
         * @brief Create a new instance of the singleton class and handle any
         * exception
         * @tparam Args Types of the arguments to pass to the constructor
         * @param name Name of the singleton
         * @param args Arguments to pass to the constructor
         */
        template<typename... Args>
        static T &SafeSingletonCreate(std::string_view name, Args &&...args) {
            try {
                return Singleton<T>::CreateInstance(std::forward<decltype(args)>(args)...);
            } catch (const std::exception &e) {
                std::cerr << "Fatal error: Failed to create singleton '" << name
                          << "': " << e.what() << std::endl;
                std::exit(EXIT_FAILURE);
            } catch (...) {
                std::cerr << "Fatel error: Unknown error while creating singleton '" << name << "'"
                          << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }

    private:
        static std::unique_ptr<T> instance;
        static std::once_flag init_flag;
    };
}  // namespace Utils

template<typename T>
std::unique_ptr<T> Utils::Singleton<T>::instance = nullptr;

template<typename T>
std::once_flag Utils::Singleton<T>::init_flag;

#endif
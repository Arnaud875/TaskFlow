#ifndef UTILS_SINGLETON_H_
#define UTILS_SINGLETON_H_

#include <cassert>
#include <memory>

namespace Utils {
    /*
     * @brief Class that implements the Singleton pattern
     * @tparam T Type of the singleton class
     */
    template<typename T>
    class Singleton {
    protected:
        Singleton() = default;
        ~Singleton() = default;

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
         * @return Reference to the singleton instance
         */
        template<typename... Args>
        static T &CreateInstance(Args &&...args) {
            assert((instance == nullptr) && "Singleton instance already created");
            instance.reset(new T(std::forward<Args>(args)...));
            return *instance;
        }

        /*
         * @brief Get the singleton instance
         * @return Reference to the singleton instance
         */
        static T &GetInstance() {
            assert((instance != nullptr) && "Singleton instance not created");
            return *instance;
        }

        /*
         * @brief Destroy the singleton instance
         */
        static void DestroyInstance() {
            assert((instance != nullptr) && "Singleton instance not created");
            instance.reset();
        }

    private:
        static std::unique_ptr<T> instance;
    };
}  // namespace Utils

template<typename T>
std::unique_ptr<T> Utils::Singleton<T>::instance = nullptr;

#endif
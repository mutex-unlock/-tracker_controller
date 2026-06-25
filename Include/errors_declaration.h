#pragma once
// исключения для обработки ошибок


#include <exception>
#include <cstdint>

namespace NTracker
{
    // перечисление с кодами возможных ошибок
    enum class ETrackerError: uint32_t
    {
        NoError               = 0,
        NotInitialized        = 1,
        UnknownTrackerType    = 2,
        UnknownTrackerNumber  = 3,
        TrackerIsNotOperable  = 4,
        DriverIsNotSet        = 5,
        InvalidArgument       = 6,
        NotSupportedOperation = 7,
        InitDriverError       = 8
    };

    // объявление класса исключений
    class TTrackerException : public std::exception
    {
    public:
        TTrackerException(ETrackerError error);

        ETrackerError code() const noexcept;
        const char* what() const noexcept override;

        // если при выполнении где-то в коде возникает ошибка,
        // то в этом месте нужно вызвать метод throw_exception()
        // Указанный метод выбросит исключение с соответствующим кодом.
        static void throw_exception(ETrackerError error);

    private:
        ETrackerError code_;        // код ошибки
    };

};  // end namespace NTracker

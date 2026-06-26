#include "errors_declaration.h"
// реализация компонента обработки ошибок с помощью исключений


using namespace NTracker;

const char* get_error_string(ETrackerError errorCode)
{
    const char* errors[] =
    {
        "No Errors",                    //NoError
        "Module is not initialized",    //NotInitialized
        "Unknown trcker type",          //UnknownTrackerType
        "Unknown tracker number",       //UnknownTrackerNumber
        "Tracker is not operable",      //TrackerIsNotOperable
        "Driver is not assigned",       //DriverIsNotSet
        "Invalid argument",             //InvalidArgument
        "Operation is not supported",   //NotSupportedOperation
        "Driver cannot be initialized", //InitDriverError
    };

    return errors[static_cast<std::size_t>(errorCode)];
}

// конструктор по-умолчанию
TTrackerException::TTrackerException(ETrackerError error):code_(error)
{

}

// геттер, возвращающий код ошибки
ETrackerError TTrackerException::code() const noexcept
{
    return code_;
}

// геттер, возвращающий текст ошибки
const char* TTrackerException::what() const noexcept
{
    return get_error_string(code_);
}


// если при выполнении где-то в коде возникает ошибка,
// то в этом месте нужно вызвать метод throw_exception()
// Указанный метод выбросит исключение с соответствующим кодом.
void TTrackerException::throw_exception(ETrackerError error)
{
    throw TTrackerException(error);
}

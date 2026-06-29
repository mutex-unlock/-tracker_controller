// Следующий блок ifdef представляет собой стандартный способ создания макросов,
// упрощающих экспорт из DLL. Все файлы в этой DLL компилируются с символом LIB_EXPORTS,
// определенным в командной строке. Этот символ не должен быть определен ни в одном проекте,
// использующем эту DLL.
// Таким образом, любой другой проект, исходные файлы которого содержат этот файл,
// будет рассматривать функции LIB_API как импортированные из DLL,
// тогда как эта DLL будет рассматривать символы, определенные с помощью этого макроса, как экспортированные.

// объявлены определения для экспортируемых функций. Эти объявления необходимы
// для компиляции DLL в среде Windows, для других платформ они неактуальны.

// Этот блок макросов (LIB_API) решает классическую проблему:
// при сборке библиотеки как DLL в Windows нужно по-разному помечать функции
// в зависимости от того, кто их использует:
//  Если мы собираем саму библиотеку (LIB_EXPORTS определён)
//      функции должны быть экспортированы из DLL, чтобы их видели другие программы.
//      Для этого используют __declspec(dllexport).
//
//  Иначе (LIB_EXPORTS НЕопределён), если мы используем библиотеку (подключаем её в другой проект)
//      функции нужно импортировать из DLL.
//      Для этого применяют __declspec(dllimport).
// На других платформах (не Windows) такие спецификаторы не нужны,
// поэтому макрос просто раскрывается в пустоту.
#if defined(_WIN32) || defined(_WIN64)
    #ifdef LIB_EXPORTS
        #define LIB_API __declspec(dllexport)
    #else
        #define LIB_API __declspec(dllimport)
    #endif
#else
    #define LIB_API
#endif

#include <stdint.h>

// объявлены типы, которые будут использоваться для входных параметров интерфейсных функций
// это те же объявления, которые использовались в исходной реализации (tracker_declarations.h)
typedef uint32_t trackerNumber_t;
typedef double trackerValue_t;
typedef uint32_t checkAlertTimeout_t;

// вместо перечислений C++ объявляются простые числовые типы.
// В экспортируемых функциях нежелательно использовать
// перечисления как типы входных параметров, потому что размер этих типов в C явно не определен
typedef uint32_t trackerType_t;
typedef uint32_t driverType_t;
typedef uint32_t alertRule_t;

// объявлены типы указателей на функцию для выполнения обратных вызовов
typedef void(*trackerValueCallback_t)(trackerNumber_t, trackerValue_t, void*);
typedef checkAlertTimeout_t(*trackerAlertCallback_t)(trackerNumber_t, trackerValue_t, void*);
typedef trackerValue_t(*onSimulateReadValue_t)(trackerNumber_t, int, void*);
typedef int (*onSimulateOperable_t)(trackerNumber_t, void*);

// перечисления используются в качестве числовых констант
// в отличие от исходной реализации здесь присутствует
// дополнительный параметр для указания контекста вызова
enum ETrackerType
{
    TT_SPOT       = 0,
    TT_SMOOTH     = 1,
    TT_DERIVATIVE = 2,
};

enum EDriverType
{
    DT_SIMULATION = 0,
    DT_USB        = 1,
    DT_WIFI       = 2,
    DT_ETHERNET   = 3
};

enum EAlertRule
{
    AR_MORE = 0,
    AR_LESS = 1
};

typedef unsigned int errorCode_t;

// метод для запуска контроллера датчиков
LIB_API errorCode_t initialize_tracker_controller();

// метод для остановки контроллера датчиков
LIB_API errorCode_t shutdown_tracker_controller();

// метод для назначения драйвера.
// Этот драйвер должен быть создан и назначен в самом начале работы,
// поскольку он будет передаваться новым создаваемым датчикам.
LIB_API errorCode_t assign_driver(driverType_t type);

// метод для получения драйвера.
// Этот драйвер должен быть создан и назначен в самом начале работы,
// поскольку он будет передаваться новым создаваемым датчикам.
LIB_API errorCode_t get_assigned_driver(driverType_t* type);

// узнать назначенный драйвер для соответствующего датчика
LIB_API errorCode_t get_tracker_driver(trackerNumber_t number,
                                       driverType_t* type);

// метод для добавления датчика
LIB_API errorCode_t add_tracker(trackerType_t type,
                                trackerNumber_t number);

// метод для удаления датчика
LIB_API errorCode_t delete_tracker(trackerNumber_t number);

// метод проверяет существует ли датчик с переданным номером
LIB_API errorCode_t check_tracker_exist(trackerNumber_t number,
                                        int* isExist);

// метод проверяет является ли датчик работоспособным
LIB_API errorCode_t is_tracker_operable(trackerNumber_t number,
                                        int* isOperable);

// метод для чтения текущего показания датчика
LIB_API errorCode_t get_tracker_value(trackerNumber_t number,
                                      trackerValue_t* value);

// осуществляется асинхронный запрос показания датчика,
// значение будет возвращаться через передаваемый обратный вызов
LIB_API errorCode_t query_tracker_value(trackerNumber_t number,
                                        trackerValueCallback_t callback,
                                        void* pContextData);

// осуществляется опрос показаний всех работоспособных датчиков,
// значения также возвращаются через обратный вызов
LIB_API errorCode_t read_tracker_values(trackerValueCallback_t callback,
                                        void* pContextData);

// можно получить минимальное значение для набора датчиков
// с номерами из указанного диапазона
LIB_API errorCode_t get_min_value(trackerNumber_t first,
                                  trackerNumber_t last,
                                  trackerValue_t* value);

// можно получить максимальное значение для набора датчиков
// с номерами из указанного диапазона
LIB_API errorCode_t get_max_value(trackerNumber_t first,
                                  trackerNumber_t last,
                                  trackerValue_t* value);

// назначается отслеживание пороговых значений датчиков
LIB_API errorCode_t set_alert(trackerNumber_t number,
                              trackerAlertCallback_t callback,
                              trackerValue_t alertValue,
                              EAlertRule alertRule,
                              checkAlertTimeout_t checkTimeoutSet,
                              void* pContextData);

// выключается отслеживание пороговых значений датчиков
LIB_API errorCode_t reset_alert(trackerNumber_t number);

LIB_API errorCode_t set_simulate_read_callback(onSimulateReadValue_t callback,
                                               void* pContextData);

LIB_API errorCode_t set_simulate_operable_callback(onSimulateOperable_t callback,
                                                   void* pContextData);

enum EErrorCode
{
    EC_NO_ERROR = 0,
    EC_NOT_INITIALIZED = 1,
    EC_UNKNOWN_TRACKER_TYPE = 2,
    EC_UNKNOWN_TRACKER_NUMBER = 3,
    EC_TRACKER_IS_NOT_OPERABLE = 4,
    EC_DRIVER_IS_NOT_SET = 5,
    EC_INVALID_ARGUMENT = 6,
    EC_NOT_SUPPORTED_OPERATION = 7,
    EC_INIT_DRIVER_ERROR = 8
};

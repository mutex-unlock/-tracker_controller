// tracker_declarations_LIB_API_C.cpp : Defines the exported functions for the DLL.
//

#include "tracker_declarations_LIB_API_C.h" // system API interface (C)
#include "API_cpp_interface.h"              // C++ API interface
#include "errors_declaration.h"             // Errors (C++)
#include "driver_implementation.h"          // Driver (C++)
#include "number_to_enum_translator.h"      // конвертирует числовое значение в перечисление

#include <initializer_list>
#include <functional>
#include <memory>

using controllerPointer_t = std::unique_ptr<NTracker::ITrackerController>;

thread_local controllerPointer_t g_TrackerController(NTracker::ITrackerController::create_tracker_controller());
thread_local NTracker::driverPointer_t g_TDriverSimulation;
thread_local NTracker::driverPointer_t g_TDriverUSB;
thread_local NTracker::driverPointer_t g_TDriverWIFI;
thread_local NTracker::driverPointer_t g_TDriverEthernet;

// метод для запуска контроллера датчиков
errorCode_t initialize_tracker_controller()
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        g_TrackerController->initialize_tracker_controller();
    }
    catch(NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }

    return error;
}

// метод для остановки контроллера датчиков
errorCode_t shutdown_tracker_controller()
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        g_TrackerController->shutdown_tracker_controller();
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }

    return error;
}

// метод-фабрика классов, в котором происходит создание класса соответствующего типа
void create_driver(NTracker::EDriverType driverType,
                   NTracker::driverPointer_t& driverPointer)
{
    if (!driverPointer)
    {
        driverPointer = NTracker::IDriver::create_driver(driverType);
        driverPointer->initialize_driver();
    }
    g_TrackerController->assign_driver(driverPointer);
}

// метод для назначения драйвера.
// Этот драйвер должен быть создан и назначен в самом начале работы,
// поскольку он будет передаваться новым создаваемым датчикам.
errorCode_t assign_driver(driverType_t driverType)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        TNumEnumTranslator<NTracker::EDriverType> convertor;

        // конвертируется переданное числовое значение в перечисление C++
        convertor.convert_to_enum(driverType,
                                  { NTracker::EDriverType::Simulation,
                                    NTracker::EDriverType::USB,
                                    NTracker::EDriverType::WiFi,
                                    NTracker::EDriverType::Ethernet}
        );

        if (convertor.is_error_convert())
        {
            return EC_INVALID_ARGUMENT;
        }

        // анализируется полученное значение перечисления,
        // и вызывается вспомогательная функция с соответствующими параметрами
        switch (convertor.get_enum_result())
        {
        case NTracker::EDriverType::Simulation:
        {
            create_driver(NTracker::EDriverType::Simulation, g_TDriverSimulation);
        }
        break;

        case NTracker::EDriverType::USB:
        {
            create_driver(NTracker::EDriverType::USB, g_TDriverUSB);
        }
        break;

        case NTracker::EDriverType::WiFi:
        {
            create_driver(NTracker::EDriverType::WiFi, g_TDriverWIFI);
        }
        break;

        case NTracker::EDriverType::Ethernet:
        {
            create_driver(NTracker::EDriverType::Ethernet, g_TDriverEthernet);
        }
        break;
        }
    } // end try

    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }

    return error;
}

// метод для получения драйвера.
// Этот драйвер должен быть создан и назначен в самом начале работы,
// поскольку он будет передаваться новым создаваемым датчикам.
errorCode_t get_assigned_driver(driverType_t* type)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        NTracker::driverPointer_t driver = g_TrackerController->get_assigned_driver();

        if (!driver)
        {
            return EC_DRIVER_IS_NOT_SET;
        }

        if (driver.get() == g_TDriverSimulation.get())
        {
            *type = DT_SIMULATION;
        }
        if (driver.get() == g_TDriverUSB.get())
        {
            *type = DT_USB;
        }
        if (driver.get() == g_TDriverWIFI.get())
        {
            *type = DT_WIFI;
        }
        if (driver.get() == g_TDriverEthernet.get())
        {
            *type = DT_ETHERNET;
        }
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }

    return error;
}

// узнать назначенный драйвер для соответствующего датчика
errorCode_t get_tracker_driver(trackerNumber_t number, driverType_t* type)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        NTracker::driverPointer_t driver = g_TrackerController->get_tracker_driver(number);

        if (!driver)
        {
            return EC_DRIVER_IS_NOT_SET;
        }
        if (dynamic_cast<NTracker::TDriverSimulation*>(driver.get()))
        {
            *type = DT_SIMULATION;
        }
        if (dynamic_cast<NTracker::TDriverUSB*>(driver.get()))
        {
            *type = DT_USB;
        }
        if (dynamic_cast<NTracker::TDriverWIFI*>(driver.get()))
        {
            *type = DT_WIFI;
        }
        if (dynamic_cast<NTracker::TDriverEthernet*>(driver.get()))
        {
            *type = DT_ETHERNET;
        }
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }

    return error;
}

// метод для добавления датчика
errorCode_t add_tracker(trackerType_t type,
                        trackerNumber_t number)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        TNumEnumTranslator<NTracker::ETrackerType> convertor;
        convertor.convert_to_enum(type,
                                { NTracker::ETrackerType::Spot,
                                  NTracker::ETrackerType::Smooth,
                                  NTracker::ETrackerType::Derivative }
        );

        if (convertor.is_error_convert())
        {
            return EC_INVALID_ARGUMENT;
        }
        g_TrackerController->add_tracker(convertor.get_enum_result(), number);

    } // end try

    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }

    return error;
}

// метод для удаления датчика
errorCode_t delete_tracker(trackerNumber_t number)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        g_TrackerController->delete_tracker(number);
    }
    catch (NTracker::TTrackerException& e)
    {
        error = static_cast<errorCode_t>(e.code());
    }

    return error;
}

// метод проверяет существует ли датчик с переданным номером
errorCode_t check_tracker_exist(trackerNumber_t number, int* isExist)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        bool bExist = g_TrackerController->check_tracker_exist(number);
        *isExist = bExist ? 1 : 0;
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }
    return error;
}

// метод проверяет является ли датчик работоспособным
errorCode_t is_tracker_operable(trackerNumber_t number,
                                int* isOperable)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        bool bOperable = g_TrackerController->check_tracker_exist(number);
        *isOperable = bOperable ? 1 : 0;
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }
    return error;
}

// метод для чтения текущего показания датчика
errorCode_t get_tracker_value(trackerNumber_t number,
                              trackerValue_t* value)
{
    errorCode_t error = EC_NO_ERROR;        // переменная – код возврата ошибки

    try
    {
        // вызов метода класса, который заключен в блок try
        *value = g_TrackerController->get_tracker_value(number);
    }
    catch (NTracker::TTrackerException& exception)          // перехват исключения
    {
        error = static_cast<errorCode_t>(exception.code()); // присваивается код ошибки
    }
    return error;                                           // возврат кода ошибки
}

// осуществляется асинхронный запрос показания датчика,
// значение будет возвращаться через передаваемый обратный вызов
errorCode_t query_tracker_value(trackerNumber_t number,
                                trackerValueCallback_t callback,
                                void* pContextData)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
       using namespace std::placeholders;
       g_TrackerController->query_tracker_value(number,std::bind(callback,_1,_2, pContextData));
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }
    return error;
}

// осуществляется опрос показаний всех работоспособных датчиков,
// значения также возвращаются через обратный вызов
errorCode_t read_tracker_values(trackerValueCallback_t callback,
                                void* pContextData)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        using namespace std::placeholders;
        // перенаправление вызовов в реализации интерфейсной функции.
        // в вызов метода интерфейсного класса read_tracker_values() передается
        // объект связывания. Функция обратного вызова, объявленная в интерфейсе API,
        // принимает 3 входных параметра : номер датчика, значение датчика и указатель на контекст
        // Когда будет происходить обратный вызов, то объект связывания вызовет
        // назначенную функцию, в которую передаст первые два параметра исходной функции,
        // а в третий параметр будет передан переданный указатель на контекст.
        g_TrackerController->read_tracker_values(std::bind(callback,_1,_2,pContextData));
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }
    return error;
}

// можно получить минимальное значение для набора датчиков
// с номерами из указанного диапазона
errorCode_t get_min_value(trackerNumber_t first,
                          trackerNumber_t last,
                          trackerValue_t* value)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        *value = g_TrackerController->get_min_value(first, last);
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }
    return error;
}

// можно получить максимальное значение для набора датчиков
// с номерами из указанного диапазона
errorCode_t get_max_value(trackerNumber_t first,
                          trackerNumber_t last,
                          trackerValue_t* value)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        *value = g_TrackerController->get_max_value(first, last);
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }
    return error;
}

// назначается отслеживание пороговых значений датчиков
errorCode_t set_alert(trackerNumber_t number,
                      trackerAlertCallback_t callback,
                      trackerValue_t alertValue,
                      EAlertRule alertRule,
                      checkAlertTimeout_t checkTimeoutSet,
                      void* pContextData)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        TNumEnumTranslator<NTracker::EAlertRule> convertor;
        convertor.convert_to_enum(alertRule, {NTracker::EAlertRule::More,
                                              NTracker::EAlertRule::Less});
        if (convertor.is_error_convert())
        {
            return EC_INVALID_ARGUMENT;
        }
        using namespace std::placeholders;
        g_TrackerController->set_alert(number,
                                    std::bind(callback,_1,_2, pContextData),
                                    alertValue,
                                    convertor.get_enum_result(),
                                    checkTimeoutSet);
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }
    return error;
}

// выключается отслеживание пороговых значений датчиков
errorCode_t reset_alert(trackerNumber_t number)
{
    errorCode_t error = EC_NO_ERROR;

    try
    {
        g_TrackerController->reset_alert(number);
    }
    catch (NTracker::TTrackerException& exception)
    {
        error = static_cast<errorCode_t>(exception.code());
    }
    return error;
}


errorCode_t set_simulate_read_callback(onSimulateReadValue_t callback,
                                       void* pContextData)
{
    using namespace std::placeholders;

    if (!g_TDriverSimulation)
    {
        return EC_NOT_SUPPORTED_OPERATION;
    }

    NTracker::TDriverSimulation* driver = 
        dynamic_cast<NTracker::TDriverSimulation*>(g_TDriverSimulation.get());

    if (callback)
    {
        driver->set_read_value(std::bind(callback, _1, _2, pContextData));
    }
    else
    {
        driver->set_operable(NTracker::TDriverSimulation::onOperable_t());
    }

    return EC_NO_ERROR;
}

errorCode_t set_simulate_operable_callback(onSimulateOperable_t callback,
                                           void* pContextData)
{
    using namespace std::placeholders;

    if (!g_TDriverSimulation)
    {
        return EC_NOT_SUPPORTED_OPERATION;
    }

    NTracker::TDriverSimulation* driver = 
        dynamic_cast<NTracker::TDriverSimulation*>(g_TDriverSimulation.get());

    if (callback)
    {
        driver->set_operable(std::bind(callback, _1, pContextData));
    }
    else
    {
        driver->set_operable(NTracker::TDriverSimulation::onOperable_t());
    }

    return EC_NO_ERROR;
}

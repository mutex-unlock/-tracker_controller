#pragma once
// общие определения типов

#include <functional>   // std::function
#include <memory>       // std::shared_ptr


namespace NTracker
{
    class ITracker; // объявление интерфейса для работы с датчиками
    class IDriver;  // объявление интерфейса для взаимодействия с аппаратурой

    // тип для номера датчика
    using trackerNumber_t     = unsigned int;
    // тип значения, возвращаемого датчиком
    using trackerValue_t      = double;
    // тип значения интервала опроса датчиков для сигнализации пороговых значений
    using checkAlertTimeout_t = unsigned int;

    // перечисление с идентификаторами типов датчиков
    enum class ETrackerType : uint32_t
    {
        Spot       = 0,     // текущее измерение
        Smooth     = 1,     // сглаженное
        Derivative = 2      // производное
    };

    // перечисление с идентификаторами типов драйверов
    enum class EDriverType : uint32_t
    {
        Simulation = 0,     // программная эмуляция работы датчика
        USB        = 1,
        Wi-Fi      = 2,
        Ethernet   = 3
    };

    // перечисление с идентификаторами правил для задания пороговых значений
    enum class EAlertRule : uint32_t
    {
        More = 0,           // превышение заданного значения
        Less = 1            // опускание ниже заданного значения
    };

    // тип для хранения указателей классов датчиков
    using trackerPointer_t     = std::shared_ptr<ITracker>;
    // тип для хранения указателей классов драйверов
    using driverPointer_t      = std::shared_ptr<IDriver>;
    // тип обратного вызова, в который передается значение датчика
    using trackerValueCallback = std::function<void(trackerNumber_t, trackerValue_t)>;
    
    // тип обратного вызова, в который передается значение датчика
    // в случае срабатывания сигнализации порогового значения.
    using trackerAlertCallback = std::function<checkAlertTimeout_t(trackerNumber_t, trackerValue_t)>;

};  // end namespace NTracker

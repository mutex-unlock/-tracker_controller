#pragma once
// интерфейс драйвера для взаимодействия с аппаратурой

#include "tracker_declarations.h"

namespace NTracker
{
    class IDriver
    {
    public:
        // метод для инициализации драйвера
        virtual void initialize_driver() = 0;

        // метод для активации датчика
        virtual void activate_tracker(trackerNumber_t number) = 0;
        // метод, возвращающий признак работоспособности датчика
        virtual bool is_operable_tracker(trackerNumber_t number) = 0;

        // метод для чтения текущих значений
        virtual trackerValue_t read_spot(trackerNumber_t number) = 0;
        // метод для чтения сглаженных значений
        virtual trackerValue_t read_smooth(trackerNumber_t number) = 0;
        // метод для чтения производных значений
        virtual trackerValue_t read_derivative(trackerNumber_t number) = 0;

        virtual ~IDriver() = default;

        // метод-фабрика классов, в котором происходит создание класса соответствующего типа
        static driverPointer_t create_driver(EDriverType type);
    };

};  // end namespace NTracker

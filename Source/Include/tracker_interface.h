#pragma once
// обобщенный интерфейсный класс ITracker для работы с датчиком

#include "tracker_declarations.h"

namespace NTracker
{

    class ITracker
    {
    public:
        // метод для настройки драйвера, с которым будет работать датчик
        virtual void set_driver(driverPointer_t driverPointer) = 0;
        // получить используемый драйвер
        virtual driverPointer_t get_driver() = 0;

        // метод для получения текущего значения датчика
        virtual double get_current_value() = 0;
        // метод для определения работоспособности датчика
        virtual bool is_operable_tracker() = 0;

        virtual ~ITracker() = default;

        // метод для создания экземпляра класса соответствующего типа
        static trackerPointer_t create_tracker(ETrackerType type,
                                               trackerNumber_t number,
                                               driverPointer_t driverPointer
        );

    };  // end class ITracker

};  // end namespace NTracker

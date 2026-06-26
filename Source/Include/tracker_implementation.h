#pragma once
// интерфейсы конкретных классов датчиков

#include "tracker_interface.h"


namespace NTracker
{

    class TTrackerAbstract : public ITracker
    {
    public:
        // метод для настройки драйвера, с которым будет работать датчик
        void set_driver(driverPointer_t driverPtr) override;
        // получить используемый драйвер
        driverPointer_t get_driver() override;
        // метод для определения работоспособности датчика
        bool is_operable_tracker() override;

    protected:
        TTrackerAbstract(trackerNumber_t assignedNumber,
                         driverPointer_t driverPointer);

        trackerNumber_t number;         // Номер датчика
        driverPointer_t driverPointer;  // Указатель на датчик
    };


    // class TTrackerSpot
    class TTrackerSpot : public TTrackerAbstract
    {
    public:
        // метод для получения текущего значения датчика
        double get_current_value() override;

        // метод для создания экземпляра класса типа TTrackerSpot
        static ITracker* create_tracker(trackerNumber_t assignedNumber,
                                        driverPointer_t driverPointer);

    protected:
        TTrackerSpot(trackerNumber_t assignedNumber,
                     driverPointer_t driverPointer);
    };


    // class TTrackerSmooth
    class TTrackerSmooth : public TTrackerAbstract
    {
    public:
        // метод для получения текущего значения датчика
        double get_current_value() override;

        // метод для создания экземпляра класса типа TTrackerSmooth
        static ITracker* create_tracker(trackerNumber_t assignedNumber,
                                        driverPointer_t driverPointer);

    protected:
        TTrackerSmooth(trackerNumber_t assignedNumber,
                       driverPointer_t driverPointer);
    };


    // class TTrackerDerivative
    class TTrackerDerivative : public TTrackerAbstract
    {
    public:
        // метод для получения текущего значения датчика
        double get_current_value() override;

        // метод для создания экземпляра класса типа TTrackerDerivative
        static ITracker* create_tracker(trackerNumber_t assignedNumber,
                                        driverPointer_t driverPointer);

    protected:
        TTrackerDerivative(trackerNumber_t assignedNumber,
                           driverPointer_t driverPointer);
    };

};  // end namespace NTracker

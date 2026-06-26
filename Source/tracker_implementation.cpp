#include "tracker_implementation.h"
#include "driver_interface.h"

// реализация интерфейсов конкретных классов датчиков

using namespace NTracker;


// abstract class TTrackerAbstract
TTrackerAbstract::TTrackerAbstract(trackerNumber_t assignedNumber,
                                   driverPointer_t driverPtr)
   : number(assignedNumber), driverPointer(driverPtr)
{
    driverPointer->activate_tracker(assignedNumber);            // активация датчика
}

// метод для настройки драйвера, с которым будет работать датчик
void TTrackerAbstract::set_driver(driverPointer_t driverPtr)    // настройка драйвера
{
    driverPointer = driverPtr;
}

// получить используемый драйвер
driverPointer_t TTrackerAbstract::get_driver()                  // получение драйвера
{
    return driverPointer;
}

// метод для определения работоспособности датчика
bool TTrackerAbstract::is_operable_tracker()
{
    return driverPointer->is_operable_tracker(number);          // работоспособность датчика
}


// class TTrackerSpot
TTrackerSpot::TTrackerSpot(trackerNumber_t assignedNumber,
                           driverPointer_t driverPointer)
   : TTrackerAbstract(assignedNumber,driverPointer)
{

}

// метод для получения текущего значения датчика
double TTrackerSpot::get_current_value()
{
    return driverPointer->read_spot(number);                    // чтение текущих значений
}

// метод для создания экземпляра класса типа TTrackerSpot
ITracker* TTrackerSpot::create_tracker(trackerNumber_t assignedNumber,
                                       driverPointer_t driverPointer)
{
    return new TTrackerSpot(assignedNumber, driverPointer);
}


// class TTrackerSmooth
TTrackerSmooth::TTrackerSmooth(trackerNumber_t assignedNumber,
                               driverPointer_t driverPointer)
   : TTrackerAbstract(assignedNumber,driverPointer)
{

}

// метод для получения текущего значения датчика
double TTrackerSmooth::get_current_value()
{
    return driverPointer->read_smooth(number);                  // чтение сглаженных значений
}

// метод для создания экземпляра класса типа TTrackerSmooth
ITracker*  TTrackerSmooth::create_tracker(trackerNumber_t assignedNumber,
                                          driverPointer_t driverPointer)
{
    return new TTrackerSmooth(assignedNumber, driverPointer);
}


// class TTrackerDerivative
TTrackerDerivative::TTrackerDerivative(trackerNumber_t assignedNumber,
                                       driverPointer_t driverPointer)
   : TTrackerAbstract(assignedNumber, driverPointer)
{

}

// метод для получения текущего значения датчика
double TTrackerDerivative::get_current_value()
{
    return driverPointer->read_derivative(number);              // чтение производных значений
}

// метод для создания экземпляра класса типа TTrackerDerivative
ITracker* TTrackerDerivative::create_tracker(trackerNumber_t assignedNumber,
                                             driverPointer_t driverPointer)
{
    return new TTrackerDerivative(assignedNumber, driverPointer);
}


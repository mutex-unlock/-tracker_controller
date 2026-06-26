#include "tracker_interface.h"
#include "tracker_implementation.h"

// реализация метода ITracker::create_tracker()
// обобщенного интерфейсного класса ITracker для работы с датчиком

using namespace NTracker;


// метод для создания экземпляра класса соответствующего типа
trackerPointer_t ITracker::create_tracker(ETrackerType trackerType,
                                          trackerNumber_t trackerNumber,
                                          driverPointer_t driverPointer)
{
    using createFunPtr_t = ITracker* (*)(trackerNumber_t trackerNumber,
                                         driverPointer_t driverPointer);

    static const createFunPtr_t arrayCreateFun[] =
    {
        TTrackerSpot::create_tracker,         // TRACKER_SPOT
        TTrackerSmooth::create_tracker,       // TRACKER_SMOOTH
        TTrackerDerivative::create_tracker    // TRACKER_DERIVATIVE
    };

    size_t index = static_cast<uint32_t>(trackerType);

    return trackerPointer_t(arrayCreateFun[index](trackerNumber, driverPointer));

}   // end ITracker::create_tracker()

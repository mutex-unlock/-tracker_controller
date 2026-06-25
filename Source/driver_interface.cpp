#include "driver_interface.h"
#include "driver_implementation.h"

using namespace NTracker;

// // метод-фабрика классов, в котором происходит создание класса соответствующего типа
driverPointer_t IDriver::create_driver(EDriverType driverType)
{
    using createFunPtr_t = IDriver * (*)();

    static const createFunPtr_t arrayCreateFun[] =
    {
        TDriverSimulation::create_driver,   //TDRIVER_SIMULATION
        TDriverUSB::create_driver,          //TDRIVER_USB
        TDriverWIFI::create_driver,         //TDRIVER_WIFI
        TDriverEthernet::create_driver      //TDRIVER_ETHERNET
    };

    size_t index = static_cast<size_t>(driverType);

    return driverPointer_t(arrayCreateFun[index]());
}


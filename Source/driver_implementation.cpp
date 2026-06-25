#include "driver_implementation.h"
// реализация конкретных драйверов классов:
// TDriverSimulation,  TDriverUSB, TDriverWIFI, TDriverEthernet

using namespace NTracker;


// class TDriverSimulation
TDriverSimulation::TDriverSimulation()
{

}

void TDriverSimulation::set_default_value(trackerValue_t value)
{
    defaultValue_ = value;
}

void TDriverSimulation::set_default_operable(bool isOperable)
{
    defaultOperable_ = isOperable;
}

void TDriverSimulation::set_read_value(onReadValue_t value)
{
    getValue_ = value;
}

void TDriverSimulation::set_operable(onOperable_t operable)
{
    getOperable_ = operable;
}

void TDriverSimulation::initialize_driver()
{

}

void TDriverSimulation::activate_tracker(trackerNumber_t number)
{

}

bool TDriverSimulation::is_operable_tracker(trackerNumber_t number)
{
    if (getOperable_)
    {
        return getOperable_(number);
    }
    else
    {
        return defaultOperable_;
    }
}

trackerValue_t TDriverSimulation::read_spot(trackerNumber_t number)
{
    if (getValue_)
    {
        return getValue_(number, RT_SPOT);
    }
    else
    {
        return defaultValue_;
    }
}

trackerValue_t TDriverSimulation::read_smooth(trackerNumber_t number)
{
    if (getValue_)
    {
        return getValue_(number, RT_SMOOTH);
    }
    else
    {
        return defaultValue_;
    }
}

trackerValue_t TDriverSimulation::read_derivative(trackerNumber_t number)
{
    if (getValue_)
    {
        return getValue_(number, RT_DERIVATIVE);
    }
    else
    {
        return defaultValue_;
    }
}

IDriver* TDriverSimulation::create_driver()
{
    return new TDriverSimulation;
}



// class TDriverUSB
TDriverUSB::TDriverUSB()
{

}

void TDriverUSB::initialize_driver()
{
    //TODO: need to be implemented
}

void TDriverUSB::activate_tracker(trackerNumber_t number)
{
    //TODO: need to be implemented
}

bool TDriverUSB::is_operable_tracker(trackerNumber_t number)
{
    //TODO: need to be implemented
    return true;
}

trackerValue_t TDriverUSB::read_spot(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

trackerValue_t TDriverUSB::read_smooth(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

trackerValue_t TDriverUSB::read_derivative(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

IDriver* TDriverUSB::create_driver()
{
    return new TDriverUSB;
}



// class TDriverWIFI
TDriverWIFI::TDriverWIFI()
{

}

void TDriverWIFI::initialize_driver()
{
    //TODO: need to be implemented
}

void TDriverWIFI::activate_tracker(trackerNumber_t number)
{
    //TODO: need to be implemented
}

bool TDriverWIFI::is_operable_tracker(trackerNumber_t number)
{
    //TODO: need to be implemented
    return true;
}

trackerValue_t TDriverWIFI::read_spot(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

trackerValue_t TDriverWIFI::read_smooth(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

trackerValue_t TDriverWIFI::read_derivative(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

IDriver* TDriverWIFI::create_driver()
{
    return new TDriverWIFI;
}



// class TDriverEthernet
TDriverEthernet::TDriverEthernet()
{

}

void TDriverEthernet::initialize_driver()
{
    //TODO: need to be implemented
}

void TDriverEthernet::activate_tracker(trackerNumber_t number)
{
    //TODO: need to be implemented
}

bool TDriverEthernet::is_operable_tracker(trackerNumber_t number)
{
    //TODO: need to be implemented
    return true;
}

trackerValue_t TDriverEthernet::read_spot(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

trackerValue_t TDriverEthernet::read_smooth(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

trackerValue_t TDriverEthernet::read_derivative(trackerNumber_t number)
{
    //TODO: need to be implemented
    return 0;
}

IDriver* TDriverEthernet::create_driver()
{
    return new TDriverEthernet;
}

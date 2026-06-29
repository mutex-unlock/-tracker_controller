#include "API_cpp_interface.h"
#include "API_cpp_interface_implementation.h"
// реализация функции create_controller() интерфейсного класса API для взаимодействия с приложением

using namespace NTracker;

// метод для создания экземпляра класса TTrackerController
ITrackerController* ITrackerController::create_tracker_controller()
{
    return new TTrackerController;
}

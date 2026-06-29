#pragma once
// Класс ITrackerController, объявляющий интерфейс API для взаимодействия с приложением


#include "tracker_declarations.h"

namespace NTracker
{

    class ITrackerController
    {
    public:
        virtual ~ITrackerController() = default;

        // метод для запуска контроллера датчиков
        virtual void initialize_tracker_controller() = 0;
        // метод для остановки контроллера датчиков
        virtual void shutdown_tracker_controller() = 0;
        
        // метод для назначения драйвера.
        // Этот драйвер должен быть создан и назначен в самом начале работы,
        // поскольку он будет передаваться новым создаваемым датчикам.
        virtual void assign_driver(driverPointer_t driver) = 0;

        // метод для получения драйвера.
        // Этот драйвер должен быть создан и назначен в самом начале работы,
        // поскольку он будет передаваться новым создаваемым датчикам.
        virtual driverPointer_t get_assigned_driver() = 0;

        // узнать назначенный драйвер для соответствующего датчика
        virtual driverPointer_t get_tracker_driver(trackerNumber_t number) = 0;

        // метод для добавления датчика
        virtual void add_tracker(ETrackerType type, trackerNumber_t number) = 0;
        // метод для удаления датчика
        virtual void delete_tracker(trackerNumber_t number) = 0;
        
        // метод проверяет существует ли датчик с переданным номером
        virtual bool check_tracker_exist(trackerNumber_t number) = 0;
        // метод проверяет является ли датчик работоспособным
        virtual bool is_tracker_operable(trackerNumber_t number) = 0;

        // метод для чтения текущего показания датчика
        virtual trackerValue_t get_tracker_value(trackerNumber_t number) = 0;

        // осуществляется асинхронный запрос показания датчика,
        // значение будет возвращаться через передаваемый обратный вызов
        virtual void query_tracker_value(trackerNumber_t number, trackerValueCallback_t callback) = 0;

        // осуществляется опрос показаний всех работоспособных датчиков,
        // значения также возвращаются через обратный вызов
        virtual void read_tracker_values(trackerValueCallback_t callback) = 0;

        // можно получить минимальное значение для набора датчиков
        // с номерами из указанного диапазона
        virtual trackerValue_t get_min_value(trackerNumber_t first, trackerNumber_t last) = 0;

        // можно получить максимальное значение для набора датчиков
        // с номерами из указанного диапазона
        virtual trackerValue_t get_max_value(trackerNumber_t first, trackerNumber_t last) = 0;

        // назначается отслеживание пороговых значений датчиков
        virtual void set_alert(trackerNumber_t number,
                              trackerAlertCallback_t callback,
                              trackerValue_t alertValue,
                              EAlertRule alertRule,
                              checkAlertTimeout_t checkTimeoutSet = 1) = 0;

        // выключается отслеживание пороговых значений датчиков
        virtual void reset_alert(trackerNumber_t number) = 0;


        // метод для создания экземпляра класса TTrackerController
        static ITrackerController* create_tracker_controller();
    };

};  // end namespace NTracker

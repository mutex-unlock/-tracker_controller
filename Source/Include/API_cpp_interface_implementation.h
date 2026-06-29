#pragma once
// Класс TTrackerController, реализующий интерфейс API интерфейсного класса
//       ITrackerController для взаимодействия с приложением

#include "API_cpp_interface.h"

#include <map>
#include <list>

namespace NTracker
{

    class ITracker;
    class IDriver;
    class TRequestQueue;
    class TAlertMonitoring;
    class TTrackerStorage;

    class TTrackerController: public ITrackerController
    {
    public:
        TTrackerController();
        ~TTrackerController();

        // метод для запуска контроллера датчиков
        void initialize_tracker_controller() override;
        // метод для остановки контроллера датчиков
        void shutdown_tracker_controller() override;
        
        // метод для назначения драйвера.
        // Этот драйвер должен быть создан и назначен в самом начале работы,
        // поскольку он будет передаваться новым создаваемым датчикам.
        void assign_driver(driverPointer_t driver) override;

        // метод для получения драйвера.
        // Этот драйвер должен быть создан и назначен в самом начале работы,
        // поскольку он будет передаваться новым создаваемым датчикам.
        driverPointer_t get_assigned_driver() override;

        // узнать назначенный драйвер для соответствующего датчика
        driverPointer_t get_tracker_driver(trackerNumber_t number) override;

        // метод для добавления датчика
        void add_tracker(ETrackerType type, trackerNumber_t number) override;
        // метод для удаления датчика
        void delete_tracker(trackerNumber_t number) override;

        // метод проверяет существует ли датчик с переданным номером
        bool check_tracker_exist(trackerNumber_t number) override;
        // метод проверяет является ли датчик работоспособным
        bool is_tracker_operable(trackerNumber_t number) override;

        // метод для чтения текущего показания датчика
        trackerValue_t get_tracker_value(trackerNumber_t number) override;

        // осуществляется асинхронный запрос показания датчика,
        // значение будет возвращаться через передаваемый обратный вызов
        void query_tracker_value(trackerNumber_t number, trackerValueCallback_t callback) override;

        // осуществляется опрос показаний всех работоспособных датчиков,
        // значения также возвращаются через обратный вызов
        void read_tracker_values(trackerValueCallback_t callback) override;

        // можно получить минимальное значение для набора датчиков
        // с номерами из указанного диапазона
        trackerValue_t get_min_value(trackerNumber_t first, trackerNumber_t last) override;

        // можно получить максимальное значение для набора датчиков
        // с номерами из указанного диапазона
        trackerValue_t get_max_value(trackerNumber_t first, trackerNumber_t last) override;

        // назначается отслеживание пороговых значений датчиков
        void set_alert(trackerNumber_t number,
                       trackerAlertCallback_t callback,
                       trackerValue_t alertValue,
                       EAlertRule alertRule,
                       checkAlertTimeout_t checkTimeoutSet = 1) override;

        // выключается отслеживание пороговых значений датчиков
        void reset_alert(trackerNumber_t number) override;

    private:
        // метод, который проверяет, была ли выполнена инициализация
        // (если нет, то выбрасывает исключение)
        void check_initialize();
        // метод проверяет, был ли установлен драйвер
        void check_set_driver();

    private:
        // контейнер для хранения датчиков
        TTrackerStorage* trackerStorage_;

        // класс для выполнения асинхронных запросов
        TRequestQueue* requestQueue_;

        // класс для отслеживания пороговых значений
        TAlertMonitoring* alertMonitoring_;

        // индикатор указывает, была ли выполнена инициализация
        bool isInitialized_;

        driverPointer_t driver_;

    };  // end class TTrackerController

};  // end namespace NTracker

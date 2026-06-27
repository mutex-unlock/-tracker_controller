#pragma once
// интерфейс класса для отслеживания пороговых значений датчиков

#include "tracker_declarations.h"

#include <map>              // контейнер с уведомлениями
#include <thread>           // для рабочего потока std::jthread и std::stop_token
#include <mutex>            // примитивы синхронизации: std::lock_guard, std::mutex

namespace NTracker
{

    class TAlertMonitoring
    {
    public:
        // метод для запуска процесса отслеживания пороговых значений
        void start_alert_monitoring();
        // метод для останова процесса отслеживания пороговых значений
        void stop_alert_monitoring();

        // добавляет датчик для отслеживания
        void add_alert(trackerNumber_t number,
                       trackerPointer_t pointer,
                       trackerAlertCallback callback,
                       trackerValue_t alertValue,
                       EAlertRule alertRule,
                       checkAlertTimeout_t checkTimeoutSet);
        
        // удаляет датчик для отслеживания
        void delete_alert(trackerNumber_t number);

    private:
        // структура, в которой хранятся данные, необходимые для отслеживания показаний датчика
        struct TAlert
        {
            TAlert() {}

            // конструктор структуры TAlert
            TAlert(trackerAlertCallback callback, trackerValue_t alertValue,
                   EAlertRule alertRule, trackerPointer_t tracker,
                   checkAlertTimeout_t checkTimeout) :
                       callback(callback), alertValue(alertValue),
                       alertRule(alertRule), tracker(tracker),
                       checkTimeout(checkTimeout), currentTimeout(0)
            {

            }

            trackerAlertCallback callback;
            trackerValue_t alertValue;
            EAlertRule alertRule;
            trackerPointer_t tracker;
            checkAlertTimeout_t checkTimeout;
            checkAlertTimeout_t currentTimeout;

        };  // end struct TAlert

        std::map<trackerNumber_t, TAlert> mapAlerts_;   // контейнер с уведомлениями
        std::jthread pollThread_;                       // C++20: autojoin and interruptible thread
        std::mutex mutex_;

        // в этом методе реализовано отслеживание показаний датчиков.
        // Поток вызывает этот метод, который циклически опрашивает назначенные датчики
        // и в случае превышения пороговых значений осуществляет обратный вызов.
        void poll(std::stop_token stoken);

    };  // end class TAlertMonitoring

};  // end namespace NTracker

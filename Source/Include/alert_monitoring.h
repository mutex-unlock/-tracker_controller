#pragma once
// интерфейс класса для отслеживания пороговых значений датчиков

#include "tracker_declarations.h"

#include <map>          // контейнер с уведомлениями
#include <thread>       // для рабочего потока std::jthread и std::stop_token
#include <mutex>        // примитивы синхронизации: std::lock_guard, std::mutex

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
        void add_alert(trackerNumber_t trackerNumber,
                       trackerPointer_t trackerPointer,
                       trackerAlertCallback_t callback,
                       trackerValue_t alertValue,
                       EAlertRule alertRule,
                       checkAlertTimeout_t checkTimeoutSet);

        // удаляет датчик для отслеживания
        void delete_alert(trackerNumber_t trackerNumber);

    private:
        // структура, в которой хранятся данные, необходимые для отслеживания показаний датчика
        struct TAlert
        {
            TAlert() {}

            // конструктор структуры TAlert
            TAlert(trackerAlertCallback_t callback,
                   trackerValue_t alertValue,
                   EAlertRule alertRule,
                   trackerPointer_t tracker,
                   checkAlertTimeout_t checkTimeout) :
                       callback_(callback),
                       alertValue_(alertValue),
                       alertRule_(alertRule),
                       trackerPointer_(tracker),
                       checkTimeout_(checkTimeout),
                       currentTimeout_(0)
            {

            }

            trackerAlertCallback_t callback_;
            trackerValue_t alertValue_;
            EAlertRule alertRule_;
            trackerPointer_t trackerPointer_;
            checkAlertTimeout_t checkTimeout_;
            checkAlertTimeout_t currentTimeout_;

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

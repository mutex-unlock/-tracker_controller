#include "alert_monitoring.h"
#include "tracker_interface.h"
// реализация класса для отслеживания пороговых значений датчиков

#include <chrono>

using namespace NTracker;


// метод для запуска процесса отслеживания пороговых значений
void TAlertMonitoring::start_alert_monitoring()
{
    if (!pollThread_.joinable())
    {
        pollThread_ = std::jthread(&TAlertMonitoring::poll, this);
    }

}   // end TAlertMonitoring::start_alert_monitoring()


// метод для останова процесса отслеживания пороговых значений
void TAlertMonitoring::stop_alert_monitoring()
{
    if (pollThread_.joinable())
    {
        pollThread_.request_stop();     // C++20: Запрос остановки потока

        pollThread_.join();
        mapAlerts_.clear();
    }

}   // end TAlertMonitoring::stop_alert_monitoring()


// в этом методе реализовано отслеживание показаний датчиков.
// Поток вызывает этот метод, который циклически опрашивает назначенные датчики
// и в случае превышения пороговых значений осуществляет обратный вызов.
void TAlertMonitoring::poll(std::stop_token stoken)
{
    using namespace std::chrono_literals;

    while (!stoken.stop_requested())    // C++20: если не запрошена остановка потока
    {
        // поток засыпает на 1 секунду, т.е. интервал опроса равен 1 секунде
        std::this_thread::sleep_for(1s);
        // блокируется мьютекс, чтобы избежать коллизий добавления/удаления элементов в контейнере.
        std::lock_guard<std::mutex> lock(mutex_);

        // осуществляется опрос элементов, хранящихся в контейнере
        for (auto& item : mapAlerts_)
        {
            TAlert& alert = item.second;
            // Текущее время опроса увеличивается на единицу
            alert.currentTimeout++;

            // Если уведомление разрешено, о чем говорит ненулевое значение checkTimeout,
            // и время последнего опроса превысило назначенное время
            if (alert.checkTimeout != 0 && alert.currentTimeout >= alert.checkTimeout)
            {
                bool triggerAlert = false;

                // тогда проверяется, имелось ли превышение пороговых значений
                // в соответствии с назначенными правилами
                if (alert.alertRule == EAlertRule::More)
                {
                    triggerAlert = alert.tracker->get_current_value() > alert.alertValue;
                }
                else
                {
                    triggerAlert = alert.tracker->get_current_value() < alert.alertValue;
                }

                // если превышение пороговых значений зафиксировано
                if (triggerAlert)
                {
                    // то осуществляется обратный вызов
                    // этот вызов возвращает следующий интервал опроса
                    alert.checkTimeout = alert.callback(item.first,
                                                        item.second.tracker->get_current_value());
                }
                // после чего текущее время сбрасывается
                alert.currentTimeout = 0;
            }

        }   // end for

    }   // end while

}   // end TAlertMonitoring::poll()


// добавляет датчик для отслеживания
void TAlertMonitoring::add_alert(trackerNumber_t number,
                                 trackerPointer_t pointer,
                                 trackerAlertCallback callback,
                                 trackerValue_t alertValue,
                                 EAlertRule alertRule,
                                 checkAlertTimeout_t checkTimeoutSet)
{
    std::lock_guard<std::mutex> lock(mutex_);

    mapAlerts_[number] = TAlert(callback, alertValue, alertRule, pointer, checkTimeoutSet);

}   // end TAlertMonitoring::add_alert()


// удаляет датчик для отслеживания
void TAlertMonitoring::delete_alert(trackerNumber_t number)
{
    std::lock_guard<std::mutex> lock(mutex_);

    mapAlerts_.erase(number);

}   // end TAlertMonitoring::delete_alert()

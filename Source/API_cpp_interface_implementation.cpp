#include "API_cpp_interface_implementation.h"
#include "tracker_interface.h"
#include "driver_interface.h"
#include "request_queue.h"
#include "alert_monitoring.h"
#include "errors_declaration.h"
#include "tracker_storage.h"
// реализация класса TTrackerController, который реализует API
// интерфейсного класса ITrackerController для взаимодействия с приложением

using namespace NTracker;

// публичные методы:

TTrackerController::TTrackerController():
    requestQueue_(new TRequestQueue),
    alertMonitoring_(new TAlertMonitoring),
    trackerStorage_(new TTrackerStorage),
    isInitialized_(false)
{

}

TTrackerController::~TTrackerController()
{
    shutdown_tracker_controller();
    delete requestQueue_;
    delete alertMonitoring_;
    delete trackerStorage_;
}

// метод для запуска контроллера датчиков
void TTrackerController::initialize_tracker_controller()
{
    requestQueue_->start_queue();
    alertMonitoring_->start_alert_monitoring();
    isInitialized_ = true;
}

// метод для остановки контроллера датчиков
void TTrackerController::shutdown_tracker_controller()
{
    requestQueue_->stop_queue();
    alertMonitoring_->stop_alert_monitoring();
    isInitialized_ = false;
}

// метод для назначения драйвера.
// Этот драйвер должен быть создан и назначен в самом начале работы,
// поскольку он будет передаваться новым создаваемым датчикам.
void  TTrackerController::assign_driver(driverPointer_t driver)
{
    driver_ = driver;
}

// метод для получения драйвера.
// Этот драйвер должен быть создан и назначен в самом начале работы,
// поскольку он будет передаваться новым создаваемым датчикам.
driverPointer_t TTrackerController::get_assigned_driver()
{
    return driver_;
}

// узнать назначенный драйвер для соответствующего датчика
driverPointer_t TTrackerController::get_tracker_driver(trackerNumber_t number)
{
    return trackerStorage_->find_tracker(number)->get_driver();
}

// метод для добавления датчика
void TTrackerController::add_tracker(ETrackerType type, trackerNumber_t number)
{
    check_set_driver();
    trackerPointer_t trackerPointer = ITracker::create_tracker(type, number, driver_);
    trackerStorage_->add_tracker(number,trackerPointer);
}

// метод для удаления датчика
void TTrackerController::delete_tracker(trackerNumber_t number)
{
    trackerStorage_->delete_tracker(number);
}

// метод проверяет существует ли датчик с переданным номером
bool TTrackerController::check_tracker_exist(trackerNumber_t number)
{
    return trackerStorage_->check_tracker_exist(number).operator bool();
}

// метод проверяет является ли датчик работоспособным
bool TTrackerController::is_tracker_operable(trackerNumber_t number)
{
    return trackerStorage_->find_tracker(number)->is_operable_tracker();
}

// метод для чтения текущего показания датчика
trackerValue_t TTrackerController::get_tracker_value(trackerNumber_t number)
{
    return trackerStorage_->find_tracker(number)->get_current_value();
}

// осуществляется асинхронный запрос показания датчика,
// значение будет возвращаться через передаваемый обратный вызов
void TTrackerController::query_tracker_value(trackerNumber_t number,
                                             trackerValueCallback_t callback)
{
    check_initialize();
    requestQueue_->add_request(number,
                               trackerStorage_->find_tracker(number),
                               callback);
}

// осуществляется опрос показаний всех работоспособных датчиков,
// значения также возвращаются через обратный вызов
void TTrackerController::read_tracker_values(trackerValueCallback_t callback)
{
    // производится проверка, инициализирован ли класс
    // если класс не проинициализирован, то функция выбросит исключение
    check_initialize();

    // происходит перебор элементов контейнера,
    // в качестве обратного вызова используется лямбда-выражение
    // контейнер будет вызывать лямбда-выражение,
    // в которое он будет передавать номер датчика и указатель на экземпляр класса
    trackerStorage_->callback_all_trackers([callback](trackerNumber_t number,
                                                      trackerPointer_t trackerPointer)
        {
            // проверяется, является ли датчик работоспособным
            if (trackerPointer->is_operable_tracker())
            {
                // выполняется соответствующий обратный вызов
                callback(number, trackerPointer->get_current_value());
            }
        }
    );
}

// класс для анализа минимального и максимального значения
class TFindMinMaxValue
{
public:
    enum MinMaxSign { MIN_VALUE = 0, MAX_VALUE = 1 };

    // конструктор, который принимает на вход следующие параметры:
    // минимальное значение диапазона номеров; максимальное значение диапазона номеров;
    // параметр, указывающий, что необходим поиск минимального либо максимального значения
    TFindMinMaxValue(trackerNumber_t first,
                     trackerNumber_t last,
                     MinMaxSign sign) :
        sign_(sign), first_(first), last_(last), count_(0)
    {
        if (sign == MAX_VALUE)
        {
            result_ = std::numeric_limits<trackerValue_t>::max();   // максимальное значение
        }
        else
        {
            result_ = std::numeric_limits<trackerValue_t>::min();   // минимальное значение
        }

        // инициализируется массив указателей на функцию
        arrayFunMinMax_[MIN_VALUE] = &TFindMinMaxValue::compare_min;
        arrayFunMinMax_[MAX_VALUE] = &TFindMinMaxValue::compare_max;
    }

    // анализ очередного значения происходит в перегруженном операторе
    // на вход подаются номер датчика и указатель на датчик
    void operator()(trackerNumber_t number, trackerPointer_t trackerPointer)
    {
        // если датчик работоспособный и его номер попадает в заданный диапазон номеров
        if ( trackerPointer->is_operable_tracker() && (number >= first_ && number <= last_) )
        {
            // зависимости от параметра поиска
            // через указатель вызывается соответствующая функция для анализа
            (this->*arrayFunMinMax_[sign_])(trackerPointer->get_current_value());
            // увеличивается счетчик просмотренных датчиков
            count_++;
        }
    }

    // геттер, возвращающий итоговый результат
    trackerValue_t result() { return result_; }
    // геттер, возвращающий количество датчиков, которые участвовали в поиске
    size_t count() { return count_; }

private:
    trackerNumber_t first_; // минимальное значение диапазона
    trackerNumber_t last_;  // максимальное значение диапазона
    MinMaxSign sign_;       // параметр для поиска
    trackerValue_t result_; // итоговый результат
    size_t count_;          // количество датчиков, которые участвовали в поиске

    using funMinMax_t = void (TFindMinMaxValue::*)(trackerValue_t value);

    void compare_min(trackerValue_t value)
    {
        if (result_ > value)
        {
            result_ = value;
        }
    }
    void compare_max(trackerValue_t value)
    {
        if (result_ < value)
        {
            result_ = value;
        }
    }

    // массив указателей на функцию
    funMinMax_t arrayFunMinMax_[2];

};  // end class TFindMinMaxValue

// можно получить минимальное значение для набора датчиков
// с номерами из указанного диапазона
trackerValue_t TTrackerController::get_min_value(trackerNumber_t first,
                                                 trackerNumber_t last)
{
    check_initialize();

    TFindMinMaxValue fmv(first, last, TFindMinMaxValue::MIN_VALUE);
    trackerStorage_->callback_all_trackers(fmv);
    return fmv.result();
}

// можно получить максимальное значение для набора датчиков
// с номерами из указанного диапазона
trackerValue_t TTrackerController::get_max_value(trackerNumber_t first,
                                                 trackerNumber_t last)
{
    check_initialize();

    TFindMinMaxValue fmv(first, last, TFindMinMaxValue::MAX_VALUE);
    trackerStorage_->callback_all_trackers(fmv);
    return fmv.result();
}


// назначается отслеживание пороговых значений датчиков
void TTrackerController::set_alert(trackerNumber_t number,
                                   trackerAlertCallback_t callback,
                                   trackerValue_t alertValue,
                                   EAlertRule alertRule,
                                   checkAlertTimeout_t checkTimeoutSet)
{
    check_initialize();
    alertMonitoring_->add_alert(number,
                                trackerStorage_->find_tracker(number),
                                callback, alertValue,
                                alertRule, checkTimeoutSet);
}

// выключается отслеживание пороговых значений датчиков
void TTrackerController::reset_alert(trackerNumber_t number)
{
    check_initialize();
    alertMonitoring_->delete_alert(number);
}


// приватные методы:

// приватный метод, который проверяет, была ли выполнена инициализация
// (если нет, то выбрасывает исключение)
void TTrackerController::check_initialize()
{
    if (!isInitialized_)
    {
        TTrackerException::throw_exception(ETrackerError::NotInitialized);
    }
}

// приватный метод проверяет, был ли установлен драйвер
void TTrackerController::check_set_driver()
{
    if (!driver_)
    {
        TTrackerException::throw_exception(ETrackerError::DriverIsNotSet);
    }
}

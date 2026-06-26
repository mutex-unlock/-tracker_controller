#include "request_queue.h"
#include "tracker_interface.h"
// реализация интерфейса класса для выполнения асинхронных вызовов callbacks.

// Для реализации асинхронных запросов объявляется очередь,
// в которую помещаются все поступающие запросы.
// Обработка очереди происходит в отдельном потоке.
// Поток извлекает очередной запрос и для него выполняет обратный вызов

using namespace NTracker;


// метод, который запускает поток обработки очереди 
void TRequestQueue::start_queue()
{
    if (!queueThread_.joinable())
    {
        queueThread_ = std::jthread(&TRequestQueue::read_request, this);
    }
}


// метод для остановки потока обработки очереди
void TRequestQueue::stop_queue()
{
    if (queueThread_.joinable())
    {
        std::unique_lock<std::mutex> lock(mutex_);
        queueThread_.request_stop();                      // C++20: Запрос остановки потока

        lock.unlock();
        conditional_.notify_one();
        queueThread_.join();
        
        requestQueue_ = decltype(requestQueue_)();
    }
}


// добавляет переданные данные в очередь
// путем создания экземпляра структуры Request
// и размещения ее в очереди requestQueue_
void TRequestQueue::add_request(trackerNumber_t trackerNumber,
                                trackerPointer_t trackerPointer,
                                trackerValueCallback callback)
{
    std::lock_guard<std::mutex> lock(mutex_);

    requestQueue_.push({ trackerNumber, trackerPointer, callback });

    conditional_.notify_one();
}


// Обработка очереди реализована в методе read_request()
// Поток обработки очереди вызывает этот метод,
// который, в свою очередь, ожидает поступления записей и обрабатывает их
void TRequestQueue::read_request(std::stop_token stoken)
{
    while (!stoken.stop_requested())
    {
        std::unique_lock<std::mutex> lock(mutex_);

        conditional_.wait(lock, [this]() {return requestQueue_.size() > 0 || stoken.stop_requested(); });

        while (requestQueue_.size() > 0 && !stoken.stop_requested())
        {
            Command cmd = requestQueue_.front();
            requestQueue_.pop();
            lock.unlock();
            cmd.callback(cmd.trackerNumber, cmd.trackerPointer->getValue());
            lock.lock();
        }
    }
}

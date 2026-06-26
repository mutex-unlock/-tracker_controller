#pragma once
// интерфейс класса для выполнения асинхронных вызовов callbacks.

// Для реализации асинхронных запросов объявляется очередь,
// в которую помещаются все поступающие запросы.
// Обработка очереди происходит в отдельном потоке.
// Поток извлекает очередной запрос и для него выполняет обратный вызов


#include "tracker_declarations.h"

#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace NTracker
{

    class TRequestQueue
    {
    public:
        // метод, который запускает поток обработки очереди
        void start_queue();
        // метод для остановки потока обработки очереди
        void stop_queue();
        
        // добавляет переданные данные в очередь
        // путем создания экземпляра структуры Request
        // и размещения ее в очереди requestQueue_
        void add_request(trackerNumber_t trackerNumber,
                         trackerPointer_t trackerPointer,
                         trackerValueCallback callback);

    private:
        // структура, в которой хранятся данные для выполнения обратного вызова
        struct Request
        {
            trackerNumber_t trackerNumber;      // номер датчика
            trackerPointer_t trackerPointer;    // указатель на класс датчика
            trackerValueCallback callback;      // объект обратного вызова
        };

        std::queue<Request> requestQueue_;      // очередь запросов
        std::condition_variable conditional_;   // cv
        std::mutex mutex_;                      // mutex
        std::jthread queueThread_;              // рабочий поток (C++20)
        
        // Обработка очереди реализована в методе read_request()
        // Поток обработки очереди вызывает этот метод,
        // который, в свою очередь, ожидает поступления записей и обрабатывает их
        void read_request(std::stop_token stoken);
    };

};  // end namespace NTracker

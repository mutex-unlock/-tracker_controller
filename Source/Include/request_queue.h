#pragma once
// интерфейс класса для выполнения асинхронных вызовов callbacks.

// Для реализации асинхронных запросов объявляется очередь,
// в которую помещаются все поступающие запросы.
// Обработка очереди происходит в отдельном потоке.
// Поток извлекает очередной запрос и для него выполняет обратный вызов


#include "tracker_declarations.h"

#include <queue>                // контейнер с callbacks
#include <mutex>
#include <condition_variable>
#include <thread>               // C++20: для рабочего потока std::jthread и std::stop_token

namespace NTracker
{
    // класса для выполнения асинхронных вызовов callbacks
    class TRequestQueue
    {
    public:
        // метод, который запускает поток обработки очереди
        void start_queue();

        // метод для остановки потока обработки очереди
        void stop_queue();

        // добавляет переданные данные в очередь
        // путем создания экземпляра структуры TRequest
        // и размещения ее в очереди requestQueue_
        void add_request(trackerNumber_t trackerNumber,
                         trackerPointer_t trackerPointer,
                         trackerValueCallback_t callback);

    private:
        // В приватном методе read_request() реализована обработка очереди.
        // Поток обработки очереди вызывает этот метод,
        // который, в свою очередь, ожидает поступления записей и обрабатывает их
        void read_request(std::stop_token stoken);

    private:
        // структура, в которой хранятся данные для выполнения обратного вызова
        struct TRequest
        {
            trackerNumber_t trackerNumber_;     // номер датчика
            trackerPointer_t trackerPointer_;   // указатель на класс датчика
            trackerValueCallback_t callback_;   // объект обратного вызова
        };

        std::queue<TRequest> requestQueue_;     // очередь запросов
        std::condition_variable conditional_;   // cv
        std::mutex mutex_;                      // mutex
        std::jthread queueThread_;              // C++20: autojoin and interruptible thread

    };

};  // end namespace NTracker

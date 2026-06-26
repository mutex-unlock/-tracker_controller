#pragma once
// интерфейс класса TTrackerStorage
// предназначен для хранения экземпляров классов для управления датчиками
// хранилище объектов реализовано в виде двоичного дерева.
// ключом здесь выступает номер датчика, содержимым является указатель на класс управления датчиком


#include "tracker_declarations.h"
#include "tracker_interface.h"

#include <map>

namespace NTracker
{

    class TTrackerStorage
    {
    public:
        // метод для добавления указателей
        void add_tracker(trackerNumber_t trackerNumber,
                         trackerPointer_t trackerPointer);

        // метод для удаления указателей
        void delete_tracker(trackerNumber_t trackerNumber);
        
        // возвращает указатель на объект класса, если с заданным номером содержится в хранилище,
        // в противном случае возвращается нулевой указатель
        trackerPointer_t check_tracker_exist(trackerNumber_t trackerNumber);
        
        // возвращает указатель на объект класса для соответствующего номера;
        // если объект отсутствует, то генерируется исключение.
        trackerPointer_t find_tracker(trackerNumber_t trackerNumber);

        // предназначен для итерации по всем хранимым объектам
        // для каждого элемента выполняется соответствующий обратный вызов
        template<typename CallbackIterate>
        void callback_all_trackers(CallbackIterate&& callback)
        {
            for (auto item : storage_)
            {
                callback(item.first, item.second);
            }
        }

    private:
        // ключ - номер датчика, значение - указатель на класс управления датчиком
        std::map<trackerNumber_t, trackerPointer_t> storage_;

    };  // end class TTrackerStorage

};  // end namespace NTracker

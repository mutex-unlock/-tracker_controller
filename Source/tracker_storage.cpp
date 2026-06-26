#include "tracker_storage.h"
#include "errors_declaration.h"
// реализация класса TTrackerStorage
// предназначен для хранения экземпляров классов для управления датчиками
// хранилище объектов реализовано в виде двоичного дерева.
// ключом здесь выступает номер датчика, содержимым является указатель на класс управления датчиком


using namespace NTracker;


// метод для добавления указателей
void TTrackerStorage::add_tracker(trackerNumber_t trackerNumber,
                                  trackerPointer_t trackerPointer)
{
    storage_.insert({ trackerNumber, trackerPointer });
}


// метод для удаления указателей
void TTrackerStorage::delete_tracker(trackerNumber_t trackerNumber)
{
    storage_.erase(trackerNumber);
}


// возвращает указатель на объект класса для соответствующего номера;
// если объект отсутствует, то генерируется исключение.
trackerPointer_t TTrackerStorage::find_tracker(trackerNumber_t trackerNumber)
{
    auto iterator = storage_.find(trackerNumber);

    if (iterator == storage_.end())
    {
        TTrackerException::throw_exception(ETrackerError::UnknownTrackerNumber);
    }
    return iterator->second;
}


// возвращает указатель на объект класса, если с заданным номером содержится в хранилище,
// в противном случае возвращается нулевой указатель
trackerPointer_t TTrackerStorage::check_tracker_exist(trackerNumber_t trackerNumber)
{
    auto iterator = storage_.find(trackerNumber);

    if (iterator != storage_.end())
    {
        return iterator->second;
    }
    else
    {
        return trackerPointer_t();
    }
}

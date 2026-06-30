#pragma once
// В функции API для задания типа драйвера используются числовые значения,
// а в интерфейсном классе используются перечисления C++.

// Для того, чтобы сконвертировать числовое значение в перечисление,
// используется вспомогательный класс TNumEnumTranslator


// шаблонный класс, параметром которого является тип перечисления
template <typename EnumType>
class TNumEnumTranslator
{
public:
    // шаблонная функция конвертации
    // параметром шаблона является тип числового значения для конвертации.
    // Функция принимает число, которое должно быть сконвертировано,
    // а также список значений перечисления
    template<typename ConvValueType>
    void convert_to_enum(ConvValueType intValue, std::initializer_list<EnumType> initList)
    {
        errorTranslateFlag_ = true;
        // пробегает по всем элементам списка
        for (EnumType item : initList)
        {
            // если какой-то из элементов списка перечисления равен переданному значению
            if (static_cast<ConvValueType>(item) == intValue)
            {
                // запоминает это значение перечисления в качестве результата
                enumResult_ = item;
                errorTranslateFlag_ = false;
                break;
            }
        }
    };  // end convert_to_enum()

    // геттер, возвращающий результат успешной конвертации числа в enum
    bool is_error_convert() const
    {
        return errorTranslateFlag_;
    }

    // геттер, возвращающий enum
    EnumType get_enum_result() const
    {
        return enumResult_;
    }

private:
    bool errorTranslateFlag_;   // ошибка конвертации числового значения в перечисление
    EnumType enumResult_;       // результат конвертации типа EnumType

};  // end template class TNumEnumTranslator

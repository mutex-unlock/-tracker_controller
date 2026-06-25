#pragma once
// объявления интерфейсов конкретных драйверов классов:
// TDriverSimulation,  TDriverUSB, TDriverWIFI, TDriverEthernet

#include "driver_interface.h"   // class IDriver

namespace NTracker
{
    // класс эмулируемого драйвера
    class TDriverSimulation : public IDriver
    {
    public:
        // перечисление для указания используемого метода чтения показателей
        enum EReadType
        {
            RT_SPOT       = 0,
            RT_SMOOTH     = 1,
            RT_DERIVATIVE = 2
        };

        // типы для обратных вызовов
        using onReadValue_t = std::function<trackerValue_t(trackerNumber_t, EReadType)>;
        using onOperable_t  = std::function<bool(trackerNumber_t)>;

        // метод для инициализации драйвера
        void initialize_driver() override;

        // методы для настройки переменных defaultValue_ и defaultOperable_
        void set_default_value(trackerValue_t value);
        void set_default_operable(bool isOperable);
        
        // производится настройка обратного вызова
        void set_read_value(onReadValue_t value);
        // производится настройка обратного вызова
        void set_operable(onOperable_t operable);

        // метод для активации датчика
        void activate_tracker(trackerNumber_t number) override;
        // метод, возвращающий признак работоспособности датчика
        bool is_operable_tracker(trackerNumber_t number) override;

        // метод для чтения текущих значений
        trackerValue_t read_spot(trackerNumber_t number) override;
        // метод для чтения сглаженных значений
        trackerValue_t read_smooth(trackerNumber_t number) override;
        // метод для чтения производных значений
        trackerValue_t read_derivative(trackerNumber_t number) override;

        // метод-фабрика классов, в котором происходит создание класса типа TDriverSimulation
        static IDriver* create_driver();

    protected:
        TDriverSimulation();

    private:
        // Переменные соответствующих типов для хранения обратных вызовов
        onReadValue_t getValue_;
        onOperable_t  getOperable_;
        
        // переменные для хранения значений по умолчанию. Эти переменные
        // настраиваются в методах setDefaultValue(), setDefaultOperable()
        trackerValue_t defaultValue_ = 0;
        bool defaultOperable_ = true;

    };  // end class TDriverSimulation


    // класс драйвера USB
    class TDriverUSB : public IDriver
    {
    public:
        // метод для инициализации драйвера
        void initialize_driver() override;
        // метод для активации датчика
        void activate_tracker(trackerNumber_t  number) override;
        // метод, возвращающий признак работоспособности датчика
        bool is_operable_tracker(trackerNumber_t  number) override;

        // метод для чтения текущих значений
        trackerValue_t read_spot(trackerNumber_t  number) override;
        // метод для чтения сглаженных значений
        trackerValue_t read_smooth(trackerNumber_t  number) override;
        // метод для чтения производных значений
        trackerValue_t read_derivative(trackerNumber_t  number) override;

        // метод-фабрика классов, в котором происходит создание класса типа TDriverUSB
        static IDriver* create();

    protected:
        TDriverUSB();

    };  // end class TDriverUSB


    // класс драйвера Wi-Fi
    class TDriverWIFI : public IDriver
    {
    public:
        // метод для инициализации драйвера
        void initialize_driver() override;
        // метод для активации датчика
        void activate_tracker(trackerNumber_t  number) override;
        // метод, возвращающий признак работоспособности датчика
        bool is_operable_tracker(trackerNumber_t  number) override;

        // метод для чтения текущих значений
        trackerValue_t read_spot(trackerNumber_t  number) override;
        // метод для чтения сглаженных значений
        trackerValue_t read_smooth(trackerNumber_t  number) override;
        // метод для чтения производных значений
        trackerValue_t read_derivative(trackerNumber_t  number) override;

        // метод-фабрика классов, в котором происходит создание класса типа TDriverWIFI
        static IDriver* create();

    protected:
        TDriverWIFI();

    };  // end class TDriverWIFI


    // класс драйвера Ethernet
    class TDriverEthernet : public IDriver
    {
    public:
        // метод для инициализации драйвера
        void initialize_driver() override;
        // метод для активации датчика
        void activate_tracker(trackerNumber_t  number) override;
        // метод, возвращающий признак работоспособности датчика
        bool is_operable_tracker(trackerNumber_t  number) override;

        // метод для чтения текущих значений
        trackerValue_t read_spot(trackerNumber_t  number) override;
        // метод для чтения сглаженных значений
        trackerValue_t read_smooth(trackerNumber_t  number) override;
        // метод для чтения производных значений
        trackerValue_t read_derivative(trackerNumber_t  number) override;

        // метод-фабрика классов, в котором происходит создание класса типа TDriverEthernet
        static IDriver* create();

    protected:
        TDriverEthernet();

    };  // end class TDriverEthernet

};  // end namespace NTracker

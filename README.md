![build](https://github.com/Sheridan/homed-angel/actions/workflows/build.yml/badge.svg)

# homed-angel
Этот проект предназначен для описания автоматизации умного дома на скриптовом языке [AngelScript](https://www.angelcode.com/angelscript/sdk/docs/manual/index.html), бакенд - [HOMEd](https://wiki.homed.dev/page/HOMEd)

Пример конфигурации можно посмотреть в homed-angel.example.json

# О скриптах
Любой скрипт с расширением `.as`, который находится по пути `scripting.location` и содержащий метод `void initialize()` будет загружен и запущен. В скрите будут изначально доступны глобальные переменные `string script_name`, `CLogger logger` и `CHomed homed`.

Основной принцип: в методе `void initialize()` нужно подписаться на необходимые события, где параметром надо будет указать имя скрипта (переменная `script_name`) и метод, который надо будет вызвать при возникновении события.

Приложение автоматически отслеживает появление и изменение мкрптов по пути `scripting.location` и запускает найденное/изменённое.

Примеры скриптов можно посмотреть в каталоге `scripts`
## Типы событий
### Событие изменения значения свойства устройства
Пример объявления функции: `void имя_метода(CProperty @property) {}`
Для подписывания на событие нужно вызвать метод subscribe класса CProperty, например
```as
homed.property(dtZigbee, "PowerSwitch", "status").subscribe(script_name, "имя_метода", false);
homed.property(dtZigbee, "LightRelay", "2", "status").subscribe(script_name, "имя_метода", false);
```
, где последний `bool` параметр указывает на создавать событие при каждом прилёте значения или только по изменению значения.

# Доступные сущности
## CLogger
### Методы
```as
void dbg(const string &msg)
void nfo(const string &msg)
void wrn(const string &msg)
void err(const string &msg)
void cry(const string &msg)
```
Класс предназначен для логгирования событий скрипта. Экземпляр класса доступен через глобальную переменную `logger`.

## EDeviceType
```as
  dtZigbee
  dtCustom
  dtUnknown
```
Перечисляемый тип, характеризующий тип усройств HOMEd

## EPropertyValueType
```as
  pvtEnum
  pvtString
  pvtBool
  pvtDouble
  pvtInt
  pvtColor
  pvtUnknown
```
Перечисляемый тип, характеризующий тип содержимого `CValue`

## CColor
### Конструкторы
```as
CColor()
CColor(uint8 R, uint8 G, uint8 B)
CColor(const int &color)
CColor(const string &color)
CColor(const CColor &other)
```
### Методы
```as
uint8 R() const
uint8 G() const
uint8 B() const
string asHexString() const
```
Один из типов `CValue`

## CValue
### Методы
```as
const string asString() const
const int asInt() const
const double asDouble() const
const bool asBool() const
const CColor asColor() const
bool isString() const
bool isInt() const
bool isDouble() const
bool isBool() const
bool isColor() const
const uint &timestamp() const
```
Значение хранилища `CStorage`, в котором хранится история значений `CProperty`

## CStorage
### Методы
```as
uint16 count() const
bool empty() const
void clear()
CValues list() const
const string type() const
const CValue &last() const
const CValue &at(uint) const
CProperty @property()
```
Хранилище истории значений `CProperty`

## CProperty
### Методы
```as
const string &name() const
const bool &readonly() const
const EPropertyValueType &valueType() const
const CStrings &enumerate() const
const double &min() const
const double &max() const
const double &step() const
CStorage @storage()
const CValue &last() const
const string type() const
CDevice @device()
CEndpoint @endpoint()
void subscribe(script_name, const string &methodName, const bool &changedOnly)
void set(const string &value)
void set(const int &value)
void set(const double &value)
void set(const bool &value)
void set(const CColor &value)
void set(const CValue &value)
```
Класс для доступа к свойствам устройств, доступных в HOMEd

## CProperties
### Методы
```as
const bool empty() const
CStrings list()
bool exists(const string &name)
CProperty@ get(const string &name)
CProperty@ get(const uint16 &index)
uint16 size()
CDevice @device()
CEndpoint @endpoint()
```
Список экземпляров `CProperty`

## CEndpoint
### Методы
```as
const string &name() const
CProperties@ properties()
CDevice @device()
```
Конечная точка устройсва, содержит списки свойств `CProperties`

## CEndpoints
### Методы
```as
const bool empty() const
CStrings list()
bool exists(const string &name)
CEndpoint@ get(const string &name)
CEndpoint@ get(const uint16 &index)
uint16 size()
CDevice @device()
```
Список экземпляров `CEndpoint`

## CDevice
### Методы
```as
const string &name() const
const EDeviceType &type() const
const string &firmware() const
const string &manufacturerName() const
const string &modelName() const
const string &description() const
const bool &interviewFinished() const
const bool &supported() const
const bool &active() const
const bool &cloud() const
const bool &discovery() const
CProperties@ properties()
CEndpoints@ endpoints()
```
Экземпляр устройства HOMEd, содержит как списки свойств `CProperties`, так и списки конечных точек `CEndpoints`

## CDevices
### Методы
```as
const bool empty() const
const EDeviceType &type() const
CStrings list()
bool exists(const string &name)
CDevice@ get(const string &name)
CDevice@ get(const uint16 &index)
uint16 size()
```
Список экземпляров `CDevice`

## CHomed
### Методы
```as
CDevices@ devices(const EDeviceType &type)
CDevice@ device(const string &name)
CDevice@ device(const EDeviceType &type, const string &name)
CEndpoint@ endpoint(const EDeviceType &type, const string &deviceName, const string &endpointName)
CProperty@ property(const EDeviceType &type, const string &deviceName, const string &propertyName)
CProperty@ property(const EDeviceType &type, const string &deviceName, const string &endpointName, const string &propertyName)
```
Оправной класс для работы с устройствами HOMEd. Экземпляр класса изначально доступен через глобальную переменную `homed`

# Сборка
Пока сервис неопакечен, его можно собрать только из исходников.

Нужно установить зависимости, склонировать репозиторий и запустить `make release`. Исполняемый файл `homed-angel` будет находиться в каталоге `.build`
## Зависимости
* PahoMqttCpp
* AngelScript
* jsoncpp

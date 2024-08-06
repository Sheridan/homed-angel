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

# Пример скрипта
К компьютеру подключены
* Мониторы
* Аудиосистема + отдельно сабвуфер.
* Тыловая подстветка мониторов

Так-же в комнате есть люстра (ВНЕЗАПНО).

В HOMEd настроен виртуальный включатель, который должен включать вышеописанную периферию. При этом, при включении люстры, тыловая подсветка должна отключаться и наоборот, включаться, когда люстра гаснет. Так-же, сабвуфер включаться автоматически не должен, но выключаться должен.

Реализация может быть такой:
```as
void managePC(string status)
{
  homed.property(dtZigbee, "PowerSocket_PCBackLight", "status").set(status);
  homed.property(dtZigbee, "PowerSocket_PCMonitors", "status").set(status);
  homed.property(dtZigbee, "PowerSocket_PCSound", "status").set(status);
  if(status == "off")
  {
    homed.property(dtZigbee, "PowerSocket_PCSubWoofer", "status").set("off");
  }
}

void onPCStatusChange(CProperty @property)
{
  string status = property.last().asString();
  if (status != "toggle")
  {
    managePC(status);
  }
}

void onLampStatusChange(CProperty @property)
{
  if(homed.property(dtZigbee, "PowerSocket_PCMonitors", "status").last().asString() == "on")
  {
    homed.property(dtZigbee, "PowerSocket_PCBackLight", "status").set(property.last().asString() == "on" ? "off" : "on");
  }
}

void initialize()
{
  homed.property(dtCustom, "Virtual_PComputer", "status").subscribe(script_name, "onPCStatusChange", true);
  homed.property(dtZigbee, "LightRelay_Hall", "1", "status").subscribe(script_name, "onLampStatusChange", true);
}
```

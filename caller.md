# Механизм вызова оповещений
Для вызова оповещений из C++ в AS применяется очередь, реализуемая в `CCaller` и шаблонные классы
* `CCallerSingle` для одного аргумента
* `CCallerDouble` для двух аргументов

В вызывающем классе создаётся структура, которая будет передана

В `CScript` создаётся `using` тип из `CCallerSingle`/`CCallerDouble` с параметром шаблона, добавляются переопределения метода `callMethod` об шаблоны и их реализация. Так же добавляются переменные из объявленных выше типов (`using`), в конструкторе инициализируется, в деструкторе удаляется. Создаётся именованный метод, возвращающий созданную переменную. В методе run вызывается метод call объявленной переменной.

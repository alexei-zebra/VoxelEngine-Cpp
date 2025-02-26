# Библиотека *entities*

Библиотека предназначена для работы с реестром сущностей.

```lua
-- Возвращает сущность по уникальному идентификатору
-- Возвращаемая таблица - та же, что доступна в компонентах сущности.
entities.get(uid: int) -> table

-- Создает указанную сущность.
-- args - таблица таблиц параметров компонентов (переменная ARGS)
-- args не является обязательным
-- структура args:
-- {префикс__имя={...}, ...}
-- префикс - id пака
-- имя - название компонента
-- префикс и имя компонента разделяются двумя подчеркиваниями
-- Возвращает обьект сущности
entities.spawn(name: str, pos: vec3, [optional] args: table) -> table

-- Проверяет наличие сущности по уникальному идентификатору.
entities.exists(uid: int) -> bool

-- Возвращает индекс определения сущности по UID
entities.get_def(uid: int) -> int

-- Возвращает имя определения сущности по индексу (строковый ID).
entities.def_name(id: int) -> str

-- Возвращает значение свойства 'hitbox' сущности
entities.def_hitbox(id: int) -> vec3

-- Возвращает индекс определения сущности по имени (числовой ID).
entities.def_index(name: str) -> int

-- Возвращает число доступных определений сущностей
entities.defs_count() -> int

-- Возвращает таблицу всех загруженных сущностей
entities.get_all() -> table

-- Возвращает таблицу загруженных сущностей по переданному списку UID
entities.get_all(uids: array<int>) -> table

-- Возвращает список UID сущностей, попадающих в прямоугольную область
-- pos - минимальный угол области
-- size - размер области
entities.get_all_in_box(pos: vec3, size: vec3) -> array<int>

-- Возвращает список UID сущностей, попадающих в радиус
-- center - центр области
-- radius - радиус области
entities.get_all_in_radius(center: vec3, radius: number) -> array<int>
```

```lua
entities.raycast(start: vec3, dir: vec3, max_distance: number,
                 ignore: int, [optional] destination: table, [optional] filter: table) -> table или nil
```

Функция является расширенным вариантом [block.raycast](libblock.md#raycast). Возвращает таблицу с результатами если луч касается блока, либо сущности.

Соответственно это повлияет на наличие полей *entity* и *block*.

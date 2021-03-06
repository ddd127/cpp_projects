# Описание


## randomized_queue
Рандомизированная очередь - это коллекция, которая предоставляет доступ к своим элементам в случайном порядке. Таким образом, каждый отдельный "взгляд" на эту коллекцию даёт случайную, независимую от других, перестановку элементов. Например, если взять два итератора на начало коллекции, а затем каждым пройти по всем элементам до конца, то эти два прохода дадут две разных, независимых друг от друга, перестановки.

### Важно:
Добавление элемента в очередь инвалидирует уже созданные итераторы

### Методы:

* ```empty``` - отвечает на вопрос о пустоте структуры данных
* ```size``` - возвращает количество элементов в структуре данных
* ```enqueue``` - добавляют в структуру данных элемент
* ```sample``` - позволяют посмотреть случайный элемент, но при этом не удаляет его
* ```dequeue``` - возвращает случайный элемент и удаляет его из дека
* ```begin``` - итератор на начало очереди, допускающий изменение элементов
* ```end``` - итератор на конец очереди для begin()
* ```cbegin``` - итератор на начало очереди, НЕ допускающий изменение элементов
* ```cend``` - итератор на конец очереди для cbegin()



## HashSet

HashSet - это коллекция для хранения элементов, позволяющая производить операции вставки, извлечения, проверки наличия, а также итерироваться по элементам коллекции за ```O(1)```

Частично повторяет интерфейс ```std::unordered_set```

Внутрення реализация - хеш-массив с open addressing схемой разрешения коллизий, допускает выбор одной из двух схем разрешения - ```linear probing``` и ```quadratic probing```

### Методы:

* ```begin``` - итератор на начало сета
* ```end``` - итератор на конец сета

* ```bool``` empty - пуст ли сет
* ```size``` - размер сета
* ```max_size``` - максимальный размер сета до следующего увеличения

* ```insert``` - вставка элемента
* ```emplace``` - конструирует элемент "на месте", без лишнего копирования
* ```emplace_hint``` - то же, что emplace
* ```erase``` - удаляет элемент (или несколько) по итератору(-ам) или значению
* ```clear``` - очистка коллекции

* ```swap``` - меняет содержимое контейнеров местами

* ```count``` - возвращает число элементов с заданным ключом
* ```find``` - возвращает итератор на элемент с данным ключом (или end())
* ```contains``` - проверяет наличие элемента
* ```equal_range``` - возвращает пару итераторов (a, b), элементы [a, b) равны искомому

* ```bucket_count``` - возвращает размер хранилища (больше, чем размер сета)
* ```max_bucket_count``` - то же, что и bucket_count
* ```bucket_size``` - всегда 1
* ```bucket``` - всегда 0

* ```load_factor``` - size() / bucket_count()
* ```max_load_factor``` - то же, что и load_factor
* ```rehash``` - перехэширует коллекцию
* ```reserve``` - резервирует место на ещё n элементов, перехэширует коллекцию

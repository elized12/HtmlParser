# HtmlParser

HtmlParser — это библиотека на C++ для парсинга и обработки HTML-документов.  
Она предоставляет удобный API для разбора HTML, построения DOM-дерева, поиска элементов и работы с тегами.

## Возможности

- Разбор HTML-строк и файлов
- Построение DOM-дерева
- Поиск и выборка узлов по тегам и атрибутам
- Обработка ошибок парсинга
- Удобная работа с HTML-тегами и атрибутами

## Структура

- `src/parser/` — основной парсер, DOM-узлы, обработка исключений
- `src/tokenizator/` — токенизация HTML, работа с токенами

## Использование в CMake

Добавьте библиотеку в свой проект через `add_subdirectory`:

```cmake
add_subdirectory(HtmlParser)
target_link_libraries(ваша_цель PRIVATE htmlparser)
```

И добавьте include-директории:

```cmake
target_include_directories(ваша_цель PRIVATE
    ${CMAKE_SOURCE_DIR}/HtmlParser/src/parser
    ${CMAKE_SOURCE_DIR}/HtmlParser/src/tokenizator
)
```

## Пример использования

```cpp
#include "html_parser.h"

int main() {
    HtmlParser parser;
    auto dom = parser.parse("<html><body><h1>Привет!</h1></body></html>");
    return 0;
}
```

## Примеры запросов поиска элементов

### 1. Поиск всех элементов по тегу

```cpp
HtmlParser parser(L"<div><span>1</span><span>2</span></div>");
auto spans = parser.getAllElement(L"span");
for (auto* node : spans) {
    std::wcout << (*node)[L"innerText"] << std::endl;
}
```

### 2. Поиск по классу

```cpp
HtmlParser parser(L"<div class='main'><span class='main'>A</span><span>B</span></div>");
auto mainClass = parser.getAllElement(L".main");
for (auto* node : mainClass) {
    std::wcout << (*node)[L"name"] << L": " << (*node)[L"innerText"] << std::endl;
}
```

### 3. Поиск по id

```cpp
HtmlParser parser(L"<div id='header'><span>Title</span></div>");
auto header = parser.getFirstElement(L"#header");
if (header) {
    std::wcout << (*header)[L"name"] << std::endl;
}
```

### 4. Поиск по тегу и классу одновременно

```cpp
HtmlParser parser(L"<div><span class='main'>A</span><span class='other'>B</span></div>");
auto query = parser.getAllElement(L"span.main");
for (auto* node : query) {
    std::wcout << (*node)[L"innerText"] << std::endl;
}
```

### 5. Поиск по тегу и id

```cpp
HtmlParser parser(L"<div><span id='unique'>A</span><span>B</span></div>");
auto node = parser.getFirstElement(L"span#unique");
if (node) {
    std::wcout << (*node)[L"innerText"] << std::endl;
}
```

### 6. Составной запрос (несколько условий через запятую)

```cpp
HtmlParser parser(L"<div><span class='main'>A</span><span id='unique'>B</span></div>");
auto nodes = parser.getAllElement(L"span.main,span#unique");
for (auto* node : nodes) {
    std::wcout << (*node)[L"innerText"] << std::endl;
}
```

### 7. Поиск вложенных элементов (цепочка запросов)

```cpp
HtmlParser parser(L"<div class='main'><span><b>Bold</b></span></div>");
auto nodes = parser.getAllElement(L"div.main span b");
for (auto* node : nodes) {
    std::wcout << (*node)[L"innerText"] << std::endl;
}
```

---

**Синтаксис запроса:**
- `tag` — по тегу
- `.class` — по классу
- `#id` — по id
- `tag.class` — тег с классом
- `tag#id` — тег с id
- `tag1 tag2` — вложенность
- `query1,query2` — несколько запросов

Используйте методы:
- `getAllElement(L"запрос")` — получить все подходящие элементы
- `getFirstElement(L"запрос")` — получить первый подходящий элемент
﻿Название: XLogo

Авторы: 

Аргиров Антон - интерфейс и сборка проекта
Андрюшкевич Сергей - интерпретатор
Овечкин Константин - лексический анализатор и хранение программы, введенной пользователем, в памяти
Бадаев Кирилл - синтаксический анализатор
Бускин Николай - работа с файлами
Горшков Вячеслав - модуль помощи

Язык программирования: Си

Компилятор: Borland C++ 7.0

XLogo - реализация широко известного обучающего графического языка Logo. Такой язык еще называют "черепешьим", т.к. позволяет управлять на экране монитора пером-"черепашкой", при этом она может "ходить" в разные стороны, оставляя за собой следы, которые представляют собой линии на экране.

Хоть язык и кажется довольно простым, для его реализации требуется много терпения и знания основ написания языков высокого уровня. Да, XLogo, как и другие языки содержит в себе такие части, как лексический анализатор (представляющий вводимые пользователем строки в виде списка лексем - единиц грамматики языка), синтаксический анализатор (проверяющий корректность введенной пользователем команды), интерпретатор (выполняющий непосредственно действия по управлению "черепашкой"). Интерпретатор позволяет пользователю увидеть плоды своего труда сразу после введения новой команды.

Языком XLogo было реализовано большинство (все ?!) средств классического языка Лого: движение "черепашки" вперед, назад, поворот на угол, установка цвета следа, оставляемого "черепашкой", типа следа, рисование дуг эллипсов, переменные и выражения. Среди экзотических средств были реализованы процедуры без параметров, псевдо-функции (переменные, меняющие свое значение в зависимости от своего назначения), циклы со счетчиком и условием, условные операторы, операторы задержки или остановки программы. Среди возможностей программной среды были реализованы точки останова, режимы редактирования, замены, вставки, загрузка с диска и сохранение программы на диск, средства листинга программы и просмотра содержимого переменных.
Лабораторная работа № 4
Тема: Псевдопараллельный сервер
На выполнение Лабораторной работы 4 дается 3 недели.
Срок – 2я неделя мая.
Задание:
1. Разработать программу однопотокового сервера, использующую асинхронный
ввод/вывод (организованный с помощью системного вызова select) обеспечивающую
псевдопараллельную работу клиентов.
2. Написать клиентскую программу, передающую сообщения на сервер.
3. Продемонстрировать асинхронную работу сервера. Например, при запуске клиента
пользователь задает число i от 1 до 10. Клиент передает серверу в цикле это число с
задержкой в i секунд между передачей. Сервер отображает на экран полученную от
клиентов информацию.
Например:
1-й клиент посылает число 1 в цикле с задержкой в 1 сек.
2-ой клиент посылает число 2 с задержкой в 2 сек.
3-й клиент посылает число 3 в цикле с задержкой в 3 сек.
Сервер отображает информацию полученную от клиентов. Если у Вас правильно
организован асинхронный ввод/вывод, то на экран со стороны сервера будет
выводиться с чередованием числа 1, 2 и 3. Причем частота появления определенного
числа будет зависеть от задержки по времени его передачи.
4. Реализация - на языке C/C++, консольные приложения.
Серверная программа должна находить номер свободного порта и выводить его на экран.
При запуске клиентской программы задавать со строки IP адрес сервера и порт.
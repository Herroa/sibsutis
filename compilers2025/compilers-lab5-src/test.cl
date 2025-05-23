class A {
    x : Int;  -- объявление поля
    x : String;  -- ошибка: дублирование имени поля
};

class B inherits A {
    x : Int;  -- ошибка: дублирование имени поля с классом A
    method1(x : Int, x : String) : Int {  -- ошибка: дублирование имени параметра
        x
    };
};

class C inherits D {  -- ошибка: класс D не существует
    method2() : Int {
        z  -- ошибка: переменная z не объявлена
    };
};

class D inherits C {  -- ошибка: циклическое наследование
    method3() : Int {
        1 + "hello"  -- ошибка: несовместимость типов
    };
};

class E {
    method4() : Int {
        "hello".length()  -- ошибка: метод length() не определен для String
    };
};

-- ошибка: отсутствует класс с методом main

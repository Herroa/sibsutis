
print_odd_descending :-
    write('Введите начало диапазона: '), read(Start),
    write('Введите конец диапазона: '), read(End),
    (Start > End -> RangeStart = End, RangeEnd = Start; RangeStart = Start, RangeEnd = End),
    print_odds(RangeEnd, RangeStart).

print_odds(Max, Min) :-
    Max >= Min,
    (Max mod 2 =:= 1 -> write(Max), nl ; true),
    NextMax is Max - 1,
    print_odds(NextMax, Min).
print_odds(_, _).


fibonacci(0, 1).
fibonacci(1, 1).
fibonacci(N, Result) :-
    N > 1,
    N1 is N - 1,
    N2 is N - 2,
    fibonacci(N1, Result1),
    fibonacci(N2, Result2),
    Result is Result1 + Result2.

find_fibonacci :-
    repeat,
    write('Введите номер числа Фибоначчи (отрицательное число для выхода): '),
    read(N),
    (N < 0 -> ! ; fibonacci(N, F), write('F('), write(N), write(') = '), write(F), nl, fail).


split_list :-
    write('Введите список: '), read(List),
    write('Введите первое число: '), read(A),
    write('Введите второе число: '), read(B),
    (A < B -> Min = A, Max = B ; Min = B, Max = A),
    split(List, Min, Max, Less, Between, Greater),
    write('Меньше '), write(Min), write(': '), write(Less), nl,
    write('От '), write(Min), write(' до '), write(Max), write(': '), write(Between), nl,
    write('Больше '), write(Max), write(': '), write(Greater), nl.


split([], _, _, [], [], []).
split([H|T], Min, Max, [H|Less], Between, Greater) :-
    H < Min, split(T, Min, Max, Less, Between, Greater).
split([H|T], Min, Max, Less, [H|Between], Greater) :-
    H >= Min, H =< Max, split(T, Min, Max, Less, Between, Greater).
split([H|T], Min, Max, Less, Between, [H|Greater]) :-
    H > Max, split(T, Min, Max, Less, Between, Greater).


most_frequent :-
    write('Введите список: '), read(List),
    count_occurrences(List, Occurrences),
    max_occurrences(Occurrences, MaxCount),
    find_most_frequent(Occurrences, MaxCount, Result),
    write('Наиболее часто встречающиеся элементы: '), write(Result), nl.

count_occurrences([], []).
count_occurrences([H|T], [[H,Count]|Rest]) :-
    count_occurrences(T, Rest1),
    increment_count(H, Rest1, Rest, Count).

increment_count(_, [], [], 1).
increment_count(Elem, [[Elem, C] | T], T, Count) :- Count is C + 1.
increment_count(Elem, [[Other, C] | T1], [[Other, C] | T2], Count) :-
    Elem \= Other, increment_count(Elem, T1, T2, Count).


max_occurrences([], 0).
max_occurrences([[_, Count]|T], MaxCount) :-
    max_occurrences(T, TempMax),
    MaxCount is max(TempMax, Count).

find_most_frequent([], _, []).
find_most_frequent([[Elem, MaxCount]|T], MaxCount, [Elem|Rest]) :-
    find_most_frequent(T, MaxCount, Rest).
find_most_frequent([_|T], MaxCount, Rest) :-
    find_most_frequent(T, MaxCount, Rest).


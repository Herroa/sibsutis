% facts
parrent(john, bob).
parrent(mary, bob).
parrent(mary, ann).
parrent(bob, liz).
parrent(bob, paul).
parrent(bob, sam).
parrent(paul, pat).


% gender
man(john).
man(bob).
man(sam).
man(paul).
man(pat).

woman(mary).
woman(liz).
woman(ann).


% rules
father(X, Y) :- parrent(X, Y), man(X).
mother(X, Y) :- parrent(X, Y), woman(X).
brother(X, Y) :- parrent(Z, X), parrent(Z, Y), man(X), X \= Y.
sister(X, Y) :- parrent(Z, X), parrent(Z, Y), woman(X), X \= Y.
grandson(X, Y) :- parrent(Z, X), parrent(Y, Z).
aunt(X, Y) :- parrent(Z, Y), sister(X, Z).
nephew(X, Y) :- parrent(Z, Y), sister(X, Z).
parrent_of_two(X) :- 
    findall(Y, parrent(X, Y), Children), 
    length(Children, 2).

man_father(X) :- man(X), parrent(X, _).

% quiries
% а) Кто отец Сэма?
% father(X, sam).

% б) Есть ли мать у Боба? (ответ должен быть true)
% mother(_, bob).

% в) Кто сестра Сэма?
% sister(X, sam).

% г) Есть ли сестра у Лиз?
% sister(X, liz).

% д) Кто брат Боба?
% brother(X, bob).

% е) Кто внуки Мэри?
% grandson(X, mary).

% ж) Чей внук Паул?
% grandson(paul, X).

% з) Кто тетя Сэма?
% aunt(X, sam).

% и) Есть ли племянники у Энн?
% nephew(ann, X).

% к) У кого ровно двое детей?
% parrent_of_two(X).

% л) Боб - продолжатель рода?
% man_father(bob).

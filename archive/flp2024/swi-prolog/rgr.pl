shift_left(List, N, Result) :-
    length(List, Len),
    Shift is N mod Len,
    append(Left, Right, List),
    length(Left, Shift),
    append(Right, Left, Result).

reverse_words(Line, ReversedLine) :-
    split_string(Line, " ", "", Words),
    reverse(Words, ReversedWords),
    atomic_list_concat(ReversedWords, " ", ReversedLine).

reverse_file(InputFile, OutputFile) :-
    open(InputFile, read, InStream),
    open(OutputFile, write, OutStream),
    process_lines(InStream, OutStream),
    close(InStream),
    close(OutStream).

process_lines(InStream, OutStream) :-
    read_line_to_string(InStream, Line),
    (   Line = end_of_file ->  true;
        reverse_words(Line, ReversedLine),
        writeln(OutStream, ReversedLine),
        process_lines(InStream, OutStream)
    ).


:- dynamic product/2.

load_db(FileName) :-
    (exists_file(FileName) ->
        consult(FileName),
        write('Database successfully loaded.'), nl
    ;
        write('Database file does not exist, new file created.'), nl
    ).

save_db(FileName) :-
    atom_string(FileAtom, FileName),
    tell(FileAtom),
    listing(product/2),
    told.

view_db :-
    (product(_, _) ->
        listing(product/2)
    ;
        write('Database is empty.'), nl
    ).

add_products :-
    write('Enter product to add ( product(Name, Price) ). Enter "stop." to break: '),
    nl,

    read(Product),
    (Product \= stop ->
        assertz(Product),
        add_products
    ).

delete_products :-
    write('Enter product to delete ( product(Name, Price) ). Enter "stop." to break: '),
    nl,

    read(Product),
    (Product \= stop ->
        (retract(Product) ->
            write('Product deleted'),
            nl
        ;
            write('Product not found: '),
            nl
        ),
        delete_products
    ).

min_price :-
    findall(Price, product(_, Price), Prices),
    (Prices \= [] ->
        min_list(Prices, MinPrice),
        findall(Name, (product(Name, Price), Price == MinPrice), Result),
        write(Result), nl
    ;
        write('Database is empty.'), nl
    ).

main_menu(FileName) :-
    repeat,

    write('Commands:'), nl,
    write('1. Print data'), nl,
    write('2. Add item'), nl,
    write('3. Delete item'), nl,
    write('4. Find min price'), nl,
    write('5. Exit'), nl,

    read(Choice),
    nl,

    handle_choice(Choice, FileName),
    Choice = 5.

handle_choice(1, _) :- view_db, !.
handle_choice(2, _) :- add_products, !.
handle_choice(3, _) :- delete_products, !.
handle_choice(4, _) :- min_price, !.
handle_choice(5, FileName) :- save_db(FileName), !.
handle_choice(_, _) :- write('Invalid command.'), nl, fail.

main :-
    write('Enter Database filename: '),
    read(FileName),
    load_db(FileName),
    main_menu(FileName).

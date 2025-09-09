set terminal pngcairo enhanced font 'Verdana,12'
set output 'binary_search_plot.png'
set title "Зависимость времени выполнения бинарного поиска от размера массива"
set xlabel "Размер массива"
set ylabel "Время выполнения, (c)"
set grid
plot "data.dat" using 1:2 with linespoints title "Бинарный поиск"

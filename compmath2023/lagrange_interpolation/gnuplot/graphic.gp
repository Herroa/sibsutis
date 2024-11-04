set terminal pdf color enhanced font 'Calibri,16' size 14cm,10cm
set output 'graph.pdf'
set key inside left top font 'Calibri,16'
set colorsequence podo

set xlabel "x" font 'Calibri,16'
set ylabel "y" font 'Calibri,16'

plot "interp.dat" with lines linetype 1 lw 1

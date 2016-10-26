set logscale x 2
unset label
set xtic auto
set ytic auto
set title "score vs permute performance"
set xlabel "permutations(n)"
set ylabel "score "
plot 'plot.dat' u 1:2 w lines title "Perm"

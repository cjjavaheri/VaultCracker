
unset label
set xtic auto
set ytic auto
set title "score vs permute performance"
set xlabel "permutations(n)"
set ylabel "score "
plot 'plot.dat' u 2:3 w lines title "Perm"

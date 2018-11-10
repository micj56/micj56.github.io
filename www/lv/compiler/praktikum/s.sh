for A in 1 2 3 4 5 6; do
    for L in Java C; do
	let N=A+1
	cp /home/jaeger/docs/fh/lehre/compiler/spl-1.2/Aufgaben/Aufgabe$N-$L.tar.gz Aufgabe$A-$L.tar.gz
	tar xvfz Aufgabe$A-$L.tar.gz
	mv Aufgabe?-$L Aufgabe$A-$L
	tar cvfz Aufgabe$A-$L.tar.gz Aufgabe$A-$L
	rm -r Aufgabe$A-$L
    done 
done

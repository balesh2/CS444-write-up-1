main: CS444-assignment1.tex changelog.tex concurrency.c mt.h
	pdflatex ./CS444-assignment1.tex
	gcc -std=99 -lrt concurrency.c
clean:
	rm -f *.pdf *.ps *.toc *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

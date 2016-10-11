code: concurrency.c mt.h
	gcc -lrt concurrency.c
pdf: CS444-assignment1.tex changelog.tex
	pdflatex ./CS444-assignment1.tex
all: code pdf
clean:
	rm -f *.pdf *.ps *.toc *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

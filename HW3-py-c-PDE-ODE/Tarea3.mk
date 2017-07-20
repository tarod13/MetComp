# Tarea3.mk

Resultados_hw3.pdf : Onda30.jpeg Onda60.jpeg Planetas.jpeg Resultados_hw3.tex
	pdflatex Resultados_hw3.tex

Onda30.jpeg : Onda.py
	python3 Onda.py

Onda60.jpeg : Onda.py
	python3 Onda.py

Planetas.jpeg : Plots_Planetas.py trayectorias.csv
	python3 Plots_Planetas.py
	
trayectorias.csv : Planetas.x
	./Planetas.x

Planetas.x : Planetas.c
	cc Planetas.c -lm -o Planetas.x

# cargas.mk

tarea1.pdf : cargas.pdf num_integral.pdf err_integral.pdf
	pdflatex tarea1.tex

cargas.pdf : cargas.py
	python3 cargas.py

num_integral.pdf : integral.py
	python3 integral.py

err_integral.pdf : integral.py
	python3 integral.py

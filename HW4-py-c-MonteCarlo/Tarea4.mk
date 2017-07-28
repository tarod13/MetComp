# Tarea4.mk

PuntoNemo.pdf : Plots.py pmax.txt
	python3 Plots.py

pmax.txt : GeographicPoint.c
	cc GeographicPoint.c -lm -o Run.x
	./Run.x

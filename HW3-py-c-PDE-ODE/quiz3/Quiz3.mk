# Quiz3.mk

all: posBalon.pdf velBalon.pdf

posBalon.pdf : Balon.py
	python3 Balon.py

velBalon.pdf : Balon.py
	python3 Balon.py

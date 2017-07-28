#QuizT4.mk

all: Param_m.pdf Param_b.pdf Modelo.pdf

Param_m.pdf Param_b.pdf Modelo.pdf: Param.py obs_data.dat
	python3 Param.py

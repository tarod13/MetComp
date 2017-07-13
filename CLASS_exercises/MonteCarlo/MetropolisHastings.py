import numpy as np

def probability_distribution(x):
	return np.exp(-x/2)

n_points = 10000
x_min = 
x_max = 
x0 = x_min+np.random.rand()*(x_max-x_min)
random_walk = [x0]
normal_std = 0.05

for point in range(0,n_points):
	xi = np.random.normal(random_walk[i], normal_std)
	alpha = probability_distribution(random_walk(point))probability_distribution(xi)
	if alpha > 1:
		random_walk.append(xi)
	elif: alpha > np.random.rand()
		random_walk.append(xi)

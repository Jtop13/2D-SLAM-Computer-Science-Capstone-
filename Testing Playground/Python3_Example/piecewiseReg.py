import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk
from scipy import optimize
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import spectral_clustering

def piecewise_linear(x, *params):
	n_segments = len(params) // 3
	segments = [params[i * 3:(i + 1) * 3] for i in range(n_segments)]

	result = np.zeros_like(x)
	for x0, y0, k in segments:
		result += np.piecewise(x, [x < x0], [lambda x: k * x + y0 - k * x0, 0])
	return result

def mypiecewise(data_file, max_segments=5, alpha=10):
	data = np.loadtxt(data_file, delimiter=' ')
	X = StandardScaler().fit_transform(data)
	x, y = X[:, 0], X[:, 1]

	best_bic = np.inf
	best_params = None

	for n_segments in range(1, max_segments + 1):
		p0 = []
		for i in range(n_segments):
			x0 = np.percentile(x, 100 * i / n_segments)
			y0 = np.percentile(y, 100 * i / n_segments)
			k = (y.max() - y.min()) / (x.max() - x.min())
			p0.extend([x0, y0, k])

		popt, pcov = optimize.curve_fit(piecewise_linear, x, y, p0=p0)
		residuals = y - piecewise_linear(x, *popt)
		mse = np.mean(residuals**2)
		bic = len(x) * np.log(mse) + alpha * n_segments * 3 * np.log(len(x))
		if bic < best_bic:
			best_bic = bic
			best_params = popt

	# ... (rest of the code remains the same)



	fig, ax = plt.subplots()
	ax.scatter(x, y, c='gray', alpha=0.5)

	n_best_segments = len(best_params) // 3
	segments = [best_params[i * 3:(i + 1) * 3] for i in range(n_best_segments)]
	colors = [plt.cm.Spectral(each) for each in np.linspace(0, 1, n_best_segments)]

	for (x0, y0, k), col in zip(segments, colors):
		x_values = np.linspace(min(x), max(x), num=1000)
		y_values = k * x_values + y0 - k * x0
		ax.plot(x_values, y_values, c=tuple(col))

	ax.set_title("Best number of segments: {}".format(n_best_segments))

	root = tk.Tk()
	canvas = FigureCanvasTkAgg(fig, master=root)
	canvas.draw()
	canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

	# Run the Tkinter event loop in a non-blocking
	root.update_idletasks()
	root.update()

	return 0

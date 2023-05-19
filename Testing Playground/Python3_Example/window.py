import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import StandardScaler

def find_line_segments(data, window_size, mse_threshold, merge_threshold):
	n = len(data)
	segments = []
	i = 0

	while i < n - window_size:
		x = data[i:i+window_size, 0].reshape(-1, 1)
		y = data[i:i+window_size, 1]

		model = LinearRegression()
		model.fit(x, y)
		y_pred = model.predict(x)
		mse = mean_squared_error(y, y_pred)

		if mse < mse_threshold:
		    segments.append((x[0][0], y[0], x[-1][0], y[-1]))
		    i += window_size
		else:
		    i += 1

	merged_segments = []
	for segment in segments:
		if not merged_segments or merged_segments[-1][2] - segment[0] > merge_threshold:
		    merged_segments.append(segment)
		else:
		    merged_segments[-1] = (merged_segments[-1][0], merged_segments[-1][1], segment[2], segment[3])

	return merged_segments

def slidingWindow(data_file):

	data = np.loadtxt(data_file, delimiter=' ')
	X = StandardScaler().fit_transform(data)

	# Define parameters
	window_size = 5
	mse_threshold = 1
	merge_threshold = 2

	# Find line segments
	segments = find_line_segments(data, window_size, mse_threshold, merge_threshold)

	# Plot data points and line segments
	plt.scatter(data[:, 0], data[:, 1], label='Data points')
	for segment in segments:
		plt.plot([segment[0], segment[2]], [segment[1], segment[3]], label='Line segment')
	plt.legend()
	plt.show()

	return 0

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk
from sklearn.cluster import DBSCAN
from sklearn.preprocessing import StandardScaler
from sklearn import metrics
from collections import Counter
import scipy.ndimage as ndimage
from scipy import optimize

											#adjusting this threshold allows for more sperate clustering
def hough_line_numpy(data, angles_count=180, threshold=5):
    max_distance = int(np.ceil(np.sqrt(data.shape[0]**2 + data.shape[1]**2)))
    accumulator = np.zeros((2 * max_distance, angles_count), dtype=np.uint64)

    angles = np.linspace(-np.pi / 2, np.pi / 2, angles_count)
    cos_angles = np.cos(angles)
    sin_angles = np.sin(angles)

    for x, y in data:
        for angle_idx, (cos_angle, sin_angle) in enumerate(zip(cos_angles, sin_angles)):
            distance = int(x * cos_angle + y * sin_angle + max_distance)
            accumulator[distance, angle_idx] += 1

    peaks = np.argwhere(accumulator > threshold)
    return accumulator, angles[peaks[:, 1]], peaks[:, 0] - max_distance

def mydbscan(data_file):
	# Load and preprocess the data
	data = np.loadtxt(data_file, delimiter=' ')
	print(data)
	X = StandardScaler().fit_transform(data)
	
	# Apply Gaussian smoothing to reduce noise
	smoothed_data = ndimage.gaussian_filter(X, sigma=1)

	# Hough Transform to find lines
	hspace, hough_angles, hough_dists = hough_line_numpy(smoothed_data)

	# Cluster detected lines using DBSCAN
	lines_data = np.column_stack((hough_angles, hough_dists))
	
	#if no lines found return
	if len(lines_data) > 0:
		print("lines detected: ", len(lines_data))		
		db = DBSCAN(eps=0.3, min_samples=2).fit(lines_data)
		labels = db.labels_
		
		# Number of clusters in labels, ignoring noise if present
		n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0)
		n_noise_ = list(labels).count(-1)
	else:
		print("No lines detected")
		return -2
		

	print("Estimated number of clusters: %d" % n_clusters_)
	print("Estimated number of noise points: %d" % n_noise_)

	# Plot results
	fig, ax = plt.subplots()
	ax.scatter(X[:, 0], X[:, 1], c='gray', alpha=0.5)

	unique_labels = set(labels)
	colors = [plt.cm.Spectral(each) for each in np.linspace(0, 1, len(unique_labels))]

	for k, col in zip(unique_labels, colors):
		if k == -1:
		    col = [0, 0, 0, 1]
		    continue

		class_member_mask = labels == k
		xy = lines_data[class_member_mask]

		for angle, dist in xy:
		    y0 = (dist - 0 * np.cos(angle)) / np.sin(angle)
		    y1 = (dist - X[-1, 0] * np.cos(angle)) / np.sin(angle)
		    ax.plot((0, X[-1, 0]), (y0, y1), c=tuple(col))

	ax.set_title("Estimated number of clusters: {}".format(n_clusters_))

	# Create a Tkinter window and canvas to display the figure
	root = tk.Tk()
	canvas = FigureCanvasTkAgg(fig, master=root)
	canvas.draw()
	canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

	# Run the Tkinter event loop in a non-blocking
	root.update_idletasks()
	root.update()

	return 0


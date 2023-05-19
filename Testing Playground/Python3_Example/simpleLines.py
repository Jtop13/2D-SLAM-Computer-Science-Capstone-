import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk
from scipy import optimize
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import spectral_clustering
from sklearn.cluster import DBSCAN
from sklearn import metrics
from scipy.spatial import KDTree

#BigO (n log n)
def order_dataset_by_proximity_new(dataset):
    kd_tree = KDTree(dataset)
    visited = set()
    ordered_dataset = []

    # Start with an arbitrary point (e.g., the first point in the dataset)
    current_index = 0

    while len(visited) < len(dataset) - 1:
        current_point = dataset[current_index]
        ordered_dataset.append(current_point)
        visited.add(current_index)

        # Find the nearest unvisited point
        _, nearest_indices = kd_tree.query(current_point, k=None, distance_upper_bound=np.inf)

        # Find the first unvisited index in the nearest_indices
        nearest_index = -1 
        for idx in nearest_indices:
            if idx not in visited:
                nearest_index = idx
                break

        if nearest_index != -1:
            current_index = nearest_index
        else:
            break

    # Append the last unvisited point
    last_unvisited_index = list(set(range(len(dataset))) - visited)[0]
    ordered_dataset.append(dataset[last_unvisited_index])

    return np.array(ordered_dataset)


def find_nearest_point_index(point, dataset, visited):
    min_distance = float('inf')
    nearest_index = -1

    for i, other_point in enumerate(dataset):
        if i in visited:
            continue

        distance = np.linalg.norm(point - other_point)

        if distance < min_distance:
            min_distance = distance
            nearest_index = i

    return nearest_index

#BigO(n^2)
def order_dataset_by_proximity(dataset):
    visited = set()
    ordered_dataset = []

    # Start with an arbitrary point (e.g., the first point in the dataset)
    current_index = 0

    while len(visited) < len(dataset) - 1:
        current_point = dataset[current_index]
        ordered_dataset.append(current_point)
        visited.add(current_index)

        # Find the nearest unvisited point
        nearest_index = find_nearest_point_index(current_point, dataset, visited)

        if nearest_index != -1:
            current_index = nearest_index
        else:
            break

    # Append the last unvisited point
    last_unvisited_index = list(set(range(len(dataset))) - visited)[0]
    ordered_dataset.append(dataset[last_unvisited_index])

    return np.array(ordered_dataset)


def simpleLines(data_file):
	data = np.loadtxt(data_file, delimiter=' ')
	#X = StandardScaler().fit_transform(data)
	x_coords, y_coords = data[:, 0], data[:, 1]

	# Convert the x and y coordinates into an array of points
	points = np.column_stack((x_coords, y_coords))

	# Set a distance threshold (epsilon) for the DBSCAN algorithm
	epsilon = 3  # Adjust this value based on your requirements

	# Perform DBSCAN clustering
	dbscan = DBSCAN(eps=epsilon, min_samples=20)
	clusters = dbscan.fit_predict(points)

	# Set up the plot
	fig, ax = plt.subplots()

	# Draw the lines within each cluster
	unique_clusters = np.unique(clusters)
	for cluster in unique_clusters:
		if cluster == -1:
			continue

		cluster_points = points[clusters == cluster]
		#an attempt at ordering
		#Big O (n^2)
		cluster_points = order_dataset_by_proximity(cluster_points)
		
		#an attempt at smoothing (moving average)
		"""window_size = 150
		weights = np.ones(window_size) / window_size
		smoothed_y = np.convolve(cluster_points[:,1], weights, mode='valid')
		smoothed_x = cluster_points[:,0][:-window_size+1]"""

		if len(cluster_points) > 1:
		    for i in range(len(cluster_points) - 1):
		        x1, y1 = cluster_points[i]
		        x2, y2 = cluster_points[i + 1]
		        #x1 = smoothed_x[i]
		        #y1 = smoothed_y[i]
		        #x2 = smoothed_x[i+1]
		        #y2 = smoothed_y[i+1]
				
				#make sure not plotting far away points
		        if (np.linalg.norm(np.array(x1, y1) - np.array(x2, y2)) < 0.2):								
		            ax.plot([x1, x2], [y1, y2], 'k-', linewidth=0.5)  # 'k-' means black line segments
	# Configure the plot
	ax.set_title('2D LIDAR SLAM Visualization')
	ax.set_xlabel('X-axis (Decimeter)')
	ax.set_ylabel('Y-axis (Decimeter)')
	ax.axis('equal')  # Set equal aspect ratio
	ax.grid(True)	

	root = tk.Tk()
	canvas = FigureCanvasTkAgg(fig, master=root)
	canvas.draw()
	canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

	# Run the Tkinter event loop in a non-blocking
	root.update_idletasks()
	root.update()

	return 0

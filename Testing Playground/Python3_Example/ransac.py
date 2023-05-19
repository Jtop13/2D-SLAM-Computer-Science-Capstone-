import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import StandardScaler
from sklearn.cluster import DBSCAN

def distance_point_to_line(p, line_params):
    a, b, c = line_params
    x, y = p
    return abs(a * x + b * y + c) / np.sqrt(a**2 + b**2)

def ransac_line(data, threshold, max_iterations):
    best_line = None
    best_inliers = []
    best_inliers_indices = []

    for _ in range(max_iterations):
        p1, p2 = data[np.random.choice(len(data), 2, replace=False)]

        a = p2[1] - p1[1]
        b = p1[0] - p2[0]
        c = p2[0] * p1[1] - p1[0] * p2[1]
        line_params = (a, b, c)

        inliers_indices = [i for i, p in enumerate(data) if distance_point_to_line(p, line_params) <= threshold]
        inliers = data[inliers_indices]

        if len(inliers) > len(best_inliers):
            best_line = line_params
            best_inliers = inliers
            best_inliers_indices = inliers_indices

    return best_line, best_inliers, best_inliers_indices

def segment_data(data, threshold, max_iterations, min_inliers_ratio):
    lines = []
    remaining_data = data.copy()

    while len(remaining_data) >= 2:
        line, inliers, inliers_indices = ransac_line(remaining_data, threshold, max_iterations)
        if len(inliers) == 0 or len(inliers) / len(data) < min_inliers_ratio:
            break
        lines.append((line, inliers))
        inliers_set = set(tuple(p) for p in inliers)
        remaining_data = np.array([p for p in remaining_data if tuple(p) not in inliers_set])

    return lines


def ransac(data_file):
	# Sample data points (x, y)
	data = np.loadtxt(data_file, delimiter=' ')
	X = StandardScaler().fit_transform(data)
	
	# Set a distance threshold (epsilon) for the DBSCAN algorithm
	epsilon = 0.2  # Adjust this value based on your requirements

	# Perform DBSCAN clustering
	dbscan = DBSCAN(eps=epsilon, min_samples=40)
	clusters = dbscan.fit_predict(X)
	
	# Find the unique cluster labels
	unique_clusters = np.unique(clusters)

	# Plot the data points and fitted lines
	fig, ax = plt.subplots()
	ax.scatter(data[:, 0], data[:, 1], label='Data Points', c=clusters, cmap='viridis')

	# Iterate through the clusters and apply RANSAC to each cluster
	for cluster_label in unique_clusters:
		if cluster_label == -1:  # Skip the noise points (label = -1)
		    continue

		# Extract data points belonging to the current cluster
		cluster_data = data[clusters == cluster_label]

		# Apply RANSAC to the cluster's data points
		threshold = 0.2  # Adjust as needed
		max_iterations = 1000  # Adjust as needed
		min_inliers_ratio = 0.2  # Adjust as needed
		lines = segment_data(cluster_data, threshold, max_iterations, min_inliers_ratio)

		# Plot the fitted lines for the current cluster
		for i, (line_params, inliers) in enumerate(lines):
		    a, b, c = line_params
		    x_min, x_max = inliers[:, 0].min(), inliers[:, 0].max()

		    # Calculate the corresponding y values for the x range
		    y_min = (-a * x_min - c) / b
		    y_max = (-a * x_max - c) / b
		    print((-a, c, b))
		    print((x_min,x_max,y_min,y_max))

		    ax.plot([x_min, x_max], [y_min, y_max], label='Cluster {}, Line {}'.format(cluster_label + 1, i + 1))

	ax.legend()
	plt.show()


	


import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import tkinter as tk


def mydbscan(data_file):
	"""
	===================================
	Demo of DBSCAN clustering algorithm
	===================================

	DBSCAN (Density-Based Spatial Clustering of Applications with Noise) finds core
	samples in regions of high density and expands clusters from them. This
	algorithm is good for data which contains clusters of similar density.

	See the :ref:`sphx_glr_auto_examples_cluster_plot_cluster_comparison.py` example
	for a demo of different clustering algorithms on 2D datasets.

	"""
	"""
	# %%
	# Data generation
	# ---------------
	#
	# We use :class:`~sklearn.datasets.make_blobs` to create 3 synthetic clusters.

	from sklearn.datasets import make_blobs
	from sklearn.preprocessing import StandardScaler

	centers = [[1, 1], [-1, -1], [1, -1]]
	X, labels_true = make_blobs(
		n_samples=750, centers=centers, cluster_std=0.4, random_state=0
	)

	X = StandardScaler().fit_transform(X)

	# %%"""

	#importing our own dataset
	import numpy as np
	from sklearn.preprocessing import StandardScaler

	#data_file = "map_data.tmp"
	data = np.loadtxt(data_file, delimiter=' ')
	X = StandardScaler().fit_transform(data)

	# We can visualize the resulting data:

	import matplotlib.pyplot as plt

	"""plt.scatter(X[:, 0], X[:, 1])
	plt.show()"""

	# Create a new figure and axis for the plot
	fig, ax = plt.subplots()

    # Plot the data on the new axis
	ax.scatter(X[:, 0], X[:, 1])

    # Create a Tkinter window and canvas to display the figure
	root = tk.Tk()
	canvas = FigureCanvasTkAgg(fig, master=root)
	canvas.draw()
	canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

	# Run the Tkinter event loop in a non-blocking manner
	root.update_idletasks()
	root.update()

	# %%
	# Compute DBSCAN
	# --------------
	#
	# One can access the labels assigned by :class:`~sklearn.cluster.DBSCAN` using
	# the `labels_` attribute. Noisy samples are given the label math:`-1`.


	from sklearn.cluster import DBSCAN
	from sklearn import metrics

	db = DBSCAN(eps=1, min_samples=2).fit(X)
	labels = db.labels_

	# Number of clusters in labels, ignoring noise if present.
	n_clusters_ = len(set(labels)) - (1 if -1 in labels else 0)
	n_noise_ = list(labels).count(-1)

	print("Estimated number of clusters: %d" % n_clusters_)
	print("Estimated number of noise points: %d" % n_noise_)

	# %%
	# Clustering algorithms are fundamentally unsupervised learning methods.
	# However, since :class:`~sklearn.datasets.make_blobs` gives access to the true
	# labels of the synthetic clusters, it is possible to use evaluation metrics
	# that leverage this "supervised" ground truth information to quantify the
	# quality of the resulting clusters. Examples of such metrics are the
	# homogeneity, completeness, V-measure, Rand-Index, Adjusted Rand-Index and
	# Adjusted Mutual Information (AMI).
	#
	# If the ground truth labels are not known, evaluation can only be performed
	# using the model results itself. In that case, the Silhouette Coefficient comes
	# in handy.
	#
	# For more information, see the
	# :ref:`sphx_glr_auto_examples_cluster_plot_adjusted_for_chance_measures.py`
	# example or the :ref:`clustering_evaluation` module.

	#print("Homogeneity: {:.3f}".format(metrics.homogeneity_score(labels_true, labels)))
	#print("Completeness: {:.3f}".format(metrics.completeness_score(labels_true, labels)))
	#print("V-measure: {:.3f}".format(metrics.v_measure_score(labels_true, labels)))
	#print("Adjusted Rand Index: {:.3f}".format(metrics.adjusted_rand_score(labels_true, labels)))
	#print(
	#    "Adjusted Mutual Information: {:.3f}".format(metrics.adjusted_mutual_info_score(labels_true, labels))
	#)
	#print("Silhouette Coefficient: {:.3f}".format(metrics.silhouette_score(X, labels)))

	# %%
	# Plot results
	# ------------
	#
	# Core samples (large dots) and non-core samples (small dots) are color-coded
	# according to the asigned cluster. Samples tagged as noise are represented in
	# black.

	unique_labels = set(labels)
	core_samples_mask = np.zeros_like(labels, dtype=bool)
	core_samples_mask[db.core_sample_indices_] = True

	colors = [plt.cm.Spectral(each) for each in np.linspace(0, 1, len(unique_labels))]
	# Create a new figure and axis for the plot
	fig, ax = plt.subplots()

	for k, col in zip(unique_labels, colors):
		if k == -1:
			# Black used for noise.
			col = [0, 0, 0, 1]

		class_member_mask = labels == k
		xy = X[class_member_mask & core_samples_mask]
		
		# Replace plt.plot with ax.plot
		ax.plot(xy[:, 0], xy[:, 1], "o", markerfacecolor=tuple(col), markeredgecolor="k", markersize=14)

		xy = X[class_member_mask & ~core_samples_mask]
		ax.plot(xy[:, 0], xy[:, 1], "o", markerfacecolor=tuple(col), markeredgecolor="k", markersize=6)

	# Set the title for the plot
	ax.set_title("Estimated number of clusters: {}".format(n_clusters_))

	# Create a Tkinter window and canvas to display the figure
	root = tk.Tk()
	canvas = FigureCanvasTkAgg(fig, master=root)
	canvas.draw()
	canvas.get_tk_widget().pack(side=tk.TOP, fill=tk.BOTH, expand=1)

	# Run the Tkinter event loop in a non-blocking manner
	root.update_idletasks()
	root.update()
	"""for k, col in zip(unique_labels, colors):
		if k == -1:
			# Black used for noise.
			col = [0, 0, 0, 1]

		class_member_mask = labels == k

		xy = X[class_member_mask & core_samples_mask]
		plt.plot(
			xy[:, 0],
			xy[:, 1],
			"o",
			markerfacecolor=tuple(col),
			markeredgecolor="k",
			markersize=14,
		)

		xy = X[class_member_mask & ~core_samples_mask]
		plt.plot(
			xy[:, 0],
			xy[:, 1],
			"o",
			markerfacecolor=tuple(col),
			markeredgecolor="k",
			markersize=6,
		)

	plt.title("Estimated number of clusters: {}".format(n_clusters_))
	plt.show()"""

	return 0

import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import StandardScaler
#from sklearn_extra.cluster import KMedoids
#from pyclustering.cluster.kmedois import kmedoids

def bic_kmeans(data, max_clusters):
	n = len(data)
	bics = []

	for k in range(1, max_clusters + 1):
		kmeans = KMeans(n_clusters=k, random_state=0).fit(data)
		rss = 0

		for i in range(k):
		    cluster_data = data[kmeans.labels_ == i]
		    x = cluster_data[:, 0].reshape(-1, 1)
		    y = cluster_data[:, 1]

		    model = LinearRegression().fit(x, y)
		    y_pred = model.predict(x)
		    rss += np.sum((y - y_pred)**2)

		bic = n * np.log(rss/n) + k * np.log(n)
		bics.append(bic)

	return np.argmin(bics) + 1

def bayKmean(data_file):
	# Sample data points (x, y)
	data = np.loadtxt(data_file, delimiter=' ')
	X = StandardScaler().fit_transform(data)

	# Define the maximum number of clusters to evaluate
	max_clusters = 4

	# Find the optimal number of line segments using BIC
	optimal_clusters = bic_kmeans(data, max_clusters)

	# Perform K-means clustering with the optimal number of clusters
	kmeans = KMeans(n_clusters=optimal_clusters, random_state=0).fit(data)

	# Plot the data points and fitted lines for each cluster
	fig, ax = plt.subplots()
	for i in range(optimal_clusters):
		cluster_data = data[kmeans.labels_ == i]
		x = cluster_data[:, 0].reshape(-1, 1)
		y = cluster_data[:, 1]

		model = LinearRegression().fit(x, y)
		y_pred = model.predict(x)

		ax.scatter(x, y, label='Cluster {}'.format(i+1))
		ax.plot(x, y_pred, label='Fitted Line {}'.format(i + 1))
	ax.legend()
	plt.show()

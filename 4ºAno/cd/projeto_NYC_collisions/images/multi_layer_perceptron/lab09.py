from sklearn.decomposition import PCA
from numpy.linalg import eig
from matplotlib.pyplot import gca, title, tight_layout, savefig
from pandas import DataFrame, read_csv
from matplotlib.pyplot import figure, xlabel, ylabel, scatter, show, subplots
from ds_charts import choose_grid, plot_clusters, plot_line

from pandas import DataFrame, read_csv
from matplotlib.pyplot import subplots, show
from ds_charts import choose_grid, plot_clusters, bar_chart,plot_line, multiple_bar_chart, compute_mse, compute_centroids, compute_mae
import numpy as np
from scipy.spatial.distance import pdist, squareform
import pandas as pd

#----
def explained_variance(folder):
	# PLOT EXPLAINED VARIANCE RATIO
	fig = figure(figsize=(12, 8))
	title('Explained variance ratio')
	xlabel('PC')
	ylabel('ratio')
	x_values = [str(i) for i in range(1, len(pca.components_) + 1)]
	bwidth = 0.5
	ax = gca()
	ax.set_xticklabels(x_values)
	ax.set_ylim(0.0, 1.0)
	ax.bar(x_values, pca.explained_variance_ratio_, width=bwidth)
	ax.plot(pca.explained_variance_ratio_)
	for i, v in enumerate(pca.explained_variance_ratio_):
		ax.text(i, v+0.05, f'{v*100:.1f}', ha='center', fontweight='bold')
	tight_layout()
	savefig(f"{folder}Explained_variance_ratio.png")

def kne(data,folder,moment='Before'):
	from sklearn.cluster import KMeans
	from sklearn.metrics import silhouette_score, davies_bouldin_score

	mse: list = []
	mae: list = []
	sc: list = []
	dbs: list = []
	fig, axs = subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
	i, j = 0, 0
	for n in range(len(N_CLUSTERS)):
			k = N_CLUSTERS[n]
			estimator = KMeans(n_clusters=k)
			estimator.fit(data)
			labels = estimator.predict(data)
			print(f">> {n} - {k}")

			mse.append(estimator.inertia_)
			sc.append(silhouette_score(data, estimator.labels_))
			mae.append(compute_mae(data.values, labels, compute_centroids(data, labels)))
			dbs.append(davies_bouldin_score(data, estimator.labels_))

			plot_clusters(data, v2, v1, estimator.labels_.astype(float), estimator.cluster_centers_, k, f'KMeans k={k}', ax=axs[i,j])
			i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
	tight_layout()
	savefig(f"{folder}KMeans_{moment}.png")

	fig, ax = subplots(2, 2, figsize=(6, 6), squeeze=False)
	plot_line(N_CLUSTERS, mse, title='KMeans MSE', xlabel='k', ylabel='MSE', ax=ax[0, 0])
	plot_line(N_CLUSTERS, sc, title='KMeans SC', xlabel='k', ylabel='SC', ax=ax[0, 1], percentage=True)
	plot_line(N_CLUSTERS, mae, title='KMeans MAE', xlabel='k', ylabel='MAE', ax=ax[1, 0])
	plot_line(N_CLUSTERS, dbs, title='KMeans DBS', xlabel='k', ylabel='DBS', ax=ax[1, 1])
	tight_layout()
	savefig(f"{folder}KMeans_MSE_SC_{moment}.png")

def em(data,folder,moment='Before'):
	from sklearn.mixture import GaussianMixture
	from sklearn.metrics import silhouette_score, davies_bouldin_score

	mse: list = []
	mae: list = []
	sc: list = []
	dbs: list = []
	_, axs = subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
	i, j = 0, 0
	for n in range(len(N_CLUSTERS)):
			k = N_CLUSTERS[n]
			estimator = GaussianMixture(n_components=k)
			estimator.fit(data)
			labels = estimator.predict(data)
			print(f"{n} - {k}")
			mse.append(compute_mse(data.values, labels, estimator.means_))
			sc.append(silhouette_score(data, labels))
			mae.append(compute_mae(data.values, labels, estimator.means_))
			dbs.append(davies_bouldin_score(data, labels))
			plot_clusters(data, v2, v1, labels.astype(float), estimator.means_, k,
											 f'EM k={k}', ax=axs[i,j])
			i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
	tight_layout()
	savefig(f"{folder}EM_{moment}.png")

	fig, ax = subplots(2, 2, figsize=(6, 6), squeeze=False)
	plot_line(N_CLUSTERS, mse, title='EM MSE', xlabel='k', ylabel='MSE', ax=ax[0, 0])
	plot_line(N_CLUSTERS, sc, title='EM SC', xlabel='k', ylabel='SC', ax=ax[0, 1], percentage=True)
	plot_line(N_CLUSTERS, mae, title='EM MAE', xlabel='k', ylabel='MAE', ax=ax[1, 0])
	plot_line(N_CLUSTERS, dbs, title='EM DBS', xlabel='k', ylabel='DBS', ax=ax[1, 1])
	tight_layout()
	savefig(f"{folder}EM_MSE_SC_{moment}.png")

def hierarqui(data,folder,moment='Before'):
	from sklearn.cluster import AgglomerativeClustering
	from sklearn.metrics import silhouette_score, davies_bouldin_score

	mse: list = []
	mae: list = []
	sc: list = []
	dbs: list = []
	rows, cols = choose_grid(len(N_CLUSTERS))
	_, axs = subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
	i, j = 0, 0
	for n in range(len(N_CLUSTERS)):
			k = N_CLUSTERS[n]
			estimator = AgglomerativeClustering(n_clusters=k)
			estimator.fit(data)
			labels = estimator.labels_
			centers = compute_centroids(data, labels)
			mse.append(compute_mse(data.values, labels, centers))
			mae.append(compute_mse(data.values, labels, centers))
			sc.append(silhouette_score(data, labels))
			dbs.append(davies_bouldin_score(data, labels))
			plot_clusters(data, v2, v1, labels, centers, k, f'Hierarchical k={k}', ax=axs[i,j])
			i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
	tight_layout()
	savefig(f"{folder}HIE_{moment}.png")

	fig, ax = subplots(2, 2, figsize=(6, 6), squeeze=False)
	plot_line(N_CLUSTERS, mse, title='Hierarchical MSE', xlabel='k', ylabel='MSE', ax=ax[0, 0])
	plot_line(N_CLUSTERS, sc, title='Hierarchical SC', xlabel='k', ylabel='SC', ax=ax[0, 1], percentage=True)
	plot_line(N_CLUSTERS, mae, title='Hierarchical MAE', xlabel='k', ylabel='MAE', ax=ax[1, 0])
	plot_line(N_CLUSTERS, dbs, title='Hierarchical DBS', xlabel='k', ylabel='DBS', ax=ax[1, 1])
	tight_layout()
	savefig(f"{folder}HIE_MSE_SC_{moment}.png")


	METRICS = ['euclidean', 'cityblock', 'chebyshev', 'cosine', 'jaccard']
	LINKS = ['complete', 'average']
	k = 3
	values_mse = {}
	values_mae = {}
	values_sc = {}
	values_dbs = {}
	rows = len(METRICS)
	cols = len(LINKS)
	_, axs = subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
	for i in range(len(METRICS)):
			mse: list = []
			mae: list = []
			sc: list = []
			dbs: list = []
			m = METRICS[i]
			for j in range(len(LINKS)):
					link = LINKS[j]
					estimator = AgglomerativeClustering(n_clusters=k, linkage=link, affinity=m )
					estimator.fit(data)
					labels = estimator.labels_
					centers = compute_centroids(data, labels)
					mse.append(compute_mse(data.values, labels, centers))
					mae.append(compute_mse(data.values, labels, centers))
					sc.append(silhouette_score(data, labels))
					dbs.append(davies_bouldin_score(data, labels))
					plot_clusters(data, v2, v1, labels, centers, k, f'Hierarchical k={k} metric={m} link={link}', ax=axs[i,j])
			values_mse[m] = mse
			values_sc[m] = sc
			values_mae[m] = mae
			values_dbs[m] = dbs
	tight_layout()
	savefig(f"{folder}HIE_Metrics_{moment}.png")

	_, ax = subplots(2, 2, figsize=(6, 6), squeeze=False)
	multiple_bar_chart(LINKS, values_mse, title=f'Hierarchical MSE', xlabel='metric', ylabel='MSE', ax=ax[0, 0])
	multiple_bar_chart(LINKS, values_sc, title=f'Hierarchical SC', xlabel='metric', ylabel='SC', ax=ax[0, 1], percentage=True)
	multiple_bar_chart(LINKS, values_mae, title=f'Hierarchical MAE', xlabel='metric', ylabel='MAE', ax=ax[1, 0])
	multiple_bar_chart(LINKS, values_dbs, title=f'Hierarchical DBS', xlabel='metric', ylabel='DBS', ax=ax[1, 1])
	tight_layout()
	savefig(f"{folder}HIE_Metrics_MSE_SC_{moment}.png")

def dbscan(data,folder,moment='Before'):
	from sklearn.cluster import DBSCAN
	from sklearn.metrics import silhouette_score, davies_bouldin_score
	import numpy as np
	from scipy.spatial.distance import pdist, squareform


	EPS = [2.5, 5, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100]
	#mse: list = []
	#mae: list = []
	#sc: list = []
	#dbs: list = []
	#rows, cols = choose_grid(len(EPS))
	#_, axs = subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
	#i, j = 0, 0
	#for n in range(len(EPS)):
	#		estimator = DBSCAN(eps=EPS[n], min_samples=2)
	#		estimator.fit(data)
	#		labels = estimator.labels_
	#		k = len(set(labels)) - (1 if -1 in labels else 0)
	#		if k > 1:
	#				centers = compute_centroids(data, labels)
	#				mse.append(compute_mse(data.values, labels, centers))
	#				mae.append(compute_mse(data.values, labels, centers))
	#				sc.append(silhouette_score(data, labels))
	#				dbs.append(davies_bouldin_score(data, labels))
	#				plot_clusters(data, v2, v1, labels.astype(float), estimator.components_, k, f'DBSCAN eps={EPS[n]} k={k}', ax=axs[i,j])
	#				i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
	#		else:
	#				mse.append(0)
	#				sc.append(0)
	#				mae.append(0)
	#				dbs.append(0)
	#tight_layout()
	#savefig(f"{folder}DBSCAN_{moment}.png")

	#fig, ax = subplots(2, 2, figsize=(6, 6), squeeze=False)
	#plot_line(EPS, mse, title='DBSCAN MSE', xlabel='eps', ylabel='MSE', ax=ax[0, 0])
	#plot_line(EPS, sc, title='DBSCAN SC', xlabel='eps', ylabel='SC', ax=ax[0, 1], percentage=True)
	#plot_line(EPS, mae, title='DBSCAN MAE', xlabel='eps', ylabel='MAE', ax=ax[1, 0])
	#plot_line(EPS, dbs, title='DBSCAN DBS', xlabel='eps', ylabel='DBS', ax=ax[1, 1])
	#tight_layout()
	#savefig(f"{folder}DBSCAN_MSE_SC_{moment}.png")

	if moment == 'Before':
		data = data.astype(int)
	print(data.values)

	METRICS = ['euclidean', 'cityblock', 'chebyshev', 'cosine', 'jaccard']
	distances = []
	for m in METRICS:
			print(data.values)
			dist = np.mean(np.mean(squareform(pdist(data.values, metric=m))))
			distances.append(dist)

	print('AVG distances among records', distances)
	distances[0] *= 0.6
	distances[1] = 80
	distances[2] *= 0.6
	distances[3] *= 0.1
	distances[4] *= 0.15
	print('CHOSEN EPS', distances)

	mse: list = []
	mae: list = []
	sc: list = []
	dbs: list = []
	rows, cols = choose_grid(len(METRICS))
	_, axs = subplots(rows, cols, figsize=(cols*5, rows*5), squeeze=False)
	i, j = 0, 0
	for n in range(len(METRICS)):
			estimator = DBSCAN(eps=distances[n], min_samples=2, metric=METRICS[n])
			estimator.fit(data)
			labels = estimator.labels_
			k = len(set(labels)) - (1 if -1 in labels else 0)
			if k > 1:
					centers = compute_centroids(data, labels)
					mse.append(compute_mse(data.values, labels, centers))
					mae.append(compute_mse(data.values, labels, centers))
					sc.append(silhouette_score(data, labels))
					dbs.append(davies_bouldin_score(data, labels))
					plot_clusters(data, v2, v1, labels.astype(float), estimator.components_, k, f'DBSCAN metric={METRICS[n]} eps={distances[n]:.2f} k={k}', ax=axs[i,j])
			else:
					mse.append(0)
					sc.append(0)
					mae.append(0)
					dbs.append(0)
			i, j = (i + 1, 0) if (n+1) % cols == 0 else (i, j + 1)
	tight_layout()
	savefig(f"{folder}DBSCAN_Metrics_{moment}.png")


	fig, ax = subplots(2, 2, figsize=(6, 6), squeeze=False)
	bar_chart(METRICS, mse, title='DBSCAN MSE', xlabel='metric', ylabel='MSE', ax=ax[0, 0])
	bar_chart(METRICS, sc, title='DBSCAN SC', xlabel='metric', ylabel='SC', ax=ax[0, 1], percentage=True)
	bar_chart(METRICS, mae, title='DBSCAN MAE', xlabel='metric', ylabel='MAE', ax=ax[1, 0])
	bar_chart(METRICS, dbs, title='DBSCAN DBS', xlabel='metric', ylabel='DBS', ax=ax[1, 1])
	tight_layout()
	savefig(f"{folder}DBSCAN_Metrics_MSE_SC_{moment}.png")

#----

def set1():
	file_tag = 'NYC_collisions_tabular__FS'
	filename = f'data/{file_tag}.csv'
	target = 'PERSON_INJURY'

	data: DataFrame = read_csv(filename)
	data.pop(target)
	return data, file_tag, filename, target, 'images/lab09/set1/'

def set2():
	file_tag = 'air_quality_tabularNum_FS_under'
	filename = f'data/{file_tag}.csv'
	target = 'ALARM'

	data: DataFrame = read_csv(filename)
	data.pop('FID')
	data.pop(target)
	return data, file_tag, filename, target, 'images/lab09/set2/'


### PCA
data, file_tag, filename, target, FOLDER = set1()


variables = data.columns.values
eixo_x = 0
eixo_y = 4
eixo_z = 7

v1 = 0
v2 = 4

N_CLUSTERS = [2, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29]
rows, cols = choose_grid(len(N_CLUSTERS))

mean = (data.mean(axis=0)).tolist()
centered_data = data - mean
cov_mtx = centered_data.cov()
eigvals, eigvecs = eig(cov_mtx)

pca = PCA()
pca.fit(centered_data)
PC = pca.components_
var = pca.explained_variance_

explained_variance(FOLDER)

transf = pca.transform(data)

_, axs = subplots(1, 2, figsize=(2*5, 1*5), squeeze=False)
axs[0,0].set_xlabel(variables[eixo_y])
axs[0,0].set_ylabel(variables[eixo_z])
axs[0,0].scatter(data.iloc[:, eixo_y], data.iloc[:, eixo_z])

axs[0,1].set_xlabel('PC1')
axs[0,1].set_ylabel('PC2')
axs[0,1].scatter(transf[:, 0], transf[:, 1])
tight_layout()
savefig(f"{FOLDER}disparcity.png")

repl = {}

for x in range(len(data.columns)):
	repl[x] = data.columns[x]

xi = pd.DataFrame(transf)
xi.rename(columns=repl, inplace=True)


### MAIN

kne(data, folder=FOLDER)
kne(xi, folder=FOLDER, moment='After')

em(data, folder=FOLDER)
em(xi, folder=FOLDER, moment='After')

hierarqui(data, folder=FOLDER)
hierarqui(xi, folder=FOLDER, moment='After')

dbscan(data, folder=FOLDER)
dbscan(xi, folder=FOLDER, moment='After')

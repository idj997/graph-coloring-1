import pandas as pd
import networkx as nx
import matplotlib.pyplot as plt

# Load edges
edges = pd.read_csv('graph_edges.csv', header=None, names=['source', 'target'])
G = nx.Graph()
G.add_edges_from(edges.values)

# Load colors
colors_df = pd.read_csv('dsatur_colors.csv')
color_map = dict(zip(colors_df['Vertex'], colors_df['Color']))

# Assign node colors
node_colors = [color_map.get(node, 0) for node in G.nodes()]

# Plot
plt.figure(figsize=(8, 6))
nx.draw(G, with_labels=True, node_color=node_colors, cmap='tab20', node_size=500)
plt.title('DSATUR Graph Coloring Visualization')
plt.show()

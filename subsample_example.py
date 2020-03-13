import numpy as np 

new_nodes = np.loadtxt('data/nodes.txt')

new_nodes = new_nodes[500000:]
indices = np.arange(0,len(new_nodes),100)
new_nodes = np.take(new_nodes, indices,axis=0)


np.savetxt('nodes.txt', new_nodes)

indices_for_edges = 500000 + indices

new_edges = np.loadtxt('data/edges.txt')

indices_to_remove = []
for i in range(len(new_edges)):
    if not new_edges[i,0] in indices_for_edges or not new_edges[i,1] in indices_for_edges:
        indices_to_remove.append(i)

new_edges = np.delete(new_edges, np.array(indices_to_remove), axis=0)
np.savetxt('edges.txt', new_edges.astype(int), fmt='%i')

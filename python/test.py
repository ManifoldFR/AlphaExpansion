import pyalphaexpansion

# Assemble the node and edge data

with open("../data/medium/nodes.txt") as f:
    node_data = f.readlines()
    node_data = [[-int(i) for i in line.split()] for line in node_data]


with open("../data/medium/edges.txt") as f:
    edge_data = f.readlines()
    edge_data = [list(map(int, line.split())) for line in edge_data]

result = pyalphaexpansion.apply_alpha_expansion(node_data, edge_data)
print(result)

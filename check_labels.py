import os
import numpy as np

with open("labels.txt") as f:
    labels_predicted = np.array([int(i) for i in f.readlines()])


with open("label_ref.txt") as f:
    labels_true = np.array([int(i) for i in f.readlines()])

print("No. of differences:", np.sum(np.abs(labels_predicted - labels_true)))

#!/usr/bin/env python3

import matplotlib.pyplot as plt

average_count = 5

# Read the data from the file
unknown_count = [i for i in range(5, 81, 5)]
nodes = []
times = []

with open('sudoku_benchmark.txt', 'r') as file:
    temp_time = []
    temp_nodes = []
    for i, line in enumerate(file):
        time, node = map(float, line.split())
        temp_time.append(time)
        temp_nodes.append(node)
        if len(temp_time) == average_count:
            times.append(sum(temp_time) / average_count)
            nodes.append(sum(temp_nodes) / average_count)
            temp_nodes = []
            temp_time = []

# Plot the time taken
plt.figure(figsize=(10, 5))
plt.subplot(1, 2, 1)
plt.plot(unknown_count, times, marker='o', label='v1')
plt.title('Sudoku Benchmark: Time Taken')
plt.xlabel(f'Unknown Count, average over {average_count} sudokus')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Plot the number of backtracks
plt.subplot(1, 2, 2)
plt.plot(unknown_count, nodes, marker='o', color='r')
plt.title('Sudoku Benchmark: Nodes explored')
plt.xlabel(f'Unknown Count, average over {average_count} sudokus')
plt.ylabel('Node Count')
plt.yscale('log')
plt.grid(True)

plt.tight_layout()
plt.savefig('sudoku_benchmark.png')
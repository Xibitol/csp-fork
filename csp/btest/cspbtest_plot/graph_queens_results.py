#!/usr/bin/env python3

import matplotlib.pyplot as plt

# Read the data from the file
n_values_pair = []
times_pair = []
n_values_odd = []
times_odd = []
backtracks_pair = []
backtracks_odd = []

with open('n_queens_benchmark.txt', 'r') as file:
    for i, line in enumerate(file):
        time, backtrack = map(float, line.split())
        if i % 2 == 0:
            times_pair.append(time)
            backtracks_pair.append(backtrack)
            n_values_pair.append(len(n_values_pair) * 2 + 4)  # Assuming n starts from 4 and increments by 2
        else:
            times_odd.append(time)
            backtracks_odd.append(backtrack)
            n_values_odd.append(len(n_values_odd) * 2 + 5)  # Assuming n starts from 5 and increments by 2

# Plot the time taken
plt.figure(figsize=(10, 5))
plt.subplot(2, 2, 1)
plt.plot(n_values_pair, times_pair, marker='o')
plt.title('N-Queens Benchmark: Time Taken, N is even')
plt.xlabel('N')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)

# Plot the number of backtracks
plt.subplot(2, 2, 2)
plt.plot(n_values_pair, backtracks_pair, marker='o', color='r')
plt.title('N-Queens Benchmark: Nodes explored, N is even')
plt.xlabel('N')
plt.ylabel('Number of Backtracks')
plt.yscale('log')
plt.grid(True)

# Plot the time taken
plt.subplot(2, 2, 3)
plt.plot(n_values_odd, times_odd, marker='o')
plt.title('N-Queens Benchmark: Time Taken, N is odd')
plt.xlabel('N')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)

# Plot the number of backtracks
plt.subplot(2, 2, 4)
plt.plot(n_values_odd, backtracks_odd, marker='o', color='r')
plt.title('N-Queens Benchmark: Nodes explored, N is odd')
plt.xlabel('N')
plt.ylabel('Number of Backtracks')
plt.yscale('log')
plt.grid(True)

# Show the plots
plt.tight_layout()
plt.savefig('n_queens_benchmark_plot.png')
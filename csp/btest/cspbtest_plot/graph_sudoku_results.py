import matplotlib.pyplot as plt

SUDOKU_RESULT_FILE = 'sudoku_benchmark.txt'
SUDOKU_FC_RESULT_FILE = 'sudoku_fc_benchmark.txt'
SUDOKU_PLOT_FILE = 'sudoku_benchmark.png'

average_count = 5

# Read the data from the first file
unknown_count = [i for i in range(5, 81, 5)]
nodes = []
times = []

with open(SUDOKU_RESULT_FILE, 'r') as file:
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

# Read the data from the second file
fc_nodes = []
fc_times = []

with open(SUDOKU_FC_RESULT_FILE, 'r') as file:
    temp_time = []
    temp_nodes = []
    for i, line in enumerate(file):
        time, node = map(float, line.split())
        temp_time.append(time)
        temp_nodes.append(node)
        if len(temp_time) == average_count:
            fc_times.append(sum(temp_time) / average_count)
            fc_nodes.append(sum(temp_nodes) / average_count)
            temp_nodes = []
            temp_time = []

# Plot the time taken
plt.figure(figsize=(10, 5))
plt.subplot(1, 2, 1)
plt.plot(unknown_count, times, marker='o', label='Standard')
plt.plot(unknown_count, fc_times, marker='o', label='Forward Checking')
plt.title('Sudoku Benchmark: Time Taken')
plt.xlabel(f'Unknown Count, average over {average_count} sudokus')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Plot the number of backtracks
plt.subplot(1, 2, 2)
plt.plot(unknown_count, nodes, marker='o', color='b', label='Standard')
plt.plot(unknown_count, fc_nodes, marker='o', color='r', label='Forward Checking')
plt.title('Sudoku Benchmark: Nodes Explored')
plt.xlabel(f'Unknown Count, average over {average_count} sudokus')
plt.ylabel('Node Count')
plt.yscale('log')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.savefig(SUDOKU_PLOT_FILE)
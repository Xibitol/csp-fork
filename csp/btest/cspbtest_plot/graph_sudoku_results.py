import matplotlib.pyplot as plt
import os

SUDOKU_RESULT_DIR = 'sudoku/'
SUDOKU_PLOT_FILE = 'sudoku/benchmark.png'

average_count = 5

# Read the data from the first file
unknown_count = [i for i in range(5, 81, 5)]
nodes = []
times = []

def read_sudoku_results(file_name, average_count):
    times = []
    nodes = []
    with open(file_name, 'r') as file:
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
    return times, nodes

plt.figure(figsize=(10, 5))

for fname in os.listdir(SUDOKU_RESULT_DIR):
    fpath = os.path.join(SUDOKU_RESULT_DIR, fname)
    if not os.path.isfile(fpath) or not fname.endswith('.txt'):
        continue
    times, nodes = read_sudoku_results(fpath, average_count)
    version = " ".join(fname.split('_')[:-1])
    plt.subplot(1, 2, 1)
    plt.plot(unknown_count, times, marker='o', label=version)
    plt.subplot(1, 2, 2)
    plt.plot(unknown_count, nodes, marker='o', label=version)

# times, nodes = read_sudoku_results(SUDOKU_RESULT_FILE, average_count)
# fc_times, fc_nodes = read_sudoku_results(SUDOKU_FC_RESULT_FILE, average_count)
# fc_ovars_times, fc_ovars_nodes = read_sudoku_results(
# 	SUDOKU_FC_OVARS_RESULT_FILE, average_count
# )

# Plot the time taken
plt.subplot(1, 2, 1)
# plt.plot(unknown_count, times, marker='o', label='Basic')
# plt.plot(unknown_count, fc_times, marker='o', label='FC')
# plt.plot(unknown_count, fc_ovars_times, marker='o', label='FCOvars')
plt.title('Sudoku Benchmark: Time Taken')
plt.xlabel(f'Unknown Count, average over {average_count} sudokus')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Plot the number of backtracks
plt.subplot(1, 2, 2)
# plt.plot(unknown_count, nodes, marker='o', color='b', label='Basic')
# plt.plot(unknown_count, fc_nodes, marker='o', color='r', label='FC')
# plt.plot(unknown_count, fc_ovars_nodes, marker='o', color='g', label='FCOvars')
plt.title('Sudoku Benchmark: Nodes Explored')
plt.xlabel(f'Unknown Count, average over {average_count} sudokus')
plt.ylabel('Node Count')
plt.yscale('log')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.savefig(SUDOKU_PLOT_FILE)
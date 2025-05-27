import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
import os

NQUEENS_RESULT_DIR = 'n_queens/'
NQUEENS_PLOT_FILE = 'n_queens/benchmark.png'

# Read the data from the files
def read_results(file_name):
    n_values_pair, times_pair, backtracks_pair = [], [], []
    n_values_odd, times_odd, backtracks_odd = [], [], []
    with open(file_name, 'r') as file:
        for i, line in enumerate(file):
            time, backtrack = map(float, line.split())
            if i % 2 == 0:
                times_pair.append(time)
                backtracks_pair.append(backtrack)
                # Assuming n starts from 4 and increments by 2
                n_values_pair.append(len(n_values_pair) * 2 + 4)
            else:
                times_odd.append(time)
                backtracks_odd.append(backtrack)
                # Assuming n starts from 5 and increments by 2
                n_values_odd.append(len(n_values_odd) * 2 + 5)

    return (
        n_values_pair, times_pair, backtracks_pair,
        n_values_odd, times_odd, backtracks_odd
	)

plt.figure(figsize=(10, 10))

for fname in os.listdir(NQUEENS_RESULT_DIR):
    fpath = os.path.join(NQUEENS_RESULT_DIR, fname)
    if not os.path.isfile(fpath) or not fname.endswith('.txt'):
        continue
    (n_values_pair, times_pair, backtracks_pair, n_values_odd, times_odd,
     backtracks_odd) = read_results(fpath)
    version = " ".join(fname.split('_')[:-1])
    plt.subplot(2, 2, 1)
    plt.plot(n_values_pair, times_pair, marker='o', label=version)
    plt.subplot(2, 2, 3)
    plt.plot(n_values_odd, times_odd, marker='o', label=version)
    plt.subplot(2, 2, 2)
    plt.plot(n_values_pair, backtracks_pair, marker='o', label=version)
    plt.subplot(2, 2, 4)
    plt.plot(n_values_odd, backtracks_odd, marker='o', label=version)

# Even N: Time
plt.subplot(2, 2, 1)
plt.title('N-Queens Benchmark: Time Taken, N is even')
plt.xlabel('N')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Even N: Backtracks
plt.subplot(2, 2, 2)
plt.title('N-Queens Benchmark: Nodes explored, N is even')
plt.xlabel('N')
plt.ylabel('Number of Backtracks')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Odd N: Time
plt.subplot(2, 2, 3)
plt.title('N-Queens Benchmark: Time Taken, N is odd')
plt.xlabel('N')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Odd N: Backtracks
plt.subplot(2, 2, 4)
plt.title('N-Queens Benchmark: Nodes explored, N is odd')
plt.xlabel('N')
plt.ylabel('Number of Backtracks')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Force x-axis to be integers for all subplots
for ax in plt.gcf().get_axes():
    ax.xaxis.set_major_locator(MaxNLocator(integer=True))

# Show the plots
plt.tight_layout()
plt.savefig(NQUEENS_PLOT_FILE)
import matplotlib.pyplot as plt

NQUEENS_RESULT_FILE = 'n_queens_benchmark.txt'
NQUEENS_FC_RESULT_FILE = 'n_queens_fc_benchmark.txt'
NQUEENS_PLOT_FILE = 'n_queens_benchmark.png'

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
                n_values_pair.append(len(n_values_pair) * 2 + 4)  # Assuming n starts from 4 and increments by 2
            else:
                times_odd.append(time)
                backtracks_odd.append(backtrack)
                n_values_odd.append(len(n_values_odd) * 2 + 5)  # Assuming n starts from 5 and increments by 2
    return n_values_pair, times_pair, backtracks_pair, n_values_odd, times_odd, backtracks_odd

# Read results for both normal and FC
n_values_pair, times_pair, backtracks_pair, n_values_odd, times_odd, backtracks_odd = read_results(NQUEENS_RESULT_FILE)
n_values_pair_fc, times_pair_fc, backtracks_pair_fc, n_values_odd_fc, times_odd_fc, backtracks_odd_fc = read_results(NQUEENS_FC_RESULT_FILE)

# Plot the time taken
plt.figure(figsize=(10, 10))

# Even N: Time
plt.subplot(2, 2, 1)
plt.plot(n_values_pair, times_pair, marker='o', label='Normal')
plt.plot(n_values_pair_fc, times_pair_fc, marker='o', label='FC')
plt.title('N-Queens Benchmark: Time Taken, N is even')
plt.xlabel('N')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Even N: Backtracks
plt.subplot(2, 2, 2)
plt.plot(n_values_pair, backtracks_pair, marker='o', color='b', label='Normal')
plt.plot(n_values_pair_fc, backtracks_pair_fc, marker='o', color='g', label='FC')
plt.title('N-Queens Benchmark: Nodes explored, N is even')
plt.xlabel('N')
plt.ylabel('Number of Backtracks')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Odd N: Time
plt.subplot(2, 2, 3)
plt.plot(n_values_odd, times_odd, marker='o', label='Normal')
plt.plot(n_values_odd_fc, times_odd_fc, marker='o', label='FC')
plt.title('N-Queens Benchmark: Time Taken, N is odd')
plt.xlabel('N')
plt.ylabel('Time (seconds)')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Odd N: Backtracks
plt.subplot(2, 2, 4)
plt.plot(n_values_odd, backtracks_odd, marker='o', color='b', label='Normal')
plt.plot(n_values_odd_fc, backtracks_odd_fc, marker='o', color='g', label='FC')
plt.title('N-Queens Benchmark: Nodes explored, N is odd')
plt.xlabel('N')
plt.ylabel('Number of Backtracks')
plt.yscale('log')
plt.grid(True)
plt.legend()

# Show the plots
plt.tight_layout()
plt.savefig(NQUEENS_PLOT_FILE)
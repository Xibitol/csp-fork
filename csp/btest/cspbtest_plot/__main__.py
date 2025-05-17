import sys
import getopt

from matplotlib import pyplot

from common import readBenchmark, plotBenchmark
from queens import NQueensContext
from sudoku import SudokuContext

# ---
GUI_ENABLED = False

try:
	options, _ = getopt.getopt(sys.argv[1:], "", ["gui"])

	for opt, value in options:
		if opt == "--gui":
			GUI_ENABLED = True
except getopt.GetoptError:
	pass

# ---
if __name__ == "__main__":
	nqueens = readBenchmark(lambda: NQueensContext())
	nqueensFig = pyplot.figure("nqueens")
	plotBenchmark(nqueens, nqueensFig)

	sudoku = readBenchmark(lambda: SudokuContext())
	sudokuFig = pyplot.figure("sudoku")
	plotBenchmark(sudoku, sudokuFig)

	if GUI_ENABLED: pyplot.show()
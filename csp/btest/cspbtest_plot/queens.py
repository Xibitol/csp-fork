from typing import Any

from matplotlib.figure import Figure
from matplotlib.gridspec import SubplotSpec

from common import BenchmarkContext, BenchmarkRecord

class NQueensContext(BenchmarkContext):

	RESULT_FILE = "n_queens_benchmark.txt"
	GROUP_SIZE = 1
	OFFSET = 4
	PLOT_FILE = "n_queens_benchmark.png"
	FIGURE_TITLE = "NQueens Benchmark"
	FIGURE_MIN_SIZE = (6, 5)

	def __init__(self):
		self.__data: dict[int, BenchmarkRecord] = {}

	# GETTERS
	def getResultFile(self) -> str:
		return NQueensContext.RESULT_FILE
	def getGroupsSize(self) -> int:
		return NQueensContext.GROUP_SIZE

	def getPlotFile(self) -> str:
		return NQueensContext.PLOT_FILE
	def getFigureTitle(self) -> str:
		return NQueensContext.FIGURE_TITLE
	def getFigureMinSize(self) -> tuple[float, float]:
		return NQueensContext.FIGURE_MIN_SIZE

	# SETTERS
	def addRecordGroup(self, records: tuple[BenchmarkRecord, ...]):
		self.__data[len(self.__data) + NQueensContext.OFFSET] = records[0]

	# FUNCTIONS
	def generateSubplots(self, figure: Figure):
		gs = figure.add_gridspec(2, 2)

		# Time
		self.generateTimeSubplot(figure, gs[0, 0])
		self.generateTimeSubplot(figure, gs[1, 0], True)

		# BacktrackCount
		self.generateBacktrackSubplot(figure, gs[0, 1])
		self.generateBacktrackSubplot(figure, gs[1, 1], True)

		return figure

	def generateTimeSubplot(self, figure: Figure,
		pos: SubplotSpec, isOdd: bool = False
	):
		BenchmarkContext.generateSubplot(figure, pos,
			{i: br.getTime() for i, br in self.__data.items()
				if isOdd and i%2 != 0 or not isOdd and i%2 == 0
			},
			title=f"Time Taken, N is {'odd' if isOdd else 'even'}",
			xlabel="N", ylabel="Time (seconds)"
		)
	def generateBacktrackSubplot(self, figure: Figure,
		pos: SubplotSpec, isOdd: bool = False
	):
		BenchmarkContext.generateSubplot(figure, pos,
			{i: br.getBacktrackCount()
				for i, br in self.__data.items()
				if isOdd and i%2 != 0 or not isOdd and i%2 == 0
			},
			title=f"Nodes explored, N is {'odd' if isOdd else 'even'}",
			xlabel="N", ylabel="Number of Backtracks",
			color="tab:red"
		)
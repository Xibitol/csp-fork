from typing import Any

from matplotlib.figure import Figure
from matplotlib.gridspec import SubplotSpec

from common import BenchmarkContext, BenchmarkRecord

class SudokuContext(BenchmarkContext):

	RESULT_FILE = "sudoku_benchmark.txt"
	GROUP_SIZE = 5
	OFFSET = 5
	PLOT_FILE = "sudoku_benchmark.png"
	FIGURE_TITLE = "Sudoku Benchmark"
	FIGURE_MIN_SIZE = (8, 5)

	def __init__(self):
		self.__data: dict[int, BenchmarkRecord] = {}

	# GETTERS
	def getResultFile(self) -> str:
		return SudokuContext.RESULT_FILE
	def getGroupsSize(self) -> int:
		return SudokuContext.GROUP_SIZE

	def getPlotFile(self) -> str:
		return SudokuContext.PLOT_FILE
	def getFigureTitle(self) -> str:
		return SudokuContext.FIGURE_TITLE
	def getFigureMinSize(self) -> tuple[float, float]:
		return SudokuContext.FIGURE_MIN_SIZE

	# SETTERS
	def addRecordGroup(self, records: tuple[BenchmarkRecord, ...]):
		index = len(self.__data)*self.getGroupsSize() + SudokuContext.OFFSET

		self.__data[index] = BenchmarkRecord(
			sum(map(lambda r: r.getTime(), records))/len(records),
			sum(map(lambda r: r.getBacktrackCount(), records))//len(records)
		)

	# FUNCTIONS
	def generateSubplots(self, figure: Figure):
		gs = figure.add_gridspec(1, 2)

		self.generateTimeSubplot(figure, gs[0, 0])
		self.generateBacktrackSubplot(figure, gs[0, 1])

		return figure

	def generateTimeSubplot(self, figure: Figure, pos: SubplotSpec
	):
		BenchmarkContext.generateSubplot(figure, pos,
			{i: br.getTime() for i, br in self.__data.items()},
			title=f"Time Taken",
			xlabel=
				f"Unknown Count, average over {self.getGroupsSize()} sudokus",
			ylabel="Time (seconds)"
		)
	def generateBacktrackSubplot(self, figure: Figure,
		pos: SubplotSpec, isOdd: bool = False
	):
		BenchmarkContext.generateSubplot(figure, pos,
			{i: br.getBacktrackCount() for i, br in self.__data.items()},
			title="Nodes explored",
			xlabel=
				f"Unknown Count, average over {self.getGroupsSize()} sudokus",
			ylabel="Number of Backtracks",
			color="tab:red"
		)
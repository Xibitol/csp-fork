from abc import ABCMeta, abstractmethod
from typing import Any, Callable, TypeAlias, TypeVar

from matplotlib.figure import Figure
from matplotlib.gridspec import SubplotSpec

ReturnType = TypeVar('ReturnType', bound=Any)
Supplier: TypeAlias = Callable[[], ReturnType]

class BenchmarkRecord():

	def __init__(self, time: float, backtrackCount: int):
		self.__time = time
		self.__backtrackCount = backtrackCount

	# GETTERS
	def getTime(self) -> float: return self.__time
	def getBacktrackCount(self) -> int: return self.__backtrackCount

class BenchmarkContext(metaclass=ABCMeta):

	# GETTERS
	@abstractmethod
	def getResultFile(self) -> str:
		pass
	@abstractmethod
	def getGroupsSize(self) -> int:
		pass

	@abstractmethod
	def getPlotFile(self) -> str:
		pass
	@abstractmethod
	def getFigureTitle(self) -> str:
		pass
	@abstractmethod
	def getFigureMinSize(self) -> tuple[float, float]:
		pass

	# SETTERS
	@abstractmethod
	def addRecordGroup(self, records: tuple[BenchmarkRecord, ...]):
		pass

	# FUNCTIONS
	@staticmethod
	def generateSubplot(figure: Figure,
		pos: SubplotSpec, values: dict[int, int | float],
		color: str = "tab:blue",
		**kwargs
	):
		keys = tuple(values.keys())

		axe = figure.add_subplot(pos,
			**kwargs,
			yscale="log"
		)
		axe.set_xticks(keys, map(lambda v: str(v), keys),
			rotation=67.5
		)
		axe.grid(True)

		axe.plot(*zip(*values.items()), marker="o", color=color)

	@abstractmethod
	def generateSubplots(self, figure: Figure):
		pass

# ---
TBenchmarkContext = TypeVar('TBenchmarkContext', bound=BenchmarkContext)

def readBenchmark(
	contextSupplier: Supplier[TBenchmarkContext]
) -> BenchmarkContext:
	context = contextSupplier()
	buffer: list[BenchmarkRecord] = []

	if context.getGroupsSize() <= 0:
		raise ValueError(
			"ReadingWorker cannot accept groups smaller than 1 element."
		)

	with open(context.getResultFile() ,'r') as f:
		for i, line in enumerate(f):
			values = line.split()
			buffer.append(BenchmarkRecord(
				float(values[0]), int(values[1])
			))

			if len(buffer) == context.getGroupsSize():
				context.addRecordGroup(tuple(buffer))
				buffer.clear()
		else:
			if len(buffer) > 0:
				context.addRecordGroup(tuple(buffer))

	return context

def plotBenchmark(context: BenchmarkContext, figure: Figure):
	context.generateSubplots(figure)

	figure.suptitle(context.getFigureTitle())
	figure.set_size_inches(context.getFigureMinSize())
	figure.tight_layout()

	figure.savefig(context.getPlotFile())
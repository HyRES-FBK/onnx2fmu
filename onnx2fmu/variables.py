import re
import numpy as np
from typing import Union
from onnx import TensorProto

from onnx2fmu.config import FMI2TYPES, FMI3TYPES, FMI_VERSIONS


CONTINUOUS = "continuous"
DISCRETE = "discrete"

class VariableFactory:

    def __init__(self,
                 name: str,
                 shape: tuple = (1, ),
                 description: str = "",
                 variability: str = CONTINUOUS,
                 fmiVersion: str = "2.0",
                 vType: TensorProto.DataType = TensorProto.FLOAT,
                 labels: None | list[str] = None,
                 start: Union[str, int, float, list] = "1.0"
                 ) -> None:
        self._setName(name=name)
        self._setShape(shape=shape)
        self.description = description
        self.variability = variability
        self._setFmiVersion(fmiVersion=fmiVersion)
        self._setType(vType)
        self._setCausality()
        self._setStartValues(start=start)
        self._setLabels(labels=labels)
        self._initializeScalarValues()

        self._context_variables = [
            "name",
            "nodeName",
            "shape",
            "description",
            "causality",
            "variability",
            "fmiVersion",
            "vType",
            "scalarValues",
        ]

    def __str__(self) -> str:
        return f"{self.__class__.__name__}" + \
            f"({self.name}, {self.variability})"

    def _setName(self, name: str) -> None:
        if not name:
            raise ValueError("Name is a required argument.")
        else:
            self.name = self._cleanName(name)
            self.nodeName = name

    def _cleanName(self, name: str):
        return re.sub(r'[^\w]', '', name)

    def _setShape(self, shape: tuple) -> None:
        if shape is None or shape == ():
            raise ValueError(f"Shape is empty. {shape}")
        self.shape = tuple(1 if dim == 0 else dim for dim in shape)

    def _setFmiVersion(self, fmiVersion: str) -> None:
        if fmiVersion not in FMI_VERSIONS:
            raise ValueError(f"{fmiVersion} is not an admissible FMI version.")
        self.fmiVersion = fmiVersion

    def _setType(self, vType: TensorProto.DataType) -> None:
        assert getattr(self, "fmiVersion") is not None
        if self.fmiVersion == "2.0":
            self.vType = FMI2TYPES[vType]
        elif self.fmiVersion == "3.0":
            self.vType = FMI3TYPES[vType]

    def _setCausality(self) -> None:
        if type(self) is VariableFactory:
            self.causality = None
        else:
            self.causality = self.__class__.__name__.lower()

    def _setStartValues(self, start: Union[str, int, float, list]) -> None:
        start_values = np.ones(shape=self.shape, dtype=np.float32)
        if isinstance(start, (str, int, float)):
            start_values *= float(start)
        elif type(start) is list:
            start_values *= np.array(start, dtype=np.float32)
        else:
            raise TypeError(f"Start values must be a string or a list, "
                            f"not {type(start)}.")
        self.startValues = start_values

    def _getStartValue(self, idx: tuple[int, ...]) -> str:
        return str(self.startValues[idx]) # type: ignore

    def _setLabels(self, labels: None | list[str] = None) -> None:
        if labels is None:
            self.labels = []
        elif isinstance(labels, list):
            self.labels = labels
        else:
            raise TypeError(f"Labels must be a list, not {type(labels)}.")

    def _initializeScalarValues(self) -> None:
        self.scalarValues = [
            {"name": self.name + "_" + "_".join([str(k) for k in idx]),
             "label": self.labels[i] if i < len(self.labels) else "",
             "start": self._getStartValue(idx=idx)}
            for i, idx in enumerate(np.ndindex(self.shape))
        ]

    def generateContext(self) -> dict[str, str]:
        return {k: getattr(self, k) for k in self._context_variables}


class Input(VariableFactory):

    def __str__(self) -> str:
        return f"{self.__class__.__name__}" + \
            f"({self.name}, {self.variability})({self.startValues[0]})"


class Output(VariableFactory):
    pass


class Local(VariableFactory):

    def __init__(self,
                 nameIn: str,
                 nameOut: str,
                 shape: tuple = (1, ),
                 description: str = "",
                 variability: str = CONTINUOUS,
                 fmiVersion: str = "2.0",
                 vType: TensorProto.DataType = TensorProto.FLOAT,
                 labels: None | list[str] = None,
                 initial: str = "exact",
                 start: Union[str, int, float, list] = "1.0"
                 ) -> None:
        self.nameIn = self._cleanName(name=nameIn)
        self.nodeNameIn = nameIn
        self.nameOut = self._cleanName(name=nameOut)
        self.nodeNameOut = nameOut
        name = "_".join([self.nameIn, self.nameOut])
        super().__init__(name=name, shape=shape, description=description,
                         variability=variability, fmiVersion=fmiVersion,
                         vType=vType, labels=labels, start=start)
        self.initial = initial
        self._context_variables += ["nameIn", "nameOut", "nodeNameIn",
                                    "nodeNameOut", "initial"]


if __name__ == "__main__":
    pass

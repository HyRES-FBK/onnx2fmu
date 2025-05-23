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
                 ) -> None:
        self.setName(name=name)
        self.setShape(shape=shape)
        self.description = description
        self.variability = variability
        self.setFmiVersion(fmiVersion=fmiVersion)
        self.setType(vType)
        self.setCausality()
        self.initializeScalarValues()

        self._context_variables = [
            "name",
            "nodeName",
            "shape",
            "description",
            "causality",
            "variability",
            "fmiVersion",
            "vType",
            "scalarValues"
        ]

    def __str__(self) -> str:
        return f"{self.__class__.__name__}" + \
            f"({self.name}, {self.variability})"

    def setName(self, name: str) -> None:
        if not name:
            raise ValueError("Name is a required argument.")
        else:
            self.name = self.cleanName(name)
            self.nodeName = name

    def cleanName(self, name: str):
        return re.sub(r'[^\w]', '', name)

    def setShape(self, shape: tuple) -> None:
        if shape is None or shape == ():
            raise ValueError(f"Shape is empty. {shape}")
        self.shape = tuple(1 if dim == 0 else dim for dim in shape)

    def setFmiVersion(self, fmiVersion: str) -> None:
        if fmiVersion not in FMI_VERSIONS:
            raise ValueError(f"{fmiVersion} is not an admissible FMI version.")
        self.fmiVersion = fmiVersion

    def setType(self, vType: TensorProto.DataType) -> None:
        assert getattr(self, "fmiVersion") is not None
        if self.fmiVersion == "2.0":
            self.vType = FMI2TYPES[vType]
        elif self.fmiVersion == "3.0":
            self.vType = FMI3TYPES[vType]

    def setCausality(self) -> None:
        if type(self) is VariableFactory:
            self.causality = None
        else:
            self.causality = self.__class__.__name__.lower()

    def initializeScalarValues(self) -> None:
        self.scalarValues = [
            {"name": self.name + "_" + "_".join([str(k) for k in idx])}
            for idx in np.ndindex(self.shape)
        ]

    def generateContext(self) -> dict[str, str]:
        return {k: getattr(self, k) for k in self._context_variables}


class Input(VariableFactory):

    def __init__(self,
                 name: str,
                 shape: tuple = (1, ),
                 description: str = "",
                 variability: str = CONTINUOUS,
                 fmiVersion: str = "2.0",
                 vType: TensorProto.DataType = TensorProto.FLOAT,
                 start: Union[str, int, float] = "1.0"
                 ) -> None:
        super().__init__(
            name=name, shape=shape, description=description,
            variability=variability, fmiVersion=fmiVersion, vType=vType)
        self.setStartValue(start)

        self._context_variables += ["start"]

    def setStartValue(self, start: Union[str, float]):
        if type(start) in [int, float]:
            start = str(float(start))
        self.start = start

    def __str__(self) -> str:
        return f"{self.__class__.__name__}" + \
            f"({self.name}, {self.variability})({self.start})"


class Output(VariableFactory):
    pass


class Local(VariableFactory):

    def __init__(self,
                 name_in: str,
                 name_out: str,
                 shape: tuple = (1, ),
                 description: str = "",
                 variability: str = CONTINUOUS,
                 fmiVersion: str = "2.0",
                 vType: TensorProto.DataType = TensorProto.FLOAT
                 ) -> None:
        self.name_in = self.cleanName(name=name_in)
        self.name_out = self.cleanName(name=name_out)
        name = "_".join([self.name_in, self.name_out])
        super().__init__(name=name, shape=shape, description=description,
                         variability=variability, fmiVersion=fmiVersion,
                         vType=vType)
        self._context_variables += ["name_in", "name_out"]


if __name__ == "__main__":
    pass

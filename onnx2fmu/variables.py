import re
from onnx import TensorProto

from onnx2fmu.config import FMI2TYPES, FMI3TYPES, FMI_VERSIONS


CONTINUOUS = "continuous"
DISCRETE = "discrete"

class VariableFactory:

    def __init__(self,
                 name: str,
                 description: str = "",
                 variability: str = CONTINUOUS,
                 fmiVersion: str = "2.0",
                 vType: TensorProto.DataType = TensorProto.FLOAT,
                 ) -> None:
        self.setName(name=name)
        self.description = description
        self.variability = variability
        self.setFmiVersion(fmiVersion=fmiVersion)
        self.setType(vType)
        self.setCausality()

        self._context_variables = [
            "name",
            "description",
            "causality",
            "variability",
            "fmiVersion",
            "vType",
        ]

    def __str__(self) -> str:
        return f"{self.__class__.__name__}" + \
            f"({self.name}, {self.causality}, {self.variability})"

    def setName(self, name: str) -> None:
        if not name:
            raise ValueError("Name is a required argument.")
        else:
            self.name = re.sub(r'[^\w]', '', name)

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

    def generate_context(self) -> dict[str, str]:
        return {k: getattr(self, k) for k in self._context_variables}


if __name__ == "__main__":
    pass
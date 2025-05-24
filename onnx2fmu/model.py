import re
import uuid
from typing import Union, Optional

from onnx2fmu.variables import Input, Output, Local


class Model:
    """
    The model factory class.
    """

    canGetAndSetFMUstate = True
    canSerializeFMUstate = True
    canNotUseMemoryManagementFunctions = True
    canHandleVariableCommunicationStepSize = True
    providesIntermediateUpdate = True
    canReturnEarlyAfterIntermediateUpdate = True
    fixedInternalStepSize = 1
    startTime = 0
    stopTime = 1

    def __init__(self,
                 name: str,
                 fmiVersion: Optional[str] = "2.0",
                 description: Optional[str] = "",
                 ) -> None:
        self.setName(name)
        self.fmiVersion = fmiVersion
        self.description = description
        self.vr_generator = (i for i in range(2**32))
        self.GUID = str(uuid.uuid4())
        self.inputs = []
        self.outputs = []
        self.locals = []

    def setName(self, name: str) -> None:
        self.name = re.sub(r'[^a-zA-Z0-9]', '', name)

    def _assignValueReferences(self, context: dict) -> dict:
        for scalar in context["scalarValues"]:
            scalar["valueReference"] = next(self.vr_generator)
        return context

    def addVariable(self, variable: Union[Input, Output, Local]) -> None:
        context = variable.generateContext()
        context = self._assignValueReferences(context=context)
        if isinstance(variable, Input):
            self.inputs.append(context)
        elif isinstance(variable, Output):
            self.outputs.append(context)
        elif isinstance(variable, Local):
            self.locals.append(context)
        else:
            raise ValueError(f"{variable} is not an admissible variable.")


if __name__ == "__main__":
    pass

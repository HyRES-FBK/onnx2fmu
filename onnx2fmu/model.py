import re
import uuid
from datetime import datetime
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
        self.vr_generator = (i for i in range(1, 2**32))
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

    def _missingInputOrOutput(self) -> bool:
        return any([len(i) == 0 for i in [self.inputs, self.outputs]])

    def generateContext(self) -> dict[str, Union[str, bool, int, list, None]]:
        if self._missingInputOrOutput():
            raise ValueError("Inputs or outputs list is empty.")
        return {
            'name': self.name,
            'description': self.description,
            'GUID': self.GUID,
            'FMIVersion': self.fmiVersion,
            'generationDateAndTime': datetime.now().isoformat(),
            'canGetAndSetFMUstate': self.canGetAndSetFMUstate,
            'canSerializeFMUstate': self.canSerializeFMUstate,
            'canNotUseMemoryManagementFunctions': \
                self.canNotUseMemoryManagementFunctions,
            'canHandleVariableCommunicationStepSize': \
                self.canHandleVariableCommunicationStepSize,
            'providesIntermediateUpdate': self.providesIntermediateUpdate,
            'canReturnEarlyAfterIntermediateUpdate': \
                self.canReturnEarlyAfterIntermediateUpdate,
            'fixedInternalStepSize': self.fixedInternalStepSize,
            'startTime': self.startTime,
            'stopTime': self.stopTime,
            'inputs': self.inputs,
            'outputs': self.outputs,
            'locals': self.locals
        }


if __name__ == "__main__":
    pass

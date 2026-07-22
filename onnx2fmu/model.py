import re
import uuid
from datetime import datetime
from typing import Union, Optional

from onnx2fmu.variables import Input, Output, Local


MODEL_EXCHANGE = "ModelExchange"
CO_SIMULATION = "CoSimulation"
DEFAULT_FMU_TYPES = [CO_SIMULATION]


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
                 fmuType: Optional[list[str]] = None,
                 ) -> None:
        self._setName(name)
        self.fmiVersion = fmiVersion
        self.description = description
        self.fmuType = fmuType if fmuType else list(DEFAULT_FMU_TYPES)
        self.modelExchange = MODEL_EXCHANGE in self.fmuType
        self.coSimulation = CO_SIMULATION in self.fmuType
        self.vr_generator = (i for i in range(1, 2**32))
        self.GUID = str(uuid.uuid4())
        self.inputs = []
        self.outputs = []
        self.locals = []

    def _setName(self, name: str) -> None:
        self.name = re.sub(r'[^a-zA-Z0-9_]', '', name)

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

    def _pairStatesAndDerivatives(self) -> list[dict]:
        """Pair each state's scalars with its derivative's scalars.

        Called after every variable has been added, so `self.inputs` /
        `self.outputs` already hold the final scalar value references.
        Injects `stateValueReference` into each derivative scalar context
        (consumed by the FMI2/3 XML `derivative` attribute) and returns the
        ordered list of state/derivative scalar name pairs consumed by
        `config.h` (`NX`) and `model.c` (the continuous-state functions).
        """
        state_lookup = {inp["name"]: inp
                        for inp in self.inputs if inp.get("isState")}
        states = []
        for output in self.outputs:
            state_name = output.get("derivativeOf")
            if not state_name:
                continue
            state = state_lookup.get(state_name)
            if state is None:
                raise ValueError(
                    f"Derivative output '{output['name']}' references "
                    f"unknown state '{state_name}'."
                )
            for state_scalar, derivative_scalar in zip(
                    state["scalarValues"], output["scalarValues"]):
                derivative_scalar["stateValueReference"] = \
                    state_scalar["valueReference"]
                states.append({
                    "stateName": state_scalar["name"],
                    "derivativeName": derivative_scalar["name"],
                })
        return states

    def generateContext(self) -> dict[str, Union[str, bool, int, list, None]]:
        if self._missingInputOrOutput():
            raise ValueError("Inputs or outputs list is empty.")
        states = self._pairStatesAndDerivatives()
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
            'modelExchange': self.modelExchange,
            'coSimulation': self.coSimulation,
            'states': states,
            'numberOfContinuousStates': len(states),
            'inputs': self.inputs,
            'outputs': self.outputs,
            'locals': self.locals
        }


if __name__ == "__main__":
    pass

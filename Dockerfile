FROM ubuntu:20.04
# Set image name
LABEL image=onnx2fmu
# Install dependencies: build-essential, cmake, git
RUN apt-get update && apt-get install -y build-essential
# Install conda
RUN apt-get install -y wget
RUN wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
RUN bash Miniconda3-latest-Linux-x86_64.sh -b
RUN rm Miniconda3-latest-Linux-x86_64.sh
# Add conda to PATH
ENV PATH="/root/miniconda3/bin:${PATH}"
# Initialize conda
RUN conda init bash
# Restart shell to activate conda
SHELL ["/bin/bash", "--login", "-c"]
# Set working folder
WORKDIR /app
# Copy all files and folders in the current directory to the container
COPY . .
# Create a new conda environment
RUN conda env create -f environment.yml
# Build FMU
RUN conda run -n onnx2fmu onnx2fmu build onnx_models/stand_model.onnx
# Test the FMU
RUN ./fmusim --input-file tests/resources/RDFrampup-CH10-CO1.5-input.csv --logging-on --log-level ok --output-file out.txt --stop-time 10 build/fmus/stand_model.fmu
# Copy the FMU to the host
CMD cp build/fmus/stand_model.fmu /stand_model.fmu

# Anomaly Detector - C++ Project

This project implements an anomaly detection system in C++. The system is designed to detect anomalies in time series data using different algorithms. The codebase includes classes and functions that manage data input, anomaly detection, and interaction with the user through a command-line interface (CLI).



![anomali](https://user-images.githubusercontent.com/84729141/164452488-bd072bce-f9e1-4da4-9231-c1cdd12c917e.jpg)


## Project Structure

- **AnomalyDetector.h / SimpleAnomalyDetector.cpp / HybridAnomalyDetector.cpp**: These files implement the core logic for anomaly detection. 
  - `SimpleAnomalyDetector`: Detects anomalies using a simple correlation-based approach.
  - `HybridAnomalyDetector`: Extends the simple detector with a more complex algorithm for better accuracy in certain cases.
  
- **minCircle.cpp / minCircle.h**: Contains algorithms to compute the minimum enclosing circle for a set of points, which is used in the hybrid anomaly detection.

- **timeseries.cpp / timeseries.h**: Manages the time series data structure, handling data input, storage, and retrieval.

- **anomaly_detection_util.cpp / anomaly_detection_util.h**: Provides utility functions for calculating statistical measures such as correlation, variance, etc., used in anomaly detection.

- **CLI.cpp / CLI.h**: Implements the command-line interface for interacting with the anomaly detection system. This includes parsing user commands and displaying results.

- **commands.h**: Contains the different commands supported by the CLI for interacting with the system.

- **Server.cpp / Server.h**: Implements the server-side logic to support remote interaction with the anomaly detection system, potentially over a network.

- **MainTrain.cpp**: The main entry point of the application, which initializes the system and starts the CLI or server as needed.

- **CMakeLists.txt**: The CMake configuration file used for building the project.

- **csvTest.csv**: A sample CSV file used for testing the anomaly detection system.

- **input.txt**: A sample input file demonstrating the commands that can be used in the CLI.

## How to Build and Run

### Prerequisites

- C++ Compiler (e.g., g++)
- CMake

### Building the Project

1. Navigate to the project directory:
   ```bash
   cd Anomaly_Detector-Cpp-main
   ```

2. Create a build directory and navigate to it:
   ```bash
   mkdir build && cd build
   ```

3. Run CMake to generate the build files:
   ```bash
   cmake ..
   ```

4. Build the project using make:
   ```bash
   make
   ```

### Running the Application

After building the project, you can run the application by executing the compiled binary. Depending on your setup, this might look something like:

```bash
./AnomalyDetector
```

You can then interact with the system using the CLI commands defined in the `commands.h` file.

### Example

To see the system in action, you can use the `csvTest.csv` file to test the anomaly detection:

```bash
./AnomalyDetector < csvTest.csv
```




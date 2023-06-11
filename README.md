# Project Name

This project is based on efishery assessment.

## Table of Contents

- [Project Overview](#project-overview)
- [Getting Started (Software)](#getting-started-software)
- [Getting Started (Hardware)](#getting-started-hardware)
- [Library Requirements](#library-requirements)
- [Usage](#usage)
- [MQTT Credential](#mqtt-credential)
- [Additional Resource](#additional-resources)

## Project Overview

The goal of this project is to develop a system using the ESP32 microcontroller as a sensor node to collect environment sensor data, and communicate with other ESP32 microcontroller as forwarderNode via Bluetooth Low Energy (BLE), the forwarderNode connect to a MQTT broker for data transmission, and provide a simple async web server interface for monitoring and control using Arduino IDE framework and platformIO.

## Getting Started (Software)

These instructions will guide you through the process of installing PlatformIO in Visual Studio Code for developing embedded systems projects.

### Software Prerequisites

- [Visual Studio Code](https://code.visualstudio.com/) installed on your system.
- Python 3.5 or later installed on your system.

### Software Installation

1. Open Visual Studio Code.

2. Click on the Extensions view located on the left sidebar (or press `Ctrl+Shift+X`/`Cmd+Shift+X`).

3. Search for "PlatformIO IDE" in the Extensions view.

4. Click on the "Install" button next to the "PlatformIO IDE" extension.

5. Restart Visual Studio Code when prompted.

6. Set up a PlatformIO project:

   - To create a new project, click on the PlatformIO icon on the left sidebar to open the PlatformIO Home. Click on the "New Project" button, select your board, and provide a name and location for the project.

   - To import an existing project, click on the PlatformIO icon and then click on the "Open Project" button. Browse to the directory where your existing project is located and select the project configuration file (platformio.ini) to import it.

7. Start coding with PlatformIO: With the project set up, you can start coding your embedded systems project using the PlatformIO features within Visual Studio Code. Create or edit source code files, configure project settings, manage libraries, and build/upload your project to the development board using the PlatformIO commands and menus provided by the extension.

## Getting Started (Hardware)
These instruction will guide you through the process of installing the hardware and mentioning all the components.

### Components
- ESP32 as sensorNode
- ESP32 as forwarderNode
- Cloudmqtt as MQTT Broker

### Firmware Installation

1. Clone or Download efishery-esp32-sensorNode & efishery-esp32-forwarderNode in this GitHub.

2. Set up a PlatformIO project:

   - Import the downloaded project, click on the PlatformIO icon and then click on the "Open Project" button. Browse to the directory where your downloaded project is located and select the project configuration file (platformio.ini) to import it.

3. Build the firmware:

   - Open the project in Visual Studio Code.
   - Navigate to the source code file that contains the firmware you want to inject into the ESP32.
   - Make any necessary modifications to the source code.
   - Click on the PlatformIO icon on the left sidebar.
   - Under "Project Tasks," click on "Build" to compile the firmware.

4. Connect the ESP32:

   - Make sure your ESP32 development board is connected to your computer via USB.

5. Upload the firmware:

   - Click on the PlatformIO icon on the left sidebar.
   - Under "Project Tasks," click on "Upload" to inject the firmware into the ESP32.
   - The firmware will be compiled again (if necessary) and then uploaded to the ESP32. The process may take a few moments.

6. Verify the firmware:

    - After the upload is complete, you can open the Serial Monitor in Visual Studio Code to verify the output from the ESP32 and ensure that the firmware is running as expected.

## Library requirements

### Flask

1. Make sure you have Python installed on your system. You can download it from the official Python website: [Python.org](https://www.python.org/downloads/).

2. Open a terminal or command prompt.

3. Install Flask using `pip`, the Python package installer, by running the following command:
   ```
   pip install flask
   ```

4. Flask should now be installed on your system. You can verify the installation by running the following command:
   ```
   flask --version
   ```

### Paho-MQTT

1. Open a terminal or command prompt.

2. Install Paho-MQTT using `pip` by running the following command:
   ```
   pip install paho-mqtt
   ```

3. Paho-MQTT should now be installed on your system. You can verify the installation by importing the library in a Python script without any errors.

### SQLAlchemy

1. Open a terminal or command prompt.

2. Install SQLAlchemy using `pip` by running the following command:
   ```
   pip install sqlalchemy
   ```

3. SQLAlchemy should now be installed on your system. You can verify the installation by importing the library in a Python script without any errors.

## Usage
In this section, step by step to run the devices will be explained:

1. Download/Clone python file on this repository {efishery-flask}.

2. run efishery_backend.py in the terminal by execute following command : 
   '''
   python efishery_backend.py
   '''

3. plug sensorNode ESP32 and forwarderNode ESP32 to the power source.

4. ESP32 sensorNode will send environment data to forwarderNode via Bluetooth Low Energy and forwarderNode will forward it to MQTT Broker.

5. python backend application subscribe to "/environment" topic, and has callback to it.

6. if there's environment data coming from the topic, backend will save the data to local database automatically.

## MQTT Credential
- server    : tailor.cloudmqtt.com
- port      : 11933
- username  : sobinwaj
- password  : 32GHLBqgoufD

### MQTT Topic
- pub_topic : /environment
- com_topic : /deviceControl  

### MQTT Command
1. Reboot payload :
   
   '''
   {"eventName":"deviceReboot","reboot":1}
   '''

## Additional Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [Visual Studio Code Documentation](https://code.visualstudio.com/docs)
# XML-Social-Network-Analyzer

XML Social Network Analyzer is a desktop application that parses, analyzes, and visualizes social network data stored in XML files. It supports operations like XML validation, formatting, minifying, JSON conversion, and compression. The tool also represents user relationships as a graph and provides network analysis features such as identifying influencers, mutual followers, and suggesting connections.

## Table of Contents
- [Installer](#installer)
- [Demo Video](#demo-video)
- [How to Build & Run the Project](#how-to-build--run-the-project)
- [Running in Command Line Mode](#running-in-command-line-mode)
- [Team Members](#team-members)

## Installer

To facilitate seamless deployment, the development team utilized Inno Setup to generate a standalone executable installer (.exe). This installation package efficiently bundles all required Dynamic Link Libraries (DLLs) using high-ratio compression, ensuring that the software has all necessary runtime dependencies without requiring manual configuration by the user.

[**Download Installer**](https://github.com/amr10w/XML-Social-Network-Analyzer/tree/main/installation)

Furthermore, the installation script is configured to automatically append the application’s binary directory to the Windows PATH environment variable. This critical configuration enables global command-line accessibility, allowing the user to invoke the CLI tool simply by typing ilovexml from any command prompt location, thereby eliminating the need to reference the absolute file path.

## Demo Video
https://github.com/user-attachments/assets/3feee4ce-2742-4cc5-8aa2-45d700f5e1cc
## How to Build & Run the Project 

This guide explains how to set up, build, and run the **XML Social Network Analyzer** using **VS Code**, **Qt 6**, and **CMake**.

---

### **Required Tools (Install Before Running)**

|Tool|Purpose|Download Link|
|---|---|---|
|**Qt 6+**|GUI framework|[https://www.qt.io/](https://www.qt.io/)|
|**CMake**|Build system|[https://cmake.org/download/](https://cmake.org/download/)|
|**VS Code Extensions**|Required inside VS Code|- _Qt Tools_  <br>- _CMake Tools_|

---

### **Clone the Repository**

Open terminal and run:
```sh
git clone https://github.com/amr10w/XML-Social-Network-Analyzer.git
```


Then open the repository folder in **VS Code**.

---

### **Step 1 — Register Qt in VS Code**

1. Press **Ctrl + Shift + P**
2. Search: **Qt: Register Qt Installation**
3. Choose the Qt folder you installed (for example):

```sh
D:\Qt
```

- This allows the VS Code Qt extension to detect `qmake`, Qt libraries, and tools.

---

###  **Step 2 — Configure CMake Tools**

1. Press **Ctrl + Shift + P**
2. Search: **CMake: Quick Start**  
	(only the first time to let CMake configure the project)
3. Press **Ctrl + Shift + P** again
4. Search: **CMake: Select a Kit**
5. Choose the kit that corresponds to **Qt 6** (e.g., _minjw or _MSVC + Qt_ or _GCC + Qt_).

---

### **Step 3 — Build & Run the Project**

#### ▶️ **Run in GUI Mode (Normal Application Run)**

Try:
``` sh
Ctrl + Shift + F5
```


If this works, the GUI will start directly.

#### If Ctrl+Shift+F5 doesn’t work:

You can:

#### Option A — Build using CMake Tools

1. Click **Build** (bottom status bar in VS Code)
2. When build finishes, you can run the target from VS Code.

#### Option B — Create a debug configuration

1. Press **Ctrl + Shift + D**
2. Click **Create launch.json**
3. Choose:
```nginx
Qt: Debug with cppvsdbg (Windows)
```

Then run using:

`Ctrl + Shift + F5`

This always starts the **GUI Mode**.

---

## Running in Command Line Mode

After the first GUI run, your build folder will contain:

```sh
ilovexml.exe
```

To run CLI mode:

```sh
cd .\build\ .\ilovexml.exe <command>
```

###  **Available Commands**

#### Validate XML

```sh
.\ilovexml.exe verify -i test.xml
# With formatting and output
.\ilovexml.exe verify -i test.xml -f -o fixed.xml
```

#### Minify XML

```sh
.\ilovexml.exe mini -i in.xml -o out.xml
```

#### Format XML (Prettify)

```sh
.\ilovexml.exe format -i input.xml -o formatted.xml
```

#### Convert to JSON

```sh
.\ilovexml.exe json -i input.xml -o output.json
```

#### Compress XML

```sh
.\ilovexml.exe compress -i sample.xml -o sample.comp
```

#### Decompress XML

```sh
.\ilovexml.exe decompress -i sample.comp -o sample_decompressed.xml
```

#### Mutual Followers

```sh
.\ilovexml.exe mutual -i data.xml -ids 1,2,3
```

#### Visualize Graph

```sh
.\ilovexml.exe draw -i data.xml -o graph.png
```

#### Search

```sh
# By Word
.\ilovexml.exe search -w wordToSearch -i data.xml

# By Topic
.\ilovexml.exe search -t topicToSearch -i data.xml
```

#### Most Active User

```sh
.\ilovexml.exe most_active -i data.xml
```

#### Most Influential User

```sh
.\ilovexml.exe most_influencer -i data.xml
```

#### Suggest Friends

```sh
.\ilovexml.exe suggest -i data.xml -id 123
```

---

### Important Notes

- Your **XML files must be inside the `build` folder** to run CLI mode easily.
- You must build the project **at least once** so that `ilovexml.exe` exists.
- GUI and CLI both use the **same executable**.

---

## Team Members

| Name | GitHub Account |
| :--- | :--- |
| **Amr Ahmed** | [amr10w](https://github.com/amr10w) |
| **Ali Ahmed** | [aliabdelmaboud](https://github.com/aliabdelmaboud) |
| **Mohamed Elsayed** | [ZICKMO](https://github.com/ZICKMO) |
| **Omar Mohamed** | [Omarzcode](https://github.com/Omarzcode) |
| **Ammar Mohamed** | [ammar-abdelghany30](https://github.com/ammar-abdelghany30) |
| **Hamza Basem** | [Hamza-Eldefrawy](https://github.com/Hamza-Eldefrawy) |
| **Anas Ayman** | [Anas-Elsab3](https://github.com/Anas-Elsab3) |
| **Ahmed Ezzat** | [Labolabo-1](https://github.com/Labolabo-1) |
| **Ahmed Ramadan** | [ahmedramadan2025](https://github.com/ahmedramadan2025) |

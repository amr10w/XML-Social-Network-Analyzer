# XML-Social-Network-Analyzer
XML Social Network Analyzer is a desktop application that parses, analyzes, and visualizes social network data stored in XML files. It supports operations like XML validation, formatting, minifying, JSON conversion, and compression. The tool also represents user relationships as a graph and provides network analysis features such as identifying influencers, mutual followers, and suggesting connections.

# Demo Video
https://github.com/user-attachments/assets/5e80f843-2166-46a0-9cb8-70f1f0594514
# How to Build & Run the Project 

This guide explains how to set up, build, and run the **XML Social Network Analyzer** using **VS Code**, **Qt 6**, and **CMake**.

---

## **Required Tools (Install Before Running)**

|Tool|Purpose|Download Link|
|---|---|---|
|**Qt 6+**|GUI framework|[https://www.qt.io/](https://www.qt.io/)|
|**CMake**|Build system|[https://cmake.org/download/](https://cmake.org/download/)|
|**VS Code Extensions**|Required inside VS Code|- _Qt Tools_  <br>- _CMake Tools_|

---

## **Clone the Repository**

Open terminal and run:
```sh
git clone https://github.com/amr10w/XML-Social-Network-Analyzer.git
```


Then open the repository folder in **VS Code**.

---

## **Step 1 — Register Qt in VS Code**

1. Press **Ctrl + Shift + P**
2. Search: **Qt: Register Qt Installation**
3. Choose the Qt folder you installed (for example):

```sh
D:\Qt
```

- This allows the VS Code Qt extension to detect `qmake`, Qt libraries, and tools.

---

##  **Step 2 — Configure CMake Tools**

1. Press **Ctrl + Shift + P**
2. Search: **CMake: Quick Start**  
	(only the first time to let CMake configure the project)
3. Press **Ctrl + Shift + P** again
4. Search: **CMake: Select a Kit**
5. Choose the kit that corresponds to **Qt 6** (e.g., _minjw or _MSVC + Qt_ or _GCC + Qt_).

---

## **Step 3 — Build & Run the Project**

### ▶️ **Run in GUI Mode (Normal Application Run)**

Try:
``` sh
Ctrl + Shift + F5
```


If this works, the GUI will start directly.

### If Ctrl+Shift+F5 doesn’t work:

You can:

### Option A — Build using CMake Tools

1. Click **Build** (bottom status bar in VS Code)
2. When build finishes, you can run the target from VS Code.

### Option B — Create a debug configuration

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

# Running in Command Line Mode

After the first GUI run, your build folder will contain:

```sh
xml_editor.exe
```

To run CLI mode:

```sh
cd .\build\ .\xml_editor.exe <command>
```

###  **Current Implemented Features**

#### Validate XML

```sh
.\xml_editor.exe verify -i test.xml
```

#### Minify XML

```sh
.\xml_editor.exe mini -i in.xml -o out.xml
```

#### Format XML

```
.\xml_editor.exe format -i input.xml -o formatted.xml
```


#### Compress XML

```sh
.\xml_editor.exe compress -i sample.xml -o sample.comp
```

#### Decompress XML

```sh
.\xml_editor.exe decompress -i sample.comp -o sample_decompressed.xml
```

---

## Important Notes

- Your **XML files must be inside the `build` folder** to run CLI mode easily.
- You must build the project **at least once** so that `xml_editor.exe` exists.
- GUI and CLI both use the **same executable**.

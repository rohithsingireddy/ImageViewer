# Image Viewer

An application that opens images and allows user to scribble on them using Gtkmm library.

![A screenshot of application](/misc/Screenshot-1.png)

## Compiling
In VSCode, pressing `F5` on *main.cpp* would compile an executable named **ImageViewer**.

In other environments, an executable can be compiled by executing the following commands in a shell under the directory where *main.cpp* is present after cloning.

```bash
make
```

To clean compiled binaries
```bash
make clean
```

## Notes
- To create a resources.c file from ui_resources/imageViewer.gresources.xml
```bash
cd ./ui_resources/
glib-compile-resources --target=resources.c --generate-source imageViewer.gresources.xml
```
**or**
```bash
make ui_resources.c
```
- Resources file should be generated whenever a file in **ui_resources** is changed.
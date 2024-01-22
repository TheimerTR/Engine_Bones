# Bones Engine 

[![EngineBones - Download](https://img.shields.io/static/v1?label=EngineBones&message=Download&color=green&logo=github)](https://github.com/TheimerTR/Engine_Bones/releases/download/V.3/Engine_Bones_v.3.zip) 

## About the Engine

Engine Bones is a 3D Game Engine developed as a part of the Videogame Engine subject by Alberto Hidalgo García and Sonia Cristina Ojeda Lanz, students at CITM (UPC) University during 3rd year of Game Design and Game Development degree. 

![image](https://raw.githubusercontent.com/TheimerTR/Engine_Bones/main/docs/images/imageEngine.png)

## Team Members
<img src="https://raw.githubusercontent.com/TheimerTR/Engine_Bones/main/docs/images/FotoMotores.jpg" width=50% height=50%>

**Alberto Hidalgo García**

* Game Objects 
* Hierarchy and Inspectors
* Resource Manager
* Component Mesh 
* Component Material 
* Drag & Drop 
* Component UI 
* Mouse Picking
* Asset Explorer
* Log Console
* Pause & Play 

**Sonia Cristina Ojeda Lanz**

* Game Objects
* Camera Component
* Transform Component
* Frustum and Frustum Culling 
* Viewports 
* Guizmos 
* AABB & OOBB
* Drag UI Components 

## Core Sub-Systems 

*  ### GameObjects & Component System 

Every entity is a GameObject which can contain components, and every component has a different functionality but derives from the same class. All GameObjects must have at least a transform component.  

* ### Resource Manager 

The Resource Manager system creates personalized documents (CFF), which are used to save the resource information for a faster loading the next time the engine is opened. Being useful to save and load resources from memory if needed again and as a result saving memory. 

* ### Cameras 

The Engine has two types of cameras, the editor camera and the game camera, being each one in a different viewport rendering at the same time. There is also the possibility of creating more than one game camera and changing the one that is active in the viewport. 

* ### Camera Culling 
For the optimization of the rendering time it was implemented frustum culling and AABB boxes to discard all the meshes outside the game camera view. 

* ### Time Management 

Makes it possible to start, stop and pause a simulation. 

## UI Sub-System 

Engine Bones has an UI implementation based on the Unity system that uses a canvas to place the UI elements. It allows the creation of customizable buttons, input texts, checkers, text and images, which can be moved and rescaled with the guizmos in the editor window and with the drag function in the game window. 

![gif1](https://raw.githubusercontent.com/TheimerTR/Engine_Bones/main/docs/images/gif1.gif)
![gif2](https://raw.githubusercontent.com/TheimerTR/Engine_Bones/main/docs/images/gif2.gif)

## Showcase

<iframe width="560" height="315" src="https://www.youtube.com/embed/Xpz1nTnS7ug?si=61FDZjZHhC6U3s2G" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" allowfullscreen></iframe>

## License

MIT License

Copyright (c) 2023 TheimerTR & SoniaOL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.








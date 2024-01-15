# Bones Engine 

## About the Engine

Engine Bones is a 3D Game Engine developed as a part of the Videogame Engine subject by Alberto Hidalgo García and Sonia Cristina Ojeda Lanz, students at CITM (UPC) University during 3rd year of Game Design and Game Development degree. 

## Team Members

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

Engine Bones has a UI implementation based on the Unity system that uses a canvas to place the UI elements. It allows the creation of customizable buttons, input texts, chechers, text and images, which can be moved and rescaled with the guizmos in the editor window and with a drag in the game window. 

![gif1](https://github.com/TheimerTR/Engine_Bones/assets/99950351/9bcd8783-e945-4b05-a592-0a4145fab40a)
![gif2](https://github.com/TheimerTR/Engine_Bones/assets/99950351/4081de75-906f-45fb-a45f-f992880613f6)




# rasterizer
A remake of a software rasterizer from 2019. Main difference is that this one uses a tile-based technique (single tile) instead of scanlines. 
It outputs directly to a Win32 framebuffer. No optimizations have been done!

## Features
* Vertex shader stage
* Perspective correct interpolation
* Depth buffer and depth testing
* Pixel shader stage
* OpenGL/DirectX styled API (agnostic)
* Emulation of vertex/index/uniform buffers
* Texture sampling
* Render output and scaling using `StretchDIBits`
* Mesh loading using custom binary format
* Basic shader with point lights

## Setup
Generate project files with [Premake 5](https://premake.github.io/docs/Using-Premake), such as:
```
λ premake5 vs2022
```
or use the included build script (requires Premake and Visual Studio 2019/Build Tools):
```
λ build {release|debug|clean}
```

![rasterizer-gif](https://user-images.githubusercontent.com/3429723/201184780-42154d0a-913b-48df-b6fa-1437dfa0bcb4.gif)

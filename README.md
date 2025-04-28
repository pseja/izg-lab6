# izg-lab6

## What should you demonstrate in the exercise?
- The ability to write simple vertex and fragment shaders that render something nice and understand the display chain.

Information sources: [GLSL specification](https://registry.khronos.org/OpenGL/specs/gl/GLSLangSpec.4.40.pdf)

## What is the challenge?
You will be given 10,000 triangles, time, view and projection matrix and the task of creating some 2D/3D objects that animate - all in vertex and fragment shaders.

## Minimum knowledge and skills required:
1. How the view chain works
    - **input:** model (vector graphics)
    - vector part (vertex shader - ) -> rasterizer -> raster part (fragment shader)
    - **output:** frame buffer (raster graphics)

2. What is a vertex shader
    - part of the GPU chip
    - takes care of vertex assembly, primitive assembly, culling, clipping, perspective division, view port transormations

3. What is a fragment shader
    - part of the GPU chip
    - takes care of 

4. How to send data between VS and FS (in, out)
    - for example by using (this uses barycentric interpolation):
        - using `out` in the vertex shader
        - using `in` in the fragment shader

5. What are uniform variables (uniform)
    - global variables from the cpp file that can be used in the fragment shader and vertex shader and cannot be modified, just used

6. What are vertex attributes
    - provided by applications though vertex buffers
    - vertex positions (gl_Position)
    - colors
    - texutres

7. What is texturing
    - mapping of 2D image onto 3D geometry

8. What are texturing coordinates (uv)
    - 2D coordinates that map each verte to a position in the texture space

9. **Mathematics (functions, matrices, linear algebra, ...)**


If you want to prepare for the exercise ahead of time, familiarize yourself with the GLSL language and write something in it. You can in the skeleton of the exercise.


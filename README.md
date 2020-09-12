# nms
# Yet another Opengl Game Engine.

1. Originally started as a project to learn OpenGL.
2. Uses a Movie Based paradigm (Movie/Scenes/Actors)
3. Based on modern C++/OpenGL 3.3/Windows (Visual Studio 2017)/GLFW
4. Is a work in progress


## Third party libs:
 - GLFW
 - glad
 - glm
 - nlohmann json
 - FreeType

 ## Features Implemented:
  - String Billboards (fixed and variable sized)
  - Font Atlas Strings
  - Run Loop (fixed timestep updates/variable time step Render) based on [Fix Your Timestep](https://gafferongames.com/post/fix_your_timestep/) article by Glen Fiedler
  - Resource Factory Caching
  - Inheritance Based Scene Heirachy
  - Decentralised Scene Switching Logic
  - Mouse/Keyboard callback Messaging
  - User Input Abstraction
  - Base classes are used to hide many of the above details.

## This is still a learning activity and there are many things to do. This includes (but not limited to):
 - Quaternian based Camera.
 - Transparent Safe and Restore
 - Transparent Macro Recording and Playback
 - Efficient Rendering mechanism
 - Boilerplate Key mapping User Interface
 - Linux Port
 - Many implementation details of efficient OpenGL
 
 ## Stretch Goals
 Note that implementation details of the stretch goals may change).
  - Scalable Multi-Threading design.  This paper by Michael Silverman [Scalable Multithreaded Game Engine Using Transactional Memory](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwiKhIyAqOLrAhVfwTgGHToxBn4QFjAAegQIBRAB&url=https%3A%2F%2Furresearch.rochester.edu%2FfileDownloadForInstitutionalItem.action%3FitemId%3D5752%26itemFileId%3D8929&usg=AOvVaw3gbYfh3JPgmmlN55aJSodX) looks promising
  - Scalable Collision Detection. [Collision Detection using Ellipsoids](http://www.peroxide.dk/papers/collision/collision.pdf)



 



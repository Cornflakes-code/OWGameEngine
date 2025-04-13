# nms
# Yet another Opengl Game Engine.
OWGameEngine began as a project to learn OpenGL and it is still performing that role. At it's heart it is a simple and understandable set of classes which can be inherited off to leverage core functionality. The design of the main Game Loop lends itself to efficient multi threading (hopefully]). The engine:
 - Focusses on the higher level objects. 
 - Uses a Movie Based paradigm (Movie/Scenes/Actors/Scenery)
 - Is based on modern C++/OpenGL 4.5/Windows (Visual Studio 2017)/GLFW/GLM
 - The nature of the main game loop favours multi-threading
 - Is a work in progress

## The functionality currently existing or 'will be completed soon' includes:
 - ResourceManager. Singleton resource cache
 - Uses fixed timestep with 'catch up' integration prior to rendering
 - UserInput. Wraps physical implementation of Keyboard and pointing device and emits logical commands via callbacks
 - Scene. Examples of a scene include Main (where all the gameplay is), Welcome screen or Key mapping screen. Provides access to the Render functionality. Apart from setup code is pretty empty. 
 - Movie. Provides a fixed timestep physics step and a variable time step render step inside a Game Loop. Swaps Scenes in and out
 - SafeAndRestore. Like the name says.
 - MacroRecorder. Provides the ability to replay game events. Must be deterministic. Only possible with fixed physics timesteps.
 - Camera
 
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
  - Resource Caching
  - Inheritance Based Scene Heirachy
  - Decentralised Scene Switching Logic
  - Mouse/Keyboard callback Messaging
  - User Input Abstraction
  - A mongrelled Actor/Component Architecture (hopefully best of both worlds)
  - No smart pointers (Ownership is striclty defined)


## This is still a learning activity and there are many things to do including (but not limited to):
 - Quaternian based Camera.
 - Transparent Safe and Restore
 - Transparent Macro Recording and Playback
 - Efficient Rendering mechanism
 - Boilerplate Key Mapping Scene
 - Linux Port
 - Many implementation details of efficient OpenGL
 - CMake functionality
 - Separation of core functionality and learning game into different repos.
 - Addition os third party libraries as git submodules
 
 ## Stretch Goals
  - Scalable Multi-Threading design.  This paper by Michael Silverman [Scalable Multithreaded Game Engine Using Transactional Memory](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwiKhIyAqOLrAhVfwTgGHToxBn4QFjAAegQIBRAB&url=https%3A%2F%2Furresearch.rochester.edu%2FfileDownloadForInstitutionalItem.action%3FitemId%3D5752%26itemFileId%3D8929&usg=AOvVaw3gbYfh3JPgmmlN55aJSodX) looks promising.
  - Scalable Collision Detection. This looks good [Collision Detection using Ellipsoids](http://www.peroxide.dk/papers/collision/collision.pdf)
 - Steam Virtual Reality



 



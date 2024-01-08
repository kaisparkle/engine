# Yet Another Engine Project
This project is a continuation of my prior renderer projects in the [opengl](https://github.com/kaisparkle/opengl) and [vulkan](https://github.com/kaisparkle/vulkan) repositories, in an attempt to re-architect them into something approaching a proper game engine.

https://streamable.com/j7di0w

## Status
The codebase is currently in a pretty rough state -- a lot of memory management work needs to be done, as well as shifting over to proper modern C++ techniques. Additionally, not much consideration has been given to performance yet -- there's a number of `std::maps` in use, and the current entity-component model isn't ideal. However, there's a rough architecture in place to build on.

### Architecture
The general architecture is a bit rough - the `Engine` class does basically all of the heavy lifting, and it holds and interacts with basically everything. Pretty much everything (e.g. the renderer, physics manager, entity manager, etc) is a singleton, which probably isn't ideal -- although I don't anticipate ever needing more than one instance of those. In the future, the "engine" should be shifted out to a library, with a separate Game or Editor application doing the work of initializing, ticking, and cleaning up.

### Rendering
Currently, only an OpenGL implementation is in place, with only diffuse textures supported. The intent is to keep the renderer relatively API-agnostic, with backends for OpenGL, and later Vulkan.

### Scene
A basic entity-component model is in place -- however, this is not a proper Entity Component System, as a lot of functionality is included in the components, rather than pure data. The latter would be more performant, and I'd like to move over to that in the future. Additionally, there isn't any scene serialization for saving/loading yet -- I'd like to work on that in the future, once things are in a more solid state.

### Editor
The current editor allows for browsing the entity list and editing their transforms, with a Dear ImGui docking interface. The editor is built-in to the binary at the moment - ideally, the engine itself would be moved to a library, with the editor being a separate application.

### Physics
Basic integration with the [Jolt Physics](https://github.com/jrouwe/JoltPhysics) library is in place, including a `Rigidbody` component that holds multiple `Colliders`. Additionally, a very basic rigidbody character controller is in place. The current architecture is fairly okay -- the `Rigidbody` component doesn't interact directly with Jolt, which is nice. However, I'd like to re-evaluate how `Colliders` are created, as well as how they are managed and stored by the physics manager.

## Goals
* Shift the actual "engine" over to a library, or multiple libraries, and have a separate application driving everything.
* Move over to proper modern C++ techniques -- namely, quit using raw pointers all over the place.
* Move to a proper ECS, rather than having Entity and Component classes that bundle a lot of functionality alongside data.
* Figure out a more optimal way to keep the renderer API-agnostic -- at the moment, anything that needs API-specific objects (textures, meshes, framebuffers, etc) are just given a void pointer, which is set to point to an API-specific object structure. Feels pretty gross, and would require a lot of maintenance to e.g. switch APIs mid-execution.
* Get a proper material system in place, rather than just using `Texture` and `Shader` objects. `Materials` should probably be an `Asset`, which hold a series of textures and a shader, so that they can all be bound in one shot at render time.
* Figure out a better way to manage importing assets. Currently, [Assimp](https://github.com/assimp/assimp) is being used for this, which is a pretty large library with a lot of un-needed functionality. Ideally, I'd standardize on specific formats, e.g. glTF and KTX.
* Work out how to serialize scenes, in order to support saving and loading. This will probably come pretty far down the line, once other things are in a more solid state.
* Get physically-based rendering going again, with proper management of lighting and shadow passes.

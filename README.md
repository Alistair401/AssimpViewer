This repository is intended to showcase my work as I go about making a 3D game using C++ and OpenGL.
I will update this readme as a kind of blog to show progress and maybe help others.

## Starting the project - 6th December 2017

I have 2 main goals to achieve with this project:
1) show that I understand a 3D game engine
2) show that I have a good enough understanding of C++ and rendering methods to use OpenGL without a higher-level wrapper library

Rather than coming up with the idea for a game, and then programming the engine for it, I will program the engine, and then come up with a simple game to wrap it all together.

I will be using the GLFW and GLEW libaries.

First milestones will be to display a window, and render a 3D object to it

## Rendering a 3D object - 19th December 2017

First milestone achieved - rendered a simple 3D object using OpenGl, textures and simple shaders.

I used the resources found here: https://learnopengl.com/ while ever so slightly out of date, they are very helpful and explain concepts well,
however it feels a bit like I'm copy pasting code (probably because I don't know the API well enough).

From now on I will be using the concepts talked about here: http://preshing.com/20171218/how-to-write-your-own-cpp-game-engine/ and only referring to the previous link for reference, not as a guide. 

The next milestone will be to load 3D objects from a standard file format.

## Loading objects from a standard file format - 23rd December 2017

There was a lot of work to do inbetween the last milestone and this one. Hardly any code remains from the last milestone to this one. I chose to load .obj files first, they're simple and there were some free assets I found here: http://kenney.nl/assets/holiday-kit that happened to be in .obj format (fair warning: they're christmas themed).

I found this API: https://github.com/syoyo/tinyobjloader which meant I could 'easily' load .obj files into OpenGL compatible vertex arrays. However, the example that comes with the API is a bit of a mess, using old code and loads of bad practices in OpenGL. I re-wrote the loading function, implemented classes to hold buffer data and attempted to draw some things. Initially all I could see was the corner of each model, and as I wasn't using any MVP transformations, it was lucky I was able to see anything at all. 

At this point I decided I'd written too much code (for the previous milestone) that I didn't understand. Again, luckily, I found this incredible course on YouTube https://www.youtube.com/playlist?list=PLRwVmtr-pp06qT6ckboaOhnm9FxmzHpbY. His explantions are amazing, simple and to the point, yet including enough context that there isn't any confusion with each concept.

Re-wrote how objects are loaded and drawn. Re-wrote how vertex attributes are set and sent to shaders. Wrote new shaders (they're basically just pass-throughs for now). Added Model and Projection transform matrices. After fixing a few little bugs and adding some keyboard controls to modify transforms, it all worked as intended.

Haven't yet decided the next milestone, maybe animation, probably some ability to move objects around the world space. We shall see.

## Starting again and skeletal animation - 5th April 2018

New repository, fresh start. 

Since December, I've had to work thoroughly on my university dissertation. Luckily, it was in C++ and used OpenGL compute shaders so I learnt a lot along the way. Coming back to this project I realised the code was extremely messy and hacky. Rather than attempting to fix the mess I had created, I decided it would be a better learning exercise to start from scratch and see what I could improve.

Rather than implementing texture loading like I had before, I chose to implement skeletal animation first as this was a bigger challenge. Assimp's way of importing animations seems unnecessarily janky to me, with inconsistent language in the documentation. Channels in an animation reference a node in the model hierarchy which reference a bone in a mesh... why not just reference a bone directly? And why is everything referenced by string comparison rather than by indexing? Maybe I don't understand enough about the animation process to see why this is useful.

Thanks to this video: https://www.youtube.com/watch?v=fDmMH8_WRok I was able to get my head around what I needed from Assimp to recreate skeletal animations. After using compute shaders in my university work, I also understood enough to pass bone transformations to OpenGL to compute GPU side. Animations now work, although could be further optimised.

Other resources I had bookmarked: \
https://gamedev.stackexchange.com/q/26382 \
https://sourceforge.net/p/assimp/discussion/817654/thread/a7bf155b/#6168

University exams are up next, so there likely won't be any progress past animation until June.

## Architecture - 7th July 2018

Getting skeletal animation working felt like a triumph. However, currently the architecture of the application is a mess of tightly coupled and monolithic classes. 

My current goals is to decouple functionality from data in the following places:

#### Rendering
Rendering functionality and OpenGL calls should not be inside the `Model` class. A rendering function should look a bit like this:

```c++
void Render(Model m);
```

This separates the rendering implementation from the model implementation, meaning if I were to switch out OpenGL with some other API, it wouldn't require any changes to the `Model` class. 

#### Skeletal Animation

In it's current form, the application treats all models as having a skeleton and animations. Obviously this shouldn't be the case.

Different types of model should extend a base `Model` class. This allows the renderer to ignore what type of model it is rendering. It also means that an animation system doesn't have to keep checking if a model has a skeleton and animations or not, it can act on just the models that matter.

#### Pose Calculation

Currently, all skeletal animation is done from within the `Model` class. This means that any change to the implementation of a `Model` is likely to affect how poses are calculated. 

Decoupling this functionality from the model would require something along the lines of:

```c++
Pose EvaluateAnimation(Animation a, Time t);
```

This is challenging as it requires some pretty integral changes to the way animations and bones are handled. Once implemented, this would help enourmously in writing a more extensible animation system.

I plan to allow for animation blending which is only possible if animation is abstracted in this way so that functions like this can then be implemented:

```c++
Pose Blend(Pose a, Pose b, float blend_factor);
```

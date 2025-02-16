# Advanced-Ray-Tracing

This repository is developed under the course [CENG795 SPECIAL TOPICS: ADVANCED RAY TRACING
](https://catalog.metu.edu.tr/course.php?prog=571&course_code=5710795). In addition to the project, we had weekly reports that explains the progress troughout the semester. You can read the blog posts in [Medium](https://medium.com/me/stories/public). Below are some of the examples that was generated via my Ray Tracer: 


<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*1q-CyI3O1BvuFU8q2QC49A.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*3h2NNuNNIzyVGzYirgSE_g.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:602/format:webp/1*f8CI1Drc-eQiQn7-PrhggQ.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*WlqvpigyU8zKo6eC0utkIQ.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:512/format:webp/1*eQc-m6wfh6IfWeTJQw9Z9Q.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*o7ajppYk2S2QgH2fkaIYbA.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*z1l0g8uymEri1zP5ubf4FQ.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*9g88vbhydfgMw9Ilif40nQ.png" alt="Example Ray Tracer Output" width="200" height="200">                                                                                                                                 
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*Q8_KUo5t4rA40czybDA-DQ.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:530/format:webp/1*m--cWFc9uT_IVZoQ7fOOiA.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*UOwwTWt3flzYX4NW_mRMMg.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*I6L69aQM9bf5TZyd4yx4qQ.png" alt="Example Ray Tracer Output" width="200" height="200"/>
<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*-IVfWXAKsSETB4o2LnxtGw.png" alt="Example Ray Tracer Output" width="200" height="200"/>


Project is fully developed with C++ from scratch. Only a few external libraries have been utilized such as lodepng, tinyexr, tinyxml etc. However they are not directly related to ray tracing logic of the project. They are used for reading images from disk, parsing xml scene files, writing exr files etc. 

## Setup

You only need g++ in order to build the whole project. 

```
mkdir build
cd build
cmake ..
make -j 12
``` 

This will build the project and create a runnable in with the name `raytracer`. Normally we run raytracers with relevant xml files as arguments. However, because all our scenes are private under the course I am not able to share one for now. If you are interested, you can get the initial scene for the game `Middle Earth Chess` which is a turn based game based on my ray tracer. Please have a look at branch `project` for it. You will find xml file [here](https://github.com/alperbahcekapili/Advanced-Ray-Tracing/blob/project/scene/main.xml). Do not forget to copy the ply and textures folders as well. 



## Middle Earth Chess

Middle earth chess is a turn based, ray tracer only game. There are 3 types of troops that you can place. Each of them has different abilities. Take a look at the presentation that explains each of the characters [here](https://docs.google.com/presentation/d/1Nho_AfJLBYVdC-WbvryTgg9T5vtreVBbY4fZoUfT79w/edit?usp=sharing). Whole code of the game is on the branch `project`. You simply run the raytracer as well as python program and they interact with each other trough named pipes. Python side is responsible for the overall logic of the game. Keeps track of the player's moves, positions of the troops on the board etc. Ray tracer part only updates the locations and other properties of objects. Following is a screenshot of the game. You can also look at the gameplay [here](https://youtu.be/4IB_GjR4eR0).  

<img src="https://miro.medium.com/v2/resize:fit:640/format:webp/1*eFQA0qLcCxFZszen3HWBXA.png" alt="Example Ray Tracer Output" width="500" height="500"/>



## Summary

I believe it was a real hastle for a semester to build up a ray tracer from ground up. I am really proud of my work here. There are lot's of room for improvement in sense of code quality. Apart from that it was a fullfilling journey that thought me a lot. I also have written a blog post for my project. You can read it [here](https://medium.com/@alperbah.13/middle-earth-chess-5fb527f2a646)





# The Coding Train Exercises

This is a collection of exercises proposed on The Coding Train Youtube channel (https://www.youtube.com/user/shiffman) but with a catch: they're implemented in C++ as UWP applications.

# How To Work On The Code

You need at least Windows 10 version 2004 and Visual Studio 2022 Preview 3. Earlier versions of Windows and Visual Studio might work but they haven't been tested. To run the exercises you need to open up *The Coding Train Exercises.sln* solution, follow any prompts that might appear to install required dependencies and to enable Developer Mode and then right click on the project you're interested in and select *Debug* -> *Start New Instance*. This will build the project, deploy it on the local machine and then run the newly created package.

# How To Work On The Visual Assets

You need at least Inkscape 1.1 and Visual Studio 2022 Preview 3. Earlier versions of Inkscape and Visual Studio might work but they haven't been tested. Each exercise has in its *Assets* directory a file with a name in the form *\<ExerciseName\>IconSource.svg*. It is this file that we work on when creating/modifying the visual assets. Each SVG file is then saved as a PDF file in the same directory with the name in the form *\<ExerciseName\>IconSource.pdf* using Inkscape's *File* -> *Save As...* functionality with the default options provided by the application. Then, in Visual Studio, open up the *Package.appxmanifest* of the project you're interested in, go to the *Visual Assets* tab, import the PDF file in the *Source* field and click on *Generate*. This will create all the necessary visual assets used by the projects. Note that Visual Studio might require you to follow some prompts to install required dependencies and to enable Developer Mode before you'll be able to access the package manifest designer.

# The Exercises

Each exercise is a self-contained project as it doesn't depend on other projects. It can be built and run independently.

## Zoog

This exercise draws an approximation of the alien called Zoog introduced at the end of https://www.youtube.com/watch?v=n2oHuKG_BQc using the Windows.UI.Composition API.

![Screenshot of the Zoog application.](ReadMeAssets/zoog_screenshot.png)

## MouseTracker

This exercise draws a line between the current and previous mouse position using Direct2D. The line's thickness changes according to the speed of the mouse. The line's color gets more red as the mouse moves to the right and more blue as the mouse moves downwards. The exercise was proposed at the end of https://www.youtube.com/watch?v=ibW4oA7-n8I.

![Screenshot of the MouseTracker application.](ReadMeAssets/mouse_tracker_screenshot.png)

## CircleMorphing

This exercise animates the color and size of a circle using key frame animations provided by Windows.UI.Composition. The proposal to animate something was stated in https://www.youtube.com/watch?v=rZ36BzXFT6Q.

![Screenshot of the CircleMorphing application.](ReadMeAssets/circle_morphing_screenshot.png)

## PointillistPainting

This exercise draws circles using Direct2D in a random position, having a randomly sized radius and a randomly generated color. The idea to create a pointillist painting based on output given by a random number generator was expressed towards the end of https://www.youtube.com/watch?v=50Rzvxvi8D0.

![Screenshot of the PointillistPainting application.](ReadMeAssets/pointillist_painting_screenshot.png)

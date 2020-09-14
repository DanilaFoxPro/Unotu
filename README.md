# Unotu
Memory augmentation software for storing, sorting and searching information.

## Concept

### The problem
I've frequently ran into problems with failing to organize information in a satisfactory manner. Be it sorting music, videos, or links. Tags seem like a good idea, but I never quite like the implementation.

### Idea
So why not try myself? I wanted to learn to do graphic stuff for a while, only making console applications before that. I could make my own GUI engine based on OpenGL and GLFW, and use notes for storing information.

### The point
The main focus is easy but advanced information access. Current ultimate idea is to use tags, but tags that have values. And implement a GUI interface for creating your own search filters.

### Example
Say you want to sort videos. You create a note for each video, and add relevant tags to it. Like: `video,cool,duration=190,author=SomeDude`. Then you can create a search filter for videos, so it only shows notes that have `video` tag and sorts them by the value of `duration` tag.

### Note
This is my current vision of the ideal implementation. It might change in the future, and most likely will. It also isn't throughly defined.

## Progress
Current goal:
 - Separate the Unotu project and the GUI engine. Perhaps call it UnotuGUI.
 - Polish and fix bugs.
 - Improve usability.

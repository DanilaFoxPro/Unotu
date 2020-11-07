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
 - Tags and search filters.
 
## Current control scheme
 
### Card browser
```
 - `Left Mouse Button`  - Edit a card.
 - `Right Mouse Button` - Remove a card.
```
### Text fields
```
 - `Left Mouse Button`  - Focus / Set caret position / Drag to select.
 - `CTRL`+`C`           - Copy selected text.
 - `CTRL`+`V`           - Paste on caret location.
 - `Backspace`          - Backspace.
 - `Delete`             - Delete.
 - `CTRL`+`E`           - Swap caret between start and end of the selection.
 - `Home`               - Move caret to the line beginning.
 - `End`                - Move caret to the line end.
 - `CTRL`+`Home`        - Move caret to the very beginning.
 - `CTRL`+`End`         - Move caret to the very end.
```

# On the off chance you're reading this message (10/31/22) I am currently editing this readmer and recording some gifs to show examples of what I've done in this project. If you want to see the full extent of what I've done, come back in a few hours, however if you want to get a sneak peek at some of the things I've done, keep reading and refresh often :)


I should preface this document by saying all of the assets in here are definitely placeholder assets (as if it wasn't obvious just looking at them haha). I wanted to focus more on the actual technical programming for framework of this project before moving on to the art/look and feel. Most of the art is in a "good enough" state for testing purposes until I find the time to work on the visual side.

Let's start from the beginning - what does the player experience when opening the game?
Upon startup, the user is greeted with a main menu level that has a few options along the left (play, practice, settings, and exit) along with their profile in the top left corner. Their character is standing on the right side in one of a few poses with a procedural breathing animation to bring a little bit of life to the screen. There's also music playing in the background but, of course, you can't hear that in the gif.
![Menu Gif](Gifs/MainMenu.mp4)

When the player clicks play, the background blurs and a server code box pops up. The game modes run fully on dedicated authoritative servers. The server code is also a for-testing feature until I get full matchmaking up and running with a web api. It's almost there, but I still have to solve a few problems since I'm building from the ground up.

Because the game runs on dedicated servers, everything you see is *fully* replicated. Once the player joins the match, they're (randomly) given one of 4 movement abilities and a couple of weapons.  

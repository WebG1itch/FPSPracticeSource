# On the off chance you're reading this message (10/31/22) I am currently editing this readmer and recording some gifs to show examples of what I've done in this project. If you want to see the full extent of what I've done, come back in a few hours, however if you want to get a sneak peek at some of the things I've done, keep reading and refresh often :)


I should preface this document by saying all of the assets in here are definitely placeholder assets (as if it wasn't obvious just looking at them haha). I wanted to focus more on the actual technical programming for framework of this project before moving on to the art/look and feel. Most of the art is in a "good enough" state for testing purposes until I find the time to work on the visual side.

Let's start from the beginning - what does the player experience when opening the game?
Upon startup, the user is greeted with a main menu level that has a few options along the left (play, practice, settings, and exit) along with their profile in the top left corner. Their character is standing on the right side in one of a few poses with a procedural breathing animation to bring a little bit of life to the screen. There's also music playing in the background but, of course, you can't hear that in the gif.

https://user-images.githubusercontent.com/72063265/199085986-9c71328b-4571-4c49-8c39-3acc617d76e5.mp4

When the player clicks play, the background blurs and a server code box pops up. The game modes run fully on dedicated authoritative servers. The server code is also a for-testing feature until I get full matchmaking up and running with a web api. It's almost there, but I still have to solve a few problems since I'm building from the ground up.

Because the game runs on dedicated servers, everything you see is *fully* replicated. Once the player joins the match, they're (randomly) given one of 4 movement abilities and a couple of weapons.  

# Abilities
All of the movement abilities have been added as special enum cases in Unreal's movement replication code to ensure that the replication of these movements is as smooth and efficient as possible.

## Grapple
The first ability is a grapple. This can stick to any surface and will pull you toward it. It works with momentum, so if you want to swing around a corner you absolutely can. The grapple will break if you swing past a certain angle from your start direction (90 degrees right now). The technical side: when you activate the ability, a projectile travels from your camera in the direction you're looking. This projectile sticks to the wall, and acts as a pivot point for your swing. The rope that's attached is currently just a cylinder that gets stretched and rotated along with you. Idealy this will be a particle system instead, but I needed something quick so it looked like an actual grapple rope.

https://user-images.githubusercontent.com/72063265/199092949-278ead64-71d5-49ba-bbbb-bcfedd260aeb.mp4

## Slide Jump
The next ability is slide jump. When the player slides, they can optionally jump right at the end of the slide. Doing so will activate the slide jump, which is essentially just a really big jump. This then activates a cooldown, so subsequent jumps will not send you flying through the air until the abilities cooldown has finished. In the following video this is demonstrated with the first jump being a slide jump, and the second jump being a regular jump after a slide because of the coodown.

https://user-images.githubusercontent.com/72063265/199095839-42bf45dc-55f2-43fd-86b5-bd402e83c462.mp4

## Blink
Third we have the blink. This ability 'dashes' the player in the direction they're moving. The dash is really fast, (almost a teleport) and is used to get out of tricky situations quickly or to confuse opponents and hit them from where they're not expecting. Because it blinks you in the direction you're *moving* you can move around without having to move your camera away from your target.

https://user-images.githubusercontent.com/72063265/199098607-c993b533-828b-40e5-afa2-428ba31d1044.mp4

## Combat Stimulant
Last and, unfortunately, least is the combat stim. I don't even really need a video demonstration because there's not much to demonstrate with this one. It was the first ability created to test how the movement replication code works. This ability simply increases the movement speed of the player, and will eventually be paired with visual indicators (like a different running animation, as well as first person effects) to make it clear the player is indeed sprinting with the combat stim. 

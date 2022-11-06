I should preface this document by saying all of the assets in here are definitely placeholder assets (as if it wasn't obvious just looking at them haha). I wanted to focus more on the actual technical programming for framework of this project before moving on to the art/look and feel. Most of the art is in a "good enough" state for testing purposes until I find the time to work on the visual side.

This project was built in Unreal Engine 5 prioritizing optimization, so almost everything added was done in C++ to keep runtime as efficient as possible.

### Sections:

[Intro](#intro)

[Juice](#juice)

[Abilities](#abilities)

[Documentation](#documentation)

# Intro

Let's start from the beginning - what does the player experience when opening the game?
Upon startup, the user is greeted with a main menu level that has a few options along the left (play, practice, settings, and exit) along with their profile in the top left corner. Their character is standing on the right side in one of a few poses with a procedural breathing animation to bring a little bit of life to the screen. There's also music playing in the background but, of course, you can't hear that in the gif.

https://user-images.githubusercontent.com/72063265/199085986-9c71328b-4571-4c49-8c39-3acc617d76e5.mp4

When the player clicks play, the background blurs and a server code box pops up. The game modes run fully on dedicated authoritative servers. The server code is also a for-testing feature until I get full matchmaking up and running with a web api. It's almost there, but I still have to solve a few problems since I'm building from the ground up.

Because the game runs on dedicated servers, everything you see is *fully* replicated. Once the player joins the match, they're (randomly) given one of 4 movement abilities and a couple of weapons - I'll get to that in a minute.

# Juice
First up is some of the juice I added. I was amazed at how much of a difference in overall game feel some of these things made. During the time I was working on the game, if I didn't have some interesting feature in mind that I wanted to add, I'd move to secondary goals which was usually juice like this. This group of features are not necessary to play the game, but boy do they make it feel much more like an actual game. This list of juicy features includes procedural animations (weapon sway, breathing, walking), match and team chat, killfeed (that will show if the kill was from a headshot or not), obstructable and bilboarded nametags, directional damage indicators, live scoreboard, and a hud (although that might be considered "required" to play the game). Below I have a few short videos demonstrating some of these features.

### Scoreboard + Killfeed:
https://user-images.githubusercontent.com/72063265/199111014-f8d6dd70-633c-45df-902d-aec4cd847fb7.mp4

### Breathing + Weapon Sway
https://user-images.githubusercontent.com/72063265/199112228-37289822-7134-494d-8ea3-7e7c6c34460b.mp4

### Directional Damage Indicators
https://user-images.githubusercontent.com/72063265/199112907-a820d8f2-0971-425b-a8d6-ca89698e4570.mp4

# Abilities
Everyone has the passive abilities to wall run and use a physics based slide while on the ground. All of the movement abilities have been added as special enum cases in Unreal's movement replication code to ensure that the replication of these movements is as smooth and efficient as possible. 

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

# Documentation
With every big project, solo or team based, good documentation is always a good idea. After writing important pieces of code, I made sure to add documentation around the functions and parameters so that it was easy to understand what's happening and where I left off. It had the added bonus of a better IDE workflow, because most modern IDE's will show documentatio of functions and classes (if done correctly) when you try to use them somewhere else. Unreal Engine's source has loads of great examples of well documented code, so I did my best to base my formatting off of that for consistency when working on the project. I've put an example from my custom movement component below

## MovementComponent
### [From MyCustomMovementComponent.h (Inventive name, I know)](https://github.com/WebG1itch/FPSPracticeSource/blob/main/Source/Impulse/Public/Character/Components/MyCharacterMovementComponent.h)
```c++
#pragma region Defaults

private:
	
	/** The maximum ground speed while running. Also determines maximum lateral speed when falling. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: Grounded", Meta = (AllowPrivateAccess = "true"))
	float DefaultMaxRunSpeed = 550.0f;

	/** The default maximum acceleration while running. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: Grounded", Meta = (AllowPrivateAccess = "true"))
	float DefaultMaxRunAcceleration = 10000.0f;

	/**
	 * Setting that affects movement control. Higher values allow faster changes in direction.
	 * If bUseSeparateBrakingFriction is false, also affects the ability to stop more quickly when braking (whenever Acceleration is zero), where it is multiplied by BrakingFrictionFactor.
	 * When braking, this property allows you to control how much friction is applied when moving across the ground, applying an opposing force that scales with current velocity.
	 * This can be used to simulate slippery surfaces such as ice or oil by changing the value (possibly based on the material pawn is standing on).
	 * @see BrakingDecelerationWalking, BrakingFriction, bUseSeparateBrakingFriction, BrakingFrictionFactor
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: Grounded", Meta = (AllowPrivateAccess = "true"))
	float DefaultGroundFriction = 5.f;

	/**
	 * Deceleration when walking and not applying acceleration. This is a constant opposing force that directly lowers velocity by a constant value.
	 * @see GroundFriction, MaxAcceleration
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: Grounded", Meta = (AllowPrivateAccess = "true"))
	float DefaultBrakingDecelerationWalking = 10000.f;

	/**
	 * Deceleration when walking and not applying acceleration. This is a constant opposing force that directly lowers velocity by a constant value.
	 * @see GroundFriction, MaxAcceleration
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: InAir", Meta = (AllowPrivateAccess = "true"))
	float DefaultGravityScale = 1.75f;

	/** Initial velocity (instantaneous vertical acceleration) when jumping. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: InAir", Meta = (AllowPrivateAccess = "true"))
	float DefaultJumpZVelocity = 550.f;

	/**
	 * When falling, amount of lateral movement control available to the character.
	 * 0 = no control, 1 = full control at max speed of MaxWalkSpeed.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: InAir", Meta = (AllowPrivateAccess = "true"))
	float DefaultAirControl = 0.2f;

	/**
	 * When falling, multiplier applied to AirControl when lateral velocity is less than AirControlBoostVelocityThreshold.
	 * Setting this to zero will disable air control boosting. Final result is clamped at 1.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: InAir", Meta = (AllowPrivateAccess = "true"))
	float DefaultAirControlBoostMultiplier = 0.f;

	/**
	 * When falling, if lateral velocity magnitude is less than this value, AirControl is multiplied by AirControlBoostMultiplier.
	 * Setting this to zero will disable air control boosting.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: InAir", Meta = (AllowPrivateAccess = "true"))
	float DefaultAirControlBoostVelocityThreshold = 0.f;

	/**
	 * Friction to apply to lateral air movement when falling.
	 * If bUseSeparateBrakingFriction is false, also affects the ability to stop more quickly when braking (whenever Acceleration is zero).
	 * @see BrakingFriction, bUseSeparateBrakingFriction
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Character Movement|Defaults: InAir", Meta = (AllowPrivateAccess = "true"))
	float DefaultFallingLateralFriction = 0.1f;

	/** The current maximum ground speed while running. Also determines maximum lateral speed when falling. */
	float CurrentMaxRunSpeed;

	/** The current maximum acceleration while running. */
	float CurrentMaxRunAcceleration;

public:

	/** Called on tick to determine if the camera should be tilted during movements. */
	void CameraTick() const;

#pragma endregion
```

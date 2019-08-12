Name: Teodor Linnik, 312728017
      Sagi Chafetz, 203867148
	  
Instructions:
Use the buttons "left" and "right" in order to change velocity direction to "left" or right".
Use the buttons "up" and "down" in order to change velocity direction to "up" or down", in other words
to make the snake to move upward to 3D direction.

Use the buttons "s"  to change view from the top to the snake point of view or vice versa.



The snake moves inside the boundaries, inside the boundaries there are blocks, walls (obstacles) which snake mast avoid,
if the snake collide with any block or any wall inside the boundary then you lost the game (collision detection). your target 
is you "eat" (collision detection) all the balls which are moving and located inside the boundaries 
and then the portal will be opened and you suppose to pass this portal in order to win the game.
	  
	  
	  
Explanations:
The snake consists from "head of snake", "tail of snake" and N "cylinders" between them.
On the top of file game.cpp there are macros: DEGREEMOVMENT , DEGREEMOVMENTY which you choose on which degree you turn by clicking 
"left" or "right" (DEGREEMOVMENT), "up" or "down" (DEGREEMOVMENTY).
In addition there are macros NUMBALLS which you decide how many balls you suppose to "eat" in order to
win the game and NUMBLOCKS which you choose how many blocks you are suppose to avoid to collide.

We used the SDL libraries in order to make a sound in case that snake "ate" any ball or collided.
It's done by the function playTune, the input of this function is path to specific sound file.

If snake switch the direction of velocity we implemented the linear skinning.

Switching the direction from the snake point of view based on rotation matrix on 3D which took into account
the direction angel "phi" and "theta" (https://en.wikipedia.org/wiki/Spherical_coordinate_system).
glm::mat3 rotX = glm::mat3(0.0);
rotX[0][0] = -cos(radAngel);
rotX[0][1] = sin(-radAngelY)*(sin(radAngel)) ;
rotX[1][0] = -sin(-radAngelY)*(sin(radAngel));  
rotX[2][0] = -sin(radAngel);
rotX[1][1] = -cos(-radAngelY);
rotX[1][2] = sin(-radAngelY);
rotX[2][1] = -sin(-radAngelY)*(cos(radAngel));
rotX[2][2] = -cos(-radAngelY)*(cos(radAngel));
rotX[0][2] = sin(radAngel)*(cos(-radAngelY));


If you are turning "up" or "down", that means that it's done on the angel "theta" that means that you are turning across 
the axis which is perpendicular to the vector on direction "phi", it's done by using.
globalSystemRot(angelYY, -glm::vec3(cos(radAngel), 0, sin(radAngel)), 6 /*zxAxis1*/);
...
case zxAxis1:
		if (mode == 0)
			theta = glm::rotate(theta, ang, rotVec);
		else
			theta = glm::rotate(glm::mat4(1), ang, rotVec) * theta;
		break;
	}
....



The base of snake movement is like on the link :https://www.youtube.com/watch?v=kZr8sR9Gwag
But on 3d and you can choose the angel of changing speed direction ("phi" and "theta").
Than means that for all elements which construct the snake the location of current element (on the current iteration)
is on the same place of location of successor element on the previous iteration, that mean that we did 
"Special snaky locomotion".

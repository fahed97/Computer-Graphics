Assignment 2b
=============

Student: *Fahed Nasser* 

## A look at the GUI:
On the left side, Light Controls. Choosing the active light, choosing the shading, the 3 light RGB components, transformations _(Translation& Rotation for point, Direction for parallel)_. On the right side we can see the Model Material - RGB.

## POINT LIGHT: 

#### Ambient Light
Ambient Light with Ambient Material only - as demonstrated in class, we can see that the model is still black.
![](../../master/Pictures_As2/PartB/p_source/Ambient_Only.png)

#### Ambient&Diffuse Light
![](../../master/Pictures_As2/PartB/p_source/Ambient_Diffuse.png)

#### Ambient, Diffuse&Specular Light
![](../../master/Pictures_As2/PartB/p_source/Ambient_Diffuse_Specular.png)


### Flat, Gouraud and Phong shading

#### Flat Shading:
![](../../master/Pictures_As2/PartB/p_source/Ambient_Diffuse_Specular_Flat.png)
2 light sources
![](../../master/Pictures_As2/PartB/p_source/2_Lights_Flat.png)

#### Gouraud Shading:
![](../../master/Pictures_As2/PartB/p_source/Ambient_Diffuse_Specular_Gouraud.png)
2 light sources
![](../../master/Pictures_As2/PartB/p_source/2_Lights_Gouraud.png)

#### Phong Shading:
The arrows demonstrate the slight difference between Gouraud and Phong. _(A more clear example is demonstrated later)_
![](../../master/Pictures_As2/PartB/p_source/Ambient_Diffuse_Specular_Phong.png)
2 light sources
![](../../master/Pictures_As2/PartB/p_source/2_Lights_Phong.png)

------------------------------------------------------------------------------------------------------------------------

## PARALLEL LIGHT: 
_the user can set the direction - the position of the drawn is only to demonstrate that there is a light source._

#### Flat Shading:
![](../../master/Pictures_As2/PartB/par_source/1_Flat.png)
Change of x,y,z direction (negative)
![](../../master/Pictures_As2/PartB/par_source/2_Flat.png)
Demonstration of x direction effect:

_Negative_
![](../../master/Pictures_As2/PartB/par_source/x_direction.png)
_Positive_
![](../../master/Pictures_As2/PartB/par_source/x_direction_2.png)



#### Gouraud Shading:
![](../../master/Pictures_As2/PartB/par_source/1_Gouraud.png)
Change of x,y,z direction (negative)
![](../../master/Pictures_As2/PartB/par_source/2_Gouraud.png)


#### Phong Shading:
The arrows demonstrate the slight difference between Gouraud and Phong. _(A more clear example is demonstrated later)_
![](../../master/Pictures_As2/PartB/par_source/1_Phong.png)
Change of x,y,z direction (negative)
![](../../master/Pictures_As2/PartB/par_source/2_Phong.png)

-------------------------------------------------------------------------------------------------------------------------
## Multiple light sources, with different types, and alpha specular effect:
The left light is parallel, the right is point:


### Flat Specular Alpha effect:
You can see that the white area does get smaller as we increase the alpha
Alpha 1          |  Alpha 5
:-------------------------:|:-------------------------:
![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Flat_SpecularAlpha.png)  |  ![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Flat_SpecularAlphaLess.png )

### Gouraud Specular Alpha effect:
You can see that the white area does get smaller as we increase the alpha
Alpha 1          |  Alpha 5
:-------------------------:|:-------------------------:
![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Gouraud_SpecularAlpha.png)  |  ![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Gouraud_SpecularAlphaLess.png )

### Phong Specular Alpha effect:
You can see that the white area does get smaller as we increase the alpha
Alpha 1          |  Alpha 5
:-------------------------:|:-------------------------:
![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Phong_SpecularAlpha.png)  |  ![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Phong_SpecularAlphaLess.png )


### Parallel light source direction change:
We can see that the left lit area does change
Parallel 1          |  Parallel 2
:-------------------------:|:-------------------------:
![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Gouraud_Parallel.png)  |  ![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Gouraud_Parallel_YPlus_XPlus.png )

We can see that the left lit area does change and gets brighter when the Z is negative and the opposite is true.
Z Positive          |  Z Negative
:-------------------------:|:-------------------------:
![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Gouraud_Parallel_ZPlus.png)  |  ![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Gouraud_Parallel_Zminus.png )


### _Gouraud_ Vs. _Phong_ :
We can see that Phong shading is a lot smoother - as requested.
#### Gouraud
![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Gouraud_SpecularAlpha.png)
#### Phong
![](../../master/Pictures_As2/PartB/par_source/Sphere_1_Phong_SpecularAlpha.png )

-----------------------------------------------------------------------------------------------------------------------

## POST PROCESSING:
I chose to implement the fog effect - Linear and exponential. I allowed the user to set a multiply factor of the start and end of fog (multiplied by max and min z which are the default beginning and end of the fog - fog density for the exponential fog)

### No Fog:
![](../../master/Pictures_As2/No_Fog.png)

### Linear fog - different parameters: 

![](../../master/Pictures_As2/Fog_Linear_1.png)

![](../../master/Pictures_As2/Fog_Linear_2.png)

![](../../master/Pictures_As2/Fog_Linear_3.png)

### Exponential fog - different density:

![](../../master/Pictures_As2/Fog_Exponential.png)

![](../../master/Pictures_As2/Fog_Exponential_2.png)

-----------------------------------------------------------------------------------------------------------------------

## FOR FUN :) Disco Ball:

![](../../master/Pictures_As2/Disco.png)

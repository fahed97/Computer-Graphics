Assignment 3
=============

Student: *Fahed Nasser*


## Section 1:
The Renderer::InitOpenGLRendering() function was meant to prepare the screen for earlier assignments. It loads the screen textures with a unique identifier, and a VAO of the buffers that describe the renderable object. Then the screen vertices are set as two triangles, with the vertices (-1, -1) (1, -1) (-1,  1) (-1,  1) (1, -1) (1,  1), that stretch over the screen and the texture is drawn over them. It also sets the screen textures coordinates that will be drawn over the triangles. Once memory is allocated for the vertices and texture, we then load the shaders (their old version, not mine right now in this assignment) and tells the shader where (pointer) to look for the vertices and texture coordinates. After InitOpenGLRendering(), we then use the color buffers to render the objects to the screen.


-----------------------------------------------------------------------------------------------------------------------

## Section 2:
I updated the MeshModel class, thus preparing the vertices, normals and textures _(which I didn't use in the stage)_, binding the buffer and so on. Since my code worked well in the early assignments and I didn't have any issues loading the MeshModels and Camera, I only adapted my code with the OpenGL structures and code. Here's the loop I used to load the vertices.

```
modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int vertexnormalindex = currentFace.GetNormalIndex(j) - 1;

			Vertex vertex;
			VertexNormal vertexnormal;
			vertex.position = vertices[vertexIndex];
			
			if(normals.size()>0)
			vertex.normal = normals[vertexnormalindex];
			

			modelVertices.push_back(vertex);
		}
```

-----------------------------------------------------------------------------------------------------------------------

## Section 3:
In the early stages of the project, I built the classes with getters and setters, so I could get any Matrix/ Variable and any other fields. I got the pipeline attributes _(Transformations, Projection etc.)_ in the Render function, and I set the uniforms accordingly. In the vertex shader, I used these uniforms and applied them to the vertices. I used the vertex shader provided in the OpenGL Demo. Some of the variables are not used in this stage like those responsible for light (normals) and textures for example. 
The code:
```
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;
	
	

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	fragTexCoords = texCoords;

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);


	

	
}
```

-----------------------------------------------------------------------------------------------------------------------

## Section 4:
Same as the vertex shader, I used the OpenGL Demo fragment shader, and of course changed the relevant code for this early stage. I changed the color of the Model, in this line _frag_color = vec4(0.5,1.f,0.8,1.f);_. Many of the variables and code line were not used in this stage, will be used later of course. 
The code:
```
#version 330 core

struct Material //Wasn't used in this stage
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
};

// We set this field's properties from the C++ code
uniform Material material;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	//vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	frag_color = vec4(0.5,1.f,0.8,1.f);
}
```

-----------------------------------------------------------------------------------------------------------------------

## Section 5:
As I mentioned above, I set the uniforms according to my code, and "sent" the Matrices and attributes of the pipeline to the shader. You observe the Model world and local matrix, the lookAt and Cinverse matrices, and eventually the projection matrix.
As a start to test the code, I used the wire frame drawing, and eventually added the fill mode. Pictures are provided.
The Code (Draws the mesh only):
```
if (cameraCount > 0)
	{

		int modelCount = scene.GetModelCount();
		Camera& camera = scene.GetActiveCamera();

		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{
			MeshModel& currentModel = scene.GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();

			// Set the uniform variables
			colorShader.setUniform("model", currentModel.getTransformationMatrix());
			colorShader.setUniform("view", camera.getLookat() * camera.getCameraTransformation());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			GLuint vao = currentModel.GetVAO();
			glBindVertexArray(currentModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
			glBindVertexArray(0);

			// Drag our model's faces (triangles) in line mode (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(currentModel.GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel.GetModelVertices().size());
			glBindVertexArray(0);
		}
	}
```
### Wire Frame:
Black BG          |  Blue BG
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/5_a.png)  |  ![](../../main/Pictures_As3/5_b.png)

Perspective Projection:
![](../../main/Pictures_As3/5_c.png)

### Fill Mode:
Orthographic          |  Perspective
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/5_d.png)  |  ![](../../main/Pictures_As3/5_e.png)


-----------------------------------------------------------------------------------------------------------------------

## Section 6:
First of all, I implemented the phong shading using the same calculations in the previous assignment, thus calculating Ambient color, which is the material ambient multiplied by the light ambient, summed with the diffuse& specular element. Here's a general overview on the models we used in earlier assignments:


![](../../main/Pictures_As3/6.jpg)
1: Point Light
2: Parallel Light, pointing in the positive Y direction

![](../../main/Pictures_As3/6_1.jpg)
1: Point Light
2: Parallel Light, pointing in the negative X direction

### Alpha effect demonstration:
In the examples provided, you may observe that the light is concentrated more, as increasing the shininess coefficient would result.

Sphere: 

Alpha 1          |  Alpha 5
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/6_alpha_1.png)  |  ![](../../main/Pictures_As3/6_alpha_5.png)

Beetle (downloaded from the provided link):

Alpha 1          |  Alpha 5
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/6_b_alpha.png)  |  ![](../../main/Pictures_As3/6_b_alpha_5.png)

### Different lights and projections:
_(You may observe the transformation on the bottom right hand corner)_

Light1:
![](../../main/Pictures_As3/6_a_1.png)
Light2:
![](../../main/Pictures_As3/6_a_2.png)
Light3:
![](../../main/Pictures_As3/6_a_3.png)

Beetle:
![](../../main/Pictures_As3/6_b_1.png)
![](../../main/Pictures_As3/6_b_2.png)
![](../../main/Pictures_As3/6_b_3.png)
![](../../main/Pictures_As3/6_b_4.png)
![](../../main/Pictures_As3/6_b_5.png)

Tea and bananas, a healthy choice
_(Parallel lights)_
![](../../main/Pictures_As3/teawithbananas.png)

-----------------------------------------------------------------------------------------------------------------------


## Section 7:
I allowed the user moving between texture coordinates mapping _(if the model comes with vt)_ and planar projection. I also implemented spherical projection (which is not required for singulars) for fun only. 

First of all I will demonstrate the difference between a texture coordinates mapping _(vt in obj file)_ and planar projection on the crate that was provided in the Demo:

#### Texture coordinates:

Crate VT          |  Crate VT
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/Textures_1.png)  |  ![](../../main/Pictures_As3/Textures_1_b.png)

#### Planar Projection:

Crate Planar         |  Crate Planar
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/Textures_2.png)  |  ![](../../main/Pictures_As3/Textures_2_b.png)

#### Teapot with B&W board - Planar Projection:

You may notice the planar effect which was demonstrated in class

Teapot Planar         |  Teapot Planar
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/Textures_3.png)  |  ![](../../main/Pictures_As3/Textures_3_b.png)

#### Some models with different textures:

Most of the models on-line were quad faced, so I triangulated them using BLENDER application

##### The UV map:
![](../../main/Data/amongus.jpg)

Among Us Character         |  Among Us Character
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/Textures_4.png)  |  ![](../../main/Pictures_As3/Textures_4_b.png)

##### The UV map:
![](../../main/Data/timberland.jpg)

Timberland Boots         |  Timberland Boots
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/Textures_5_b.png)  |  ![](../../main/Pictures_As3/Textures_5_c.png)

##### Fruits:

![](../../main/Pictures_As3/fruits.png)

##### Fruits with lights:

Watermelon and Banana         |  Watermelon and Banana with different Z
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/fruits_with_lights.png)  |  ![](../../main/Pictures_As3/fruits_with_lights_b.png)

##### Added Features:

I also tried to do more with textures and lighting, thus instead of passing a diffuse, specular rgb's as a material parameters, I passed a sampler2D map, thus the diffuse part consists of the texture picture provided with the model, and the specular part consists of a reflection/ specular material picture. A lot of models were provided with the specular part, which made the lighting more realistic.


Watermelon without specular map:

![](../../main/Pictures_As3/7_nospecular.png)

Watermelon with specular map:

![](../../main/Pictures_As3/7_specular.png)

we can see how the specular map gave us a more sense of reflection. The mapping and its quality of course depends on the picture provided with the model.

The specular map:

![](../../main/Data/Watermelon_4k_Roughnessmap.png)

 
_p.s this specular mapping was made as an added feature, so it should not be graded if it's below quality and requirements. I followed the learnopengl textures and lighting for this section._

-----------------------------------------------------------------------------------------------------------------------

## Section 8:

I implemented normal mapping or bumping with the help of https://learnopengl.com/Advanced-Lighting/Normal-Mapping . Basically, what was needed to do is, instead of relying on the normals which are provided in the obj file, we rely on the normals generated using the normals texture picture and using a TBN matrix, which is a Tangents, Bitangents and Normals matrix. With a little of math and geometry, the normals we take into consideration when calculating the diffuse and specular lights are calculated, thus giving us a sense of a "bumpy" surface.

Let's take the watermelon as an example, you may see that without the normal mapping the watermelon is rather flat, and has no "Terrain" and dust that we usually see. 

Normal Map:

![](../../main/Data/wataermelonnormalreport.png) 

Watermelon No Normal Mapping         |  Watermelon With Normal Mapping
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/watermelon_nonormals_.png)  |  ![](../../main/Pictures_As3/watermelon_withnormals_.png)

Watermelon with diffuse texture only: 

![](../../main/Pictures_As3/8_nonormal.png)  

Watermelon with normal mapping:

![](../../main/Pictures_As3/8_withnormal.png)

The brick wall is clearly visible as flat without normal mapping, when normal mapping is added, you can see the difference.

Normal Map:

![](../../main/Data/brickwall_normalreport.png) 

BrickWall with Normal Mapping         |  BrickWall with Normal Mapping
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/8_withnormal_color.png)  |  ![](../../main/Pictures_As3/8_withnormal_color_1.png)

BrickWall         |  BrickWall with Normal Mapping
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/8_nonormal_2.png)  |  ![](../../main/Pictures_As3/8_withnormal_2.png)

BrickWall         |  BrickWall with Normal Mapping
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/8_nonormal_3.png)  |  ![](../../main/Pictures_As3/8_withnormal_3.png)

Normal Map:

![](../../main/Data/lemonnormalsreport.png) 

Lemon No Normal Mapping         |  Lemon With Normal Mapping
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/Lemon_nonormals_.png)  |  ![](../../main/Pictures_As3/Lemon_withnormals_.png)

Lemon         |  Lemon with Normal Mapping
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/8_nonormal_1.png)  |  ![](../../main/Pictures_As3/8_withnormal_1.png)

Normal Map:

![](../../main/Pictures_As3/bootsreport.jpg) 



Boots No Normal Mapping         |  Boots With Normal Mapping
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/boots_nonormal.png)  |  ![](../../main/Pictures_As3/boots_withnormal.png)


Rock         |  Rock with Normal Mapping
:-------------------------:|:-------------------------:
![](../../main/Pictures_As3/8_nonormal_4.png)  |  ![](../../main/Pictures_As3/8_withnormal_4.png)
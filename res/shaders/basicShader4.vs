

const int MAX_JOINTS = 50; //max joints allowed in a skeleton
const int MAX_WEIGHTS = 3; //max number of joints that can affect a vertex

in vec3 position;
in vec2 texCoords;
in vec3 normal;
in vec3 color;
in vec3 weights;
in ivec3 jointIndices;



out vec2 pass_texCoords;
out vec3 pass_normal;
out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;
out vec3 position0;

uniform mat4 jointTransforms[MAX_JOINTS];
uniform mat4 MVP;
uniform mat4 Normal;
uniform int linksNum;
uniform int flag2;
uniform int shapeIndex;
uniform ivec4 jointIndices2;
varying vec3 weight0;


void main()
{
    mat4 totalSum = mat4(0);
     
    if(flag2 == 0){
    vec4 totalLocalPos = vec4(0.0);
	vec4 totalNormal = vec4(0.0);
	mat4 matrix = mat4(1);


	gl_Position = MVP * totalLocalPos; //vec4(position, 1.0);//
	pass_normal = totalNormal.xyz;
	color0 = color.xyz;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	//position0 = vec3(Normal * vec4(position, 1.0));
	texCoord0 = texCoords;
	
	
	
	
	}else{
	
	
	    texCoord0 = texCoords;
		color0 = color;
		weight0 = weights;
		normal0 = (Normal * vec4(normal, 0.0)).xyz;
	

		if (  shapeIndex >= (6+linksNum) || shapeIndex < 6 ) {
			gl_Position = MVP * vec4(position, 1.0); //In case that the shape index is not one
			// of the snake element.
		}
		else {
			
			
			if (shapeIndex < (6+linksNum-1)) {
				totalSum = totalSum +   jointTransforms[shapeIndex+1-6]*weights[2];
			}
			
			if (shapeIndex > 6) {
			
				totalSum  = totalSum + jointTransforms[shapeIndex-1-6]*weights[0];
			}
			
			
			totalSum = totalSum +  jointTransforms[shapeIndex-6]*weights[1];

			gl_Position = totalSum * vec4(position, 1.0);
		}

		

	}
}

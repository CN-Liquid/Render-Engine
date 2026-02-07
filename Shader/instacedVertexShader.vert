#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout (location = 2) in vec4 ModelCol1;
layout (location = 3) in vec4 ModelCol2;
layout (location = 4) in vec4 ModelCol3;
layout (location = 5) in vec4 ModelCol4;
layout(location = 6) in vec3 textCoords;

uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normals;
out vec3 textCoord;

void main() {
 mat4 model = mat4(ModelCol1 , ModelCol2 ,ModelCol3 , ModelCol4);
  vec4 worldPosition = model * vec4(aPos.xyz,1.0f) ;
  gl_Position = projection * view *worldPosition;
  fragPos = vec3(worldPosition);
  normals = normal;
  textCoord = textCoords;
}
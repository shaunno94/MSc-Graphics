#version 450 core
 
uniform float particleSize;

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN[];

out Vertex {
	vec2 texCoord;
	vec4 colour;
} OUT;
 
void main() {	
	for(int i = 0; i < gl_in.length(); ++i) {
		OUT.colour = IN[i].colour;

		//top right
		gl_Position = gl_in[ i ].gl_Position;
		gl_Position.x += particleSize;
		gl_Position.y += particleSize;
		OUT.texCoord = vec2(1,0);

		EmitVertex();

		//Top Left
		gl_Position = gl_in[ i ].gl_Position;
		gl_Position.x -= particleSize;
		gl_Position.y += particleSize;
		OUT.texCoord = vec2(0,0);
		EmitVertex();

		//bottom right
		gl_Position = gl_in[ i ].gl_Position;
		gl_Position.x += particleSize;
		gl_Position.y -= particleSize;
		OUT.texCoord = vec2(1,1);
		EmitVertex();

		//bottom Left
		gl_Position = gl_in[ i ].gl_Position;
		gl_Position.x -= particleSize;
		gl_Position.y -= particleSize;
		OUT.texCoord = vec2(0,1);
		EmitVertex();

		EndPrimitive();
	}
}
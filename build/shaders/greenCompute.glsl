#version 430 core

layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba8, binding = 0) uniform image2D framebuffer;

void main() {
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(float(pos.x)/2550, float(pos.y)/1440, 0.0, 1.0);
    imageStore(framebuffer, pos, color);
}
namespace Shaders {
const char* blueCompute = R"(
#version 430 core

layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba8, binding = 0) uniform image2D framebuffer;

void main() {
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(0.0, 0.0, 1.0, 1.0);
    imageStore(framebuffer, pos, color);
})";
const char* greenCompute = R"(
#version 430 core

layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba8, binding = 0) uniform image2D framebuffer;

void main() {
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(0.0, 1.0, 0.0, 1.0);
    imageStore(framebuffer, pos, color);
})";
const char* redCompute = R"(
#version 430 core

layout(local_size_x = 8, local_size_y = 8) in;
layout(rgba8, binding = 0) uniform image2D framebuffer;

void main() {
    ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
    vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
    imageStore(framebuffer, pos, color);
})";
}
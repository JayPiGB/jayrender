#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform float ambientStr;
uniform float specularStr;
uniform int shininess;

uniform vec3 objectColor;
uniform vec3 lightColor;

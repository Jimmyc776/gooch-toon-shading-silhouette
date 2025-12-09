#version 330 core

out vec4 frag_color;

// Reuse Phong's structs and uniforms
struct AmbientLight {
    bool enabled;
    vec3 ambient;
};

struct PointLight {
    bool enabled;
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    vec3 attenuation;
};

struct DirectionalLight {
    bool enabled;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 world_position;
in vec3 world_normal;
in vec2 tex_coord;

uniform vec3 camera_position;
uniform Material material;
uniform AmbientLight ambient_light;
uniform PointLight point_light;
uniform DirectionalLight directional_light;

// NEW: Gooch-specific uniforms
uniform vec3 cool_color = vec3(0.0, 0.2, 0.8);     // Blue undertone
uniform vec3 warm_color = vec3(1.0, 0.6, 0.1);      // Yellow undertone
uniform float alpha = 0.1;   // How much object color in cool
uniform float beta = 0.2;    // How much object color in warm

// Keep Phong's functions for specular
vec3 CalcPointLightSpecular(vec3 normal, vec3 view_dir) {
    PointLight light = point_light;
    vec3 light_dir = normalize(light.position - world_position);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_intensity = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess * 0.5);
    vec3 specular_color = specular_intensity * light.specular * material.specular * 0.5;

    float distance = length(light.position - world_position);
    float attenuation = 1.0 / (light.attenuation.x +
        light.attenuation.y * distance +
        light.attenuation.z * (distance * distance));

    return attenuation * specular_color;
}

vec3 CalcDirectionalLightSpecular(vec3 normal, vec3 view_dir) {
    DirectionalLight light = directional_light;
    vec3 light_dir = normalize(-light.direction);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float specular_intensity = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess * 0.5);
    vec3 specular_color = specular_intensity * light.specular * material.specular * 0.5;
    return specular_color;
}

void main() {
    vec3 normal = normalize(world_normal);
    vec3 view_dir = normalize(camera_position - world_position);

    // Gooch cool-to-warm shading
    vec3 k_cool = cool_color + alpha * material.diffuse;
    vec3 k_warm = warm_color + beta * material.diffuse;

    // Use point light or directional light for shading direction
    vec3 light_dir;
    if (point_light.enabled) {
        light_dir = normalize(point_light.position - world_position);
    } else if (directional_light.enabled) {
        light_dir = normalize(-directional_light.direction);
    } else {
        light_dir = vec3(0.0, 1.0, 0.0);  // Default up
    }

    float NdotL = dot(normal, light_dir);
    float t = pow((NdotL + 1.0) * 0.5, 1.7);

    vec3 gooch_diffuse = mix(k_cool, k_warm, t);

    // Add specular highlights (keep crisp whites)
    vec3 specular = vec3(0.0);
    if (point_light.enabled) {
        specular += CalcPointLightSpecular(normal, view_dir);
    }
    if (directional_light.enabled) {
        specular += CalcDirectionalLightSpecular(normal, view_dir);
    }

    // Add a bit of ambient
    vec3 ambient = vec3(0.0);
    if (ambient_light.enabled) {
        ambient = ambient_light.ambient * material.ambient;
    }

    vec3 final_color = clamp(gooch_diffuse + specular + ambient * 0.1, 0.0, 1.0);
    frag_color = vec4(final_color, 1.0);
}

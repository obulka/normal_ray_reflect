# N_RayReflect

This BlinkScript Gizmo allows you to perform single-bounce raytracing, only requiring a camera, normals AOV, and HDRI.

![Daft_Punk_v002](https://github.com/obulka/normal_ray_reflect/assets/21975584/b80d9742-4d1b-403a-9aa1-1d3c31d28dd3)

<p align="center">
  <img src="https://github.com/obulka/normal_ray_reflect/assets/21975584/ddf6d475-7ec0-4f21-bbef-50c371dcfbd0" alt="animated" />
</p>

Specular surfaces, as well as diffuse, and even transmissive, are supported. These surface properties can be a single constant value, or driven by input images to create materials as seen in the above turntable.

## Setup

Simply clone/download this repo and add the following line to your `init.py`: `nuke.pluginAddPath("/path/to/normal_ray_reflect/src/python")`, replacing "`/path/to`" with the actual path to the repository. The gizmo will be available as "N_RayReflect" the next time you launch Nuke. There is an example in the `examples` directory; simply insert a normal pass to get started.

## Inputs

- hdri
  - The HDRI image that will be seen in reflections/lighting
- camera
  - The camera to shoot rays out of
- normals
  - The normals pass that will be used to reflect, refract, and diffuse the rays
- diffuse
  - The diffuse colour of the surface represented by the normals pass if the diffuse "Use Input" is checked
- specular
  - The specular colour of the surface represented by the normals pass if the specular "Use Input" is checked
- transmission
  - The transmission colour of the surface represented by the normals pass if the transmission "Use Input" is checked
- specRoughness
  - The specular roughness values of the surface represented by the normals pass if the specular roughness "Use Input" is checked
- transRoughness
  - The transmission roughness values of the surface represented by the normals pass if the transmission roughness "Use Input" is checked

## Knobs

- Normals
  - The channels to use as the normals pass.
- Ray Samples
  - The number of samples to use per pixel.
- Incident Refractive Index
  - The refractive index of the incoming ray medium.
- Refracted Refractive Index
  - The refractive index of the outgoing ray medium.
- HDRI Offset Angle
  - Rotate the HDRI by this amount.
- Diffuse Colour
  - The diffuse colour of the surface if the diffuse "Use Input" knob is not checked
- Specular Colour
  - The specular colour of the surface if the specular "Use Input" knob is not checked
- Transmission Colour
  - The transmission colour of the surface if the transmission "Use Input" knob is not checked
- Specular Roughness
  - The specular roughness of the surface if the specular roughness "Use Input" knob is not checked
- Transmission Roughness
  - The transmission roughness of the surface if the transmission roughness "Use Input" knob is not checked
- Use Input
  - These knobs toggle between the use of the corresponding knobs as a surface property, or the corresponding input
    - If you use the knob, ("Use Input" false) the entire surface will have the same value, which can be good for testing
    - If you use the input, ("Use Input" true) the surface property can be specified in an image, and therefore each pixel can take a different value
- Enable Precomputed Irradiance
  - Use a precomputed irradiance for diffuse lighting. This will require only one sample rather than many in order to converge.
- Irradiance Blur Size
  - Blur the HDRI by this amount before using it to compute the irradiance. This can help reduce artifacts caused by small, bright, light sources without increasing the 'Irradiance Samples'.
- Irradiance Samples
  - The number of samples in the horizontal direction that will be used to compute the irradiance of a hemisphere of the HDRI. Half this many samples will be used in the vertical direction.
- Output Irradiance
  - Enable this to view the irradiance.

## Limitations

- There are no secondary reflections for any material
  - The transmissive objects are treated as shells
    - They have no back wall so rays are not refracted or reflected off the back of the shape
    - If the rays are meant to enter and exit the object the final refracted direction, and therefore the final colour will not be physically accurate

## References
- Examples courtesy of Riley Gray

# normal_ray_reflect

This gizmo allows you to reflect, diffuse, and refract rays using a normal pass and an HDRI.

![example_normals](https://user-images.githubusercontent.com/21975584/195250014-08f770d8-36e9-4d8c-9c58-093bbf26f634.png)
![example_noise](https://user-images.githubusercontent.com/21975584/195250110-d225efa3-494a-4b96-8bc0-6d3d8942bbf2.png)
![example_specular](https://user-images.githubusercontent.com/21975584/195250043-a39fb2ba-7bbb-4b7c-8b26-922dca7505ab.png)
![example_specular_roughness](https://user-images.githubusercontent.com/21975584/195250049-977c4394-83ae-49a5-9fff-f9f3580fac84.png)
![example_diffuse](https://user-images.githubusercontent.com/21975584/195250065-cd0cb4d7-c28f-48a5-85a7-4d77b6aedbc7.png)
![example_transmission](https://user-images.githubusercontent.com/21975584/195250077-cd1d98ae-3aad-44b5-b757-dd31f7333b77.png)
![example_transmission_roughness](https://user-images.githubusercontent.com/21975584/195250091-6179244d-4c33-44ce-88d7-2b26ae223e79.png)
![example_transmission_and_spec_roughness](https://user-images.githubusercontent.com/21975584/195250100-bcbe0472-7a8a-4e6b-8863-a8b6ef4dfe12.png)

## Setup

Simply clone/download this repo and add the following line to your `init.py`: `nuke.pluginAddPath("/path/to/normal_ray_reflect/src/python")`, replacing "`/path/to`" with the actual path to the repository. The gizmo will be available as "N_RayReflect" the next time you launch Nuke. There is an example in the `examples` directory; simply insert a normal pass to get started.

## Inputs

- hdri
  - The HDRI image that will be seen in reflections/lighting
- camera
  - The camera to shoot rays out of
- normals
  - The normals pass that will be used to reflect, refract, and diffuse the rays
- surface
  - The surface properties at each pixel location
  - R is specular
  - G is specular roughness
  - B is transmission
  - A is transmission roughness
  - The diffuse contribution is 1 - specular - transmission
  - specular + transmission should not be greater than 1 for physical accuracy

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
- Enable Precomputed Irradiance
  - Use a precomputed irradiance for diffuse lighting. This will require only one sample rather than many in order to converge.
- Irradiance Blur Size
  - Blur the HDRI by this amount before using it to compute the irradiance. This can help reduce artifacts caused by small, bright, light sources without increasing the 'Irradiance Samples'.
- Irradiance Samples
  - The number of samples in the horizontal direction that will be used to compute the irradiance of a hemisphere of the HDRI. Half this many samples will be used in the vertical direction.
- Output Irradiance
  - Enable this to view the irradiance.

## References
- HDRI used in examples from Greg Zaal - https://polyhaven.com/a/aft_lounge

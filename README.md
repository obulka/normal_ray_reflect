# normal_ray_reflect

This gizmo allows you to reflect, diffuse, and transmit rays using a normals pass and an HDRI.

## Setup

Simply clone/download this repo and add the following line to your `init.py`: `nuke.pluginAddPath("/path/to/normal_ray_reflect/src/python")`, replacing "`/path/to`" with the actual path to the repository. The gizmo will be available as "N_RayReflect" the next time you launch Nuke. There is an example in the `examples` directory; simply insert a normal pass to get started.

## Inputs

- hdri
  - The HDRI image that will be seen in reflections/lighting
- camera
  - The camera to shoot rays out of
- normals
  - The normals pass that will be used to reflect, transmit, and diffuse the rays
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
  - The channels to use as the normals pass
- Ray Samples
  - The number of samples to use per pixel
- Incident Refractive Index
  - The refractive index of the incoming ray medium
- Refracted Refractive Index
  - The refractive index of the outgoing ray medium
- HDRI Offset Angle
  - Rotate the HDRI by this amount

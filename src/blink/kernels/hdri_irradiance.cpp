// Copyright 2022 by Owen Bulka.
// All rights reserved.
// This file is released under the "MIT License Agreement".
// Please see the LICENSE.md file that should have been included as part
// of this package.


/**
 * Get the equivalent theta and phi values that lie between [0, 2 * PI),
 * and [0, PI) respectively.
 *
 * @arg angles: The spherical angles in radians.
 *
 * @returns: The equivalent theta and phi.
 */
inline float2 normalizeAngles(const float2 &angles)
{
    float2 normalizedAngles = float2(
        fmod(angles.x, 2.0f * PI),
        fmod(angles.y, PI)
    );
    normalizedAngles.x += 2 * PI * (normalizedAngles.x < 0);
    normalizedAngles.y += PI * (normalizedAngles.y < 0);

    return normalizedAngles;
}


/**
 * Convert a cartesion unit vector to spherical.
 *
 * @arg rayDirection: The cartesion unit vector.
 *
 * @returns: The spherical angles in radians.
 */
inline float2 cartesionUnitVectorToSpherical(const float3 &rayDirection)
{
    return normalizeAngles(float2(
        atan2(rayDirection.z, rayDirection.x),
        acos(rayDirection.y)
    ));
}


/**
 * Convert a spherical unit vector (unit radius) to cartesion.
 *
 * @arg angles: The spherical angles in radians.
 *
 * @returns: The equivalent cartesion vector.
 */
inline float3 sphericalUnitVectorToCartesion(const float2 &angles)
{
    const float sinPhi = sin(angles.y);
    return float3(
        cos(angles.x) * sinPhi,
        cos(angles.y),
        sin(angles.x) * sinPhi
    );
}


/**
 * Convert the uv position in a latlong image to angles.
 *
 * @arg uvPosition: The UV position.
 *
 * @returns: The equivalent angles in radians.
 */
inline float2 uvPositionToAngles(const float2 &uvPosition)
{
    return float2(
        (uvPosition.x + 1.0f) * PI,
        (1.0f - uvPosition.y) * PI / 2.0f
    );
}


/**
 * Convert location of a pixel in an image into UV.
 *
 * @arg pixelLocation: The x, and y positions of the pixel.
 * @arg format: The image width, and height.
 *
 * @returns: The UV position.
 */
inline float2 pixelsToUV(const float2 &pixelLocation, const float2 &format)
{
    return float2(
        2.0f * pixelLocation.x / format.x - 1.0f,
        2.0f * pixelLocation.y / format.y - 1.0f
    );
}


kernel HDRIrradiance : ImageComputationKernel<ePixelWise>
{
    Image<eRead, eAccessRandom, eEdgeClamped> hdri; // the input image
    Image<eWrite> dst; // the output image

    param:
        int2 _samples;

    local:
        float2 __hdriPixelSize;
        float3 __up;
        float2 __sampleStep;


    /**
     * Give the parameters labels and default values.
     */
    void define()
    {
        defineParam(_samples, "Samples", int2(100, 50));
    }


    /**
     * Initialize the local variables.
     */
    void init()
    {
        __hdriPixelSize = float2(hdri.bounds.width() / (2.0f * PI), hdri.bounds.height() / PI);
        __up = float3(0, 1, 0);

        __sampleStep = float2(
            2.0f * PI / (float) _samples.x,
            PI / (2.0f * (float) _samples.y)
        );
    }


    /**
     * Get the value of hdri the ray would hit at infinite distance
     *
     * @arg rayDirection: The direction of the ray.
     *
     * @returns: The colour of the pixel in the direction of the ray.
     */
    float4 readHDRIValue(float3 rayDirection)
    {
        const float2 angles = cartesionUnitVectorToSpherical(rayDirection);

        // Why does bilinear give nans? :(
        return hdri(
            round(__hdriPixelSize.x * angles.x) - 1,
            round(hdri.bounds.height() - (__hdriPixelSize.y * angles.y)) - 1
        );
    }


    /**
     * Compute the irradiance of a pixel.
     *
     * @arg pos: The x, and y location we are currently processing.
     */
    void process(int2 pos)
    {
        const float2 uvPosition = pixelsToUV(
            float2(pos.x, pos.y),
            float2(hdri.bounds.width(), hdri.bounds.height())
        );
        const float3 direction = sphericalUnitVectorToCartesion(
            uvPositionToAngles(uvPosition)
        );

        const float3 tangentRight = normalize(cross(__up, direction));
        const float3 tangentUp = normalize(cross(direction, tangentRight));

        float4 irradiance = float4(0);

        for (float theta = 0.0f; theta < 2.0f * PI; theta += __sampleStep.x)
        {
            for (float phi = PI / 2.0f; phi > 0.0f; phi -= __sampleStep.y)
            {
                const float3 tangent = sphericalUnitVectorToCartesion(float2(theta, phi));
                const float3 sampleDirection = (
                    tangent.x * tangentRight
                    + tangent.z * tangentUp
                    + tangent.y * direction
                );

                irradiance += readHDRIValue(sampleDirection) * cos(phi) * sin(phi);
            }
        }

        dst() = PI * irradiance / (float) (_samples.x * _samples.y);
    }
};

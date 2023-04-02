// Copyright 2022 by Owen Bulka.
// All rights reserved.
// This file is released under the "MIT License Agreement".
// Please see the LICENSE.md file that should have been included as part
// of this package.

//
// BlinkScript Normal Reflections
//


//
// Math
//


/**
 * Blend linearly between two values.
 *
 * @arg value0: The first value.
 * @arg value1: The second value.
 * @arg weight: The blend weight, 1 will return value0, and 0 will
 *     return value1.
 *
 * @returns: The blended value.
 */
inline float blend(const float value0, const float value1, const float weight)
{
    return value1 + weight * (value0 - value1);
}


/**
 * Blend linearly between two values.
 *
 * @arg value0: The first value.
 * @arg value1: The second value.
 * @arg weight: The blend weight, 1 will return value0, and 0 will
 *     return value1.
 *
 * @returns: The blended value.
 */
inline float3 blend(const float3 &value0, const float3 &value1, const float weight)
{
    return value1 + weight * (value0 - value1);
}


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
 * @arg thetaOffset: Offset the theta angle by this amount.
 *
 * @returns: The spherical angles in radians.
 */
inline float2 cartesionUnitVectorToSpherical(
        const float3 &rayDirection,
        const float thetaOffset)
{
    return normalizeAngles(float2(
        atan2(rayDirection.z, rayDirection.x) + thetaOffset,
        acos(rayDirection.y)
    ));
}


/**
 * Get the position component of a world matrix.
 *
 * @arg worldMatrix: The world matrix.
 * @arg position: The location to store the position.
 */
inline void positionFromWorldMatrix(const float4x4 &worldMatrix, float3 &position)
{
    position = float3(
        worldMatrix[0][3],
        worldMatrix[1][3],
        worldMatrix[2][3]
    );
}


/**
 * Saturate a value ie. clamp between 0 and 1
 *
 * @arg value: The value to saturate
 *
 * @returns: The clamped value
 */
inline float saturate(float value)
{
    return clamp(value, 0.0f, 1.0f);
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


/**
 * Compute the aspect ratio from image format.
 *
 * @arg height_: The height of the image.
 * @arg width_: The width of the image.
 *
 * @returns: The aspect ratio.
 */
inline float aspectRatio(const float height_, const float width_)
{
    return height_ / width_;
}


/**
 * Multiply a 3d vector by a 3x3 matrix.
 *
 * @arg m: The matrix that will transform the vector.
 * @arg v: The vector to transform.
 * @arg out: The location to store the resulting vector.
 */
inline float3 matmul(const float3x3 &m, const float3 &v)
{
    return float3(
        m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
        m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
        m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
    );
}


/**
 * Multiply a 4d vector by a 4x4 matrix.
 *
 * @arg m: The matrix that will transform the vector.
 * @arg v: The vector to transform.
 * @arg out: The location to store the resulting vector.
 */
inline void matmul(const float4x4 &m, const float4 &v, float4 &out)
{
    for (int i=0; i < 4; i++)
    {
        out[i] = 0;

        for (int j=0; j < 4; j++)
        {
            out[i] += m[i][j] * v[j];
        }
    }
}


/**
 * Multiply a 4d vector by a 4x4 matrix.
 *
 * @arg m: The matrix that will transform the vector.
 * @arg v: The vector to transform.
 * @arg out: The location to store the resulting vector.
 */
inline float4 matmul(const float4x4 &m, const float4 &v)
{
    float4 out;
    matmul(m, v, out);
    return out;
}


/**
 * Convert degrees to radians.
 *
 * @arg angle: The angle in degrees.
 *
 * @returns: The angle in radians.
 */
inline float degreesToRadians(const float angle)
{
    return angle * PI / 180.0f;
}


/**
 * Compute the fractional portion of the value. Ex. 3.5 returns 0.5
 *
 * @arg value: The value to get the fractional portion of.
 *
 * @returns: The fractional portion of the value.
 */
inline float fract(const float value)
{
    return value - floor(value);
}


/**
 * The positive part of the vector. Ie. any negative values will be 0.
 *
 * @arg vector: The vector.
 *
 * @returns: The positive part of the vector.
 */
inline float positivePart(const float value)
{
    return max(value, 0.0f);
}


/**
 * Get a rotation matrix from an axis and an angle about that axis.
 *
 * @arg angles: The rotation angles in radians.
 * @arg out: The location to store the rotation matrix.
 */
inline void axisAngleRotationMatrix(const float3 &axis, const float angle, float3x3 &out)
{
    const float cosAngle = cos(angle);
    const float oneMinusCosAngle = 1.0f - cosAngle;
    const float sinAngle = sin(angle);

    const float3 axisSquared = axis * axis;

    const float axisXY = axis.x * axis.y * oneMinusCosAngle;
    const float axisXZ = axis.x * axis.z * oneMinusCosAngle;
    const float axisYZ = axis.y * axis.z * oneMinusCosAngle;

    const float3 axisSinAngle = axis * sinAngle;

    out[0][0] = cosAngle + axisSquared.x * oneMinusCosAngle;
    out[0][1] = axisXY - axisSinAngle.z;
    out[0][2] = axisXZ + axisSinAngle.y;
    out[1][0] = axisXY + axisSinAngle.z;
    out[1][1] = cosAngle + axisSquared.y * oneMinusCosAngle;
    out[1][2] = axisYZ - axisSinAngle.x;
    out[2][0] = axisXZ - axisSinAngle.y;
    out[2][1] = axisYZ + axisSinAngle.x;
    out[2][2] = cosAngle + axisSquared.z * oneMinusCosAngle;
}


/**
 * Get the angle and axis to use to rotate a vector onto another.
 *
 * @arg axis: The rotation angles in radians.
 * @arg out: The location to store the axis.
 *
 * @returns: The angle.
 */
inline float getAngleAndAxisBetweenVectors(
        const float3 &vector0,
        const float3 &vector1,
        float3 &axis)
{
    const float3 perpendicularVector = cross(vector0, vector1);
    if (length(perpendicularVector) > 0.0f)
    {
        axis = normalize(perpendicularVector);
    }
    else if (vector1.z != 0.0f || vector1.y != 0.0f)
    {
        axis = normalize(cross(float3(1, 0, 0), vector1));
    }
    else if (vector1.x != 0.0f || vector1.z != 0.0f)
    {
        axis = normalize(cross(float3(0, 1, 0), vector1));
    }
    else if (vector1.x != 0.0f || vector1.y != 0.0f)
    {
        axis = normalize(cross(float3(0, 0, 1), vector1));
    }
    else
    {
        axis = vector0;
    }
    return acos(dot(vector0, vector1));
}


/**
 * Align a vector that has been defined relative to an axis with another
 * axis. For example if a vector has been chosen randomly in a
 * particular hemisphere, rotate that hemisphere to align with a new
 * axis.
 *
 * @arg unalignedAxis: The axis, about which, the vector was defined.
 * @arg alignDirection: The axis to align with.
 * @arg vectorToAlign: The vector that was defined relative to
 *     unalignedAxis.
 *
 * @returns: 
 */
inline float3 alignWithDirection(
        const float3 &unalignedAxis,
        const float3 &alignDirection,
        const float3 &vectorToAlign)
{
    float3 rotationAxis;
    const float angle = getAngleAndAxisBetweenVectors(
        unalignedAxis,
        alignDirection,
        rotationAxis
    );

    if (angle == 0.0f)
    {
        return vectorToAlign;
    }

    float3x3 rotationMatrix;
    axisAngleRotationMatrix(rotationAxis, angle, rotationMatrix);

    return matmul(rotationMatrix, vectorToAlign);
}


//
// Random
//


/**
 * Get a random value on the interval [0, 1].
 *
 * @arg seed: The random seed.
 *
 * @returns: A random value on the interval [0, 1].
 */
inline float random(const float seed)
{
    return fract(sin(seed * 91.3458f) * 47453.5453f);
}


/**
 * Get a random value on the interval [0, 1].
 *
 * @arg seed: The random seed.
 *
 * @returns: A random value on the interval [0, 1].
 */
inline float2 random(const float2 &seed)
{
    return float2(
        random(seed.x),
        random(seed.y)
    );
}


/**
 * Create a random unit vector in the hemisphere aligned along the
 * z-axis, with a distribution that is cosine weighted.
 *
 * @arg seed: The random seed.
 *
 * @returns: A random unit vector.
 */
float3 cosineDirectionInZHemisphere(const float2 &seed)
{
    const float uniform = random(seed.x);
    const float r = sqrt(uniform);
    const float angle = 2 * PI * random(seed.y);
 
    const float x = r * cos(angle);
    const float y = r * sin(angle);
 
    return float3(x, y, sqrt(positivePart(1 - uniform)));
}


/**
 * Create a random unit vector in the hemisphere aligned along the
 * given axis, with a distribution that is cosine weighted.
 *
 * @arg axis: The axis to align the hemisphere with.
 * @arg seed: The random seed.
 *
 * @returns: A random unit vector.
 */
float3 cosineDirectionInHemisphere(const float3 &axis, const float2 &seed)
{
    return normalize(alignWithDirection(
        float3(0, 0, 1),
        axis,
        cosineDirectionInZHemisphere(seed)
    ));
}


//
// Camera
//


/**
 * Create a projection matrix for a camera.
 *
 * @arg focalLength: The focal length of the camera.
 * @arg horizontalAperture: The horizontal aperture of the camera.
 * @arg aspect: The aspect ratio of the camera.
 * @arg nearPlane: The distance to the near plane of the camera.
 * @arg farPlane: The distance to the far plane of the camera.
 *
 * @returns: The camera's projection matrix.
 */
float4x4 projectionMatrix(
        const float focalLength,
        const float horizontalAperture,
        const float aspect,
        const float nearPlane,
        const float farPlane)
{
    float farMinusNear = farPlane - nearPlane;
    return float4x4(
        2 * focalLength / horizontalAperture, 0, 0, 0,
        0, 2 * focalLength / horizontalAperture / aspect, 0, 0,
        0, 0, -(farPlane + nearPlane) / farMinusNear, -2 * (farPlane * nearPlane) / farMinusNear,
        0, 0, -1, 0
    );
}


/**
 * Generate a ray out of a camera.
 *
 * @arg cameraWorldMatrix: The camera matrix.
 * @arg inverseProjectionMatrix: The inverse of the projection matrix.
 * @arg uvPosition: The UV position in the resulting image.
 * @arg rayOrigin: Will store the origin of the ray.
 * @arg rayDirection: Will store the direction of the ray.
 */
void createCameraRay(
        const float4x4 &cameraWorldMatrix,
        const float4x4 &inverseProjectionMatrix,
        const float2 &uvPosition,
        float3 &rayOrigin,
        float3 &rayDirection)
{
    positionFromWorldMatrix(cameraWorldMatrix, rayOrigin);
    float4 direction = matmul(
        inverseProjectionMatrix,
        float4(uvPosition.x, uvPosition.y, 0, 1)
    );
    matmul(
        cameraWorldMatrix,
        float4(direction.x, direction.y, direction.z, 0),
        direction
    );
    rayDirection = normalize(float3(direction.x, direction.y, direction.z));
}


//
// Surface Interaction
//


/**
 * Reflect a ray off of a surface.
 *
 * @arg incidentRayDirection: The incident direction.
 * @arg surfaceNormalDirection: The normal to the surface.
 */
inline float3 reflectRayOffSurface(
        const float3 &incidentRayDirection,
        const float3 &surfaceNormalDirection)
{
    return normalize(
        incidentRayDirection
        - 2 * dot(incidentRayDirection, surfaceNormalDirection) * surfaceNormalDirection
    );
}


/**
 * Refract a ray through a surface.
 *
 * @arg incidentRayDirection: The incident direction.
 * @arg surfaceNormalDirection: The normal to the surface.
 * @arg incidentRefractiveIndex: The refractive index the incident ray
 *     is travelling through.
 * @arg refractedRefractiveIndex: The refractive index the refracted ray
 *     will be travelling through.
 *
 * @returns: The refracted ray direction.
 */
inline float3 refractRayThroughSurface(
        const float3 &incidentRayDirection,
        const float3 &surfaceNormalDirection,
        const float incidentRefractiveIndex,
        const float refractedRefractiveIndex)
{
    const float refractiveRatio = incidentRefractiveIndex / refractedRefractiveIndex;
    const float cosIncident = -dot(incidentRayDirection, surfaceNormalDirection);
    const float sinTransmittedSquared = refractiveRatio * refractiveRatio * (
        1.0f - cosIncident * cosIncident
    );
    if (sinTransmittedSquared > 1.0f)
    {
        return reflectRayOffSurface(incidentRayDirection, surfaceNormalDirection);
    }
    const float cosTransmitted = sqrt(1.0f - sinTransmittedSquared);
    return normalize(
        refractiveRatio * incidentRayDirection
        + (refractiveRatio * cosIncident - cosTransmitted) * surfaceNormalDirection
    );
}


/**
 * Compute the schlick, simplified fresnel reflection coefficient.
 *
 * @arg incidentRayDirection: The incident direction.
 * @arg surfaceNormalDirection: The normal to the surface.
 * @arg incidentRefractiveIndex: The refractive index the incident ray
 *     is travelling through.
 * @arg refractedRefractiveIndex: The refractive index the refracted ray
 *     will be travelling through.
 *
 * @returns: The reflection coefficient.
 */
float schlickReflectionCoefficient(
        const float3 &incidentRayDirection,
        const float3 &surfaceNormalDirection,
        const float incidentRefractiveIndex,
        const float refractedRefractiveIndex)
{
    const float parallelCoefficient = pow(
        (incidentRefractiveIndex - refractedRefractiveIndex)
        / (incidentRefractiveIndex + refractedRefractiveIndex),
        2
    );
    float cosX = -dot(surfaceNormalDirection, incidentRayDirection);
    if (incidentRefractiveIndex > refractedRefractiveIndex)
    {
        const float refractiveRatio = incidentRefractiveIndex / refractedRefractiveIndex;
        const float sinTransmittedSquared = refractiveRatio * refractiveRatio * (
            1.0f - cosX * cosX
        );
        if (sinTransmittedSquared > 1.0f)
        {
            return 1.0f;
        }
        cosX = sqrt(1.0f - sinTransmittedSquared);
    }
    return parallelCoefficient + (1.0f - parallelCoefficient) * pow(1.0f - cosX, 5);
}


kernel NormalReflectionKernel : ImageComputationKernel<ePixelWise>
{
    Image<eRead, eAccessPoint, eEdgeClamped> normals;
    Image<eRead, eAccessPoint, eEdgeClamped> seeds;
    Image<eRead, eAccessPoint, eEdgeClamped> diffuse;
    Image<eRead, eAccessPoint, eEdgeClamped> specular;
    Image<eRead, eAccessPoint, eEdgeClamped> transmission;
    Image<eRead, eAccessPoint, eEdgeClamped> material;

    Image<eRead, eAccessRandom, eEdgeClamped> hdri;
    Image<eRead, eAccessRandom, eEdgeClamped> irradiance;

    // the output image
    Image<eWrite> dst;


    param:
        // These parameters are made available to the user.

        // Camera params
        float _focalLength;
        float _horizontalAperture;
        float _nearPlane;
        float _farPlane;
        float4x4 _cameraWorldMatrix;

        // Image params
        float _formatWidth;
        float _formatHeight;

        float _hdriOffsetAngle;
        bool _usePrecomputedIrradiance;

        // Ray Params
        int _samples;

        float _incidentRefractiveIndex;
        float _refractedRefractiveIndex;


    local:
        // These local variables are not exposed to the user.

        float4x4 __inverseCameraProjectionMatrix;
        float __aperture;

        float2 __hdriPixelSize;
        float __hdriOffsetRadians;
        float2 __irradiancePixelSize;


    /**
     * Give the parameters labels and default values.
     */
    void define()
    {
        // Camera params
        defineParam(_focalLength, "Focal Length", 50.0f);
        defineParam(_horizontalAperture, "Horizontal Aperture", 24.576f);
        defineParam(_nearPlane, "Near Plane", 0.1f);
        defineParam(_farPlane, "Far Plane", 10000.0f);
        defineParam(
            _cameraWorldMatrix,
            "Camera World Matrix",
            float4x4(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
            )
        );

        // Image params
        defineParam(_formatHeight, "Screen Height", 2160.0f);
        defineParam(_formatWidth, "Screen Width", 3840.0f);
        defineParam(_hdriOffsetAngle, "HDRI Offset Angle", 0.0f);
        defineParam(_usePrecomputedIrradiance, "Use Precomputed Irradiance", true);

        // Ray Params
        defineParam(_samples, "Samples", 1);
        defineParam(_incidentRefractiveIndex, "Incident Refractive Index", 1.0f);
        defineParam(_refractedRefractiveIndex, "Refracted Refractive Index", 1.33f);
    }


    /**
     * Initialize the local variables.
     */
    void init()
    {
        float aspect = aspectRatio(_formatHeight, _formatWidth);
        float4x4 cameraProjectionMatrix = projectionMatrix(
            _focalLength,
            _horizontalAperture,
            aspect,
            _nearPlane,
            _farPlane
        );
        __inverseCameraProjectionMatrix = cameraProjectionMatrix.invert();

        __hdriPixelSize = float2(
            hdri.bounds.width() / (2 * PI),
            hdri.bounds.height() / PI
        );
        __irradiancePixelSize = float2(
            irradiance.bounds.width() / (2 * PI),
            irradiance.bounds.height() / PI
        );
        __hdriOffsetRadians = degreesToRadians(_hdriOffsetAngle);
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
        const float2 angles = cartesionUnitVectorToSpherical(rayDirection, __hdriOffsetRadians);

        // Should be able to say image access is eEdgeClamped and not do this
        // but I see nan pixels sooo... :(
        const float2 indices = clamp(
            float2(
                __hdriPixelSize.x * angles.x,
                hdri.bounds.height() - (__hdriPixelSize.y * angles.y)
            ),
            float2(0),
            float2(hdri.bounds.width(), hdri.bounds.height()) - 1.0f
        );

        return bilinear(hdri, indices.x, indices.y);
    }


    /**
     * Get the value of irradiance the hdri would provide in a direction
     *
     * @arg rayDirection: The direction of the ray.
     *
     * @returns: The colour of the pixel in the direction of the ray.
     */
    inline float4 readIrradianceValue(float3 rayDirection)
    {
        const float2 angles = cartesionUnitVectorToSpherical(rayDirection, __hdriOffsetRadians);

        // Should be able to say image access is eEdgeClamped and not do this
        // but I see nan pixels sooo... :(
        const float2 indices = clamp(
            float2(
                __irradiancePixelSize.x * angles.x,
                irradiance.bounds.height() - (__irradiancePixelSize.y * angles.y)
            ),
            float2(0),
            float2(irradiance.bounds.width(), irradiance.bounds.height()) - 1.0f
        );

        return bilinear(irradiance, indices.x, indices.y);
    }


    /**
     * Create a ray out of the camera
     *
     * @arg pixelLocation: The x, and y locations of the pixel.
     * @arg rayOrigin: The location to store the origin of the new ray.
     * @arg rayDirection: The location to store the direction of the new
     *     ray.
     */
    void getCameraRay(
            const float2 &seed,
            const float2 &pixelLocation,
            float3 &rayOrigin,
            float3 &rayDirection)
    {
        const float2 uvCoordinates = pixelsToUV(
            pixelLocation + random(seed),
            float2(_formatWidth, _formatHeight)
        );

        createCameraRay(
            _cameraWorldMatrix,
            __inverseCameraProjectionMatrix,
            uvCoordinates,
            rayOrigin,
            rayDirection
        );
    }


    /**
     * Compute a raymarched pixel value.
     *
     * @arg pos: The x, and y location we are currently processing.
     */
    void process(int2 pos)
    {
        const float2 pixelLocation = float2(pos.x, pos.y);

        SampleType(seeds) seed4 = seeds();
        float2 seed0 = float2(seed4.x, seed4.y);
        float2 seed1 = float2(seed4.z, seed4.w);

        SampleType(normals) normal = normals();
        const float3 normalDirection = float3(
            normal.x,
            normal.y,
            normal.z
        );

        const float4 diffuseColour = diffuse();
        const float4 specularColour = specular();
        const float4 transmissionColour = transmission();
        const float4 materialProperties = material();

        const float specular = saturate(specularColour.w);
        float transmission;
        if (specular + transmissionColour.w > 1.0f)
        {
            transmission = 1.0f - specular;
        }
        else
        {
            transmission = saturate(transmissionColour.w);
        }
        const float diffuse = saturate(1.0f - transmission - specular);

        const float specularRoughness = materialProperties.x * materialProperties.x;
        const float transmissionRoughness = materialProperties.y * materialProperties.y;
        // TODO: use material properties z and w slots to represent something, maybe anisotropy

        float4 resultPixel = float4(0);

        for (int sample=1; sample <= _samples; sample++)
        {
            // Generate a ray from the camera
            float3 rayOrigin;
            float3 rayDirection;
            getCameraRay(
                seed0,
                pixelLocation,
                rayOrigin,
                rayDirection
            );

            if (
                normalDirection.x != 0.0f
                || normalDirection.y != 0.0f
                || normalDirection.z != 0.0f
            ) {
                // Get the diffuse direction for the next ray
                const float3 diffuseDirection = cosineDirectionInHemisphere(
                    normalDirection,
                    seed1
                );

                if (diffuse > 0.0f)
                {
                    if (_usePrecomputedIrradiance)
                    {
                        resultPixel += diffuse * diffuseColour * readIrradianceValue(normalDirection);
                    }
                    else
                    {
                        resultPixel += diffuse * diffuseColour * readHDRIValue(diffuseDirection);
                    }
                }
                float fresnelSpecular = specular;
                if (transmission > 0.0f || specular > 0.0f)
                {
                    const float reflectivity = schlickReflectionCoefficient(
                        rayDirection,
                        normalDirection,
                        _incidentRefractiveIndex,
                        _refractedRefractiveIndex
                    );

                    fresnelSpecular = blend(1.0f, specular, reflectivity);

                    if (transmission > 0.0f)
                    {
                        resultPixel += (
                            transmission * transmissionColour * (1.0f - fresnelSpecular)
                            * readHDRIValue(normalize(blend(
                                diffuseDirection,
                                refractRayThroughSurface(
                                    rayDirection,
                                    normalDirection,
                                    _incidentRefractiveIndex,
                                    _refractedRefractiveIndex
                                ),
                                transmissionRoughness
                            )))
                            / (1.0f - specular)
                        );
                        
                    }
                }
                if (fresnelSpecular > 0.0f)
                {
                    resultPixel += (
                        fresnelSpecular * specularColour
                        * readHDRIValue(normalize(blend(
                            diffuseDirection,
                            reflectRayOffSurface(rayDirection, normalDirection),
                            specularRoughness
                        )))
                    );
                }
            }
            else
            {
                resultPixel += readHDRIValue(rayDirection);
            }

            // Update the seeds in as unbiased a way as we can think of
            seed0 = random(seed1 + random(seed0));
            float x = seed0.x;
            seed0.x = seed0.y;
            seed0.y = x;
            seed1 = random(seed0 + random(seed1));
            x = seed1.x;
            seed1.x = seed1.y;
            seed1.y = seed0.x;
            seed0.x = x;
        }

        dst() = resultPixel / (float) _samples;
    }
};

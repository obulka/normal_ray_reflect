// Copyright 2022 by Owen Bulka.
// All rights reserved.
// This file is released under the "MIT License Agreement".
// Please see the LICENSE.md file that should have been included as part
// of this package.


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
inline float4 random(const float4 &seed)
{
    return float4(
        random(seed.x),
        random(seed.y),
        random(seed.z),
        random(seed.w)
    );
}


kernel Random : ImageComputationKernel<ePixelWise>
{
    Image<eRead, eAccessPoint, eEdgeNone> seed; // the input image
    Image<eWrite> dst; // the output image


    param:
        float2 _inclusiveRange;

    local:
        float __rangeLength;


    /**
     * Give the parameters labels and default values.
     */
    void define()
    {
        defineParam(_inclusiveRange, "Inclusive Range", float2(0, 1));
    }


    /**
     * Initialize the local variables.
     */
    void init()
    {
        __rangeLength = _inclusiveRange.y - _inclusiveRange.x;
    }


    /**
     * Compute a random pixel value.
     */
    void process()
    {
        dst() = __rangeLength * random(seed()) + _inclusiveRange.x;
    }
};

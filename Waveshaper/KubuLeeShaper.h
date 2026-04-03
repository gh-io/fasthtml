#pragma once
#include <cmath>
#include <algorithm>

class KubuLeeShaper
{
public:
    void prepare(double sampleRate)
    {
        sr = sampleRate;
        reset();
        updateDcCoeff();
    }

    void reset()
    {
        x1 = 0.0f;
        y1 = 0.0f;
    }

    void setDrive(float value)      { drive = std::max(0.01f, value); }
    void setAsymmetry(float value)  { asymmetry = std::clamp(value, 0.0f, 2.0f); }
    void setMix(float value)        { mix = std::clamp(value, 0.0f, 1.0f); }
    void setOutput(float value)     { outputTrim = value; }

    float processSample(float input)
    {
        const float dry = input;

        // Pre-gain
        float x = input * drive;

        // Keep exponentials stable
        x = std::clamp(x, -6.0f, 6.0f);

        // Original voicing scale
        x *= 0.686306f;

        // Dynamic asymmetry
        const float a = 1.0f + asymmetry * std::exp(-0.75f * std::sqrt(std::fabs(x)));

        // Main shaping
        const float ex   = std::exp(x);
        const float enx  = 1.0f / ex;         // exp(-x)
        const float enxa = std::exp(-x * a);  // exp(-x*a)

        float wet = (ex - enxa) / (ex + enx);

        // DC blocking
        wet = processDcBlock(wet);

        // Output trim
        wet *= outputTrim;

        // Dry/Wet mix
        return dry + mix * (wet - dry);
    }

private:
    float processDcBlock(float input)
    {
        const float y = input - x1 + R * y1;
        x1 = input;
        y1 = y;
        return y;
    }

    void updateDcCoeff()
    {
        // ~20 Hz DC blocker
        R = std::exp(-2.0f * 3.14159265359f * 20.0f / static_cast<float>(sr));
    }

    double sr = 44100.0;

    float drive = 2.0f;
    float asymmetry = 1.0f;
    float mix = 1.0f;
    float outputTrim = 0.8f;

    float x1 = 0.0f;
    float y1 = 0.0f;
    float R = 0.995f;
};

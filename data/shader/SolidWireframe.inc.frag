uniform float lineWidth;
uniform vec4 lineColor;

struct SolidWireframeData
{
    vec4 edgeA;
    vec4 edgeB;
    int configuration;
};

vec4 shadeLine(const in vec4 color, const in SolidWireframeData data)
{
    // Find the smallest distance between the fragment and a triangle edge
    float d;
    if (data.configuration == 0) {
        // Common configuration
        d = min(data.edgeA.x, data.edgeA.y);
        d = min(d, data.edgeA.z);
    }
    else {
        // Handle configuration where screen space projection breaks down
        // Compute and compare the squared distances
        vec2 AF = gl_FragCoord.xy - data.edgeA.xy;
        float sqAF = dot(AF, AF);
        float AFcosA = dot(AF, data.edgeA.zw);
        d = abs(sqAF - AFcosA * AFcosA);

        vec2 BF = gl_FragCoord.xy - data.edgeB.xy;
        float sqBF = dot(BF, BF);
        float BFcosB = dot(BF, data.edgeB.zw);
        d = min(d, abs(sqBF - BFcosB * BFcosB));

        // Only need to care about the 3rd edge for some configurations.
        if (data.configuration == 1 || data.configuration == 2 ||
            data.configuration == 4) {
            float AFcosA0 = dot(AF, normalize(data.edgeB.xy - data.edgeA.xy));
            d = min(d, abs(sqAF - AFcosA0 * AFcosA0));
        }

        d = sqrt(d);
    }

    // Blend between line color and phong color
    float mixVal;
    if (d < lineWidth - 1.0) { mixVal = 1.0; }
    else if (d > lineWidth + 1.0) {
        mixVal = 0.0;
    }
    else {
        float x = d - (lineWidth - 1.0);
        mixVal = exp2(-2.0 * (x * x));
    }

    return mix(color, lineColor, mixVal);
}

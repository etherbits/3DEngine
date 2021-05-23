struct VSOut
{
	float4 color : COLOR;
	float4 pos: SV_POSITION;
};

cbuffer cBuffer
{
	row_major matrix transform;
};

VSOut main(float2 pos : POSITION, float4 color : COLOR)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 0.f, 1.f), transform);
	vso.color = color;
	return vso;
};
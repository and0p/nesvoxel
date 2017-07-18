struct MirrorState
{
	int x;
	int y;
};

cbuffer WorldMatrixBuffer
{
	matrix worldMatrix;
};
cbuffer ViewMatrixBuffer
{
	matrix viewMatrix;
};
cbuffer ProjectionMatrixBuffer
{
	matrix projectionMatrix;
};

cbuffer MirrorBuffer
{
	MirrorState mirrorState;
};

float4 main(float4 pos : POSITION) : SV_POSITION
{
	float4 output;
	output = mul(pos, worldMatrix);
	output = mul(output, viewMatrix);
	output = mul(output, projectionMatrix);
	return output;
}
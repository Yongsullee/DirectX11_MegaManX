#include "stdafx.h"
#include "Line.h"
#include "Systems/Device.h"

ID3D11Buffer* vertexBuffer;
ID3D11InputLayout* inputLayout;
D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
{
	{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA},
	{"COLOR",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
};

Line::Line(wstring shaderFile, D3DXVECTOR2 start, D3DXVECTOR2 end, int LineNum_key)
	: LineNum(LineNum_key)
	, shaderFile(shaderFile)
	, bDrawCollision(false)
	, startPos(start), endPos(end)
{
	CreateBuffer(shaderFile);

	MiddlePos = D3DXVECTOR2(endPos.x - (endPos.x - startPos.x) * 1 / 2, endPos.y - (endPos.y - startPos.y) * 1 / 2);

	Position(end - start);
	setAngle(start, end);
	SetGradient(start, end);

	Scale(D3DXVECTOR2(1, 1));

	UpdateWorld();
}

Line::~Line()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);
}


void Line::setStartPos(D3DXVECTOR2 sPos)
{
	startPos = sPos;

	CreateBuffer(shaderFile);
	Position(endPos - sPos);
	MiddlePos = D3DXVECTOR2(endPos.x - (endPos.x - sPos.x) * 1 / 2, endPos.y - (endPos.y - sPos.y) * 1 / 2);

	setAngle(startPos, endPos);
	SetGradient(startPos, endPos);

	Scale(D3DXVECTOR2(1, 1));

	UpdateWorld();

}

void Line::setEndPos(D3DXVECTOR2 ePos)
{
	endPos = ePos;

	CreateBuffer(shaderFile);
	Position(ePos - startPos);

	setAngle(startPos, endPos);
	SetGradient(startPos, endPos);

	MiddlePos = D3DXVECTOR2(ePos.x - (endPos.x - startPos.x) * 1 / 2, ePos.y - (endPos.y - startPos.y) * 1 / 2);

	Scale(D3DXVECTOR2(1, 1));

	UpdateWorld();
}

void Line::SetGradient(D3DXVECTOR2 & start, D3DXVECTOR2 & end)
{
	float num = end.y - start.y;
	float denom = end.x - start.x;

	gradient = num / denom;
}

void Line::setAngle(D3DXVECTOR2 sPos, D3DXVECTOR2 ePos)
{
	double dx = ePos.x - sPos.x;
	double dy = ePos.y - sPos.y;

	angle = atan2(dy, dx) * (180.0 / Math::PI);
}

void Line::CreateBuffer(wstring shaderFile)
{
	bDrawBound = false; 

	shader = new Shader(shaderFile);
	boundShader = new Shader(Shaders + L"14_Bounding.fx"); 

	Rotation(0, 0, 0);
	D3DXMatrixIdentity(&world);

	vertices[0].Position = D3DXVECTOR3(startPos.x, startPos.y, 0.0f);
	vertices[1].Position = D3DXVECTOR3(endPos.x - (endPos.x - startPos.x) * 2, endPos.y - (endPos.y - startPos.y) * 2, 0.0f); 

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 2;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;


		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);

		assert(SUCCEEDED(hr));
	}

	/// Bound Vertex
	boundVertices[0].Position = D3DXVECTOR3(startPos.x, startPos.y + 1, 0.0f);
	boundVertices[1].Position = D3DXVECTOR3(startPos.x, startPos.y - 1, 0.0f);
	boundVertices[2].Position = D3DXVECTOR3(endPos.x - (endPos.x - startPos.x) * 2, endPos.y - (endPos.y - startPos.y) * 2 - 1, 0.0f);
	boundVertices[3].Position = D3DXVECTOR3(endPos.x - (endPos.x - startPos.x) * 2, endPos.y - (endPos.y - startPos.y) * 2 + 1, 0.0f);
	boundVertices[4].Position = D3DXVECTOR3(startPos.x, startPos.y + 1, 0.0f);

	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(BoundVertex) * 5;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = boundVertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &boundVertexBuffer);

		assert(SUCCEEDED(hr));
	}
}

void Line::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	setAngle(startPos, endPos);

	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

	boundShader->AsMatrix("View")->SetMatrix(V);  
	boundShader->AsMatrix("Projection")->SetMatrix(P);
}

void Line::Render()
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	shader->Draw(0, 0, 0);

	if (bDrawBound == true)
	{
		stride = sizeof(BoundVertex);
		offset = 0;

		DeviceContext->IASetVertexBuffers(0, 1, &boundVertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		boundShader->Draw(0, 0, 0);
	}

}

void Line::Position(float x, float y)
{
	Position(D3DXVECTOR2(x, y));
}

void Line::Position(D3DXVECTOR2 & vec)
{
	position = vec;

	UpdateWorld();
}

void Line::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Line::Scale(D3DXVECTOR2 & vec)
{
	scale = vec;

	UpdateWorld();
}

void Line::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3(x, y, z));
}

void Line::Rotation(D3DXVECTOR3 & vec)
{
	rotation = vec;

}

void Line::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void Line::RotationDegree(D3DXVECTOR3 & vec)
{
	vec.x = Math::ToRadian(vec.x);
	vec.y = Math::ToRadian(vec.y);
	vec.z = Math::ToRadian(vec.z);

	Rotation(vec);
}

D3DXVECTOR3 Line::RotationDegree()
{
	D3DXVECTOR3 vec;
	vec.x = Math::ToDegree(rotation.x);
	vec.y = Math::ToDegree(rotation.y);
	vec.z = Math::ToDegree(rotation.z);

	return vec;
}

void Line::UpdateWorld()
{
	D3DXMATRIX W, S, T;

	D3DXMatrixScaling(&S, scale.x, scale.y, 1);
	D3DXMatrixTranslation(&T, position.x, position.y, 0);

	W = S * T;

	shader->AsMatrix("World")->SetMatrix(W);
	boundShader->AsMatrix("World")->SetMatrix(W);
}

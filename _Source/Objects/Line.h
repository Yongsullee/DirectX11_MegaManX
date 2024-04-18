#pragma once

struct BoundVertex;

class  Line {

public:
	Line(wstring shaderFile, D3DXVECTOR2 start, D3DXVECTOR2 end, int LineNum_key);
	~Line();

	virtual void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

public:
	void Position(float x, float y);
	void Position(D3DXVECTOR2& vec);
	D3DXVECTOR2 Position() { return position; }

	void Scale(float x, float y);
	void Scale(D3DXVECTOR2& vec);
	D3DXVECTOR2 Scale() { return scale; }

	void Rotation(float x, float y, float z);
	void Rotation(D3DXVECTOR3& vec);
	D3DXVECTOR3 Rotation() { return rotation; }

	void RotationDegree(float x, float y, float z);
	void RotationDegree(D3DXVECTOR3& vec);
	D3DXVECTOR3 RotationDegree();

public:
	void DrawBound(bool val) { bDrawBound = val; }

	void DrawCollision(bool val) { bDrawCollision = val; }
	bool GetDrawCollision() { return bDrawCollision; }

public:
	void setStartPos(D3DXVECTOR2 sPos);
	void setEndPos(D3DXVECTOR2 ePos);
	D3DXVECTOR2& getStartPos() { return startPos; }
	D3DXVECTOR2& getEndPos() { return endPos; }

public:
	void SetGradient(D3DXVECTOR2& start, D3DXVECTOR2& end);
	float Gradient() { return gradient; }

public:
	float & getAngle() { return angle; };
	void setAngle(D3DXVECTOR2 sPos, D3DXVECTOR2 ePos);

private:
	void CreateBuffer(wstring shaderFile);
	void UpdateWorld();

private:
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR3 rotation;
	float angle;
	float gradient;

private:
	wstring shaderFile;

	Shader* shader;
	ID3D11Buffer* vertexBuffer;

private:

	D3DXVECTOR2 startPos;
	D3DXVECTOR2 endPos;
	D3DXVECTOR2 MiddlePos;
	int LineNum;

private:
	bool bDrawBound;
	bool bDrawCollision;

	Shader* boundShader;
	ID3D11Buffer* boundVertexBuffer;

	D3DXMATRIX world;

private:
	struct Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR3 Color;
	};
	struct BoundVertex
	{
		D3DXVECTOR3 Position;
	};

	Vertex vertices[2];
	BoundVertex boundVertices[5];
};
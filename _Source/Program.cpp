#include "stdafx.h"
#include "Systems/Device.h"
#include "Scenes/Scene.h"
#include "Viewer/Freedom.h"
#include "Viewer/Following.h"
#include "Scenes/PlayStage.h"
#include "Objects/Player.h"

SceneValues* values;
Player* player;
vector<Scene* > scenes;

void InitScene()
{
	values = new SceneValues;
	values->MainCamera = nullptr;
	D3DXMatrixIdentity(&values->Projection);

	scenes.push_back(new PlayStage(values));
}

void DestroyScene()
{
	for (Scene* scene : scenes)
		SAFE_DELETE(scene);

	SAFE_DELETE(values);
}

D3DXVECTOR2 horizontal(0, 800), vertical(0, 600);

void Update()
{
	for (Scene* scene : scenes)
		scene->Update();
}


void Render()
{
	D3DXCOLOR bgColor = D3DXCOLOR(0, 0, 0, 1);
	DeviceContext->ClearRenderTargetView(RTV, (float*)bgColor);
	{
		for (Scene* scene : scenes)
			scene->Render();
	}
	ImGui::Render();

	DirectWrite::GetDC()->BeginDraw();
	{
		wstring text = L"";

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 600;
		rect.bottom = 20;

		text = L"Frame Per Second : " + to_wstring((int)ImGui::GetIO().Framerate);
		DirectWrite::RenderText(text, rect);

		rect.top += 20;
		rect.bottom += 20;

		text = L"Camera Position : ";
		text += to_wstring((int)values->MainCamera->Position().x);
		text += L", ";
		text += to_wstring((int)values->MainCamera->Position().y);


		DirectWrite::RenderText(text, rect);
	}

	DirectWrite::GetDC()->EndDraw();


	SwapChain->Present(0, 0);
}
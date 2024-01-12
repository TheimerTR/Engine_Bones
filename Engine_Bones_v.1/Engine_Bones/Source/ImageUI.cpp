#include "ImageUI.h"
#include "Application.h"
#include "ImporterTexture.h"
#include "TextureManager.h"
#include "CanvasUI.h"

#include "External/Glew/include/glew.h"
#include "External/ImGui/imgui.h"
#include "External/ImGui/backends/imgui_impl_sdl2.h"
#include "External/ImGui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include "External/SDL/include/SDL_opengl.h"
#endif

ImageUI::ImageUI(UI_Type type, GameObject* gameObject, uint width, uint heigt, uint PosX, uint PosY, const char* imagePath) : ComponentUI(type, gameObject, width, heigt, PosX, PosY, imagePath)
{
	gmAtached = gameObject;

	image_Path = new Texture();

	scaleToCanvas = false;
}

ImageUI::~ImageUI()
{

}
void ImageUI::ShowInfo()
{
	ComponentMaterial* mat = dynamic_cast<ComponentMaterial*>(gmAtached->GetComponentGameObject(ComponentType::MATERIAL));

	if (mat->texture != nullptr)
	{
		if (mat->texture->TextureID != 0)
		{
			image_Path = mat->texture;
		}

		color[3] = 0;
	}
	else
	{
		color[3] = 255;
	}

	if(scaleToCanvas)
	{
		CanvasUI* canv = dynamic_cast<CanvasUI*>(app->editor->Canvas->GetComponentGameObject(ComponentType::CANVAS));
		widthPanel = canv->widthPanel;
		heigthPanel = canv->heigthPanel;
	}

	if (ImGui::TreeNode("Image"))
	{
		ImGui::Checkbox("Active", &gmAtached->isActive);

		ImGui::Text("Actual Image:");

		if (mat->texture != nullptr)
		{
			if (mat->texture->TextureID != 0)
			{
				ImGui::Image((void*)mat->texture->TextureID, ImVec2(mat->texture->imageWidth, mat->texture->imageHeight));
			}
			else
			{
				ImGui::Text("No image selected in Texture Component");
			}
		}

		ImGui::Separator();
		ImGui::Checkbox("Scale image to canvas", &scaleToCanvas);

		ImGui::TreePop();
	}
}
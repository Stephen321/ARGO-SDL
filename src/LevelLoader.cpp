#include "LevelLoader.h"

void LevelLoader::LoadJson(const char* filename, RenderSystem& renderSystem, std::map<TextureID, SDL_Texture*>& _textureHolder)
{
	Factory factory = Factory();
	FILE* fp = NULL;
	fopen_s(&fp, filename, "rb");
	Document doc;
	if (fp != NULL)
	{
		char readBuffer[65536];
		FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		doc.ParseStream(is);
	}
	else
	{
		std::cout << "ERROR LOADING : " << filename << std::endl;
		system("PAUSE");
		exit(0);
	}
	fclose(fp);

	int tileWidth = doc["tilewidth"].GetInt();
	int tileHeight = doc["tileheight"].GetInt();

	const Value& layerArray = doc["layers"];

	const Value& backgroundLayer = layerArray[0u];
	int backgroundLayerWidth = backgroundLayer["width"].GetInt();
	int backgroundLayerHeight = backgroundLayer["height"].GetInt();

	const Value& backgroundDataArray = backgroundLayer["data"];

	int index = 0;
	for (int y = 0; y < backgroundLayerHeight; y++)
	{
		for (int x = 0; x < backgroundLayerWidth; x++)
		{
			switch (backgroundDataArray[index].GetInt())
			{
			case 1:

				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
			}

			renderSystem.AddEntity(factory.CreateTile(_textureHolder[TextureID::Wall], x * tileWidth, y * tileHeight, tileWidth, tileHeight));
			index++;
		}
	}



	const Value& entitylayer = layerArray[1];
	int entityLayerWidth = entitylayer["width"].GetInt();
	int entityLayerHeight = entitylayer["height"].GetInt();

	const Value& entityDataArray = entitylayer["data"];

	index = 0;
	for (int y = 0; y < entityLayerHeight; y++)
	{
		for (int x = 0; x < entityLayerWidth; x++)
		{
			switch (entityDataArray[index].GetInt())
			{
			case 4:

				break;
			case 5:
				break;
			case 6:
				renderSystem.AddEntity(factory.CreateWall(_textureHolder[TextureID::Wall], x * tileWidth, y * tileHeight, tileWidth, tileHeight));
				break;
			default:
				break;
			}

			
			index++;
		}
	}
}

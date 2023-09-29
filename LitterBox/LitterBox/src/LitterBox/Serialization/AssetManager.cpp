#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "AssetManager.h"
#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)


namespace LB
{
    AssetManager* ASSETMANAGER = nullptr;

   
    
    AssetManager::AssetManager()
    {
        if (!ASSETMANAGER)
			ASSETMANAGER = this;
		else
			std::cerr << "Asset Manager already exists" << std::endl;

        LoadSounds();
        LoadTextures();
        LoadPrefabs();
    }
    /*!************************************************************************
     * \brief This function creates and ADDS a texture to the map with the appropriate ID
     * 
     * \param[in] fileName 
     * \return TextureData* 
     **************************************************************************/
    TextureData* AssetManager::CreateTexture(const std::string &fileName)
    {
        TextureData* cachedTexture = new TextureData();

        stbi_set_flip_vertically_on_load(1);
        //In the future, this will change to the a map from the asset manager
        cachedTexture->stbBuffer = stbi_load(fileName.c_str(),&cachedTexture->width,&cachedTexture->height,&cachedTexture->fluff,4);
        if(!cachedTexture->stbBuffer)
        {
            std::cerr<< "Texture filepath :" <<
            fileName << "NOT FOUND!\n";
            
        } 
        // else //if it doesn't exist, we set some funny png so we know
        // {
        //     std::string funnyPng{"funny.png"};
        //     cachedTexture->stbBuffer = stbi_load(funnyPng.c_str(),&cachedTexture->width,&cachedTexture->height,&cachedTexture->fluff,4);
        // }
        glCreateTextures(GL_TEXTURE_2D, 1, &cachedTexture->id);
	    glTextureStorage2D(cachedTexture->id, 1, GL_RGBA8, cachedTexture->width, cachedTexture->height);
	    glTextureSubImage2D(cachedTexture->id, 0, 0, 0, cachedTexture->width, cachedTexture->height,
		GL_RGBA, GL_UNSIGNED_BYTE, cachedTexture->stbBuffer);

        // textureDataMap[textureID] = cachedTexture;
	    stbi_image_free(cachedTexture->stbBuffer);


	    glTextureParameteri(cachedTexture->id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTextureParameteri(cachedTexture->id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        return cachedTexture;
    }
    bool AssetManager::AddTexture(const std::string &fileName, const std::string &textureName)
    {
        if(Textures.size() >= 32)
        {
            //Max textures reached
            return false;
        }
        int i{0};
        for(; i<32; ++i)
        {
            if(!TextureSlots[i]) break;
        }
        Textures.emplace(std::make_pair(textureName,std::make_pair(CreateTexture(fileName),i)));
        //textures.emplace(std::make_pair(name, std::make_pair(LB::ASSETMANAGER->CreateTexture(file_path), i)));

        //getting the pair then the texture id
	    glBindTextureUnit(0 + i, Textures.find(textureName)->second.first->id);
	    TextureSlots[i] = true;

	return true;
    }
    void AssetManager::LoadTextures()
    {
        TextureFilePaths[TextureNames::NONE];    
        AddTexture("../Assets/Textures/walk.png", "run");
        AddTexture("../Assets/Textures/test3.png", "pine");
        AddTexture("../Assets/Textures/cat.png", "cat");
        AddTexture("../Assets/Textures/Environment_Background.png", "bg");


    }
    void AssetManager::LoadSounds()
    {
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("../Assets/Sounds/Enemy hurt.wav", FMOD_DEFAULT, nullptr, &sampleSound);
        if (AUDIOMANAGER->result != FMOD_OK) std::cout << "UNABLE TO LOAD AHH SOUND!!\n";
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("../Assets/Sounds/EXPLOSION.wav", FMOD_DEFAULT, nullptr, &explosionSound);
        if (AUDIOMANAGER->result != FMOD_OK) std::cout << "UNABLE TO LOAD EXPLODE SOUND!!\n";
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("../Assets/Sounds/Oof.wav", FMOD_DEFAULT, nullptr, &ahhSound);
        if (AUDIOMANAGER->result != FMOD_OK) std::cout << "UNABLE TO LOAD OOF SOUND!!\n";

    }

    void AssetManager::LoadPrefabs()
    {
        //PineappleObject = FACTORY->SpawnGameObject({ "CPRender","CPRigidBody" });
        PineappleObject = FACTORY->CreateGameObject();
        //AvatarObject = FACTORY->CreateGameObject();
        //std::cout <<"Pineapple component size : " << PineappleObject->GetComponents().size() << '\n';

        JSONSerializer stream;
        stream.DeserializeFromFile("../Assets/Prefabs/pineapple", *PineappleObject);

        //stream.DeserializeFromFile("../Assets/Prefabs/apple",*AvatarObject);
    }

}

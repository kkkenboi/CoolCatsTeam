/*!************************************************************************
 \file				AssetManager.cpp
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains the implementation of AssetManager 
                    functions. 

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

//CRUCIAL FOR GRAPHICS (Textures)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "AssetManager.h"  
#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)


namespace LB
{
    //Global pointer for AssetManager Singleton
    AssetManager* ASSETMANAGER = nullptr;
    
    /*!************************************************************************
     * \brief Construct a new Asset Manager:: Asset Manager object
     * On initialise, AssetManager will load all the data!
     * (TODO : Maybe queue it to load AFTER all other systems have loaded...)
     **************************************************************************/
    AssetManager::AssetManager()
    {
        //Basic singleton stuff
        if (!ASSETMANAGER)
			ASSETMANAGER = this;
		else
			std::cerr << "Asset Manager already exists" << std::endl;

        //Load all assets here
        LoadSounds();
        LoadTextures();
        LoadPrefabs();
    }
    /*!************************************************************************
     * \brief This function creates and ADDS a texture to the map with the appropriate ID
     * 
     * \return TextureData* texture data pointer 
     **************************************************************************/
    TextureData* AssetManager::CreateTexture(const std::string &fileName)
    {
        //This might be dangerous, research sharedpointers 
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

    /*!***********************************************************************
    * \brief 
    * Adds the texture from a file to the texture map
    * The texture map consists of TEXTUREDATA : ID
    **************************************************************************/  
    bool AssetManager::AddTexture(const std::string &fileName, const std::string &textureName)
    {
        //We need to check if we have too many textures
        if(Textures.size() >= 32)
        {
            //Max textures reached
            return false;
        }
        //This lets us keep track of the id
        int i{0};
        for(; i<32; ++i)
        {
            if(!TextureSlots[i]) break;
        }
        //The texture is tagged to the ID, which is then tagged to the texture name
        // e.g "cat" : "<catTextureData,ID>" where cat is the name of the texture.
        Textures.emplace(std::make_pair(textureName,std::make_pair(CreateTexture(fileName),i)));
        //textures.emplace(std::make_pair(name, std::make_pair(LB::ASSETMANAGER->CreateTexture(file_path), i)));

        //getting the pair then the texture id
	    glBindTextureUnit(0 + i, Textures.find(textureName)->second.first->id);
	    TextureSlots[i] = true;

	    return true;
    }

    /*!***********************************************************************
    * \brief Loads all textures from file paths into the relevant maps
    * 
    **************************************************************************/
    void AssetManager::LoadTextures()
    {
        //TODO File path prefix to be set by enum
        //TODO Texture names to be tagged to enum
        //TODO Texture IDs to be stored in json instead
        TextureFilePaths[TextureNames::NONE];    
        AddTexture("../Assets/Textures/walk.png", "run");
        AddTexture("../Assets/Textures/test3.png", "pine");
        AddTexture("../Assets/Textures/cat.png", "cat");
        AddTexture("../Assets/Textures/Environment_Background.png", "bg");
    }

    /*!***********************************************************************
    * \brief Loads all sounds and creates an instance of them for use
    * 
    **************************************************************************/
    void AssetManager::LoadSounds()
    {
        //! NOTE!!! LOADING AUDIO IS AN EXTREMLY EXPENSIVE PROCESS!!!
        //* In the future, We might want to put this into another thread to load it async
        //TODO File path prefix to be set by enum
        //TODO Audio names to be tagged to enum
        //TODO Audio IDs to be stored in json
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("../Assets/Sounds/Enemy hurt.wav", FMOD_DEFAULT, nullptr, &sampleSound);
        if (AUDIOMANAGER->result != FMOD_OK) std::cout << "UNABLE TO LOAD AHH SOUND!!\n";
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("../Assets/Sounds/EXPLOSION.wav", FMOD_DEFAULT, nullptr, &explosionSound);
        if (AUDIOMANAGER->result != FMOD_OK) std::cout << "UNABLE TO LOAD EXPLODE SOUND!!\n";
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("../Assets/Sounds/Oof.wav", FMOD_DEFAULT, nullptr, &ahhSound);
        if (AUDIOMANAGER->result != FMOD_OK) std::cout << "UNABLE TO LOAD OOF SOUND!!\n";

    }

    /*!***********************************************************************
    * \brief Loads all prefabs from their json data and creates an instance of them
    * (TODO : Load instances directly into the GOManager!)
    **************************************************************************/
    void AssetManager::LoadPrefabs()
    {
        //TODO Some refactoring for creating empty game objects
        //PineappleObject = FACTORY->SpawnGameObject({ "CPRender","CPRigidBody" });
        PineappleObject = FACTORY->CreateGameObject();
        //AvatarObject = FACTORY->CreateGameObject();
        //std::cout <<"Pineapple component size : " << PineappleObject->GetComponents().size() << '\n';
        //* Don't touch this, it works!
        JSONSerializer stream;
        stream.DeserializeFromFile("../Assets/Prefabs/pineapple", *PineappleObject);

        //stream.DeserializeFromFile("../Assets/Prefabs/apple",*AvatarObject);
    }

}

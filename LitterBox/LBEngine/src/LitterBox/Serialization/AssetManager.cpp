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
        SetSystemName("AssetManager System"); 
        //Basic singleton stuff
        if (!ASSETMANAGER)
			ASSETMANAGER = this;
		else
			std::cerr << "Asset Manager already exists" << std::endl;

    }
    void AssetManager::Initialize()
    {
        DebuggerLog("Assetmanager is initializing");
        Textures["none"];
        //Load all assets here
        LoadSounds();
        LoadTextures();
        LoadPrefabs();
        LoadKeyBinds();
    }
    /*!************************************************************************
     * \brief This function creates and ADDS a texture to the map with the appropriate ID
     * 
     * \return TextureData* texture data pointer 
     **************************************************************************/
    std::shared_ptr<TextureData> AssetManager::CreateTexture(const std::string &fileName)
    {
        //This might be dangerous, research sharedpointers 
        //TextureData* cachedTexture = new TextureData();
        std::shared_ptr<TextureData> cachedTexture = std::make_shared<TextureData>();

        stbi_set_flip_vertically_on_load(1);
        //In the future, this will change to the a map from the asset manager
        cachedTexture->stbBuffer = stbi_load(fileName.c_str(),&cachedTexture->width,&cachedTexture->height,&cachedTexture->fluff,4);
        if(!cachedTexture->stbBuffer)
        {
            DebuggerLogErrorFormat("Texture filepath %s not found!", fileName);
            //std::string funnyPng{"Assets/Textures/despair.png"};
            //cachedTexture->stbBuffer = stbi_load(funnyPng.c_str(),&cachedTexture->width,&cachedTexture->height,&cachedTexture->fluff,4);
            cachedTexture->id = -1;
            return cachedTexture;
        } 
         //else //if it doesn't exist, we set some funny png so we know
         //{
         //}
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
        //TODO have a check for editor vs game
        //      if editor then start from 0 otherwise start from 1
        int i{1}; //START FROM 1 BECAUSE texture unit 0 will be reserved for ImGUI texture
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

    bool AssetManager::RemoveTexture(const std::string& name)
    {
        if (Textures.find(name) == Textures.end())
        {
            DebuggerLogErrorFormat("Unable to find %s in Textures!", name);
            return false;
        }
        TextureSlots[Textures.find(name)->second.second] = false;
        Textures.erase(name);   //since it's a shared pointer it shoulddddd deallocate properly right???
        return true;
    }

    void AssetManager::FlushTextures()
    {
        Textures.clear();
        for (int i{ 0 }; i < 32; ++i)
        {
            TextureSlots[i] = false;
        }
    }

    const int AssetManager::GetTextureIndex(const std::string& name) const
    {
        if (Textures.find(name) == Textures.end())
        {
            DebuggerLogWarning("Texture " + name + " can't be found!");
            return -1;  //return an invalid index for a graceful fail
        }
        if (name == "none")
        {
            return 0;
        }
        return Textures.find(name)->second.second;
    }
    const std::string AssetManager::GetTextureName(const int& index) const
    {
        if (index == 0)
        {
            return "none";
        }
        for (const auto& elem : Textures)
        {
            if (elem.second.second == index)
            {
                return elem.first;
            }
        }
        DebuggerLogWarning("Texture index : " + std::to_string(index) + " can't be found!");
        return "";
    }

    /*!***********************************************************************
    * \brief Loads all textures from file paths into the relevant maps
    * 
    **************************************************************************/
    void AssetManager::LoadTextures()
    {
        //First we get the json file containing all the texture paths : texture names
        Document _jsonFile = JSONSerializer::GetJSONFile("Editor/Jason/TextureFilePaths.json");
        //then we loop through the json file and add each texture with its corresponding name
        for (Value::ConstMemberIterator itr = _jsonFile.MemberBegin();
            itr != _jsonFile.MemberEnd(); ++itr)
        {
            if (!itr->name.IsString())
            {
                DebuggerLogError("Invalid Texture file path! Needs to be std::string!");
                return;
            }
            if (!itr->value.IsString())
            {
                DebuggerLogError("Invalid Texture name! Needs to be std::string!");
                return;
            }

            AddTexture(itr->name.GetString(), itr->value.GetString());
            //The texture filepaths map is for us to keep track in our json
            TextureFilePaths[itr->name.GetString()] = itr->value.GetString();
        }
        //Incase there are any changes, we just regenerate the file
        GenerateTextureFilePathsJson();


        //AddTexture("../Assets/Textures/despair.png", "despair");
        //TextureFilePaths["../Assets/Textures/despair.png"] = "despair";
        //AddTexture("../Assets/Textures/walk.png", "run");
        //TextureFilePaths["../Assets/Textures/walk.png"] = "run";
        //AddTexture("../Assets/Textures/test3.png", "pine");
        //TextureFilePaths["../Assets/Textures/test3.png"] = "pine";
        //AddTexture("../Assets/Textures/cat.png", "cat");
        //TextureFilePaths["../Assets/Textures/cat.png"] = "cat";
        //AddTexture("../Assets/Textures/Environment_Background.png", "bg");
        //TextureFilePaths["../Assets/Textures/Environment_Background.png"] = "bg";
        //GenerateTextureFilePathsJson();
        

        //This is to test if the get texture name function works
        //std::cout << "Test GetTextName : " << GetTextureName(GetTextureIndex("bg")) << '\n';
    }

    void AssetManager::GenerateTextureFilePathsJson()
    {
        JSONSerializer stream;
        Document _jsonFile;
        Document::AllocatorType& alloc = _jsonFile.GetAllocator();
        _jsonFile.SetObject();
        for (const auto& elem : TextureFilePaths)
        {
            Value key(elem.first.c_str(), alloc);
            Value val(elem.second.c_str(), alloc);
            _jsonFile.AddMember(key, val, alloc);
        }
        
        stream.SaveToJSON("Editor/Jason/TextureFilePaths.json", _jsonFile);
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
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("Assets/Audio/Enemy hurt.wav", FMOD_DEFAULT, nullptr, &sampleSound);
        if (AUDIOMANAGER->result != FMOD_OK) std::cout << "UNABLE TO LOAD AHH SOUND!!\n";
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("Assets/Audio/EXPLOSION.wav", FMOD_DEFAULT, nullptr, &explosionSound);
        if (AUDIOMANAGER->result != FMOD_OK) std::cout << "UNABLE TO LOAD EXPLODE SOUND!!\n";
        AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound("Assets/Audio/Oof.wav", FMOD_DEFAULT, nullptr, &ahhSound);
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
        //JSONSerializer stream;
        //stream.DeserializeFromFile("Assets/Prefabs/pineapple", *PineappleObject);
        JSONSerializer::DeserializeFromFile("pineapple.json", *PineappleObject);

        //stream.DeserializeFromFile("../Assets/Prefabs/apple",*AvatarObject);
    }
    std::string AssetManager::KeyCodeToString(KeyCode KeyCodeToFind) 
    {
        for (const auto& elem : KeyCodeTable)
        {
            if (elem.second == static_cast<int>(KeyCodeToFind))
            {
                return elem.first;
            }
        }
        DebuggerLogError("Unable to find Keycode " + std::to_string(static_cast<int>(KeyCodeToFind)) + " in KeyCodeTable!");
        return "";
    }
    KeyCode AssetManager::StringToKeyCode(std::string keycode)
    {
        std::map<std::string, int>::iterator iter = KeyCodeTable.find(keycode);
        if (iter == KeyCodeTable.end())
        {
            DebuggerLogError("Unable to find Keycode " + keycode + " in KeyCodeTable!");
            return KeyCode();
        }
        return static_cast<KeyCode>(iter->second);
    }
    void AssetManager::LoadKeyBinds()
    {
        //We need to load the keycode table from json first
        LoadKeyCodeTable();
        Document _jsonFile = JSONSerializer::GetJSONFile("Editor/Jason/KeyBinds.json");
        //Then we get the keybinds json and go through each member
        for (Value::ConstMemberIterator itr = _jsonFile.MemberBegin();
            itr != _jsonFile.MemberEnd(); ++itr)
        {
            if (!itr->name.IsString())
            {
                DebuggerLogError("Invalid Keybind name! Needs to be std::string!");
                return;
            }
            if (!itr->value.IsString())
            {
                DebuggerLogError("Invalid Key code name! Needs to be std::string!");
                return;
            }
            KeyBindNameMap[itr->name.GetString()] = itr->value.GetString();
        }
        //We regenerate the file again just incase the user made any changes
        GenerateKeyBindsJson();
    }
    void AssetManager::LoadKeyCodeTable()
    {
        Document _jsonFile = JSONSerializer::GetJSONFile("Editor/Jason/KeyCodeTable.json");
        for (Value::ConstMemberIterator itr = _jsonFile.MemberBegin();
            itr != _jsonFile.MemberEnd(); ++itr)
        {
            if (!itr->name.IsString())
            {
                DebuggerLogError("Invalid KeyCode name! Needs to be std::string!");
                return;
            }
            if (!itr->value.IsInt())
            {
                DebuggerLogError("Invalid KeyCode! Needs be int!");
                return;
            }
            KeyCodeTable[itr->name.GetString()] = itr->value.GetInt();
        }
    }
    void AssetManager::GenerateKeyBindsJson()
    {
        Document _jsonFile;
        Document::AllocatorType& alloc = _jsonFile.GetAllocator();
        _jsonFile.SetObject();
        for (const auto& elem : KeyBindNameMap)
        {
            Value key(elem.first.c_str(), alloc);
            Value val(elem.second.c_str(), alloc);
            _jsonFile.AddMember(key, val, alloc);
        }
        JSONSerializer::SaveToJSON("Editor/Jason/KeyBinds.json", _jsonFile);
        //To test if the keycode and string to keycode function works
        //std::cout << KeyCodeToString(StringToKeyCode("KEY_J")) << '\n';
    }

    //Helper function to generate the keycode table
    void AssetManager::GenerateKeyCodeTable()
    {
        //This generates the keycode mapping 
        //So that our keybindmaps are human readable
        //Basic json serializer stuff
        Document _jsonFile;
        Document::AllocatorType& alloc = _jsonFile.GetAllocator();
        _jsonFile.SetObject();
        //We need to populate the map first before we can add it to the json
        //Mouse buttons
        KeyCodeTable["KEY_MOUSE_1"] = 0;
        KeyCodeTable["KEY_MOUSE_2"] = 1;
        //Numpad buttons
        int i = GLFW_KEY_0;
        for (int j{ 0 }; j < 10; ++j)
        {
            std::string keyName = "KEY_" + std::to_string(j);
            KeyCodeTable[keyName] = i++;
        }
        //Alpha Keys
        i = GLFW_KEY_A;
        for (int k{ 0 }; k < 26; ++k)
        {
            char character = (char)(i);
            std::string keyName = "KEY_";
            keyName += character;
            KeyCodeTable[keyName] = i++;
        }
        //then we save it to json
        //personally, I feel that we prolly don't have to do this
        //because we could probably skip a whole I/O operation by just doing the above
        for (const auto& elem : KeyCodeTable)
        {
            Value key(elem.first.c_str(), alloc);
            _jsonFile.AddMember(key, elem.second, alloc);
        }
        JSONSerializer::SaveToJSON("Editor/Jason/KeyCodeTable.json", _jsonFile);
    }
}

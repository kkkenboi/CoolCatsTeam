/*!************************************************************************
 \file				AssetManager.h
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				The AssetManager handles the loading of data from file
                    and stores them in maps of instances for use througout
                    the engine. This allows users to only have to load 
                    data once and then create copies of the data.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
//FOR SERIALIZATION
#include "Serializer.h"
#include "LitterBox/Core/System.h" //AssetManager is a system
#pragma warning(push, 0)
//FOR AUDIO FILES
#include "../../dependencies/FMOD/core/inc/fmod.hpp"
#pragma warning(pop)
//GAME OBJECT COMPONENT RELATED
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Renderer/Renderer.h"

#include <memory>
#include "LitterBox/Engine/Input.h"


namespace LB
{   
    /*!***********************************************************************
     * \brief 
     * Struct that holds texture data. (essentially the same as textures)
     **************************************************************************/
    struct TextureData
    {
        //Ctor for the texture data, with the stuff initialised to 0
        TextureData():id{0},stbBuffer{},width{0},height{0},fluff{0}{};
        //Copy Ctor for the texture data
        TextureData(const TextureData& texture):id{texture.id},stbBuffer{texture.stbBuffer},width{texture.width},height{texture.height},fluff{texture.fluff}{};
        //Dtor for the texture data
        ~TextureData(){ glDeleteTextures(1, &id); }
        //Member variables
        unsigned int id;
        unsigned char* stbBuffer;
        int width,height,fluff;
    };

   


    /*!***********************************************************************
     * \brief 
     * Enum name for asset ID to asset name
     **************************************************************************/
    enum class ASSETTYPE
    {
        TEXTURE,
        AUDIO,
        PREFAB
    };

    /*!***********************************************************************
     * \brief 
     * Asset Manager system that loads assets during engine initialisation
     * This class will also handle the file paths in the future
     **************************************************************************/
    class AssetManager : public ISystem
    {
        public:
        /*!***********************************************************************
         * \brief Construct a new Asset Manager object
         * 
         **************************************************************************/
        AssetManager();        


        void Initialize() override;
        /*!***********************************************************************
         * \brief 
         * Create a Texture object 
         **************************************************************************/
        std::shared_ptr<TextureData> CreateTexture(const std::string& fileName);
        void ImportAssets();
        /*!***********************************************************************
         * \brief 
         * Adds the texture from a file to the texture map
         * The texture map consists of TEXTUREDATA : ID
         **************************************************************************/
        bool AddTexture(const std::string& fileName, const std::string& textureName);
        bool RemoveTexture(const std::string& name);
        void FlushTextures();
        //The map then maps the name to said pair like so:
        //"SpriteName" : texturePair

        /*!***********************************************************************
         * \brief 
         * Texture map that maps texture names to their texture pairs like so:
         * "SpriteName" : texturePair
         **************************************************************************/
        std::map<std::string, std::pair<std::shared_ptr<TextureData>,int>> Textures;

        //Maps enums to the file paths
        // 0 (NONE) : "../Assets/cat.png"

        /*!***********************************************************************
         * \brief 
         * Texture file path map that maps texture enum names to file paths
         * "../Assets/Textures/name.png" : "name"
         **************************************************************************/
        std::map<std::string, std::string> TextureFilePaths;



        /*!***********************************************************************
         * \brief 
         * Get the Texture Index object for the renderer
         **************************************************************************/
        const int GetTextureUnit(const std::string& name) const;
        const std::string GetTextureName(const int& index) const;
        const unsigned int GetTextureIndex(const std::string& name) const;

        void GenerateTextureFilePathsJson();

        /*!***********************************************************************
         * \brief Loads all textures from file paths into the relevant maps
         * 
         **************************************************************************/
        void LoadTextures();

        /*!***********************************************************************
         * \brief Loads all sounds and creates an instance of them for use
         * 
         **************************************************************************/
         //ID : name of audio clip file
        //[0] : Shared_Ptr to audio clip
        std::map<std::string, FMOD::Sound*> SoundMap;
        void ImportSounds();
        void LoadSounds();
        //Stores the meta data of all files
        //"Filepath" : timesincecreated
        std::map<std::string, int> metaFileMap;

        /*!***********************************************************************
         * \brief Loads all prefabs from their json data and creates an instance of them
         * (TODO : Load instances directly into the GOManager!)
         **************************************************************************/
        void LoadPrefabs();

        void SpawnGameObject(std::string fileName, Vec2<float> pos = {0,0});

        std::string KeyCodeToString(KeyCode keycode);
        KeyCode StringToKeyCode(std::string keycode);
        void LoadKeyBinds();
        void LoadKeyCodeTable();
        void GenerateKeyBindsJson();
        void GenerateKeyCodeTable();
        //Map of keybind names to their respective ints
        // "PrintDebug" : 0
        std::map <std::string, std::string> KeyBindNameMap;

        //Map of keycodes to their respective keybind enum
        //"KEY_J" : 74
        std::map <std::string,int> KeyCodeTable;


        /*!***********************************************************************
         * \brief Sound instances to be used throughout the engine
         * To be a map of sounds in the future
         **************************************************************************/
        FMOD::Sound* sampleSound = nullptr;
        FMOD::Sound* explosionSound = nullptr;
        FMOD::Sound* ahhSound = nullptr;

        /*!***********************************************************************
         * \brief Prefab instances to be used throughout the engine
         * To be directly instantiated into the GOManager in the future 
         **************************************************************************/
        GameObject* PineappleObject = nullptr;
        GameObject* AvatarObject = nullptr;


        //Map of Prefab IDs to their instances
        std::map<std::string, std::shared_ptr<GameObject>> Prefabs;
        //Map of Prefab file paths to their IDs
        std::map<std::string, std::string> PrefabFilePathsMap;

        private:
        /*!***********************************************************************
         * \brief Map of file names to their file paths
         * e.g "Char Sprite : /assets/charsprite.png"    
         **************************************************************************/
        std::map<std::string, std::string> nameToFilePath;


        /*!***********************************************************************
         * \brief TextureID int used to keep track of textureIDs
         * 
         **************************************************************************/
        int textureID=0; 

        /*!***********************************************************************
         * \brief Array to keep track of how many textures we have
         * 
         **************************************************************************/
        bool TextureSlots[32]{false};

    };

    extern AssetManager* ASSETMANAGER;

}
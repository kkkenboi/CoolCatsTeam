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


namespace LB
{   
    /**
     * \brief 
     * Struct that holds texture data. (essentially the same as textures)
     */
    struct TextureData
    {
        TextureData():id{0},stbBuffer{},width{0},height{0},fluff{0}{};
        TextureData(const TextureData& texture):id{texture.id},stbBuffer{texture.stbBuffer},width{texture.width},height{texture.height},fluff{texture.fluff}{};
        ~TextureData(){};
        unsigned int id;
        unsigned char* stbBuffer;
        int width,height,fluff;
    };

    /**
     * \brief 
     * Enum names for textures, allows for easy mapping of texture ID to a name
     */
    enum TextureNames
    {
        NONE = 0,
        RUN,
        PINEAPPLE,
        BACKGROUND
    };

    /**
     * \brief 
     * Enum name for asset ID to asset name
     */
    enum class ASSETTYPE
    {
        TEXTURE,
        AUDIO,
        PREFAB
    };

    /**
     * \brief 
     * Asset Manager system that loads assets during engine initialisation
     * This class will also handle the file paths in the future
     */
    class AssetManager : public ISystem
    {
        public:
        /**
         * \brief Construct a new Asset Manager object
         * 
         */
        AssetManager();        

        /**
         * \brief 
         * Create a Texture object 
         */
        TextureData* CreateTexture(const std::string& fileName);

        /**
         * \brief 
         * Adds the texture from a file to the texture map
         * The texture map consists of TEXTUREDATA : ID
         */
        bool AddTexture(const std::string& fileName, const std::string& textureName);
        //The map then maps the name to said pair like so:
        //"SpriteName" : texturePair

        /**
         * \brief 
         * Texture map that maps texture names to their texture pairs like so:
         * "SpriteName" : texturePair
         */ 
        std::map<std::string, std::pair<const TextureData*,int>> Textures;

        //Maps enums to the file paths
        // 0 (NONE) : "../Assets/cat.png"

        /**
         * \brief 
         * Texture file path map that maps texture enum names to file paths
         * Allows for storing ID : Filepath in json
         * "1" : "/Assets/Pineapple.png"
         */ 
        std::map<TextureNames, std::string> TextureFilePaths;

        /**
         * \brief 
         * Get the Texture Index object for the renderer
         */
        const int GetTextureIndex(const std::string& name) const { return Textures.find(name)->second.second; }

        /**
         * \brief Loads all textures from file paths into the relevant maps
         * 
         */
        void LoadTextures();

        /**
         * \brief Loads all sounds and creates an instance of them for use
         * 
         */
        void LoadSounds();

        /**
         * \brief Loads all prefabs from their json data and creates an instance of them
         * (TODO : Load instances directly into the GOManager!)
         */
        void LoadPrefabs();

        /**
         * \brief Sound instances to be used throughout the engine
         * To be a map of sounds in the future
         */
        FMOD::Sound* sampleSound = nullptr;
        FMOD::Sound* explosionSound = nullptr;
        FMOD::Sound* ahhSound = nullptr;

        /**
         * \brief Prefab instances to be used throughout the engine
         * To be directly instantiated into the GOManager in the future 
         */
        GameObject* PineappleObject = nullptr;
        GameObject* AvatarObject = nullptr;

        private:
        /**
         * \brief Map of file names to their file paths
         * e.g "Char Sprite : /assets/charsprite.png"    
         */
        std::map<std::string, std::string> nameToFilePath;
        //Textue IDs to their texture data
        //e.g "0 : *TextureData of character"
        // std::unordered_map<int,TextureData*> textureDataMap;

        //Texture names to their texture pair
        //where the texture pair is
        //TextureData* : int channel the texture uses.

        /**
         * \brief TextureID int used to keep track of textureIDs
         * 
         */
        int textureID=0; 
        //Keeps track of how many textures we can have.

        /**
         * \brief Array to keep track of how many textures we have
         * 
         */
        bool TextureSlots[32]{false};

    };

    extern AssetManager* ASSETMANAGER;

}
/*!************************************************************************
 \file				AssetManager.h
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				04/02/2024
 \brief				The AssetManager handles the loading of data from file
                    and stores them in maps of instances for use througout
                    the engine. This allows users to only have to load 
                    data once and then create copies of the data.

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
//FOR SERIALIZATION
#include "Serializer.h"
#include "LitterBox/Core/System.h" //AssetManager is a system
#include "FileSystemManager.h"
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
#include "LitterBox/Animation/SpriteSheet.h"
#include <ft2build.h>
#include FT_FREETYPE_H

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

    struct CharData
    {
        unsigned int TextureID{};
        LB::Vec2<unsigned int> Size{};
        LB::Vec2<FT_Int> Bearing{};
        unsigned int Advance{};
        unsigned int font{};
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

        /*!************************************************************************
        * \brief Initializes the Asset Manager
        * Subscribes to the application focus event and imports the assets
        * 
        **************************************************************************/
        void Initialize() override;

        /*!************************************************************************
        * \brief Destructs the Asset manager
        * 
        **************************************************************************/
        void Destroy() override;

        /*!************************************************************************
        * \brief Imports ALL assets.
        * Is only ever called once on asset manager initialise!
        **************************************************************************/
        void ImportAssets();

        /*!************************************************************************
        * \brief Function that compares with the meta file and puts all the new files
        * into a vector of file paths and returns it
        * \return std::vector<std::filesystem::path> Filepaths of all new files  
        **************************************************************************/
        std::vector<std::filesystem::path> GetNewFiles();

        
        //e.g "Boom" : Shared_Ptr to audio clip
        std::map<std::string, FMOD::Sound*> SoundMap;

        //Stores the meta data of all files
        //"Filepath" : timesincecreated
        std::map<std::string, long long> metaFileMap;

        //Stores the filepath to ID for ALL files
        // "stemmedName : path" This is so we don't have to keep searching all the time
        // e.g "joe" : "C://Users//joe.png"
        std::map<std::string, std::string> assetMap;
    
        /*!***********************************************************************
        * \brief Spawns a gameobject at specified location (defaulted to 0,0)
        * Usage : SpawnGameObject("joe") where joe is the name of the prefab.
        * 
        **************************************************************************/  
        GameObject* SpawnGameObject(std::string fileName, Vec2<float> pos = {0,0});
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      TEXTURE STUFF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*!***********************************************************************
         * \brief 
         * Create a Texture object 
         **************************************************************************/
        std::shared_ptr<TextureData> CreateTexture(const std::string& fileName);

        /*!***********************************************************************
         * \brief 
         * Adds the texture from a file to the texture map
         * The texture map consists of TEXTUREDATA : ID
         **************************************************************************/
        bool AddTexture(const std::string& fileName, const std::string& textureName);

        /*!***********************************************************************
        * \brief Removes the specified texture from the map and from the 
        * buffer thing e.g RemoveTexture("joe"); 
        * The function assumes you're removing .pngs that have been stemmed
        **************************************************************************/  
        bool RemoveTexture(const std::string& name);

        /*!***********************************************************************
        * \brief Clears and removes all textures
        **************************************************************************/ 
        void FlushTextures();

        /*!***********************************************************************
         * \brief 
         * Texture map that maps texture names to their texture pairs like so:
         * "SpriteName" : texturePair
         **************************************************************************/
        std::map<std::string, std::pair<std::shared_ptr<TextureData>,int>> Textures;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      SPRITESHEET STUFF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::map<std::string, SpriteSheet> SpriteSheets;
        
        SpriteSheet const& GetSpriteSheet(std::string const& name) const;

        SpriteSheet& GetSpriteSheet(std::string const& name);

        Sprite GetSprite(std::string& spriteSheetName, int index);

        /*!***********************************************************************
        * \brief Gets the texture unit ID
        * Usage : GetTextureUnit("joe"); where joe is assumed to be a png
        **************************************************************************/  
        const int GetTextureUnit(const std::string& name);

        /*!***********************************************************************
        * \brief Gets the texture name from ID
        * 
        **************************************************************************/  
        const std::string GetTextureName(const int& index);

        /*!***********************************************************************
        * \brief Gets the texture ID from name
        * Usage : GetTextureIndex("amongus") where amongus is a png
        **************************************************************************/ 
        const int GetTextureIndex(const std::string& name);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      ANIMATION STUFF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::map<std::string, AnimationController> AnimControllers;

        std::map<std::string, AnimationState> AnimStates;

        //std::vector<std::string> fontNames;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      KEYCODE STUFF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /*!***********************************************************************
        * \brief Searches the map to return the stringified keycode from keycode
        * 
        **************************************************************************/ 
        std::string KeyCodeToString(KeyCode keycode);

        /*!***********************************************************************
        * \brief Searches the map to return the keycode from the string
        * 
        **************************************************************************/  
        KeyCode StringToKeyCode(std::string keycode);

        /*!***********************************************************************
        * \brief Loads all the keybinds from the keybinds json
        * 
        **************************************************************************/ 
        void LoadKeyBinds();
        /*!***********************************************************************
        * \brief Loads the keycode table (keycode to string)
        * 
        **************************************************************************/          
        void LoadKeyCodeTable();
        /*!***********************************************************************
        * \brief Helper function to generate keybinds json if it's ever lost
        * 
        **************************************************************************/         
        void GenerateKeyBindsJson();

        /*!***********************************************************************
        * \brief Helper function to generate keycode table if it's ever lost
        * 
        **************************************************************************/        
       void GenerateKeyCodeTable();
        //Map of keybind names to their respective ints
        // "PrintDebug" : 0
        std::map <std::string, std::string> KeyBindNameMap;

        //Map of keycodes to their respective keybind enum
        //"KEY_J" : 74
        std::map <std::string,int> KeyCodeTable;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                      FONT & SHADER STUFF
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //std::map<std::string, std::map<char, CharData>> font_glyphs;

        void LoadFonts(void* textR);

        void LoadShader(const std::string& shader_file_name, unsigned int& shader_handle);


        //This is the folder name where the meta file will be stored
        //basically %APPDATA%/PurrfectPutt
        char* _appData{ nullptr };
        std::filesystem::path folderName{ "PurrfectPutt" };
        std::filesystem::path defaultDirectoryPath{ std::filesystem::current_path() / std::filesystem::path{"Assets"} };
        private:

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
    /*!************************************************************************
     * \brief Global function to reimport assets (called on application refocus)
     * Global function because it's called by OnApplicationFocus Event which 
     * only takes in non-member functions.
     **************************************************************************/
    void ReimportAssets();
    extern AssetManager* ASSETMANAGER;

}
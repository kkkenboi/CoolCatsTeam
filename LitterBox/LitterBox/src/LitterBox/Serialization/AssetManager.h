#pragma once
//#include <string>
//#include <map>
//#include <unordered_map>
#include "Serializer.h"
#include "LitterBox/Core/System.h"
#include "../../dependencies/FMOD/core/inc/fmod.hpp"
#include "LitterBox/Audio/AudioManager.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Renderer/Renderer.h"
#include <vector>


namespace LB
{   
    
    struct TextureData
    {
        TextureData():id{0},stbBuffer{},width{0},height{0},fluff{0}{};
        TextureData(const TextureData& texture):id{texture.id},stbBuffer{texture.stbBuffer},width{texture.width},height{texture.height},fluff{texture.fluff}{};
        ~TextureData(){};
        unsigned int id;
        unsigned char* stbBuffer;
        int width,height,fluff;
    };
    enum TextureNames
    {
        NONE = 0,
        RUN,
        PINEAPPLE,
        BACKGROUND
    };
    enum class ASSETTYPE
    {
        TEXTURE,
        AUDIO,
        PREFAB
    };
    class AssetManager : public ISystem
    {
        public:
        AssetManager();        
        TextureData* CreateTexture(const std::string& fileName);
        bool AddTexture(const std::string& fileName, const std::string& textureName);
        //The map then maps the name to said pair like so:
        //"SpriteName" : texturePair
        std::map<std::string, std::pair<const TextureData*,int>> Textures;

        //Maps enums to the file paths
        // 0 (NONE) : "../Assets/cat.png"
        std::map<TextureNames, std::string> TextureFilePaths;

        const int GetTextureIndex(const std::string& name) const { return Textures.find(name)->second.second; }
        void LoadTextures();
        void LoadSounds();
        void LoadPrefabs();

        FMOD::Sound* sampleSound = nullptr;
        FMOD::Sound* explosionSound = nullptr;
        FMOD::Sound* ahhSound = nullptr;

        GameObject* PineappleObject = nullptr;
        GameObject* AvatarObject = nullptr;

        std::vector<const CPRender*> renderObjs;
        private:
        //File names to their file paths
        //e.g "Char Sprite : /assets/charsprite.png"   
        std::map<std::string, std::string> nameToFilePath;
        //Textue IDs to their texture data
        //e.g "0 : *TextureData of character"
        // std::unordered_map<int,TextureData*> textureDataMap;

        //Texture names to their texture pair
        //where the texture pair is
        //TextureData* : int channel the texture uses.
        int textureID=0; 
        //Keeps track of how many textures we can have.
        bool TextureSlots[32]{false};

    };

    extern AssetManager* ASSETMANAGER;

}
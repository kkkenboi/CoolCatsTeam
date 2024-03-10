/*!************************************************************************
 \file				AssetManager.cpp
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				22/11/2023
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
#include "LitterBox/Core/Core.h"
#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <chrono>
#pragma warning(pop)
#pragma warning(disable: 4996)

namespace LB
{
    //Global pointer for AssetManager Singleton
    AssetManager* ASSETMANAGER = nullptr;
    
    /*!************************************************************************
     * \brief Construct a new Asset Manager:: Asset Manager object
     * 
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

    
    /*!************************************************************************
     * \brief Global function to reimport assets (called on application refocus)
     * Global function because it's called by OnApplicationFocus Event which 
     * only takes in non-member functions.
     **************************************************************************/
    void ReimportAssets()
    {
        //First we have to grab our meta file so that we can compare 
        ASSETMANAGER->_appData = getenv("APPDATA");
        std::filesystem::path appData{ ASSETMANAGER->_appData };
        Document _metaJson = JSONSerializer::GetJSONFile((appData/ASSETMANAGER->folderName / std::filesystem::path("MetaFiles.json")).string());
        Document::AllocatorType& metaAlloc = _metaJson.GetAllocator();

        //Then we get all the file types 
        std::vector<std::filesystem::path> TextureFilePaths = FILESYSTEM->GetFilesOfType(".png");
        std::vector<std::filesystem::path> SpriteSheetFilePaths = FILESYSTEM->GetFilesOfType(".spritesheet");
        std::vector<std::filesystem::path> SoundFilePaths = FILESYSTEM->GetFilesOfType(".wav");
        std::vector<std::filesystem::path> AnimControllerPaths = FILESYSTEM->GetFilesOfType(".controller");
        std::vector<std::filesystem::path> AnimStatePaths = FILESYSTEM->GetFilesOfType(".anim");
        //then we check if there's any NEW assets
        for (const auto& textureFP : TextureFilePaths)
        {
            if (ASSETMANAGER->metaFileMap.find(textureFP.string()) == ASSETMANAGER->metaFileMap.end())
            {
                DebuggerLogFormat("FOUND NEW TEXTURE : %s, IMPORTING NOW!", textureFP.stem().string().c_str());
                //Meta map tracks the filepath : filetime e.g C://Users//joe.png : 4849584
                ASSETMANAGER->metaFileMap[textureFP.string()] = FILESYSTEM->GetFileTime(textureFP);
                //Asset map tracks filename : filepath e.g joe : C://Users//joe.png
                ASSETMANAGER->assetMap[textureFP.filename().stem().string()] = textureFP.string();
                //We use the filepath for both the key and value for convenience...
                //This is so that we can also have 2 files of the same name but different IDs
                ASSETMANAGER->AddTexture(textureFP.string(), textureFP.string());

                //then we also need to update our json by adding the new member
                Value metaKey(Value(textureFP.string().c_str(), metaAlloc), metaAlloc);
                _metaJson.AddMember(metaKey, FILESYSTEM->GetFileTime(textureFP), metaAlloc);
            }
        }
        //This one is for the sprite sheets reimporting
        for (const auto& spriteSheetFP : SpriteSheetFilePaths)
        {
            if (ASSETMANAGER->metaFileMap.find(spriteSheetFP.string()) == ASSETMANAGER->metaFileMap.end())
            {
                DebuggerLogFormat("FOUND NEW TEXTURE : %s, IMPORTING NOW!", spriteSheetFP.stem().string().c_str());
                ASSETMANAGER->metaFileMap[spriteSheetFP.string()] = FILESYSTEM->GetFileTime(spriteSheetFP);
                ASSETMANAGER->assetMap[spriteSheetFP.filename().stem().string()] = spriteSheetFP.string();
                //Once we write to the meta file, we can load the sprite sheet 
                Value metaKey(Value(spriteSheetFP.string().c_str(), metaAlloc), metaAlloc);
                _metaJson.AddMember(metaKey, FILESYSTEM->GetFileTime(spriteSheetFP), metaAlloc);

                JSONSerializer::DeserializeFromFile(spriteSheetFP.filename().stem().string(), ASSETMANAGER->SpriteSheets[spriteSheetFP.filename().stem().string()]);
            }
        }
        //Now for the animator and anim controller reimport
        for (const auto& animController : AnimControllerPaths)
        {
            if (ASSETMANAGER->metaFileMap.find(animController.string()) == ASSETMANAGER->metaFileMap.end())
            {
                DebuggerLogFormat("FOUND NEW ANIM CONTROLLER : %s, IMPORTING NOW!", animController.stem().string().c_str());
                ASSETMANAGER->metaFileMap[animController.string()] = FILESYSTEM->GetFileTime(animController);
                ASSETMANAGER->assetMap[animController.filename().stem().string()] = animController.string();

                Value metaKey(Value(animController.string().c_str(), metaAlloc), metaAlloc);
                _metaJson.AddMember(metaKey, FILESYSTEM->GetFileTime(animController), metaAlloc);

                JSONSerializer::DeserializeFromFile(animController.filename().stem().string(), ASSETMANAGER->AnimControllers[animController.filename().stem().string()]);
            }
        }
        
        for (const auto& animState : AnimStatePaths)
        {
            if (ASSETMANAGER->metaFileMap.find(animState.string()) == ASSETMANAGER->metaFileMap.end())
            {
                DebuggerLogFormat("FOUND NEW ANIM STATE : %s, IMPORTING NOW!", animState.stem().string().c_str());
                ASSETMANAGER->metaFileMap[animState.string()] = FILESYSTEM->GetFileTime(animState);
                ASSETMANAGER->assetMap[animState.filename().stem().string()] = animState.string();

                Value metaKey(Value(animState.string().c_str(), metaAlloc), metaAlloc);
                _metaJson.AddMember(metaKey, FILESYSTEM->GetFileTime(animState), metaAlloc);

                JSONSerializer::DeserializeFromFile(animState.filename().stem().string(), ASSETMANAGER->AnimStates[animState.filename().stem().string()]);
            }
        }
        //AUDIOMANAGER->StopAllChannels();
        //Same thing as above but for audio
        for (const auto& soundFP : SoundFilePaths)
        {
            if (ASSETMANAGER->metaFileMap.find(soundFP.string()) == ASSETMANAGER->metaFileMap.end())
            {
                DebuggerLogFormat("FOUND NEW SOUND : %s, IMPORTING NOW!", soundFP.stem().string().c_str());
                ASSETMANAGER->metaFileMap[soundFP.string()] = FILESYSTEM->GetFileTime(soundFP);
                //I don't check for result here because well, the filepath 100% exists
                AUDIOMANAGER->audioSystem->createSound(soundFP.string().c_str(), FMOD_DEFAULT, nullptr, &ASSETMANAGER->SoundMap[soundFP.stem().string()]);
                //write to json
                Value metaKey(Value(soundFP.string().c_str(), metaAlloc), metaAlloc);
                _metaJson.AddMember(metaKey, FILESYSTEM->GetFileTime(soundFP), metaAlloc);
            }
        }
        //Now that we finish checking for NEW assets, we check if any assets were deleted
        //or moved or updated. 
        //To check for deleted/moved, we can reference our metamap vs the metajson for the 
        //missing filepaths
        //To check for updated, we compare the file times in metamap vs metajson
        //First we loop through the map (filepath : filetime)
        for (const auto& metaData : ASSETMANAGER->metaFileMap)
        {
            //And we recalculate the file times for all the files in our meta map
            //We convert the string back to a std::filesystem::filepath so we can stem it and whatnot

            //We check if the file is still there. If it isn't then...
            std::filesystem::path deletedFile{ metaData.first };
            if (!std::filesystem::exists(deletedFile))
            {   
                //We check the extension so we can remove the texture
                if (deletedFile.filename().extension().string() == ".png")
                {
                    ASSETMANAGER->RemoveTexture(deletedFile.filename().stem().string());
                } //checking if it's a sound file
                if (deletedFile.filename().extension().string() == ".wav")
                {
                    ASSETMANAGER->SoundMap[deletedFile.string()]->release();
                }
                if (deletedFile.filename().extension().string() == ".spritesheet")
                {
                    ASSETMANAGER->SpriteSheets.erase(deletedFile.string());
                }
                if (deletedFile.filename().extension().string() == ".anim")
                {
                    ASSETMANAGER->AnimStates.erase(deletedFile.string());

                }
                if (deletedFile.filename().extension().string() == ".controller")
                {
                    ASSETMANAGER->AnimControllers.erase(deletedFile.string());

                }
                DebuggerLogFormat("%s can't be found anymore! Removing...", deletedFile.filename().string().c_str());
                //Then we set the time to some impossible time and continue the loop
                ASSETMANAGER->metaFileMap[metaData.first] = -1;
                //The entries don't get deleted because well... there's no point and it feels risky. 
                
                continue;
            }
            //If the file exists, we check the time if it's been updated
            long long fileTime = FILESYSTEM->GetFileTime(metaData.first);
            //if there's any difference, we know it's been changed
            if (ASSETMANAGER->metaFileMap[metaData.first] != fileTime)
            {
                std::filesystem::path metaFP{ metaData.first };
                DebuggerLogFormat("Found updated file %s with new time %d", metaFP.filename().string().c_str(), fileTime);
                if (metaFP.filename().extension().string() == ".png")
                {
                    DebuggerLogFormat("PNG file name : %s", metaFP.string().c_str());
                    ASSETMANAGER->RemoveTexture(metaFP.filename().stem().string());

                    ASSETMANAGER->AddTexture(metaFP.string(), metaFP.string());
                }
                if (metaFP.filename().extension().string() == ".spritesheet")
                {
                    DebuggerLogFormat("SpriteSheet file name : %s", metaFP.string().c_str());
                    //I think we don't have to remove it, we can just straight up edit it 
                    JSONSerializer::DeserializeFromFile(metaFP.filename().stem().string(), ASSETMANAGER->SpriteSheets[metaFP.filename().stem().string()]);
                }
                if (metaFP.filename().extension().string() == ".anim")
                {
                    DebuggerLogFormat("Anim state file name : %s", metaFP.string().c_str());
                    //I think we don't have to remove it, we can just straight up edit it 
                    JSONSerializer::DeserializeFromFile(metaFP.filename().stem().string(), ASSETMANAGER->AnimStates[metaFP.filename().stem().string()]);
                }
                if (metaFP.filename().extension().string() == ".controller")
                {
                    DebuggerLogFormat("Anim controller file name : %s", metaFP.string().c_str());
                    //I think we don't have to remove it, we can just straight up edit it 
                    JSONSerializer::DeserializeFromFile(metaFP.filename().stem().string(), ASSETMANAGER->AnimControllers[metaFP.filename().stem().string()]);
                }
                if (metaFP.filename().extension().string() == ".wav")
                {
                    DebuggerLogFormat("WAV file name : %s", metaFP.string().c_str());
                    ASSETMANAGER->SoundMap[metaFP.filename().stem().string()]->release();
                    AUDIOMANAGER->audioSystem->createSound(metaFP.string().c_str(), FMOD_DEFAULT, nullptr, &ASSETMANAGER->SoundMap[metaFP.filename().stem().string()]);
                }
                //then we update it's time
                _metaJson[metaData.first.c_str()] = fileTime;
                ASSETMANAGER->metaFileMap[metaData.first] = fileTime;
            }
        //We save to json once we're done
        }
        JSONSerializer::SaveToJSON((appData / ASSETMANAGER->folderName / std::filesystem::path("MetaFiles.json")).string(), _metaJson);
        //JSONSerializer::SaveToJSON(FILESYSTEM->GetFilePath("MetaFiles.json").string(), _metaJson);
    }


    /*!************************************************************************
     * \brief Initializes the Asset Manager
     * Subscribes to the application focus event and imports the assets
     * 
     **************************************************************************/
    void AssetManager::Initialize()
    {
        WINDOWSSYSTEM->OnApplicationFocus.Subscribe(ReimportAssets);
        DebuggerLog("Assetmanager is initializing");
        
        Textures["none"];
        //Load all assets here
        ImportAssets();
        LoadKeyBinds();
    }

        
    /*!************************************************************************
     * \brief Destructs the Asset manager
     * 
     **************************************************************************/
    void AssetManager::Destroy()
    {
        //shouldn't need to do this but just in case
        for (const auto& texture : Textures)
        {
            texture.second.first.~shared_ptr();
        }
    }

    /*!************************************************************************
     * \brief This function creates and ADDS a texture to the map with the appropriate ID
     * 
     * \return TextureData* texture data pointer 
     **************************************************************************/
    std::shared_ptr<TextureData> AssetManager::CreateTexture(const std::string &fileName)
    {
        //This might be dangerous, research sharedpointers 
        std::shared_ptr<TextureData> cachedTexture = std::make_shared<TextureData>();
        stbi_set_flip_vertically_on_load(1);
        /*std::filesystem::path texturePath = FILESYSTEM->GetFilePath(fileName)*/;
        cachedTexture->stbBuffer = stbi_load(fileName.c_str(), &cachedTexture->width, &cachedTexture->height, &cachedTexture->fluff, 4);
        if(!cachedTexture->stbBuffer)
        {
            DebuggerLogWarningFormat("Texture filepath %s not found!", fileName.c_str());
            //std::string funnyPng{"Assets/Textures/despair.png"};
            //cachedTexture->stbBuffer = stbi_load(funnyPng.c_str(),&cachedTexture->width,&cachedTexture->height,&cachedTexture->fluff,4);
            cachedTexture->id = 0;
            return cachedTexture;
        } 
      
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



    /*!************************************************************************
     * \brief Imports ALL assets.
     * 
     **************************************************************************/
    void AssetManager::ImportAssets()
    {
        //On engine load, we will grab all the assets and their types
        //We also make a map of the AssetName : File path e.g joe : C://User//joe.png
        //We also need to initialise the metafilemap with the the metadata, and all of this is also
        //generated when we import our assets. We should only ever call this function ONCE.

        //First we get the meta file data and put it into a vector
        _appData = getenv("APPDATA");
        std::filesystem::path appData(_appData);
        if (!std::filesystem::exists(appData/folderName))
        {
            std::filesystem::create_directory(appData / folderName);
        }
        Document _metaFile;// = JSONSerializer::GetJSONFile(FILESYSTEM->GetFilePath("MetaFiles.json").string());
        Document::AllocatorType& metaAlloc = _metaFile.GetAllocator();

        std::vector<std::filesystem::path> TextureFilePaths = FILESYSTEM->GetFilesOfType(".png");
        std::vector<std::filesystem::path> SpriteSheetPaths = FILESYSTEM->GetFilesOfType(".spritesheet");
        std::vector<std::filesystem::path> AnimControllerPaths = FILESYSTEM->GetFilesOfType(".controller");
        std::vector<std::filesystem::path> AnimStatePaths = FILESYSTEM->GetFilesOfType(".anim");
        std::vector<std::filesystem::path> SoundFilePaths = FILESYSTEM->GetFilesOfType(".wav");
        std::vector<std::filesystem::path> ttfFontPaths = FILESYSTEM->GetFilesOfType(".ttf");
        std::vector<std::filesystem::path> otfFontPaths = FILESYSTEM->GetFilesOfType(".otf");
        std::vector<std::filesystem::path> ShaderPaths = FILESYSTEM->GetFilesOfType(".shader");

        //Adding the fonts to the asset map first, probably add it to the meta file in the future
        for (const auto& f : ttfFontPaths)
        {
            assetMap[f.filename().stem().string()] = f.string();
        }
        for (const auto& f : otfFontPaths)
        {
            assetMap[f.filename().stem().string()] = f.string();
        }

        for (const auto& f : ShaderPaths)
            assetMap[f.filename().stem().string()] = f.string();

        //We grab all the files and put them into a vector (I don't concate them because I need them separate)
        //Now we start making the meta file
        _metaFile.SetObject();
        for (const auto& t : TextureFilePaths)
        {
            Value metaKey(Value(t.string().c_str(), metaAlloc), metaAlloc);
            long long fileTime = FILESYSTEM->GetFileTime(t);
            _metaFile.AddMember(metaKey, fileTime, metaAlloc);

            //AssetMap => "joe" : "C://User//joe.png"
            assetMap[t.filename().stem().string()] = t.string();
            //MetaFileMap => "C://User//joe.png" : 5748394
            metaFileMap[t.string()] = fileTime;

            //Texture ID is a bit fucked, but it's 
            //"C://User//joe.png" : "C://User//joe.png"
            AddTexture(t.string(), t.string());
        }
        // Then we do the same for spritesheets
        for (const auto& s : SpriteSheetPaths)
        {
            Value metaKey(Value(s.string().c_str(), metaAlloc), metaAlloc);
            long long fileTime = FILESYSTEM->GetFileTime(s);
            _metaFile.AddMember(metaKey, fileTime, metaAlloc);

            assetMap[s.filename().stem().string()] = s.string();
            metaFileMap[s.string()] = fileTime;

            JSONSerializer::DeserializeFromFile(s.filename().stem().string(), SpriteSheets[s.filename().stem().string()]);
        }
        // Then we do the same for animation states
        for (const auto& s : AnimStatePaths)
        {
            Value metaKey(Value(s.string().c_str(), metaAlloc), metaAlloc);
            long long fileTime = FILESYSTEM->GetFileTime(s);
            _metaFile.AddMember(metaKey, fileTime, metaAlloc);

            assetMap[s.filename().stem().string()] = s.string();
            metaFileMap[s.string()] = fileTime;

            JSONSerializer::DeserializeFromFile(s.filename().stem().string(), AnimStates[s.filename().stem().string()]);
        }
        // Then we do the same for animation controllers
        for (const auto& s : AnimControllerPaths)
        {
            Value metaKey(Value(s.string().c_str(), metaAlloc), metaAlloc);
            long long fileTime = FILESYSTEM->GetFileTime(s);
            _metaFile.AddMember(metaKey, fileTime, metaAlloc);

            assetMap[s.filename().stem().string()] = s.string();
            metaFileMap[s.string()] = fileTime;

            JSONSerializer::DeserializeFromFile(s.filename().stem().string(), AnimControllers[s.filename().stem().string()]);
        }
        //Then we do the same for sounds
        for (const auto& s : SoundFilePaths)
        {
            Value metaKey(Value(s.string().c_str(), metaAlloc), metaAlloc);
            long long fileTime = FILESYSTEM->GetFileTime(s);

            _metaFile.AddMember(metaKey, fileTime, metaAlloc);
            //AssetMap => "BOOM" : "C://Users//BOOM.wav"
            assetMap[s.filename().stem().string()] = s.string();
            //MetaFileMap => "C://Users//BOOM.wav" : 567858498
            metaFileMap[s.string()] = fileTime;

            //This also adds it into the sound map, all the way at the right side lol...
            //I would make it a shared pointer but I'm really not sure how to wrap it...
            //SoundMap => "BOOM" : FMOD::Sound*
            AUDIOMANAGER->result = AUDIOMANAGER->audioSystem->createSound(s.string().c_str(), FMOD_DEFAULT, nullptr, &SoundMap[s.stem().string()]);
            if (AUDIOMANAGER->result != FMOD_OK) DebuggerLogWarningFormat("UNABLE TO FIND %s SOUND!", s.string().c_str());
        }
        JSONSerializer::SaveToJSON((appData / folderName / std::filesystem::path("MetaFiles.json")).string(), _metaFile);
    }

    /*!************************************************************************
     * \brief Function that compares with the meta file and puts all the new files
     * into a vector of file paths and returns it
     * \return std::vector<std::filesystem::path> Filepaths of all new files  
    **************************************************************************/
    std::vector<std::filesystem::path> AssetManager::GetNewFiles()
    {
        //This will contain the paths to new files
        std::vector<std::filesystem::path> NewFiles;

        //We want to also create/update the meta file everytime we import
        Document _metaFile = JSONSerializer::GetJSONFile(FILESYSTEM->GetFilePath("MetaFiles.json").string());
        //Document::AllocatorType& metaAlloc = _metaFile.GetAllocator(); NOTREFERENCED

        //This vector will contain the OLD meta file paths
        std::vector<std::filesystem::path> metaPaths;
        //We load up the metapath with the OLD ones from file
        for (Value::ConstMemberIterator itr = _metaFile.MemberBegin();
            itr != _metaFile.MemberEnd(); ++itr)
        {
            //We make a filepath from it first and check if it exists
            std::filesystem::path metaFilePath{ itr->name.GetString() };
            if (std::filesystem::exists(metaFilePath))
            {
                metaPaths.push_back(std::filesystem::path{ itr->name.GetString() });
            } //if it doesn't exist then we just don't add it
            //I think this should be okay since we haven't created any textures yet
        }
        //We need to sort this because we're going to compare the set differences...
        std::sort(metaPaths.begin(), metaPaths.end());


        //Now we make a vector that will contain ALL the files
        std::vector<std::filesystem::path> AllFiles;
        //Grab ALL sounds and ALL textures
        std::vector<std::filesystem::path> TextureFilePaths = FILESYSTEM->GetFilesOfType(".png");
        std::vector<std::filesystem::path> SoundFilePaths = FILESYSTEM->GetFilesOfType(".wav");
        //Sort them so we can merge the two vectors 
        std::sort(TextureFilePaths.begin(), TextureFilePaths.end());
        std::sort(SoundFilePaths.begin(), SoundFilePaths.end());
        //We merge it into the vector of AllNewFiles so we can cross compare with the OLD metapaths
        std::merge(TextureFilePaths.begin(), TextureFilePaths.end(), SoundFilePaths.begin(), SoundFilePaths.end(),
            std::back_inserter(AllFiles));


        //We take ALLFiles - OLDFiles = NEWFiles
        std::set_difference(AllFiles.begin(), AllFiles.end(),
            metaPaths.begin(), metaPaths.end(),
            std::inserter(NewFiles, NewFiles.begin()));

        //Uncomment to check for the files
        //for (const auto& path : NewFiles) {std::cout << "new paths : " << path.stem().string() << '\n';}
        return NewFiles;
    }

    /*!***********************************************************************
    * \brief 
    * Adds the texture from a file to the texture map
    * The texture map consists of TEXTUREDATA : ID
    **************************************************************************/  
    bool AssetManager::AddTexture(const std::string &fileName, const std::string &textureName)
    {   //!Note that the filename received here is the full file path
        //Texture name is the same. Could probably refactor in the future...
        //We need to check if we have too many textures
        if(Textures.size() >= 31)
        {
            //Max textures reached
            DebuggerLogWarning("Warning! Max textures reached!!");
            return false;
        }
        //This lets us keep track of the id
        //TODO have a check for editor vs game
        //      if editor then start from 0 otherwise start from 1
        int i{1}; //START FROM 2 BECAUSE texture unit 0 will be reserved for ImGUI texture
        for(; i<31; ++i)
        {
            if(!TextureSlots[i]) break;
        }

        //The texture is tagged to the ID, which is then tagged to the texture name
        // e.g "C://Users//cat.png" : "<catTextureData,ID>" where cat is the name of the texture.
        Textures.emplace(std::make_pair(textureName,std::make_pair(CreateTexture(fileName),i)));
        //textures.emplace(std::make_pair(name, std::make_pair(LB::ASSETMANAGER->CreateTexture(file_path), i)));

        //getting the pair then the texture id
	    glBindTextureUnit(0 + i, Textures.find(textureName)->second.first->id);
	    TextureSlots[i] = true;

	    return true;
    }

    /*!***********************************************************************
     * \brief
     * Adds the texture from a file to the texture map
     * The texture map consists of TEXTUREDATA : ID
    **************************************************************************/
    bool AssetManager::AddReservedTexture(unsigned int texture)
    {
        //getting the pair then the texture id
        glBindTextureUnit(31, texture);
        TextureSlots[31] = true;

        return true;
    }

    /*!***********************************************************************
     * \brief
     * Adds the texture from a file to the texture map
     * The texture map consists of TEXTUREDATA : ID
    **************************************************************************/
    bool AssetManager::RemoveReservedTexture()
    {
        //else we set it to true and bind the texture
        glBindTextureUnit(31, 0);
        TextureSlots[0] = false;
        return true;
    }

    /*!***********************************************************************
    * \brief Removes the specified texture from the map and from the 
    * buffer thing e.g RemoveTexture("joe"); 
    * The function assumes you're removing .pngs that have been stemmed
    **************************************************************************/  
    bool AssetManager::RemoveTexture(const std::string& name)
    {   //Since we have the name mapped to a value we can just use our map
        if (Textures.find(assetMap[name]) == Textures.end())
        {
            DebuggerLogErrorFormat("Unable to find %s in Textures!", name.c_str());
            return false;
        }
        TextureSlots[Textures.find(assetMap[name])->second.second] = false;
        Textures.erase(assetMap[name]);   //since it's a shared pointer it shoulddddd deallocate properly right???
        return true;
    }

    /*!***********************************************************************
    * \brief Clears and removes all textures
    **************************************************************************/  
    void AssetManager::FlushTextures()
    {
        Textures.clear();
        for (int i{ 0 }; i < 32; ++i)
        {
            TextureSlots[i] = false;
        }
    }

    SpriteSheet const& AssetManager::GetSpriteSheet(std::string const& name) const
    {
        if (SpriteSheets.find(name) != SpriteSheets.end()) {
            return SpriteSheets.at(name);
        }
        DebuggerLogErrorFormat("Unable to find %s in SpriteSheets!", name.c_str());
        return SpriteSheets.begin()->second;   //SFNIAE
    }

    SpriteSheet& AssetManager::GetSpriteSheet(std::string const& name)
    {
        if (SpriteSheets.find(name) != SpriteSheets.end()) {
            return SpriteSheets[name];
        }
        DebuggerLogErrorFormat("Unable to find %s in SpriteSheets!", name.c_str());
        return SpriteSheets.begin()->second;   //SFNIAE
    }

    Sprite AssetManager::GetSprite(std::string& spriteSheetName, int index)
    {
        if (GetSpriteSheet(spriteSheetName).GetName() != "") {
            return GetSpriteSheet(spriteSheetName)[index];
        }
        DebuggerLogErrorFormat("Unable to find %s in Sprite!", spriteSheetName.c_str());
        return Sprite();
    }

    /*!***********************************************************************
    * \brief Gets the texture unit ID
    * Usage : GetTextureUnit("joe"); where joe is assumed to be a png
    **************************************************************************/  
    const int AssetManager::GetTextureUnit(const std::string& name)
    {
        
        if (Textures.find(assetMap[name]) == Textures.end())
        {
            DebuggerLogWarning("Texture " + name + " can't be found!");
            return -1;  //return an invalid index for a graceful fail
        }
        if (name == "none")
        {
            return -1;
        }
        return Textures.find(assetMap[name])->second.second;
    }

    /*!***********************************************************************
    * \brief Gets the texture name from ID
    * 
    **************************************************************************/  
    const std::string AssetManager::GetTextureName(const int& index)
    {
        if (index == 0)
        {
            return "none";
        }
        for (const auto& elem : Textures)
        {
            if (elem.second.second == index)
            {
                std::filesystem::path tempPath{ elem.first };
                return tempPath.filename().stem().string();
            }
        }
        DebuggerLogWarning("Texture index : " + std::to_string(index) + " can't be found!");
        return "";
    }

    /*!***********************************************************************
    * \brief Gets the texture ID from name
    * Usage : GetTextureIndex("amongus") where amongus is a png
    **************************************************************************/  
    const int AssetManager::GetTextureIndex(const std::string& name) 
    {
        if (Textures.find(assetMap[name]) == Textures.end())
        {
            DebuggerLogWarning("Texture " + name + " can't be found!");
            return -1;  //return an invalid index for a graceful fail
        }
        if (name == "none")
        {
            return -1;
        }
        return Textures.find(assetMap[name])->second.first->id;
    }

    /*!***********************************************************************
    * \brief Spawns a gameobject at specified location (defaulted to 0,0)
    * Usage : SpawnGameObject("joe") where joe is the name of the prefab.
    * 
    **************************************************************************/  
    GameObject* AssetManager::SpawnGameObject(std::string fileName, Vec2<float> pos)
    {
        GameObject* prefab = FACTORY->SpawnGameObject();
        //prefab->Deserialize(JSONSerializer::GetJSONFile(FILESYSTEM->GetFilePath(fileName).string()));
        JSONSerializer::DeserializeFromFile(fileName, *prefab);
        if (!(pos == Vec2<float>{0, 0}))
        prefab->GetComponent<CPTransform>()->SetPosition(pos);

        return prefab;
    }

    /*!***********************************************************************
    * \brief Searches the map to return the stringified keycode from keycode
    * 
    **************************************************************************/  
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


    /*!***********************************************************************
    * \brief Searches the map to return the keycode from the string
    * 
    **************************************************************************/  
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

    /*!***********************************************************************
    * \brief Loads all the keybinds from the keybinds json
    * 
    **************************************************************************/  
    void AssetManager::LoadKeyBinds()
    {
        //We need to load the keycode table from json first
        LoadKeyCodeTable();
        //Can probablyyyyy use the filesystem for this in the future
        //Document _jsonFile = JSONSerializer::GetJSONFile("Editor/Jason/KeyBinds.json");
        Document _jsonFile = JSONSerializer::GetJSONFile(FILESYSTEM->GetFilePath("KeyBinds").string());
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
        if (CORE->IsEditorMode())
        {
            GenerateKeyBindsJson();
        }
    }

    /*!***********************************************************************
    * \brief Loads the keycode table (keycode to string)
    * 
    **************************************************************************/    
    void AssetManager::LoadKeyCodeTable()
    {
        //Document _jsonFile = JSONSerializer::GetJSONFile("Editor/Jason/KeyCodeTable.json");
        Document _jsonFile = JSONSerializer::GetJSONFile(FILESYSTEM->GetFilePath("KeyCodeTable").string());
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


    /*!***********************************************************************
    * \brief Helper function to generate keybinds json if it's ever lost
    * 
    **************************************************************************/    
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
        DebuggerLog( FILESYSTEM->GetFilePath("KeyBinds").string());
        JSONSerializer::SaveToJSON(FILESYSTEM->GetFilePath("KeyBinds").string(), _jsonFile);
        //To test if the keycode and string to keycode function works
        //std::cout << KeyCodeToString(StringToKeyCode("KEY_J")) << '\n';
    }

    

    /*!***********************************************************************
    * \brief Helper function to generate keycode table if it's ever lost
    * 
    **************************************************************************/    
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
        DebuggerLog(FILESYSTEM->GetFilePath("KeyCodeTable").string());

        JSONSerializer::SaveToJSON(FILESYSTEM->GetFilePath("KeyCodeTable").string(), _jsonFile);
    }


    void AssetManager::LoadFonts(void* textR)
    {
        //Get fonts
        auto fonts{ LB::FILESYSTEM->GetFilesOfType(".otf") };
        auto fonts2{ LB::FILESYSTEM->GetFilesOfType(".ttf") };

        //variables
        FT_Library ft{};
        FT_Face font{};

        std::map<char, Renderer::Character> Characters{};
        Renderer::TextRenderer* textRender{ 
            reinterpret_cast<Renderer::TextRenderer*>
            (textR) };

        unsigned int largest_height{ 0 };
        //-------------------LOAD FONT------------------------
        //init freetype lib
        if (FT_Init_FreeType(&ft)) {
            DebuggerLogError("ERROR On the freetype: could not init the lib");
        }

        //load all fonts both otf and ttf
        for (auto const& e : fonts) {
            //load font
            if (FT_New_Face(ft, e.string().c_str(), 0, &font)) {
                DebuggerLogError("ERROR on the freetype: could not load font");
            }
            //set default font face
            FT_Set_Pixel_Sizes(font, 0, 50); //the width is 0 so it is based off the height value

            unsigned int maxAscent{}, maxDescent{};
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            largest_height = 0;
            for (unsigned char c{}; c < 128; ++c) {
                //load glyph
                if (FT_Load_Char(font, c, FT_LOAD_RENDER)) {
                    DebuggerLogErrorFormat("ERROR on the freetype: could not load glyph %c", c);
                    continue;
                }
                //generate texture
                unsigned int character_glyph;
                glGenTextures(1, &character_glyph);
                glBindTexture(GL_TEXTURE_2D, character_glyph);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                    font->glyph->bitmap.width, font->glyph->bitmap.rows,
                    0, GL_RED, GL_UNSIGNED_BYTE, font->glyph->bitmap.buffer);
                //set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                //store that shit
                Renderer::Character sc = {
                    character_glyph,
                    LB::Vec2<unsigned int>{font->glyph->bitmap.width, font->glyph->bitmap.rows},
                    LB::Vec2<FT_Int>{font->glyph->bitmap_left, font->glyph->bitmap_top},
                    static_cast<unsigned int>(font->glyph->advance.x)
                };
                Characters.emplace(std::pair<char, Renderer::Character>(c, sc));

                maxAscent = maxAscent < font->glyph->bitmap_top ? font->glyph->bitmap_top : maxAscent;
                maxDescent = maxDescent < font->glyph->metrics.height - font->glyph->bitmap_top ?
                    font->glyph->metrics.height - font->glyph->bitmap_top : maxDescent;
            }
            //insert the height
            largest_height = maxAscent + maxDescent;
            textRender->get_heights_map().emplace(e.stem().string(), largest_height);

            //-------------------LOAD FONT------------------------
            textRender->get_font_map().emplace(std::make_pair(e.stem().string(), Characters));
            Characters.clear();
            //free up all the used resources from FT
            FT_Done_Face(font);
        }

        for (auto const& e : fonts2) {
            //load font
            if (FT_New_Face(ft, e.string().c_str(), 0, &font)) {
                DebuggerLogError("ERROR on the freetype: could not load font");
            }
            //set default font face
            FT_Set_Pixel_Sizes(font, 0, 50); //the width is 0 so it is based off the height value

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            unsigned int maxAscent{}, maxDescent{};
            for (unsigned char c{}; c < 128; ++c) {
                //load glyph
                if (FT_Load_Char(font, c, FT_LOAD_RENDER)) {
                    DebuggerLogErrorFormat("ERROR on the freetype: could not load glyph %c", c);
                    continue;
                }
                //generate texture
                unsigned int character_glyph;
                glGenTextures(1, &character_glyph);
                glBindTexture(GL_TEXTURE_2D, character_glyph);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                    font->glyph->bitmap.width, font->glyph->bitmap.rows,
                    0, GL_RED, GL_UNSIGNED_BYTE, font->glyph->bitmap.buffer);
                //set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                //store that shit
                Renderer::Character sc = {
                    character_glyph,
                    LB::Vec2<unsigned int>{font->glyph->bitmap.width, font->glyph->bitmap.rows},
                    LB::Vec2<FT_Int>{font->glyph->bitmap_left, font->glyph->bitmap_top},
                    static_cast<unsigned int>(font->glyph->advance.x)
                };
                Characters.emplace(std::pair<char, Renderer::Character>(c, sc));

                maxAscent = maxAscent < font->glyph->bitmap_top ? font->glyph->bitmap_top : maxAscent;
                maxDescent = maxDescent < font->glyph->metrics.height - font->glyph->bitmap_top ?
                    font->glyph->metrics.height - font->glyph->bitmap_top : maxDescent;
            }
            //insert the height
            largest_height = maxAscent + maxDescent;
            textRender->get_heights_map().emplace(e.stem().string(), largest_height);
            //-------------------LOAD FONT------------------------

            //-------------------LOAD FONT------------------------
            textRender->get_font_map().emplace(std::make_pair(e.stem().string(), Characters));
            Characters.clear();
            //free up all the used resources from FT
            FT_Done_Face(font);
        }
        //free ft lib
        FT_Done_FreeType(ft);

        //create the vertex array and buffer
        glGenVertexArrays(1, &textRender->get_vertex_array());
        glGenBuffers(1, &textRender->get_vertex_buffer());
        glBindVertexArray(textRender->get_vertex_array());
        glBindBuffer(GL_ARRAY_BUFFER, textRender->get_vertex_buffer());
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        //setup the shader program
        LoadShader("Assets/Shaders/text.shader", textRender->get_shader());
    }

    void AssetManager::LoadShader(const std::string& shader_file_name, unsigned int& shader_handle)
    {
        shader_source shd_pgm{ shader_parser(shader_file_name.c_str()) };
        shader_handle = create_shader(shd_pgm.vtx_shd.c_str(), shd_pgm.frg_shd.c_str());
    }
}

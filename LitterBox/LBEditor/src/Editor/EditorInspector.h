/*!************************************************************************
 \file				EditorHierarchy.h
 \author(s)			Kenji Brannon Chong, Vanessa Chua Siew Jin
 \par DP email(s):	kenjibrannon.c@digipen.edu, vanessasiewjin.chua@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains functions declarations for the hierarchy layer of the
 Editor. This is to ensure that there is a overview of all the game objects
 in the game.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Physics/ColliderManager.h"

namespace LB
{
    class EditorInspector : public Layer
    {
    public:

        /*!***********************************************************************
          \brief
          Constructor for the EditorInspector class.
          \return
          Nothing.
        *************************************************************************/
        EditorInspector(std::string layerName);

        /*!***********************************************************************
          \brief
          Initializes the EditorInspector layer.
          \return
          Nothing.
        *************************************************************************/
        void Initialize() override;

        /*!***********************************************************************
          \brief
          Updates the EditorInspector layer.
          \return
          Nothing.
        *************************************************************************/
        void UpdateLayer() override;

        /*!***********************************************************************
          \brief
          Destroys the EditorInspector layer.
          \return
          Nothing.
        *************************************************************************/
        void Destroy() {}

        /*!***********************************************************************
          \brief
          Update the currently inspected game object.
          \return
          Nothing.
        *************************************************************************/
        void UpdateInspectedGO(GameObject* newInspectedGO);

        /*!***********************************************************************
          \brief
          Get the currently inspected game object.
          \return
          A pointer to the currently inspected GameObject.
        *************************************************************************/
        GameObject* GetInspectedGO();

        /*!***********************************************************************
          \brief
          Check if a game object is currently being inspected.
          \return
          True if a game object is being inspected, false otherwise.
        *************************************************************************/
        bool IsGOInspected();
        bool isPrefab = false;
    private:
        GameObject* m_inspectedGO{ nullptr };
        char m_inspectedName[256]{}, m_nameBuffer1[64]{};
    };
    
    // For event subscription
    /*!***********************************************************************
      \brief
      To get the original function, UpdateInspectedGO called as an event.
      \return
      Nothing.
    *************************************************************************/
    void UpdateInspectedGO(GameObject* newInspectedGO);

    /*!***********************************************************************
      \brief
      To get the original function, DeselectObject called as an event.
      \return
      Nothing.
    *************************************************************************/
    void DeselectObject(bool isPlaying);

    /*!***********************************************************************
      \brief
      To get the original function, DeselectObject called as an event, overloaded.
      \return
      Nothing.
    *************************************************************************/
    void DeselectObject(Scene* newScene);

    extern EditorInspector* EDITORINSPECTOR;
}


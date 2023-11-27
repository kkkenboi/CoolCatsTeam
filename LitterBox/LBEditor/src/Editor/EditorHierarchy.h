/*!************************************************************************
 \file				EditorHierarchy.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				03/11/2023
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
#include "LitterBox/Scene/SceneManager.h"

namespace LB
{
    class EditorHierarchy : public Layer
    {
    public:

        /*!***********************************************************************
          \brief
          Default Constructor for EditorHierarchy
          \return
          Nothing.
        *************************************************************************/
        EditorHierarchy(std::string layerName);

        /*!***********************************************************************
          \brief
          Initializes the EditorHierarchy layer.
          \return
          Nothing.
        *************************************************************************/
        void Initialize() override;

        /*!***********************************************************************
          \brief
          Updates the EditorHierarchy layer.
          \return
          Nothing.
        *************************************************************************/
        void UpdateLayer() override;

        /*!***********************************************************************
          \brief
          Destroys the EditorHierarchy layer.
          \return
          Nothing.
        *************************************************************************/
        void Destroy() {}

        /*!***********************************************************************
          \brief
          Draws the root of the hierarchy.
          \return
          Nothing.
        *************************************************************************/
        void DrawRoot();

        /*!***********************************************************************
          \brief
          Draws an item in the hierarchy.
          \return
          Nothing.
        *************************************************************************/
        bool DrawItem(CPTransform* item);

        /*!***********************************************************************
          \brief
          Updates the loaded scene in the hierarchy 
          \return
          Nothing.
        *************************************************************************/
        void UpdateSceneLoaded(Scene* loadedScene);

        /*!***********************************************************************
          \brief
          Updates the currently clicked item in the hierarchy.
          \return
          Nothing.
        *************************************************************************/
        void UpdateClickedItem(CPTransform* newClickedItem);

        /*!***********************************************************************
          \brief
          Deletes the clicked GameObject
        *************************************************************************/
        void DeleteSelectedObject();

        /*!***********************************************************************
          \brief
          Deselected but does not delete the clicked GameObject
        *************************************************************************/
        void DeselectSelectedObject();

        /*!***********************************************************************
          \brief
          Returns the current GameObject clicked, if any
        *************************************************************************/
        CPTransform* GetClickedItem();

        // Lets any subscribers know a new object has been selected in the hierarchy
        Event<GameObject*> onNewObjectSelected;

    private:
        CPTransform* m_draggedItem{ nullptr };
        CPTransform* m_clickedItem{ nullptr };
        Scene* m_loadedScene{ nullptr };
    };

    // For Event Subscription
    /*!***********************************************************************
      \brief
      Updates the loaded scene.
    *************************************************************************/
    void UpdateSceneLoaded(Scene* loadedScene);
    /*!***********************************************************************
      \brief
      Deletes the clicked GameObject
    *************************************************************************/
    void DeleteSelectedObject();
    /*!***********************************************************************
      \brief
      Checks if the gameobject deleted during gameplay is the same as the
      one inspected.
    *************************************************************************/
    void CheckGameObjectDeleted(GameObject* deletedGO);

    extern EditorHierarchy* EDITORHIERACHY;
}

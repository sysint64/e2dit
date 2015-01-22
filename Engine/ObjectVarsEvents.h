#ifndef H_OBJECT_VARS_EVENT
#define H_OBJECT_VARS_EVENT

// Texture 2D Set Values ----------------------------------------------------------------------->

void SetTex2DValues (Texture2D *Val, UIElement *Element);

// Events
void PrevBgVarProgress  (UIElement *Sender);
void PrevTexVarProgress (UIElement *Sender);
//
void SetTex2DPathFinder (UIElement *Sender);
void SetTex2DPath       (UIElement *Sender);
void SetTex2DWrapS      (UIElement *Sender);
void SetTex2DWrapT      (UIElement *Sender);
void SetTex2DFilter     (UIElement *Sender);
void SetTex2DMipmaps    (UIElement *Sender);
void SetTex2DPremA      (UIElement *Sender);

Texture2D *CurVarTexture = 0;

// <----------------------------------------------------------------------- Texture 2D Set Values

#endif

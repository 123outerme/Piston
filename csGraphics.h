#ifndef CSGRAPHICS_H_INCLUDED
#define CSGRAPHICS_H_INCLUDED

/* ++ CoSprite 2D Engine ++
  -- initCoSprite() error codes:  --
  error code 0: No error
  error code 1: SDL systems failed to initialize
  error code 2: Window could not be created
  error code 3: Renderer failed to initialize
*/

#ifndef COSPRITE_VERSION
    #define COSPRITE_VERSION_MAJOR 0
    #define COSPRITE_VERSION_MINOR 9
    #define COSPRITE_VERSION_PATCH 1
    #define COSPRITE_VERSION "0.9.1"
#endif //COSPRITE_VERSION
#define SDL_MAIN_HANDLED 1

//#includes:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "SDL/SDL.h"       //This is included because it's an SDL2 program... duh
#include "SDL/SDL_image.h" //This is included so we can use PNGs.
#include "SDL/SDL_ttf.h"   //This is included for text stuff
#include "SDL/SDL_mixer.h" //This is included for audio


//#defines:
#ifndef bool
    #define bool char
    #define false 0
    #define true 1
    #define boolToString(bool) (bool ? "true" : "false")
#endif // bool
#ifndef NULL
    #define NULL ((void*) 0)
#endif //NULL
#ifndef PI
    #define PI (3.14159265359879)
    #define radToDeg(x) (180.0 * x / PI)
    #define degToRad(x) (x * PI / 180.0)
#endif // PI
#ifndef MAX_PATH
#define MAX_PATH	(260)
#endif  //MAX_PATH



//struct definitions:
typedef struct _coSprite {
    SDL_Window** windows;
    int windowsOpen;
    SDL_Renderer* mainRenderer;
    TTF_Font* mainFont;
    int windowW;
    int windowH;
    SDL_Color colorKey;
    bool canDrawText;
    int soundVolume;
    int musicVolume;
} coSprite;

typedef struct _cDoubleRect {
    double x;
    double y;
    double w;
    double h;
} cDoubleRect;

typedef struct _cDoublePt {
    double x;
    double y;
} cDoublePt;

typedef struct _cSprite {
    SDL_Texture* texture;
    char textureFilepath[MAX_PATH];
    int id;
    cDoubleRect drawRect;
    cDoubleRect srcClipRect;
    cDoublePt center;
    double scale;
    SDL_RendererFlip flip;
    double degrees;
    int drawPriority;  /**< 0 - not drawn. 1-5 - drawn. Lower number = drawn later */
    bool fixed;  /**< if true, won't be affected by camera movement */
    void* subclass;  /**< fill with any extraneous data or pointer to another struct */
} cSprite;

/*typedef struct _cCircle {
    cDoublePt pt;
    double r;
    cDoublePt center;
    double scale;
    SDL_RendererFlip flip;
    double degrees;
    int drawPriority;  / **< 0 - not drawn. 1-5 - drawn. Lower number = drawn later * /
    bool fixed;  / **< if true, won't be affected by camera movement * /
} cCircle;*/

typedef struct _c2DModel {  //essentially a 2D version of a wireframe model: A collection of sprites with relative coordinates
    cSprite* sprites;
    int numSprites;
    cDoubleRect rect;
    cDoublePt center;
    double scale;
    SDL_RendererFlip flip;
    double degrees;
    int drawPriority;
    bool fixed;  /**< if true, won't be affected by camera movement */
    void* subclass;
} c2DModel;

typedef struct _cText {
    char* string;
    SDL_Texture* texture;
    cDoubleRect rect;
    int drawPriority; /**< 0 - not drawn. 1-5 - drawn. Lower number = drawn later */
    SDL_Color textColor;
    SDL_Color bgColor;
    SDL_RendererFlip flip;
    double degrees;
    bool fixed;  /**< if true, won't be affected by camera movement */
} cText;

typedef struct _cCamera {
    cDoubleRect rect;
    double scale;
    double degrees;
} cCamera;

typedef struct _cResource {
    void* subclass;
    void (*drawingRoutine)(void*);
    int drawPriority; /**< 0 - not drawn. 1-5 - drawn. Lower number = drawn later */
} cResource;

typedef struct _cScene {
    SDL_Color bgColor;
    cCamera* camera;
    cSprite** sprites;
    int spriteCount;
    c2DModel** models;
    int modelCount;
    cResource** resources;
    int resCount;
    cText** strings;
    int stringCount;
} cScene;

//function prototypes:

//initialization
int initCoSprite();
void closeCoSprite();
int openCWindow(SDL_Window* windowPtr, char* windowName, int windowWidth, int windowHeight);
void closeCWindow(int windowPos);
bool loadIMG(char* imgPath, SDL_Texture** dest);
bool loadTTFont(char* filePath, TTF_Font** dest, int sizeInPts);
int* loadTextTexture(char* text, SDL_Texture** dest, int maxW, SDL_Color color, bool isBlended);

//drawing
void initCSprite(cSprite* sprite, SDL_Texture* texture, char* textureFilepath, int id, cDoubleRect drawRect, cDoubleRect srcClipRect, cDoublePt* center, double scale, SDL_RendererFlip flip, double degrees, bool fixed, void* subclass, int drawPriority);
void destroyCSprite(cSprite* sprite);
void drawCSprite(cSprite sprite, cCamera camera, bool update, bool fixedOverride);
void initC2DModel(c2DModel* model, cSprite* sprites, int numSprites, cDoublePt position, cDoublePt* center, double scale, SDL_RendererFlip flip, double degrees, bool fixed, void* subclass, int drawPriority);
void destroyC2DModel(c2DModel* model);
void importC2DModel(c2DModel* model, char* filepath);
void exportC2DModel(c2DModel* model, char* filepath);
void drawC2DModel(c2DModel model, cCamera camera, bool update);
void initCText(cText* text, char* string, cDoubleRect rect, SDL_Color textColor, SDL_Color bgColor, SDL_RendererFlip flip, double degrees, bool fixed, int drawPriority);
void destroyCText(cText* text);
void drawCText(cText text, cCamera camera, bool update);
void initCResource(cResource* res, void* subclass, void (*drawingRoutine)(void*), int drawPriority);
void drawCResource(cResource* res);
void destroyCResource(cResource* res);
void initCCamera(cCamera* camera, cDoubleRect rect, double scale, double degrees);
void destroyCCamera(cCamera* camera);
void initCScene(cScene* scenePtr, SDL_Color bgColor, cCamera* camera, cSprite* sprites[], int spriteCount, c2DModel* models[], int modelCount, cResource* resources[], int resCount, cText* strings[], int stringCount);
void destroyCScene(cScene* scenePtr);
void drawCScene(cScene* scenePtr, bool clearScreen, bool redraw);
void drawText(char* input, int x, int y, int maxW, int maxH, SDL_Color color, bool render);
bool checkCSpriteCollision(cSprite sprite1, cSprite sprite2);
bool checkC2DModelCollision(c2DModel model1, c2DModel model2, bool fast);

cDoublePt rotatePoint(cDoublePt pt, cDoublePt center, int degrees);

//file I/O
int createFile(char* filePath);
int checkFile(char* filePath, int desiredLines);
int appendLine(char* filePath, char* stuff, bool addNewline);
int replaceLine(char* filePath, int lineNum, char* stuff, int maxLength, bool addNewline);
char* readLine(char* filePath, int lineNum, int maxLength, char** output);


//global variable declarations:
SDL_Window* mainWindow;
coSprite global;

#endif // CSGRAPHICS_H_INCLUDED

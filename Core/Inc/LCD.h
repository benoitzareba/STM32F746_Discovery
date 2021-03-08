//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// HEADER      :  LCD.h
//
//=============================================================================
#ifndef _LCD_H_
#define _LCD_H_

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "TypeDefs.h"
#include "SDRAM.h"
#include "stm32f7xx.h"
#include "rk043fn48h.h"
#include "../../../Utilities/Fonts/fonts.h"

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define MAX_LAYER_NUMBER                     ((UINT32)2)
#define LCD_LayerCfgTypeDef                  LTDC_LayerCfgTypeDef
#define LTDC_ACTIVE_LAYER                    ((UINT32)1)
#define LCD_OK                               ((UINT8)0x00)
#define LCD_ERROR                            ((UINT8)0x01)
#define LCD_TIMEOUT                          ((UINT8)0x02)
#define LCD_FB_START_ADDRESS                 ((UINT32)0xC0000000)
#define LCD_COLOR_BLUE                       ((UINT32)0xFF0000FF)
#define LCD_COLOR_GREEN                      ((UINT32)0xFF00FF00)
#define LCD_COLOR_RED                        ((UINT32)0xFFFF0000)
#define LCD_COLOR_CYAN                       ((UINT32)0xFF00FFFF)
#define LCD_COLOR_MAGENTA                    ((UINT32)0xFFFF00FF)
#define LCD_COLOR_YELLOW                     ((UINT32)0xFFFFFF00)
#define LCD_COLOR_LIGHTBLUE                  ((UINT32)0xFF8080FF)
#define LCD_COLOR_LIGHTGREEN                 ((UINT32)0xFF80FF80)
#define LCD_COLOR_LIGHTRED                   ((UINT32)0xFFFF8080)
#define LCD_COLOR_LIGHTCYAN                  ((UINT32)0xFF80FFFF)
#define LCD_COLOR_LIGHTMAGENTA               ((UINT32)0xFFFF80FF)
#define LCD_COLOR_LIGHTYELLOW                ((UINT32)0xFFFFFF80)
#define LCD_COLOR_DARKBLUE                   ((UINT32)0xFF000080)
#define LCD_COLOR_DARKGREEN                  ((UINT32)0xFF008000)
#define LCD_COLOR_DARKRED                    ((UINT32)0xFF800000)
#define LCD_COLOR_DARKCYAN                   ((UINT32)0xFF008080)
#define LCD_COLOR_DARKMAGENTA                ((UINT32)0xFF800080)
#define LCD_COLOR_DARKYELLOW                 ((UINT32)0xFF808000)
#define LCD_COLOR_WHITE                      ((UINT32)0xFFFFFFFF)
#define LCD_COLOR_LIGHTGRAY                  ((UINT32)0xFFD3D3D3)
#define LCD_COLOR_GRAY                       ((UINT32)0xFF808080)
#define LCD_COLOR_DARKGRAY                   ((UINT32)0xFF404040)
#define LCD_COLOR_BLACK                      ((UINT32)0xFF000000)
#define LCD_COLOR_BROWN                      ((UINT32)0xFFA52A2A)
#define LCD_COLOR_ORANGE                     ((UINT32)0xFFFFA500)
#define LCD_COLOR_TRANSPARENT                ((UINT32)0xFF000000)
#define LCD_COLOR_BACKGROUND_PAGE            ((UINT32)0xFF27303A)
#define LCD_COLOR_BACKGROUND_ITEM            ((UINT32)0xFF373D44)

#define LCD_FONT_24                          Font24
#define LCD_FONT_20                          Font20
#define LCD_FONT_16                          Font16
#define LCD_FONT_12                          Font12
#define LCD_FONT_8                           Font8
#define LCD_DEFAULT_FONT                     LCD_FONT_24

#define LCD_RELOAD_IMMEDIATE                 ((UINT32)LTDC_SRCR_IMR)
#define LCD_RELOAD_VERTICAL_BLANKING         ((UINT32)LTDC_SRCR_VBR)

#define LCD_DISP_PIN                         GPIO_PIN_12
#define LCD_DISP_GPIO_PORT                   GPIOI
#define LCD_DISP_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOI_CLK_ENABLE()
#define LCD_DISP_GPIO_CLK_DISABLE()          __HAL_RCC_GPIOI_CLK_DISABLE()

#define LCD_BL_CTRL_PIN                      GPIO_PIN_3
#define LCD_BL_CTRL_GPIO_PORT                GPIOK
#define LCD_BL_CTRL_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOK_CLK_ENABLE()
#define LCD_BL_CTRL_GPIO_CLK_DISABLE()       __HAL_RCC_GPIOK_CLK_DISABLE()

typedef enum
{
   CENTER_MODE             = 0x01,
   RIGHT_MODE              = 0x02,
   LEFT_MODE               = 0x03
} Text_AlignModeTypdef;

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------
typedef struct
{
   UINT32   TextColor;
   UINT32   BackColor;
   sFONT    *pFont;
} LCD_DrawPropTypeDef;

typedef struct
{
   INT16 X;
   INT16 Y;
} Point, * pPoint;

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
UINT8    LCD_Init                      (void);
UINT8    LCD_DeInit                    (void);
UINT8    LCD_Init                      (void);
UINT8    LCD_DeInit                    (void);
UINT32   LCD_GetXSize                  (void);
UINT32   LCD_GetYSize                  (void);
void     LCD_SetXSize                  (UINT32 imageWidthPixels);
void     LCD_SetYSize                  (UINT32 imageHeightPixels);
void     LCD_LayerDefaultInit          (UINT16 LayerIndex, UINT32 FrameBuffer);
void     LCD_LayerRgb565Init           (UINT16 LayerIndex, UINT32 FB_Address);
void     LCD_SetTransparency           (UINT32 LayerIndex, UINT8 Transparency);
void     LCD_SetTransparency_NoReload  (UINT32 LayerIndex, UINT8 Transparency);
void     LCD_SetLayerAddress           (UINT32 LayerIndex, UINT32 Address);
void     LCD_SetLayerAddress_NoReload  (UINT32 LayerIndex, UINT32 Address);
void     LCD_SetColorKeying            (UINT32 LayerIndex, UINT32 RGBValue);
void     LCD_SetColorKeying_NoReload   (UINT32 LayerIndex, UINT32 RGBValue);
void     LCD_ResetColorKeying          (UINT32 LayerIndex);
void     LCD_ResetColorKeying_NoReload (UINT32 LayerIndex);
void     LCD_SetLayerWindow            (UINT16 LayerIndex, UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height);
void     LCD_SetLayerWindow_NoReload   (UINT16 LayerIndex, UINT16 Xpos, uint16_t Ypos, UINT16 Width, UINT16 Height);
void     LCD_SelectLayer               (UINT32 LayerIndex);
void     LCD_SetLayerVisible           (UINT32 LayerIndex, FunctionalState State);
void     LCD_SetLayerVisible_NoReload  (UINT32 LayerIndex, FunctionalState State);
void     LCD_Reload                    (UINT32 ReloadType);
void     LCD_SetTextColor              (UINT32 Color);
UINT32   LCD_GetTextColor              (void);
void     LCD_SetBackColor              (UINT32 Color);
UINT32   LCD_GetBackColor              (void);
void     LCD_SetFont                   (sFONT *fonts);
sFONT*   LCD_GetFont                   (void);
UINT32   LCD_ReadPixel                 (UINT16 Xpos, UINT16 Ypos);
void     LCD_DrawPixel                 (UINT16 Xpos, UINT16 Ypos, UINT32 pixel);
void     LCD_Clear                     (UINT32 Color);
void     LCD_ClearStringLine           (UINT32 Line);
void     LCD_DisplayStringAtLine       (UINT16 Line, UINT8 *ptr);
void     LCD_DisplayStringAt           (UINT16 Xpos, UINT16 Ypos, UINT8 *Text, Text_AlignModeTypdef Mode);
void     LCD_DisplayChar               (UINT16 Xpos, UINT16 Ypos, UINT8 Ascii);
void     LCD_DrawHLine                 (UINT16 Xpos, UINT16 Ypos, UINT16 Length);
void     LCD_DrawVLine                 (UINT16 Xpos, UINT16 Ypos, UINT16 Length);
void     LCD_DrawLine                  (UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2);
void     LCD_DrawRect                  (UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height);
void     LCD_DrawCircle                (UINT16 Xpos, UINT16 Ypos, UINT16 Radius);
void     LCD_DrawPolygon               (pPoint Points, UINT16 PointCount);
void     LCD_DrawEllipse               (INT32 Xpos, INT32 Ypos, INT32 XRadius, INT32 YRadius);
void     LCD_DrawBitmap                (UINT32 Xpos, UINT32 Ypos, UINT8 *pbmp);
void     LCD_FillRect                  (UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height);
void     LCD_FillCircle                (UINT16 Xpos, UINT16 Ypos, UINT16 Radius);
void     LCD_FillPolygon               (pPoint Points, UINT16 PointCount);
void     LCD_FillEllipse               (INT32 Xpos, INT32 Ypos, INT32 XRadius, INT32 YRadius);
void     LCD_DisplayOff                (void);
void     LCD_DisplayOn                 (void);
void     LCD_MspInit                   (LTDC_HandleTypeDef *hltdc, void *Params);
void     LCD_MspDeInit                 (LTDC_HandleTypeDef *hltdc, void *Params);
void     LCD_ClockConfig               (LTDC_HandleTypeDef *hltdc, void *Params);
void     LCD_DrawRectWithThickness     (UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height, UINT8 Thickness);
void     LCD_DrawRectButton            (UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height, UINT32 color, CHAR8 *txt, BOOL selected);
void     LCD_DrawHLineThickness        (UINT16 Xpos, UINT16 Ypos, UINT16 Length, UINT8 Thickness);


#endif

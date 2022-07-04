//=============================================================================
//
// PROJECT     :  STM32F746-Discovery
// MODULE      :  LCD.c
// AUTHOR      :  Benoit ZAREBA
//
//=============================================================================

//=============================================================================
//--- DECLARATIONS
//=============================================================================

//-----------------------------------------------------------------------------
// Included files
//-----------------------------------------------------------------------------
#include "LCD.h"
#include <math.h>

//-----------------------------------------------------------------------------
// Constants : defines and enumerations
//-----------------------------------------------------------------------------
#define POLY_X(Z)                   ((INT32)((Points + Z)->X))
#define POLY_Y(Z)                   ((INT32)((Points + Z)->Y))
#define ABS(X)                      ((X) > 0 ? (X) : -(X))

//-----------------------------------------------------------------------------
// Structures and types
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// External variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------

//---------- Functions ----------
static void _DrawChar               (UINT16 Xpos, UINT16 Ypos, const UINT8 *c);
static void _FillTriangle           (UINT16 x1, UINT16 x2, UINT16 x3, UINT16 y1, UINT16 y2, UINT16 y3);
static void _FillBuffer             (UINT32 LayerIndex, void *pDst, UINT32 xSize, UINT32 ySize, UINT32 OffLine, UINT32 ColorIndex);
static void _ConvertLineToARGB8888  (void * pSrc, void *pDst, UINT32 xSize, UINT32 ColorMode);

//-----------------------------------------------------------------------------
// Private variables and functions
//-----------------------------------------------------------------------------

//---------- Variables ----------
static LTDC_HandleTypeDef           hLtdcHandler;
static DMA2D_HandleTypeDef          hDma2dHandler;
static UINT32                       ActiveLayer = 0;
static LCD_DrawPropTypeDef          DrawProp[MAX_LAYER_NUMBER];

//---------- Functions ----------

//=============================================================================
//--- DEFINITIONS
//=============================================================================

//-----------------------------------------------------------------------------
// FONCTION    : _DrawChar
//
// DESCRIPTION : Draws a character on LCD.
//-----------------------------------------------------------------------------
static void _DrawChar (UINT16 Xpos, UINT16 Ypos, const UINT8 *c)
{
   UINT32 i        = 0;
   UINT32 j        = 0;
   UINT16 height;
   UINT16 width;
   UINT8  offset;
   UINT8  *pchar;
   UINT32 line;

   height = DrawProp[ActiveLayer].pFont->Height;
   width  = DrawProp[ActiveLayer].pFont->Width;

   offset =  8 * ((width + 7) / 8) - width ;

   for (i = 0; i < height; i++)
   {
      pchar = ((UINT8 *)c + (width + 7) / 8 * i);

      switch (((width + 7) / 8))
      {
         case 1:
            line =  pchar[0];
            break;

         case 2:
            line =  (pchar[0]<< 8) | pchar[1];
            break;

         case 3:
         default:
            line =  (pchar[0]<< 16) | (pchar[1]<< 8) | pchar[2];
            break;
      }

      for (j = 0; j < width; j++)
      {
         if (line & (1 << (width- j + offset- 1)))
         {
            LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].TextColor);
         }
         else
         {
            LCD_DrawPixel((Xpos + j), Ypos, DrawProp[ActiveLayer].BackColor);
         }
      }

      Ypos++;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : _FillTriangle
//
// DESCRIPTION : Fills a triangle (between 3 points).
//-----------------------------------------------------------------------------
static void _FillTriangle (UINT16 x1, UINT16 x2, UINT16 x3, UINT16 y1, UINT16 y2, UINT16 y3)
{
   INT16 deltax      = 0;
   INT16 deltay      = 0;
   INT16 x           = 0;
   INT16 y           = 0;
   INT16 xinc1       = 0;
   INT16 xinc2       = 0;
   INT16 yinc1       = 0;
   INT16 yinc2       = 0;
   INT16 den         = 0;
   INT16 num         = 0;
   INT16 num_add     = 0;
   INT16 num_pixels  = 0;
   INT16 curpixel    = 0;

   //--- The difference between the x's
   deltax = ABS(x2 - x1);

   //--- The difference between the y's
   deltay = ABS(y2 - y1);
   x = x1;                       //--- Start x off at the first pixel
   y = y1;                       //--- Start y off at the first pixel

   //--- The x-values are increasing
   if (x2 >= x1)
   {
      xinc1 = 1;
      xinc2 = 1;
   }
   else //--- The x-values are decreasing
   {
      xinc1 = -1;
      xinc2 = -1;
   }

   //--- The y-values are increasing
   if (y2 >= y1)
   {
      yinc1 = 1;
      yinc2 = 1;
   }
   else //--- The y-values are decreasing
   {
      yinc1 = -1;
      yinc2 = -1;
   }

   //--- There is at least one x-value for every y-value
   if (deltax >= deltay)
   {
      //--- Don't change the x when numerator >= denominator
      xinc1 = 0;

      //--- Don't change the y for every iteration
      yinc2 = 0;

      den         = deltax;
      num         = deltax / 2;
      num_add     = deltay;

      //--- There are more x-values than y-values
      num_pixels  = deltax;
   }
   else //--- There is at least one y-value for every x-value
   {
      //--- Don't change the x for every iteration
      xinc2 = 0;

      //--- Don't change the y when numerator >= denominator
      yinc1 = 0;

      den         = deltay;
      num         = deltay / 2;
      num_add     = deltax;

      //--- There are more y-values than x-values
      num_pixels  = deltay;
   }

   for (curpixel = 0; curpixel <= num_pixels; curpixel++)
   {
      LCD_DrawLine(x, y, x3, y3);

      //--- Increase the numerator by the top of the fraction
      num += num_add;

      //--- Check if numerator >= denominator
      if (num >= den)
      {
         //--- Calculate the new numerator value
         num -= den;

         //--- Change the x as appropriate
         x += xinc1;

         //--- Change the y as appropriate
         y += yinc1;
      }

      //--- Change the x as appropriate
      x += xinc2;

      //--- Change the y as appropriate
      y += yinc2;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : _FillBuffer
//
// DESCRIPTION : Fills a buffer.
//-----------------------------------------------------------------------------
static void _FillBuffer (UINT32 LayerIndex, void *pDst, UINT32 xSize, UINT32 ySize, UINT32 OffLine, UINT32 ColorIndex)
{
   //--- Register to memory mode with ARGB8888 as color Mode
   hDma2dHandler.Init.Mode = DMA2D_R2M;

   if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
   {
      //--- RGB565 format
      hDma2dHandler.Init.ColorMode = DMA2D_RGB565;
   }
   else
   {
      //--- ARGB8888 format
      hDma2dHandler.Init.ColorMode = DMA2D_ARGB8888;
   }

   hDma2dHandler.Init.OutputOffset = OffLine;

   hDma2dHandler.Instance = DMA2D;

   //--- DMA2D Initialization
   if (HAL_DMA2D_Init(&hDma2dHandler) == HAL_OK)
   {
      if (HAL_DMA2D_ConfigLayer(&hDma2dHandler, LayerIndex) == HAL_OK)
      {
         if (HAL_DMA2D_Start(&hDma2dHandler, ColorIndex, (UINT32)pDst, xSize, ySize) == HAL_OK)
         {
            //--- Polling For DMA transfer
            HAL_DMA2D_PollForTransfer(&hDma2dHandler, 10);
         }
      }
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : _ConvertLineToARGB8888
//
// DESCRIPTION : Converts a line to an ARGB8888 pixel format.
//-----------------------------------------------------------------------------
static void _ConvertLineToARGB8888 (void *pSrc, void *pDst, UINT32 xSize, UINT32 ColorMode)
{
   //--- Configure the DMA2D Mode, Color Mode and output offset
   hDma2dHandler.Init.Mode                   = DMA2D_M2M_PFC;
   hDma2dHandler.Init.ColorMode              = DMA2D_ARGB8888;
   hDma2dHandler.Init.OutputOffset           = 0;

   //--- Foreground Configuration
   hDma2dHandler.LayerCfg[1].AlphaMode       = DMA2D_NO_MODIF_ALPHA;
   hDma2dHandler.LayerCfg[1].InputAlpha      = 0xFF;
   hDma2dHandler.LayerCfg[1].InputColorMode  = ColorMode;
   hDma2dHandler.LayerCfg[1].InputOffset     = 0;

   hDma2dHandler.Instance = DMA2D;

   //--- DMA2D Initialization
   if (HAL_DMA2D_Init(&hDma2dHandler) == HAL_OK)
   {
      if (HAL_DMA2D_ConfigLayer(&hDma2dHandler, 1) == HAL_OK)
      {
         if (HAL_DMA2D_Start(&hDma2dHandler, (UINT32)pSrc, (UINT32)pDst, xSize, 1) == HAL_OK)
         {
            //--- Polling For DMA transfer
            HAL_DMA2D_PollForTransfer(&hDma2dHandler, 10);
         }
      }
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_Init
//
// DESCRIPTION : Initializes the LCD : RK043FN48H
//-----------------------------------------------------------------------------
UINT8 LCD_Init (void)
{
   //--- Timing Configuration
   hLtdcHandler.Init.HorizontalSync       = (RK043FN48H_HSYNC - 1);
   hLtdcHandler.Init.VerticalSync         = (RK043FN48H_VSYNC - 1);
   hLtdcHandler.Init.AccumulatedHBP       = (RK043FN48H_HSYNC + RK043FN48H_HBP - 1);
   hLtdcHandler.Init.AccumulatedVBP       = (RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
   hLtdcHandler.Init.AccumulatedActiveH   = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP - 1);
   hLtdcHandler.Init.AccumulatedActiveW   = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP - 1);
   hLtdcHandler.Init.TotalHeigh           = (RK043FN48H_HEIGHT + RK043FN48H_VSYNC + RK043FN48H_VBP + RK043FN48H_VFP - 1);
   hLtdcHandler.Init.TotalWidth           = (RK043FN48H_WIDTH + RK043FN48H_HSYNC + RK043FN48H_HBP + RK043FN48H_HFP - 1);

   //--- LCD clock configuration
   LCD_ClockConfig(&hLtdcHandler, NULL);

   //--- Initialize the LCD pixel width and pixel height
   hLtdcHandler.LayerCfg->ImageWidth      = RK043FN48H_WIDTH;
   hLtdcHandler.LayerCfg->ImageHeight     = RK043FN48H_HEIGHT;

   //--- Background value
   hLtdcHandler.Init.Backcolor.Blue       = 0;
   hLtdcHandler.Init.Backcolor.Green      = 0;
   hLtdcHandler.Init.Backcolor.Red        = 0;

   //--- Polarity
   hLtdcHandler.Init.HSPolarity           = LTDC_HSPOLARITY_AL;
   hLtdcHandler.Init.VSPolarity           = LTDC_VSPOLARITY_AL;
   hLtdcHandler.Init.DEPolarity           = LTDC_DEPOLARITY_AL;
   hLtdcHandler.Init.PCPolarity           = LTDC_PCPOLARITY_IPC;
   hLtdcHandler.Instance                  = LTDC;

   if (HAL_LTDC_GetState(&hLtdcHandler) == HAL_LTDC_STATE_RESET)
   {
      //--- Initialize the LCD MSP
      LCD_MspInit(&hLtdcHandler, NULL);
   }

   HAL_LTDC_Init(&hLtdcHandler);

   //--- Assert display enable LCD_DISP pin
   HAL_GPIO_WritePin(LCD_DISP_GPIO_PORT, LCD_DISP_PIN, GPIO_PIN_SET);

   //--- Assert backlight LCD_BL_CTRL pin
   HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);

#if !defined(DATA_IN_ExtSDRAM)
   //--- Initialize the SDRAM
   SDRAM_Init();
#endif

   //--- Initialize the font
   LCD_SetFont(&LCD_DEFAULT_FONT);

   LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
   LCD_SelectLayer(LTDC_ACTIVE_LAYER);

   return LCD_OK;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DeInit
//
// DESCRIPTION : DeInitializes the LCD.
//-----------------------------------------------------------------------------
UINT8 LCD_DeInit (void)
{
   //--- Initialize the hLtdcHandler Instance parameter
   hLtdcHandler.Instance = LTDC;

   //--- Disable LTDC block
   __HAL_LTDC_DISABLE(&hLtdcHandler);

   //--- DeInit the LTDC
   HAL_LTDC_DeInit(&hLtdcHandler);

   //--- DeInit the LTDC MSP
   LCD_MspDeInit(&hLtdcHandler, NULL);

   return LCD_OK;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_GetXSize
//
// DESCRIPTION : Gets the LCD X size
//-----------------------------------------------------------------------------
UINT32 LCD_GetXSize (void)
{
   return hLtdcHandler.LayerCfg[ActiveLayer].ImageWidth;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_GetYSize
//
// DESCRIPTION : Used LCD Y size
//-----------------------------------------------------------------------------
UINT32 LCD_GetYSize (void)
{
   return hLtdcHandler.LayerCfg[ActiveLayer].ImageHeight;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetXSize
//
// DESCRIPTION : Set the LCD X size.
//-----------------------------------------------------------------------------
void LCD_SetXSize (UINT32 imageWidthPixels)
{
   hLtdcHandler.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetYSize
//
// DESCRIPTION : Set the LCD Y size.
//-----------------------------------------------------------------------------
void LCD_SetYSize (UINT32 imageHeightPixels)
{
   hLtdcHandler.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_LayerDefaultInit
//
// DESCRIPTION : Initializes the LCD layer in ARGB8888 format (32 bits per pixel).
//-----------------------------------------------------------------------------
void LCD_LayerDefaultInit (UINT16 LayerIndex, UINT32 FB_Address)
{
   LCD_LayerCfgTypeDef  layer_cfg;

   //--- Layer Init
   layer_cfg.WindowX0          = 0;
   layer_cfg.WindowX1          = LCD_GetXSize();
   layer_cfg.WindowY0          = 0;
   layer_cfg.WindowY1          = LCD_GetYSize();
   layer_cfg.PixelFormat       = LTDC_PIXEL_FORMAT_ARGB8888;
   layer_cfg.FBStartAdress     = FB_Address;
   layer_cfg.Alpha             = 255;
   layer_cfg.Alpha0            = 0;
   layer_cfg.Backcolor.Blue    = 0;
   layer_cfg.Backcolor.Green   = 0;
   layer_cfg.Backcolor.Red     = 0;
   layer_cfg.BlendingFactor1   = LTDC_BLENDING_FACTOR1_PAxCA;
   layer_cfg.BlendingFactor2   = LTDC_BLENDING_FACTOR2_PAxCA;
   layer_cfg.ImageWidth        = LCD_GetXSize();
   layer_cfg.ImageHeight       = LCD_GetYSize();

   HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex);

   DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
   DrawProp[LayerIndex].pFont     = &Font24;
   DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_LayerRgb565Init
//
// DESCRIPTION : Initializes the LCD layer in RGB565 format (16 bits per pixel).
//-----------------------------------------------------------------------------
void LCD_LayerRgb565Init (UINT16 LayerIndex, UINT32 FB_Address)
{
   LCD_LayerCfgTypeDef  layer_cfg;

   //--- Layer Init
   layer_cfg.WindowX0         = 0;
   layer_cfg.WindowX1         = LCD_GetXSize();
   layer_cfg.WindowY0         = 0;
   layer_cfg.WindowY1         = LCD_GetYSize();
   layer_cfg.PixelFormat      = LTDC_PIXEL_FORMAT_RGB565;
   layer_cfg.FBStartAdress    = FB_Address;
   layer_cfg.Alpha            = 255;
   layer_cfg.Alpha0           = 0;
   layer_cfg.Backcolor.Blue   = 0;
   layer_cfg.Backcolor.Green  = 0;
   layer_cfg.Backcolor.Red    = 0;
   layer_cfg.BlendingFactor1  = LTDC_BLENDING_FACTOR1_PAxCA;
   layer_cfg.BlendingFactor2  = LTDC_BLENDING_FACTOR2_PAxCA;
   layer_cfg.ImageWidth       = LCD_GetXSize();
   layer_cfg.ImageHeight      = LCD_GetYSize();

   HAL_LTDC_ConfigLayer(&hLtdcHandler, &layer_cfg, LayerIndex);

   DrawProp[LayerIndex].BackColor = LCD_COLOR_WHITE;
   DrawProp[LayerIndex].pFont     = &Font24;
   DrawProp[LayerIndex].TextColor = LCD_COLOR_BLACK;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SelectLayer
//
// DESCRIPTION : Selects the LCD Layer.
//-----------------------------------------------------------------------------
void LCD_SelectLayer (UINT32 LayerIndex)
{
   ActiveLayer = LayerIndex;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetLayerVisible
//
// DESCRIPTION : Sets an LCD Layer visible.
//-----------------------------------------------------------------------------
void LCD_SetLayerVisible (UINT32 LayerIndex, FunctionalState State)
{
   if (State == ENABLE)
   {
      __HAL_LTDC_LAYER_ENABLE(&hLtdcHandler, LayerIndex);
   }
   else
   {
      __HAL_LTDC_LAYER_DISABLE(&hLtdcHandler, LayerIndex);
   }
   __HAL_LTDC_RELOAD_CONFIG(&hLtdcHandler);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetLayerVisible_NoReload
//
// DESCRIPTION : Sets an LCD Layer visible without reloading.
//-----------------------------------------------------------------------------
void LCD_SetLayerVisible_NoReload (UINT32 LayerIndex, FunctionalState State)
{
   if (State == ENABLE)
   {
      __HAL_LTDC_LAYER_ENABLE(&hLtdcHandler, LayerIndex);
   }
   else
   {
      __HAL_LTDC_LAYER_DISABLE(&hLtdcHandler, LayerIndex);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetTransparency
//
// DESCRIPTION : Configures the transparency.
//-----------------------------------------------------------------------------
void LCD_SetTransparency (UINT32 LayerIndex, UINT8 Transparency)
{
   HAL_LTDC_SetAlpha(&hLtdcHandler, Transparency, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetTransparency_NoReload
//
// DESCRIPTION : Configures the transparency without reloading.
//-----------------------------------------------------------------------------
void LCD_SetTransparency_NoReload (UINT32 LayerIndex, UINT8 Transparency)
{
   HAL_LTDC_SetAlpha_NoReload(&hLtdcHandler, Transparency, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetLayerAddress
//
// DESCRIPTION : Sets an LCD layer frame buffer address.
//-----------------------------------------------------------------------------
void LCD_SetLayerAddress (UINT32 LayerIndex, UINT32 Address)
{
   HAL_LTDC_SetAddress(&hLtdcHandler, Address, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetLayerAddress_NoReload
//
// DESCRIPTION : Sets an LCD layer frame buffer address without reloading.
//-----------------------------------------------------------------------------
void LCD_SetLayerAddress_NoReload (UINT32 LayerIndex, UINT32 Address)
{
   HAL_LTDC_SetAddress_NoReload(&hLtdcHandler, Address, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetLayerWindow
//
// DESCRIPTION : Sets display window.
//-----------------------------------------------------------------------------
void LCD_SetLayerWindow (UINT16 LayerIndex, UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height)
{
   //--- Reconfigure the layer size
   HAL_LTDC_SetWindowSize(&hLtdcHandler, Width, Height, LayerIndex);

   //--- Reconfigure the layer position
   HAL_LTDC_SetWindowPosition(&hLtdcHandler, Xpos, Ypos, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetLayerWindow_NoReload
//
// DESCRIPTION : Sets display window without reloading.
//-----------------------------------------------------------------------------
void LCD_SetLayerWindow_NoReload (UINT16 LayerIndex, UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height)
{
   //--- Reconfigure the layer size
   HAL_LTDC_SetWindowSize_NoReload(&hLtdcHandler, Width, Height, LayerIndex);

   //--- Reconfigure the layer position
   HAL_LTDC_SetWindowPosition_NoReload(&hLtdcHandler, Xpos, Ypos, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetColorKeying
//
// DESCRIPTION : Configures and sets the color keying.
//-----------------------------------------------------------------------------
void LCD_SetColorKeying (UINT32 LayerIndex, UINT32 RGBValue)
{
   //--- Configure and Enable the color Keying for LCD Layer
   HAL_LTDC_ConfigColorKeying(&hLtdcHandler, RGBValue, LayerIndex);
   HAL_LTDC_EnableColorKeying(&hLtdcHandler, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetColorKeying_NoReload
//
// DESCRIPTION : Configures and sets the color keying without reloading.
//-----------------------------------------------------------------------------
void LCD_SetColorKeying_NoReload(UINT32 LayerIndex, UINT32 RGBValue)
{
   //--- Configure and Enable the color Keying for LCD Layer */
   HAL_LTDC_ConfigColorKeying_NoReload(&hLtdcHandler, RGBValue, LayerIndex);
   HAL_LTDC_EnableColorKeying_NoReload(&hLtdcHandler, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_ResetColorKeying
//
// DESCRIPTION : Disables the color keying.
//-----------------------------------------------------------------------------
void LCD_ResetColorKeying (UINT32 LayerIndex)
{
   //--- Disable the color Keying for LCD Layer
   HAL_LTDC_DisableColorKeying(&hLtdcHandler, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_ResetColorKeying_NoReload
//
// DESCRIPTION : Disables the color keying without reloading.
//-----------------------------------------------------------------------------
void LCD_ResetColorKeying_NoReload (UINT32 LayerIndex)
{
   //--- Disable the color Keying for LCD Layer
   HAL_LTDC_DisableColorKeying_NoReload(&hLtdcHandler, LayerIndex);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_Reload
//
// DESCRIPTION : Disables the color keying without reloading.
//-----------------------------------------------------------------------------
void LCD_Reload (UINT32 ReloadType)
{
   HAL_LTDC_Reload (&hLtdcHandler, ReloadType);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetTextColor
//
// DESCRIPTION : Sets the LCD text color.
//-----------------------------------------------------------------------------
void LCD_SetTextColor (UINT32 Color)
{
   /*
   u_UINT32 dummy32;
   u_UINT32 dummyConv;

   dummy32.val = Color;

   dummy32.tab[0] = (dummy32.tab[0] & 0xF8) >> 3;
   dummy32.tab[1] = (dummy32.tab[1] & 0xFC) >> 2;
   dummy32.tab[2] = (dummy32.tab[2] & 0xF8) >> 3;

   dummyConv.val = dummy32.tab[0] | (dummy32.tab[1] << 5) | (dummy32.tab[2] << 11);
   */

   DrawProp[ActiveLayer].TextColor = Color;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_GetTextColor
//
// DESCRIPTION : Gets the LCD text color.
//-----------------------------------------------------------------------------
UINT32 LCD_GetTextColor (void)
{
   return DrawProp[ActiveLayer].TextColor;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetBackColor
//
// DESCRIPTION : Sets the LCD background color.
//-----------------------------------------------------------------------------
void LCD_SetBackColor(UINT32 Color)
{
   DrawProp[ActiveLayer].BackColor = Color;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_GetBackColor
//
// DESCRIPTION : Gets the LCD background color.
//-----------------------------------------------------------------------------
UINT32 LCD_GetBackColor(void)
{
   return DrawProp[ActiveLayer].BackColor;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_SetFont
//
// DESCRIPTION : Sets the LCD text font.
//-----------------------------------------------------------------------------
void LCD_SetFont (sFONT *fonts)
{
   DrawProp[ActiveLayer].pFont = fonts;
}

//-----------------------------------------------------------------------------
// FONCTION    : *LCD_GetFont
//
// DESCRIPTION : Gets the LCD text font.
//-----------------------------------------------------------------------------
sFONT *LCD_GetFont (void)
{
   return DrawProp[ActiveLayer].pFont;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_ReadPixel
//
// DESCRIPTION : Reads an LCD pixel.
//-----------------------------------------------------------------------------
UINT32 LCD_ReadPixel (UINT16 Xpos, UINT16 Ypos)
{
   UINT32 ret = 0;

   if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB8888)
   {
      //--- Read data value from SDRAM memory
      ret = *(__IO UINT32*) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos)));
   }
   else if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB888)
   {
      //--- Read data value from SDRAM memory
      ret = (*(__IO UINT32*) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos))) & 0x00FFFFFF);
   }
   else if ((hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565) || (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_ARGB4444) || (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_AL88))
   {
      //--- Read data value from SDRAM memory
      ret = *(__IO UINT16*) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));
   }
   else
   {
      //--- Read data value from SDRAM memory
      ret = *(__IO UINT8*) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos)));
   }

   return ret;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_Clear
//
// DESCRIPTION : Clears the hole LCD.
//-----------------------------------------------------------------------------
void LCD_Clear (UINT32 Color)
{
   //--- Clear the LCD
   _FillBuffer(ActiveLayer, (UINT32 *)(hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress), LCD_GetXSize(), LCD_GetYSize(), 0, Color);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_ClearStringLine
//
// DESCRIPTION : Clears the selected line.
//-----------------------------------------------------------------------------
void LCD_ClearStringLine (UINT32 Line)
{
   UINT32 color_backup = DrawProp[ActiveLayer].TextColor;

   DrawProp[ActiveLayer].TextColor = DrawProp[ActiveLayer].BackColor;

   //--- Draw rectangle with background color
   LCD_FillRect(0, (Line * DrawProp[ActiveLayer].pFont->Height), LCD_GetXSize(), DrawProp[ActiveLayer].pFont->Height);

   DrawProp[ActiveLayer].TextColor = color_backup;
   LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DisplayChar
//
// DESCRIPTION : Displays one character.
//-----------------------------------------------------------------------------
void LCD_DisplayChar (UINT16 Xpos, UINT16 Ypos, UINT8 Ascii)
{
   _DrawChar(Xpos, Ypos, &DrawProp[ActiveLayer].pFont->table[(Ascii-' ') * DrawProp[ActiveLayer].pFont->Height * ((DrawProp[ActiveLayer].pFont->Width + 7) / 8)]);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DisplayStringAt
//
// DESCRIPTION : Displays characters on the LCD.
//-----------------------------------------------------------------------------
void LCD_DisplayStringAt (UINT16 Xpos, UINT16 Ypos, UINT8 *Text, Text_AlignModeTypdef Mode)
{
   UINT16 ref_column = 1;
   UINT16 i = 0;
   UINT32 size = 0;
   UINT32 xsize = 0;
   UINT8  *ptr = Text;

   //--- Get the text size
   while (*ptr++) size ++ ;

   //--- Characters number per line
   xsize = (LCD_GetXSize()/DrawProp[ActiveLayer].pFont->Width);

   switch (Mode)
   {
      case CENTER_MODE:
      {
         ref_column = Xpos + ((xsize - size)* DrawProp[ActiveLayer].pFont->Width) / 2;
         break;
      }

      case LEFT_MODE:
      {
         ref_column = Xpos;
         break;
      }

      case RIGHT_MODE:
      {
         ref_column = - Xpos + ((xsize - size)*DrawProp[ActiveLayer].pFont->Width);
         break;
      }

      default:
      {
         ref_column = Xpos;
         break;
      }
   }

   //--- Check that the Start column is located in the screen
   if ((ref_column < 1) || (ref_column >= 0x8000))
   {
      ref_column = 1;
   }

   //--- Send the string character by character on LCD
   while ((*Text != 0) & (((LCD_GetXSize() - (i*DrawProp[ActiveLayer].pFont->Width)) & 0xFFFF) >= DrawProp[ActiveLayer].pFont->Width))
   {
      //--- Display one character on LCD
      LCD_DisplayChar(ref_column, Ypos, *Text);

      //--- Decrement the column position by 16 */
      ref_column += DrawProp[ActiveLayer].pFont->Width;

      //--- Point on the next character
      Text++;
      i++;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DisplayStringAtLine
//
// DESCRIPTION : Displays a maximum of 60 characters on the LCD.
//-----------------------------------------------------------------------------
void LCD_DisplayStringAtLine (UINT16 Line, UINT8 *ptr)
{
   LCD_DisplayStringAt(0, LINE(Line), ptr, LEFT_MODE);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawHLine
//
// DESCRIPTION : Draws an horizontal line.
//-----------------------------------------------------------------------------
void LCD_DrawHLine (UINT16 Xpos, UINT16 Ypos, UINT16 Length)
{
   UINT32  Xaddress = 0;

   //--- Get the line address
   if(hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
   {
      //--- RGB565 format
      Xaddress = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);
   }
   else
   {
      //--- ARGB8888 format
      Xaddress = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
   }

   //--- Write line
   _FillBuffer(ActiveLayer, (UINT32 *)Xaddress, Length, 1, 0, DrawProp[ActiveLayer].TextColor);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawVLine
//
// DESCRIPTION : Draws a vertical line.
//-----------------------------------------------------------------------------
void LCD_DrawVLine (UINT16 Xpos, UINT16 Ypos, UINT16 Length)
{
   UINT32  Xaddress = 0;

   //--- Get the line address
   if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
   {
      //--- RGB565 format
      Xaddress = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);
   }
   else
   {
      //--- ARGB8888 format
      Xaddress = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
   }

   //--- Write line
   _FillBuffer(ActiveLayer, (UINT32 *)Xaddress, 1, Length, (LCD_GetXSize() - 1), DrawProp[ActiveLayer].TextColor);
}

/**
  * @brief  Draws an uni-line (between two points).
  * @param  x1: Point 1 X position
  * @param  y1: Point 1 Y position
  * @param  x2: Point 2 X position
  * @param  y2: Point 2 Y position
  * @retval None
  */
//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawLine
//
// DESCRIPTION : Draws an uni-line (between two points).
//-----------------------------------------------------------------------------
void LCD_DrawLine (UINT16 x1, UINT16 y1, UINT16 x2, UINT16 y2)
{
   INT16 deltax       = 0;
   INT16 deltay       = 0;
   INT16 x            = 0;
   INT16 y            = 0;
   INT16 xinc1        = 0;
   INT16 xinc2        = 0;
   INT16 yinc1        = 0;
   INT16 yinc2        = 0;
   INT16 den          = 0;
   INT16 num          = 0;
   INT16 num_add      = 0;
   INT16 num_pixels   = 0;
   INT16 curpixel     = 0;

   //--- The difference between the x's
   deltax = ABS(x2 - x1);

   //--- The difference between the y's
   deltay = ABS(y2 - y1);

   //--- Start x off at the first pixel
   x = x1;

   //--- Start y off at the first pixel
   y = y1;

   //--- The x-values are increasing
   if (x2 >= x1)
   {
      xinc1 = 1;
      xinc2 = 1;
   }
   else //--- The x-values are decreasing
   {
      xinc1 = -1;
      xinc2 = -1;
   }

   //--- The y-values are increasing
   if (y2 >= y1)
   {
      yinc1 = 1;
      yinc2 = 1;
   }
   else //--- The y-values are decreasing
   {
      yinc1 = -1;
      yinc2 = -1;
   }

   //--- There is at least one x-value for every y-value
   if (deltax >= deltay)
   {
      xinc1 = 0;

      //--- Don't change the y for every iteration
      yinc2 = 0;
      den = deltax;
      num = deltax / 2;
      num_add = deltay;

      //--- There are more x-values than y-values
      num_pixels = deltax;
   }
   else //--- There is at least one y-value for every x-value
   {
      //--- Don't change the x for every iteration
      xinc2 = 0;

      //--- Don't change the y when numerator >= denominator
      yinc1 = 0;
      den = deltay;
      num = deltay / 2;
      num_add = deltax;

      //--- There are more y-values than x-values
      num_pixels = deltay;
   }

   for (curpixel = 0; curpixel <= num_pixels; curpixel++)
   {
      //--- Draw the current pixel
      LCD_DrawPixel(x, y, DrawProp[ActiveLayer].TextColor);

      //--- Increase the numerator by the top of the fraction
      num += num_add;

      //--- Check if numerator >= denominator
      if (num >= den)
      {
         //--- Calculate the new numerator value
         num -= den;

         //--- Change the x as appropriate
         x += xinc1;

         //--- Change the y as appropriate
         y += yinc1;
      }

      //--- Change the x as appropriate
      x += xinc2;

      //--- Change the y as appropriate
      y += yinc2;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawRect
//
// DESCRIPTION : Draws a rectangle.
//-----------------------------------------------------------------------------
void LCD_DrawRect (UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height)
{
   //--- Draw horizontal lines
   LCD_DrawHLine(Xpos, Ypos, Width);
   LCD_DrawHLine(Xpos, (Ypos+ Height), Width);

   //--- Draw vertical lines
   LCD_DrawVLine(Xpos, Ypos, Height);
   LCD_DrawVLine((Xpos + Width), Ypos, Height + 1);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawCircle
//
// DESCRIPTION : Draws a circle.
//-----------------------------------------------------------------------------
void LCD_DrawCircle (UINT16 Xpos, UINT16 Ypos, UINT16 Radius)
{
   INT32   decision;
   UINT32  current_x;
   UINT32  current_y;

   decision = 3 - (Radius << 1);
   current_x = 0;
   current_y = Radius;

   while (current_x <= current_y)
   {
      LCD_DrawPixel((Xpos + current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos - current_x), (Ypos - current_y), DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos + current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos - current_y), (Ypos - current_x), DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos + current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos - current_x), (Ypos + current_y), DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos + current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);

      LCD_DrawPixel((Xpos - current_y), (Ypos + current_x), DrawProp[ActiveLayer].TextColor);

      if (decision < 0)
      {
         decision += (current_x << 2) + 6;
      }
      else
      {
         decision += ((current_x - current_y) << 2) + 10;
         current_y--;
      }

      current_x++;
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawFilledCircle
//
// DESCRIPTION : Draws a circle.
//-----------------------------------------------------------------------------
void LCD_DrawFilledCircle (UINT16 x0, UINT16 y0, UINT16 radius)
{
   INT32 x;
   INT32 y;
   INT32 d;
   INT32 deltaE;
   INT32 deltaNE;

   x = 0;
   y = radius;
   d = 1 - radius;
   deltaE = 3;
   deltaNE = -2 * radius + 5;

   LCD_DrawHLine(x0 - x, y0 - y, x + x); /* Top */
   LCD_DrawHLine(x0 - y, y0 - x, y + y); /* Upper middle */

   LCD_DrawHLine(x0 - y, y0 + x, y + y); /* Lower middle */
   LCD_DrawHLine(x0 - x, y0 + y, x + x); /* Bottom */

   while (y > x)
   {
       if (d < 0)
       {
           /* East */
           d += deltaE;
           deltaE += 2;
           deltaNE += 2;
       }
       else
       {
           /* Northeast */
           d += deltaNE;
           deltaE += 2;
           deltaNE += 4;
           --y;
       }
       ++x;

         LCD_DrawHLine(x0 - x, y0 - y, x + x); /* Top */
         LCD_DrawHLine(x0 - y, y0 - x, y + y); /* Upper middle */

         LCD_DrawHLine(x0 - y, y0 + x, y + y); /* Lower middle */
         LCD_DrawHLine(x0 - x, y0 + y, x + x); /* Bottom */
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawProgressCircle
//
// DESCRIPTION : Draws a progress circle
//-----------------------------------------------------------------------------
void LCD_DrawProgressCircle (UINT16 posX, UINT16 posY, FLOAT32 width, UINT8 fromProgress, UINT8 toProgress, UINT8 radius, UINT32 color)
{
   FLOAT32 step = 2*M_PI/100.0;
   FLOAT32 angle;
   FLOAT32 from;
   FLOAT32 to;
   FLOAT32 x0, y0, x1, y1;

   if (fromProgress <= toProgress)
   {
      from = fromProgress * step - (M_PI/2);
      to = toProgress * step - (M_PI/2);
   }
   else
   {
      from = toProgress * step -(M_PI/2);
      to = fromProgress * step - (M_PI/2);
   }

   for (angle = from; angle < to; angle += 0.01)
   {
      x0 = radius * cos(angle);
      y0 = radius * sin(angle);

      x1 = (FLOAT32)(((FLOAT32)radius * width) * cos(angle));
      y1 = (FLOAT32)(((FLOAT32)radius * width) * sin(angle));

      LCD_SetTextColor(color);
      LCD_DrawLine(posX + x0, posY + y0, posX + x1, posY + y1);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawPolygon
//
// DESCRIPTION : Draws an poly-line (between many points).
//-----------------------------------------------------------------------------
void LCD_DrawPolygon (pPoint Points, UINT16 PointCount)
{
   INT16 x = 0;
   INT16 y = 0;

   if (PointCount < 2)
   {
      return;
   }

   LCD_DrawLine(Points->X, Points->Y, (Points+PointCount-1)->X, (Points+PointCount-1)->Y);

   while(--PointCount)
   {
      x = Points->X;
      y = Points->Y;
      Points++;
      LCD_DrawLine(x, y, Points->X, Points->Y);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawEllipse
//
// DESCRIPTION : Draws an ellipse on LCD.
//-----------------------------------------------------------------------------
void LCD_DrawEllipse (INT32 Xpos, INT32 Ypos, INT32 XRadius, INT32 YRadius)
{
   INT32 x        = 0;
   INT32 y        = -YRadius;
   INT32 err      = 2-2*XRadius;
   INT32 e2;
   FLOAT32 k      = 0;
   FLOAT32 rad1   = 0;
   FLOAT32 rad2   = 0;

   rad1 = XRadius;
   rad2 = YRadius;

   k = (float)(rad2/rad1);

   do
   {
      LCD_DrawPixel((Xpos-(UINT16)(x/k)), (Ypos+y), DrawProp[ActiveLayer].TextColor);
      LCD_DrawPixel((Xpos+(UINT16)(x/k)), (Ypos+y), DrawProp[ActiveLayer].TextColor);
      LCD_DrawPixel((Xpos+(UINT16)(x/k)), (Ypos-y), DrawProp[ActiveLayer].TextColor);
      LCD_DrawPixel((Xpos-(UINT16)(x/k)), (Ypos-y), DrawProp[ActiveLayer].TextColor);

      e2 = err;
      if (e2 <= x)
      {
         err += ++x*2+1;
         if (-y == x && e2 <= y)
            e2 = 0;
      }

      if (e2 > y)
         err += ++y*2+1;
   } while (y <= 0);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawPixel
//
// DESCRIPTION : Draws a pixel on LCD.
//-----------------------------------------------------------------------------
void LCD_DrawPixel (UINT16 Xpos, UINT16 Ypos, UINT32 RGB_Code)
{
   //--- Write data value to all SDRAM memory
   if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
   {
      //--- RGB565 format
      *(__IO UINT16*) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (2*(Ypos*LCD_GetXSize() + Xpos))) = (UINT16)RGB_Code;
   }
   else
   {
      //--- ARGB8888 format
      *(__IO UINT32*) (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (4*(Ypos*LCD_GetXSize() + Xpos))) = RGB_Code;
   }
}

/**
  * @brief  Draws a bitmap picture loaded in the internal Flash in ARGB888 format (32 bits per pixel).
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pbmp: Pointer to Bmp picture address in the internal Flash
  * @retval None
  */
//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawBitmap
//
// DESCRIPTION : Draws a bitmap picture loaded in the internal Flash in ARGB888 format (32 bits per pixel).
//-----------------------------------------------------------------------------
void LCD_DrawBitmap (UINT32 Xpos, UINT32 Ypos, UINT8 *pbmp)
{
   UINT32 index = 0;
   UINT32 width = 0;
   UINT32 height = 0;
   UINT32 bit_pixel = 0;
   UINT32 address;
   UINT32 input_color_mode = 0;

   //--- Get bitmap data address offset
   index = pbmp[10] + (pbmp[11] << 8) + (pbmp[12] << 16)  + (pbmp[13] << 24);

   //--- Read bitmap width
   width = pbmp[18] + (pbmp[19] << 8) + (pbmp[20] << 16)  + (pbmp[21] << 24);

   //--- Read bitmap height
   height = pbmp[22] + (pbmp[23] << 8) + (pbmp[24] << 16)  + (pbmp[25] << 24);

   //--- Read bit/pixel
   bit_pixel = pbmp[28] + (pbmp[29] << 8);

   //--- Set the address
   address = hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress + (((LCD_GetXSize()*Ypos) + Xpos)*(4));

   //--- Get the layer pixel format
   if ((bit_pixel / 8) == 4)
   {
      input_color_mode = CM_ARGB8888;
   }
   else if ((bit_pixel / 8) == 2)
   {
      input_color_mode = CM_RGB565;
   }
   else
   {
      input_color_mode = CM_RGB888;
   }

   //--- Bypass the bitmap header
   pbmp += (index + (width * (height - 1) * (bit_pixel / 8)));

   //--- Convert picture to ARGB8888 pixel format
   for (index = 0; index < height; index++)
   {
      //--- Pixel format conversion
      _ConvertLineToARGB8888((UINT32 *)pbmp, (UINT32 *)address, width, input_color_mode);

      //--- Increment the source and destination buffers
      address  += (LCD_GetXSize()*4);
      pbmp     -= width*(bit_pixel/8);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_FillRect
//
// DESCRIPTION : Draws a full rectangle.
//-----------------------------------------------------------------------------
void LCD_FillRect (UINT16 Xpos, UINT16 Ypos, UINT16 Width, UINT16 Height)
{
   UINT16 y;

   /*
   UINT32 x_address = 0;

   //--- Set the text color
   LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);

   //--- Get the rectangle start address
   if (hLtdcHandler.LayerCfg[ActiveLayer].PixelFormat == LTDC_PIXEL_FORMAT_RGB565)
   {
      //--- RGB565 format
      x_address = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 2*(LCD_GetXSize()*Ypos + Xpos);
   }
   else
   {
      //--- ARGB8888 format
      x_address = (hLtdcHandler.LayerCfg[ActiveLayer].FBStartAdress) + 4*(LCD_GetXSize()*Ypos + Xpos);
   }

   //--- Fill the rectangle
   _FillBuffer(ActiveLayer, (UINT32 *)x_address, Width, Height, (LCD_GetXSize() - Width), DrawProp[ActiveLayer].TextColor);
   */

   for (y = 0; y < Height; y++)
      LCD_DrawHLine(Xpos, y + Ypos, Width);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_FillCircle
//
// DESCRIPTION : Draws a full circle.
//-----------------------------------------------------------------------------
void LCD_FillCircle (UINT16 Xpos, UINT16 Ypos, UINT16 Radius)
{
#if 0
   INT32  decision;
   UINT32 current_x;
   UINT32 current_y;

   decision = 3 - (Radius << 1);

   current_x = 0;
   current_y = Radius;

   LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);

   while (current_x <= current_y)
   {
      if (current_y > 0)
      {
         LCD_DrawHLine(Xpos - current_y, Ypos + current_x, 2*current_y);
         LCD_DrawHLine(Xpos - current_y, Ypos - current_x, 2*current_y);
      }

      if (current_x > 0)
      {
         LCD_DrawHLine(Xpos - current_x, Ypos - current_y, 2*current_x);
         LCD_DrawHLine(Xpos - current_x, Ypos + current_y, 2*current_x);
      }

      if (decision < 0)
      {

      }
      else
      {
         decision += ((current_x - current_y) << 2) + 10;
         current_y--;
      }

      current_x++;
   }

   LCD_SetTextColor(DrawProp[ActiveLayer].TextColor);
   LCD_DrawCircle(Xpos, Ypos, Radius);
#endif
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_FillPolygon
//
// DESCRIPTION : Draws a full poly-line (between many points).
//-----------------------------------------------------------------------------
void LCD_FillPolygon (pPoint Points, UINT16 PointCount)
{
   INT16 X               = 0;
   INT16 Y               = 0;
   INT16 X2              = 0;
   INT16 Y2              = 0;
   INT16 X_center        = 0;
   INT16 Y_center        = 0;
   INT16 X_first         = 0;
   INT16 Y_first         = 0;
   INT16 pixelX          = 0;
   INT16 pixelY          = 0;
   INT16 counter         = 0;
   UINT16 image_left     = 0;
   UINT16 image_right    = 0;
   UINT16 image_top      = 0;
   UINT16 image_bottom   = 0;

   image_left   = image_right  = Points->X;
   image_top    = image_bottom = Points->Y;

   for (counter = 1; counter < PointCount; counter++)
   {
      pixelX = POLY_X(counter);

      if (pixelX < image_left)
      {
         image_left = pixelX;
      }

      if (pixelX > image_right)
      {
         image_right = pixelX;
      }

      pixelY = POLY_Y(counter);

      if (pixelY < image_top)
      {
         image_top = pixelY;
      }

      if (pixelY > image_bottom)
      {
         image_bottom = pixelY;
      }
   }

   if (PointCount < 2)
   {
      return;
   }

   X_center = (image_left + image_right) / 2;
   Y_center = (image_bottom + image_top) / 2;

   X_first = Points->X;
   Y_first = Points->Y;

   while(--PointCount)
   {
      X = Points->X;
      Y = Points->Y;

      Points++;

      X2 = Points->X;
      Y2 = Points->Y;

      _FillTriangle(X, X2, X_center, Y, Y2, Y_center);
      _FillTriangle(X, X_center, X2, Y, Y_center, Y2);
      _FillTriangle(X_center, X2, X, Y_center, Y2, Y);
   }

   _FillTriangle(X_first, X2, X_center, Y_first, Y2, Y_center);
   _FillTriangle(X_first, X_center, X2, Y_first, Y_center, Y2);
   _FillTriangle(X_center, X2, X_first, Y_center, Y2, Y_first);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_FillEllipse
//
// DESCRIPTION : Draws a full ellipse.
//-----------------------------------------------------------------------------
void LCD_FillEllipse (INT32 Xpos, INT32 Ypos, INT32 XRadius, INT32 YRadius)
{
   INT32 x        = 0;
   INT32 y        = -YRadius;
   INT32 err      = 2 - 2 * XRadius;
   INT32 e2;
   FLOAT32 k      = 0;
   FLOAT32 rad1   = 0;
   FLOAT32 rad2   = 0;

   rad1 = XRadius;
   rad2 = YRadius;

   k = (FLOAT32)(rad2 / rad1);

   do
   {
      LCD_DrawHLine((Xpos - (UINT16)(x / k)), (Ypos + y), (2 * (UINT16)(x / k) + 1));
      LCD_DrawHLine((Xpos - (UINT16)(x / k)), (Ypos - y), (2 * (UINT16)(x / k) + 1));

      e2 = err;
      if (e2 <= x)
      {
         err += ++x * 2 + 1;

         if (-y == x && e2 <= y)
            e2 = 0;
      }

      if (e2 > y)
         err += ++y * 2 + 1;
   } while (y <= 0);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DisplayOn
//
// DESCRIPTION : Enables the display.
//-----------------------------------------------------------------------------
void LCD_DisplayOn (void)
{
   //--- Display On
   __HAL_LTDC_ENABLE(&hLtdcHandler);

   //--- Assert LCD_DISP pin
   HAL_GPIO_WritePin(LCD_DISP_GPIO_PORT, LCD_DISP_PIN, GPIO_PIN_SET);

   //--- Assert LCD_BL_CTRL pin
   HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_SET);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DisplayOff
//
// DESCRIPTION : Disables the display.
//-----------------------------------------------------------------------------
void LCD_DisplayOff (void)
{
   //--- Display Off
   __HAL_LTDC_DISABLE(&hLtdcHandler);

   //--- De-assert LCD_DISP pin
   HAL_GPIO_WritePin(LCD_DISP_GPIO_PORT, LCD_DISP_PIN, GPIO_PIN_RESET);

   //--- De-assert LCD_BL_CTRL pin
   HAL_GPIO_WritePin(LCD_BL_CTRL_GPIO_PORT, LCD_BL_CTRL_PIN, GPIO_PIN_RESET);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_MspInit
//
// DESCRIPTION : Initializes the LTDC MSP.
//-----------------------------------------------------------------------------
__weak void LCD_MspInit (LTDC_HandleTypeDef *hltdc, void *Params)
{
   GPIO_InitTypeDef gpio_init_structure;

   //--- Enable the LTDC and DMA2D clocks
   __HAL_RCC_LTDC_CLK_ENABLE();
   __HAL_RCC_DMA2D_CLK_ENABLE();

   //--- Enable GPIOs clock
   __HAL_RCC_GPIOE_CLK_ENABLE();
   __HAL_RCC_GPIOG_CLK_ENABLE();
   __HAL_RCC_GPIOI_CLK_ENABLE();
   __HAL_RCC_GPIOJ_CLK_ENABLE();
   __HAL_RCC_GPIOK_CLK_ENABLE();

   LCD_DISP_GPIO_CLK_ENABLE();
   LCD_BL_CTRL_GPIO_CLK_ENABLE();

   //--- LTDC Pins configuration
   //---- GPIOE configuration
   gpio_init_structure.Pin       = GPIO_PIN_4;
   gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
   gpio_init_structure.Pull      = GPIO_NOPULL;
   gpio_init_structure.Speed     = GPIO_SPEED_FAST;
   gpio_init_structure.Alternate = GPIO_AF14_LTDC;
   HAL_GPIO_Init(GPIOE, &gpio_init_structure);

   //---- GPIOG configuration
   gpio_init_structure.Pin       = GPIO_PIN_12;
   gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
   gpio_init_structure.Alternate = GPIO_AF9_LTDC;
   HAL_GPIO_Init(GPIOG, &gpio_init_structure);

   //---- GPIOI LTDC alternate configuration
   gpio_init_structure.Pin       = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
   gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
   gpio_init_structure.Alternate = GPIO_AF14_LTDC;
   HAL_GPIO_Init(GPIOI, &gpio_init_structure);

   //--- GPIOJ configuration
   gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                   GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                   GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                   GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
   gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
   gpio_init_structure.Alternate = GPIO_AF14_LTDC;
   HAL_GPIO_Init(GPIOJ, &gpio_init_structure);

   //--- GPIOK configuration
   gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | \
   GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
   gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
   gpio_init_structure.Alternate = GPIO_AF14_LTDC;
   HAL_GPIO_Init(GPIOK, &gpio_init_structure);

   //--- LCD_DISP GPIO configuration
   gpio_init_structure.Pin       = LCD_DISP_PIN;
   gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
   HAL_GPIO_Init(LCD_DISP_GPIO_PORT, &gpio_init_structure);

   //--- LCD_BL_CTRL GPIO configuration
   gpio_init_structure.Pin       = LCD_BL_CTRL_PIN;
   gpio_init_structure.Mode      = GPIO_MODE_OUTPUT_PP;
   HAL_GPIO_Init(LCD_BL_CTRL_GPIO_PORT, &gpio_init_structure);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_MspDeInit
//
// DESCRIPTION : DeInitializes LCD MSP.
//-----------------------------------------------------------------------------
__weak void LCD_MspDeInit (LTDC_HandleTypeDef *hltdc, void *Params)
{
   GPIO_InitTypeDef  gpio_init_structure;

   //--- Disable LTDC block
   __HAL_LTDC_DISABLE(hltdc);

   //--- LTDC Pins deactivation
   //---- GPIOE deactivation
   gpio_init_structure.Pin       = GPIO_PIN_4;
   HAL_GPIO_DeInit(GPIOE, gpio_init_structure.Pin);

   //--- GPIOG deactivation
   gpio_init_structure.Pin       = GPIO_PIN_12;
   HAL_GPIO_DeInit(GPIOG, gpio_init_structure.Pin);

   //--- GPIOI deactivation
   gpio_init_structure.Pin       = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_12 | \
                                   GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
   HAL_GPIO_DeInit(GPIOI, gpio_init_structure.Pin);

   //--- GPIOJ deactivation
   gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                   GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                   GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                   GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
   HAL_GPIO_DeInit(GPIOJ, gpio_init_structure.Pin);

   //--- GPIOK deactivation
   gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | \
   GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
   HAL_GPIO_DeInit(GPIOK, gpio_init_structure.Pin);

   //--- Disable LTDC clock
   __HAL_RCC_LTDC_CLK_DISABLE();
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_ClockConfig
//
// DESCRIPTION : Clock Config.
//-----------------------------------------------------------------------------
__weak void LCD_ClockConfig (LTDC_HandleTypeDef *hltdc, void *Params)
{
   static RCC_PeriphCLKInitTypeDef periph_clk_init_struct;

   //--- RK043FN48H LCD clock configuration
   //--- PLLSAI_VCO Input = HSE_VALUE/PLL_M = 1 Mhz
   //--- PLLSAI_VCO Output = PLLSAI_VCO Input * PLLSAIN = 192 Mhz
   //--- PLLLCDCLK = PLLSAI_VCO Output/PLLSAIR = 192/5 = 38.4 Mhz
   //--- LTDC clock frequency = PLLLCDCLK / LTDC_PLLSAI_DIVR_4 = 38.4/4 = 9.6Mhz

   periph_clk_init_struct.PeriphClockSelection   = RCC_PERIPHCLK_LTDC;
   periph_clk_init_struct.PLLSAI.PLLSAIN         = 192;
   periph_clk_init_struct.PLLSAI.PLLSAIR         = RK043FN48H_FREQUENCY_DIVIDER;
   periph_clk_init_struct.PLLSAIDivR             = RCC_PLLSAIDIVR_4;
   HAL_RCCEx_PeriphCLKConfig(&periph_clk_init_struct);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawRectWithThickness
//
// DESCRIPTION : Draw rectangle with thickness
//-----------------------------------------------------------------------------
void LCD_DrawRectWithThickness (UINT16 xPos, UINT16 yPos, UINT16 width, UINT16 height, UINT8 thickness)
{
   UINT8 i;

   for (i = 0; i < thickness; i++)
      LCD_DrawRect(xPos + i, yPos + i, width - i * 2, height - i * 2);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawRectButton
//
// DESCRIPTION : Draw rectangle button
//-----------------------------------------------------------------------------
void LCD_DrawRectButton (UINT16 xPos, UINT16 yPos, UINT16 width, UINT16 height, UINT32 color, CHAR8 *txt, BOOL selected)
{
   UINT8 strSize = 0;
   UINT8 thickness = 10;

   LCD_SetTextColor(color);

   if (selected == TRUE)
   {
      yPos -= 15;
      thickness = 25;
   }

   LCD_DrawHLineThickness(xPos, yPos + height - 9, width, thickness);

   LCD_SetFont(&LCD_FONT_16);

   LCD_SetTextColor(LCD_COLOR_LIGHTGRAY);

   if (selected == TRUE)
      yPos += 5;

   strSize = STRING_LEN(txt);
   LCD_SetBackColor(LCD_COLOR_BACKGROUND_ITEM);
   LCD_DisplayStringAt(xPos + ((width / 2) - ((LCD_FONT_16.Width * strSize) / 2)), yPos + (LCD_FONT_16.Height / 2) + 10, txt, LEFT_MODE);
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawHLineThickness
//
// DESCRIPTION : Draw horizontal line with thickness
//-----------------------------------------------------------------------------
void LCD_DrawHLineThickness (UINT16 Xpos, UINT16 Ypos, UINT16 Length, UINT8 Thickness)
{
   UINT8 i;

   for (i = 0; i < Thickness; i++)
   {
      LCD_DrawHLine(Xpos, Ypos + i, Length);
   }
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_GetStringWidth
//
// DESCRIPTION : Draw horizontal line with thickness
//-----------------------------------------------------------------------------
UINT16 LCD_GetStringWidth (STRING str)
{
   UINT8 arraySize;
   sFONT* font;

   arraySize = STRING_LEN(str);
   font = LCD_GetFont();

   return font->Width * arraySize;
}

//-----------------------------------------------------------------------------
// FONCTION    : LCD_DrawSmallTriangle
//
// DESCRIPTION : Draw small triangle
//-----------------------------------------------------------------------------
void LCD_DrawSmallTriangle (UINT16 xPos, UINT16 yPos, UINT8 dir, UINT16 size, UINT32 color)
{
   UINT16   xInc;
   UINT16   yInc;
   UINT16   i, j;
   UINT16   x, y;

   if ((dir == LEFT_DIR) || (dir == RIGHT_DIR))
   {
      xInc = dir - 1;
      yInc = 0;
   }
   else
   {
      xInc = 0;
      yInc = 2 - dir;
   }

   if ((size % 2) == 0)
   {
      for(i = 0; i < size; i++)
      {
         for(j = 0; j < (2 * i + 2); j++)
         {
            x = (xPos - xInc * i + yInc * (j - i));
            y = (yPos + xInc * (j - i) - yInc * i);
            LCD_DrawPixel(x, y, color);
         }
      }
   }
   else
   {
      for(i = 0; i < size; i++)
      {
         for(j = 0; j < (2 * i + 1); j++)
         {
            x = (xPos - xInc * i + yInc * (j - i));
            y = (yPos + xInc * (j - i) - yInc * i);
            LCD_DrawPixel(x, y, color);
         }
      }
   }
}

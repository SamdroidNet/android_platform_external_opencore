/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
/*
------------------------------------------------------------------------------
 INPUT AND OUTPUT DEFINITIONS

 Inputs:
	xpos = x half-pixel of (x,y) coordinates within a VOP; motion
	       compensated coordinates; native data type
	ypos = y half-pixel of (x,y) coordinates within a VOP; motion
	       compensated coordinates; native data type
	comp = pointer to 8-bit compensated prediction values within a VOP;
	       computed by this module (i/o); full-pel resolution; 8-bit data
	c_prev = pointer to previous 8-bit prediction values within a VOP;
		 values range from (0-255); full-pel resolution; 8-bit data
	sh_d = pointer to residual values used to compensate the predicted
	       value; values range from (-512 to 511); full-pel resolution;
	       native data type
	width = width of the VOP in pixels (x axis); full-pel resolution;
		native data type
	height = height of the VOP in pixels (y axis); full-pel resolution;
		 native data type
	rnd1 = rounding value for case when one dimension uses half-pel
	       resolution; native data type
	rnd2 = rounding value for case when two dimensions uses half-pel
	       resolution; native data type

 Outputs:
	returns 1

 Local Stores/Buffers/Pointers Needed:
	None

 Global Stores/Buffers/Pointers Needed:
	None

 Pointers and Buffers Modified:
	comp = buffer contains newly computed compensated prediction values

 Local Stores Modified:
	None

 Global Stores Modified:
	None

------------------------------------------------------------------------------
 FUNCTION DESCRIPTION

 Summary:

 This function performs motion compensated prediction for the case where
 the motion vector points to a block outside the VOP. The function interpolates
 the pixels that are outside the VOP using the boundary pixels for the block.
 Once the values are interpolated, the pixel values are computed for a block
 in the current VOP. The prediction values are generated by averaging pixel
 values in the previous VOP; the block position in the previous frame is
 computed from the current block's motion vector. The computed pixel values
 are calculated by adding the prediction values to the block residual values.

 Details:

 First, this functions determines which VOP boundary(ies) the motion vector
 is outside, i.e., left, right, top, bottom. xpos is compared to the left and
 right boundaries; ypos is compared to the top and bottom boundaries. The number
 of block pixels inside the the boundary in the x and y directions are stored
 in endx and endy, respectively. If the entire block is inside the x or y
 boundary, the respectively end is set to 0.

 After the boundaries are tested, any pixels lying outside a boundary are
 interpolated from the boundary pixels. For example, if the block is outside the
 bottom boundary, boundary pixels alone the bottom of the VOP as used to
 interpolated those pixels lying outside the bottom boundary. The interpolation
 used is a simple column-wise or row-wise copy of the boundary pixels (inside the
 block) depending on which boundary the block is outside. In our example, each
 boundary pixel would be copied column-wise to the pixel beneath it. If the
 block was outside right boundary, the boundary pixels would be copied row-wise
 to the pixel to the right of it. If the block was outside both an x and y
 boundary, the boundary pixels would be copied row-wise for the portion of the
 block outside the x boundary, and column-wise for the portion of the block
 outside the y boundary. And so on.

 Once the pixel interpolation is complete, the motion compensated output values
 (comp[]) are calculed from the motion compensated prediction (pred[])values and
 the residual values (sh_d[]) of the current frame. The prediction values are
 generated by averaging pixel values in the previous VOP; the block position in
 the previous frame is computed from the current block's motion vector. The
 computed pixel values are calculated by adding the prediction values to the
 block residual values.

*/

/*----------------------------------------------------------------------------
; INCLUDES
----------------------------------------------------------------------------*/
#include "mp4dec_lib.h"
#include "motion_comp.h"

#define PAD_CORNER {	temp = *prev; \
			temp |= (temp<<8);  \
			temp |= (temp<<16); \
			*((uint32*)ptr) = temp; \
			*((uint32*)(ptr+4)) = temp;  \
			*((uint32*)(ptr+=16)) = temp;  \
			*((uint32*)(ptr+4)) = temp;  \
			*((uint32*)(ptr+=16)) = temp;  \
			*((uint32*)(ptr+4)) = temp;  \
			*((uint32*)(ptr+=16)) = temp;  \
			*((uint32*)(ptr+4)) = temp;  \
			*((uint32*)(ptr+=16)) = temp;  \
			*((uint32*)(ptr+4)) = temp;  \
			*((uint32*)(ptr+=16)) = temp;  \
			*((uint32*)(ptr+4)) = temp;  \
			*((uint32*)(ptr+=16)) = temp;  \
			*((uint32*)(ptr+4)) = temp;  \
			*((uint32*)(ptr+=16)) = temp;  \
			*((uint32*)(ptr+4)) = temp;  }

#define PAD_ROW  {  temp = *((uint32*)prev); \
					temp2 = *((uint32*)(prev+4)); \
			*((uint32*)ptr) =  temp;\
			*((uint32*)(ptr+4)) =  temp2; \
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp2;\
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp2;\
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp2;\
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp2;\
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp2;\
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp2;\
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp2;}

#define PAD_EXTRA_4x8			{	temp = *((uint32*)(prev+8)); \
				*((uint32*)ptr) =  temp; \
				*((uint32*)(ptr+=16)) = temp; \
				*((uint32*)(ptr+=16)) = temp; \
				*((uint32*)(ptr+=16)) = temp; \
				*((uint32*)(ptr+=16)) = temp; \
				*((uint32*)(ptr+=16)) = temp; \
				*((uint32*)(ptr+=16)) = temp; \
				*((uint32*)(ptr+=16)) = temp; }

#define PAD_COL	{ temp = *prev; \
			temp|=(temp<<8);  temp|=(temp<<16); \
			*((uint32*)ptr) = temp; \
			*((uint32*)(ptr+4)) = temp; \
			temp = *(prev+=16); \
			temp|=(temp<<8);  temp|=(temp<<16); \
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp; \
			temp = *(prev+=16); \
			temp|=(temp<<8);  temp|=(temp<<16); \
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp; \
			temp = *(prev+=16); \
			temp|=(temp<<8);  temp|=(temp<<16); \
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp; \
			temp = *(prev+=16); \
			temp|=(temp<<8);  temp|=(temp<<16); \
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp; \
			temp = *(prev+=16); \
			temp|=(temp<<8);  temp|=(temp<<16); \
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp; \
			temp = *(prev+=16); \
			temp|=(temp<<8);  temp|=(temp<<16); \
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp; \
			temp = *(prev+=16); \
			temp|=(temp<<8);  temp|=(temp<<16); \
			*((uint32*)(ptr+=16)) = temp; \
			*((uint32*)(ptr+4)) = temp;}

/* copy 8x8 block */
#define COPY_BLOCK  {			*((uint32*)ptr) = *((uint32*)prev); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4));  }

#define COPY_12x8		{		*((uint32*)ptr) = *((uint32*)prev); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+8)) = *((uint32*)(prev+8)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+8)) = *((uint32*)(prev+8)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+8)) = *((uint32*)(prev+8)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+8)) = *((uint32*)(prev+8)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+8)) = *((uint32*)(prev+8)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+8)) = *((uint32*)(prev+8)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+8)) = *((uint32*)(prev+8)); \
			*((uint32*)(ptr+=16)) = *((uint32*)(prev+=width)); \
			*((uint32*)(ptr+4)) = *((uint32*)(prev+4)); \
			*((uint32*)(ptr+8)) = *((uint32*)(prev+8)); }

/*----------------------------------------------------------------------------
; FUNCTION CODE
----------------------------------------------------------------------------*/
int GetPredOutside(
    int xpos,		/* i */
    int ypos,		/* i */
    uint8 *c_prev,		/* i */
    uint8 *pred_block,		/* i */
    int width,		/* i */
    int height,		/* i */
    int rnd1,		/* i */
    int pred_width
)
{
    /*----------------------------------------------------------------------------
    ; Define all local variables
    ----------------------------------------------------------------------------*/
    uint8	*prev;		/* pointers to adjacent pixels in the    */
    uint8	pred[256];	/* storage for padded pixel values, 16x16 */
    uint8   *ptr;
    int xoffset;
    uint32 temp, temp2;

    /*----------------------------------------------------------------------------
    ; Function body here
    ----------------------------------------------------------------------------*/
    /* saturate xpos and ypos */
    if (xpos < -16) xpos = -16;
    if (xpos > ((width - 1) << 1)) xpos = (width - 1) << 1;
    if (ypos < -16) ypos = -16;
    if (ypos > ((height - 1) << 1)) ypos = (height - 1) << 1;

    if (xpos < 0)
    {
        if (ypos < 0) /* pad top left of frame */
        {
            /* copy the block */
            ptr = pred + (8 << 4) + 8;
            prev = c_prev;
            COPY_BLOCK

            /* pad the corner */
            ptr = pred;
            prev = pred + (8 << 4) + 8;
            PAD_CORNER

            /* pad top */
            ptr = pred + 8;
            prev = pred + (8 << 4) + 8;
            PAD_ROW

            /* pad left */
            ptr = pred + (8 << 4);
            prev = pred + (8 << 4) + 8;
            PAD_COL


            ptr = pred + (((ypos >> 1) + 8) << 4) + (xpos >> 1) + 8;

            GetPredAdvBTable[ypos&1][xpos&1](ptr, pred_block, 16, (pred_width << 1) | rnd1);

            return 1;
        }
        else if ((ypos >> 1) < (height - B_SIZE)) /* pad left of frame */
        {
            /* copy block */
            ptr = pred + 8;
            prev = c_prev + (ypos >> 1) * width;
            COPY_BLOCK
            /* copy extra line */
            *((uint32*)(ptr += 16)) = *((uint32*)(prev += width));
            *((uint32*)(ptr + 4)) = *((uint32*)(prev + 4));

            /* pad left */
            ptr = pred;
            prev = pred + 8;
            PAD_COL
            /* pad extra line */
            temp = *(prev += 16);
            temp |= (temp << 8);
            temp |= (temp << 16);
            *((uint32*)(ptr += 16)) = temp;
            *((uint32*)(ptr + 4)) = temp;

            ptr = pred + 8 + (xpos >> 1);

            GetPredAdvBTable[ypos&1][xpos&1](ptr, pred_block, 16, (pred_width << 1) | rnd1);

            return 1;
        }
        else /* pad bottom left of frame */
        {
            /* copy the block */
            ptr = pred + 8; /* point to the center */
            prev = c_prev + width * (height - 8);
            COPY_BLOCK

            /* pad the corner */
            ptr = pred + (8 << 4);
            prev = ptr - 8;
            PAD_CORNER

            /* pad bottom */
            ptr = pred + (8 << 4) + 8;
            prev = ptr - 16;
            PAD_ROW

            /* pad left */
            ptr = pred ;
            prev = ptr + 8;
            PAD_COL

            ptr = pred + 8 + (((ypos >> 1) - (height - 8)) << 4) + (xpos >> 1);

            GetPredAdvBTable[ypos&1][xpos&1](ptr, pred_block, 16, (pred_width << 1) | rnd1);

            return 1;
        }
    }
    else if ((xpos >> 1) < (width - B_SIZE))
    {
        if (ypos < 0) /* pad top of frame */
        {
            xoffset = xpos >> 1;
            xoffset = xoffset & 0x3; /* word align ptr */

            /* copy block */
            ptr = pred + (8 << 4);
            prev = c_prev + (xpos >> 1) - xoffset;

            if (xoffset || (xpos&1)) /* copy extra 4x8 */
            {
                COPY_12x8
            }
            else
            {
                COPY_BLOCK
            }

            /* pad top */
            ptr = pred;
            prev = pred + (8 << 4);
            PAD_ROW
            if (xoffset || (xpos&1)) /* pad extra 4x8 */
            {
                ptr = pred + 8;
                PAD_EXTRA_4x8
            }

            ptr = pred + (((ypos >> 1) + 8) << 4) + xoffset;

            GetPredAdvBTable[ypos&1][xpos&1](ptr, pred_block, 16, (pred_width << 1) | rnd1);

            return 1;
        }
        else /* pad bottom of frame */
        {
            xoffset = xpos >> 1;
            xoffset = xoffset & 0x3; /* word align ptr */
            /* copy block */
            ptr = pred ;
            prev = c_prev + width * (height - 8) + (xpos >> 1) - xoffset;
            if (xoffset  || (xpos&1))
            {
                COPY_12x8
            }
            else
            {
                COPY_BLOCK
            }

            /* pad bottom */
            ptr = pred + (8 << 4);
            prev = ptr - 16;
            PAD_ROW
            if (xoffset || (xpos&1))
            {
                ptr = pred + (8 << 4) + 8;
                PAD_EXTRA_4x8
            }

            ptr = pred + (((ypos >> 1) - (height - 8)) << 4) + xoffset;

            GetPredAdvBTable[ypos&1][xpos&1](ptr, pred_block, 16, (pred_width << 1) | rnd1);

            return 1;
        }
    }
    else
    {
        if (ypos < 0) /* pad top right of frame */
        {
            /* copy block */
            ptr = pred + (8 << 4);
            prev = c_prev + width - 8;
            COPY_BLOCK

            /* pad top-right */
            ptr = pred + 8;
            prev = pred + (8 << 4) + 7;
            PAD_CORNER

            /* pad top */
            ptr = pred ;
            prev = pred + (8 << 4);
            PAD_ROW;

            /* pad right */
            ptr = pred + (8 << 4) + 8;
            prev = ptr - 1;
            PAD_COL;

            ptr = pred + ((8 + (ypos >> 1)) << 4) + (8 - (width - (xpos >> 1)));

            GetPredAdvBTable[ypos&1][xpos&1](ptr, pred_block, 16, (pred_width << 1) | rnd1);

            return 1;
        }
        else if ((ypos >> 1) < (height - B_SIZE)) /* pad right of frame */
        {
            /* copy block */
            ptr = pred;
            prev = c_prev + (ypos >> 1) * width + width - 8;
            COPY_BLOCK
            /* copy extra line */
            *((uint32*)(ptr += 16)) = *((uint32*)(prev += width));
            *((uint32*)(ptr + 4)) = *((uint32*)(prev + 4));

            /* pad right */
            ptr = pred + 8;
            prev = ptr - 1;
            PAD_COL;
            /* pad extra line */
            temp = *(prev += 16);
            temp |= (temp << 8);
            temp |= (temp << 16);
            *((uint32*)(ptr += 16)) = temp;
            *((uint32*)(ptr + 4)) = temp;


            ptr = pred + 8 - (width - (xpos >> 1));

            GetPredAdvBTable[ypos&1][xpos&1](ptr, pred_block, 16, (pred_width << 1) | rnd1);

            return 1;

        }
        else /* pad bottom right of frame */
        {
            /* copy block */
            ptr = pred;
            prev = c_prev + width * (height - 8) + width - 8;
            COPY_BLOCK

            /* pad bottom-right */
            ptr = pred + (8 << 4) + 8;
            prev = ptr - 17;
            PAD_CORNER

            /* pad right */
            ptr = pred + 8;
            prev = ptr - 1;
            PAD_COL

            /* pad bottom */
            ptr = pred + (8 << 4);
            prev = ptr - 16;
            PAD_ROW

            ptr = pred + 8 - (width - (xpos >> 1)) + ((8 - (height - (ypos >> 1))) << 4);

            GetPredAdvBTable[ypos&1][xpos&1](ptr, pred_block, 16, (pred_width << 1) | rnd1);

            return 1;
        }
    }
}

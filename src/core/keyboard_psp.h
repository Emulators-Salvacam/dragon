/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

# ifndef _KEYBOARD_PSP_H_
# define _KEYBOARD_PSP_H_

#ifdef __cplusplus
extern "C" {
#endif

 enum dragon_keys_emum {

   DRAGONK_0,          
   DRAGONK_1,          
   DRAGONK_2,          
   DRAGONK_3,          
   DRAGONK_4,          
   DRAGONK_5,          
   DRAGONK_6,          
   DRAGONK_7,          
   DRAGONK_8,          
   DRAGONK_9,          

   DRAGONK_A,          
   DRAGONK_B,          
   DRAGONK_C,          
   DRAGONK_D,          
   DRAGONK_E,          
   DRAGONK_F,          
   DRAGONK_G,          
   DRAGONK_H,          
   DRAGONK_I,          
   DRAGONK_J,          
   DRAGONK_K,          
   DRAGONK_L,          
   DRAGONK_M,          
   DRAGONK_N,          
   DRAGONK_O,          
   DRAGONK_P,          
   DRAGONK_Q,          
   DRAGONK_R,          
   DRAGONK_S,          
   DRAGONK_T,          
   DRAGONK_U,          
   DRAGONK_V,          
   DRAGONK_W,          
   DRAGONK_X,          
   DRAGONK_Y,          
   DRAGONK_Z,          

   DRAGONK_a,          
   DRAGONK_b,          
   DRAGONK_c,          
   DRAGONK_d,          
   DRAGONK_e,          
   DRAGONK_f,          
   DRAGONK_g,          
   DRAGONK_h,          
   DRAGONK_i,          
   DRAGONK_j,          
   DRAGONK_k,          
   DRAGONK_l,          
   DRAGONK_m,          
   DRAGONK_n,          
   DRAGONK_o,          
   DRAGONK_p,          
   DRAGONK_q,          
   DRAGONK_r,          
   DRAGONK_s,          
   DRAGONK_t,          
   DRAGONK_u,          
   DRAGONK_v,          
   DRAGONK_w,          
   DRAGONK_x,          
   DRAGONK_y,          
   DRAGONK_z,          

   DRAGONK_QUOTE,
   DRAGONK_COMMA,
   DRAGONK_LESS,       
   DRAGONK_PERIOD,     
   DRAGONK_GREATER,    
   DRAGONK_SEMICOLON,  
   DRAGONK_COLON,  
   DRAGONK_UNDERSCORE, 
   DRAGONK_PIPE,
   DRAGONK_EQUAL,
   DRAGONK_PLUS,
   DRAGONK_TILDA,
   DRAGONK_DBLQUOTE,
   DRAGONK_QUESTION,
   DRAGONK_SLASH,
   DRAGONK_MINUS,  
   DRAGONK_LBRACKET,
   DRAGONK_RBRACKET,
   DRAGONK_LCBRACE,
   DRAGONK_RCBRACE,
   DRAGONK_SPACE,
   DRAGONK_EXCLAMATN,
   DRAGONK_AT,
   DRAGONK_HASH,
   DRAGONK_DOLLAR,
   DRAGONK_PERCENT,
   DRAGONK_POWER,
   DRAGONK_AMPERSAND,
   DRAGONK_ASTERISK,
   DRAGONK_LPAREN,
   DRAGONK_RPAREN,
   DRAGONK_BACKSLASH,
   DRAGONK_BACKQUOTE,

   DRAGONK_TAB,        
   DRAGONK_BACKSPACE,        
   DRAGONK_LEFT,        
   DRAGONK_RIGHT,        
   DRAGONK_UP,        
   DRAGONK_DOWN,        
   DRAGONK_DELETE,        
   DRAGONK_RETURN,        
   DRAGONK_SHIFT,        
   
   DRAGONK_JOY_UP,     
   DRAGONK_JOY_DOWN,   
   DRAGONK_JOY_LEFT,   
   DRAGONK_JOY_RIGHT,  
   DRAGONK_JOY_FIRE,  
   CMD_FPS,
   CMD_JOY,
   CMD_RENDER,
   CMD_LOAD,
   CMD_SAVE,
   CMD_RESET,
   CMD_AUTOFIRE,
   CMD_INCFIRE,
   CMD_DECFIRE,
   CMD_SCREEN,

   DRAGONK_MAX_KEY      

  };

  extern void dragon_key_press(int key_id);
  extern void dragon_key_release(int key_id);

#ifdef __cplusplus
}
#endif
# endif

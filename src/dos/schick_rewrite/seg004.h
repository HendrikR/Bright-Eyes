extern void set_var_to_zero();
extern void clear_ani();
extern void restore_rect(PhysPt ptr1, Bit8u *ptr2, unsigned short x, unsigned short y, char n, char m);
extern void restore_rect_rle(PhysPt ptr1, Bit8u *ptr2, unsigned short x, unsigned short y, char n ,char m, unsigned short v1);
extern void draw_mouse_cursor();
extern void save_mouse_bg();
extern void restore_mouse_bg();
extern void schick_set_video();
extern void schick_reset_video();
extern void do_h_line(PhysPt ptr,unsigned short x1, unsigned short x2, unsigned short y, char color);
extern void do_v_line(PhysPt ptr, unsigned short x, unsigned short y1, unsigned short y2, char color);
extern void do_border(PhysPt dst, unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, char color);
extern void do_pic_copy(unsigned short mode);
extern void do_save_rect();
extern void do_fill_rect(RealPt dst, unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short color);

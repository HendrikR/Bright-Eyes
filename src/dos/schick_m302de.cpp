#include <stdlib.h>
#include <string.h>

#include "schick.h"

#include "schick_rewrite/seg002.h"
#include "schick_rewrite/seg003.h"
#include "schick_rewrite/seg007.h"
#include "schick_rewrite/seg008.h"
#include "schick_rewrite/seg098.h"

/* dice table */
static char dice_tab[4] = {6, 20, 3, 4};

static int seg002(unsigned short offs) {
	switch (offs) {

	case 0x0017:
	case 0x0045:	/* wird bei Musikmenu aufgerufen */
	case 0x0209:
	case 0x06fe:
	case 0x0832:	/* GUI Radio */
	case 0x0856:	/* Betrunken */
		return 0;
	case 0x0c0e: {
		short index = CPU_Pop16();
		CPU_Push16(index);

		unsigned int retval = get_readlength2(index);
		D1_LOG("get_readlength2(%d) = %d\n", index, retval);

		reg_ax = retval & 0xffff;
		reg_dx = (retval>>16) & 0xffff;

		return 1;
	}
	case 0x0c28: {
		unsigned short index = CPU_Pop16();
		CPU_Push16(index);

		D1_LOG("ReadDatfile()\n");
		return 0;
	}
	case 0x0c72:
	case 0x0cb6:
			return 0;
	case 0x0d27: {
		unsigned short index = CPU_Pop16();
		CPU_Push16(index);
		D1_LOG("OpenAndSeekDatfile(%u)\n", index);
		return 0;
	}
	case 0x0ed2:
			return 0;
	case 0x1634: {
		/* Leaf Function */
		unsigned short v1 = CPU_Pop16();
		unsigned short v2 = CPU_Pop16();
		unsigned short v3 = CPU_Pop16();
		unsigned short v4 = CPU_Pop16();
		CPU_Push16(v4);
		CPU_Push16(v3);
		CPU_Push16(v2);
		CPU_Push16(v1);

		D1_LOG("cmp_smth(%d, %d, %d, %d);\n", v1, v2, v3, v4);
		D1_LOG("ds:299c = %d\n", real_readw(ds, 0x299c));
		D1_LOG("ds:299e = %d\n", real_readw(ds, 0x299e));

		short retval = cmp_smth(v1, v2, v3, v4);
		D1_LOG("Should return %d\n", retval);
		reg_ax = retval;

		return 1;
	}
	case 0x16fd:
	case 0x1802:
	case 0x18b3:	/* Leaf Function */
	case 0x1921:
	case 0x192b:
	case 0x1a34:
	case 0x1cf2:	/* Shop: Item zurücklegen */
	case 0x1d67:
	case 0x1ecc:
	case 0x21ab:
	case 0x232a:
			 return 0;
	case 0x25ce: {
		reg_ax = get_current_season();
		return 1;
		}
	case 0x274e:
	case 0x2e26:	/* Wunder Rondra: Starker Schwertarm */
	case 0x2e69:	/* Wunder Rondra: Starker Schwertarm  Leaf Function */
	case 0x2f7a:	/* Leaf Function - near&far */
	case 0x3071:
	case 0x3230:
	case 0x37c4:
			return 0;

	case 0x3b4f:  {
		D1_INFO("set_and_spin_lock()\n");
		return 1;
	}
	case 0x3ca6:
	case 0x3dbb:	/* Schiffsfahrt */
	case 0x3ebb:
	case 0x4016:	/* Kopierschutzabfrage */
	case 0x404f:
	case 0x40d1:
	case 0x41cd:
		return 0;

	case 0x43e7: {
		D1_LOG("set_to_ff()\n");
		set_to_ff();
		return 1;
	}
	case 0x43fd:
		return 0;
	case 0x4485: {
		short val = CPU_Pop16();
		CPU_Push16(val);

		reg_ax = mod_timer(val);
		D1_LOG("mod_timer(%d) = %d\n", val, reg_eax);
		return 1;
	}
	case 0x44aa:
		 return 0;
	case 0x4559: {

		reg_ax = can_merge_group();
		D1_LOG("can_merge_group() = %d\n", (short)reg_ax);

		return 1;
	}
	case 0x45db: {
		short val = CPU_Pop16();
		CPU_Push16(val);

		D1_LOG("div16(%d)\n", val);

		reg_ax = div16(val);

		return 1;
	}
	case 0x45ea:	/* Leaf Function - far only */
	case 0x4658:	/* Leaf Function - far only */
	case 0x4707:	/* Leaf Function - far & near */
	case 0x472b:	/* No Leaf - far only, calls only 0x4707 */
		return 0;

	case 0x48b1: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = check_hero(MemBase + Real2Phys(hero));

		D1_LOG("check_hero(%s) = %d\n",
			schick_getCharname(hero), reg_ax);

		return 1;
	}
	case 0x49d8: {
		RealPt  ptr = CPU_Pop32();
		CPU_Push32(ptr);

		D1_LOG("istHeldBeiSinnenUndGruppe(%s)\n",
			schick_getCharname(ptr));
		return 0;
	}
	case 0x4a05:
		return 0;
	case 0x4a87: {
		RealPt hero = CPU_Pop32();
		short val = CPU_Pop16();
		CPU_Push16(val);
		CPU_Push32(hero);

		add_hero_ae(MemBase + Real2Phys(hero), val);

		D1_LOG("add_hero_ae(%s, %d)\n",
				schick_getCharname(hero), val);

		return 1;
	}
	case 0x4adc:
	case 0x4df3:
		/* Wunder TSA heilt ganze Gruppe 6x */
		return 0;
	case 0x4ff9: {
		/* Eigenschaftsprobe */
		RealPt hero = CPU_Pop32();
		unsigned attrib = CPU_Pop16();
		signed bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(attrib);
		CPU_Push32(hero);

		reg_ax = test_attrib(MemBase + Real2Phys(hero),
				attrib, bonus);

		return 1;
	}
	case 0x504e: {
		/* Talent-/Zauber-Probe */
		unsigned hero = CPU_Pop32();
		unsigned short attrib1 = CPU_Pop16();
		unsigned short attrib2 = CPU_Pop16();
		unsigned short attrib3 = CPU_Pop16();
		signed short bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(attrib3);
		CPU_Push16(attrib2);
		CPU_Push16(attrib1);
		CPU_Push32(hero);

		reg_ax = test_attrib3(MemBase + Real2Phys(hero),
				attrib1, attrib2, attrib3, bonus);

		return 1;
	}
	case 0x515e:
		 return 0;
	case 0x51c2: {
		unsigned int money;

		money = get_party_money();
		D1_INFO("Aktuelles Gruppenvermögen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);

		reg_ax = money & 0xffff;
		reg_dx = (money>>16) & 0xffff;

		return 1;
	}
	case 0x5221: {
		unsigned int money = CPU_Pop32();
		CPU_Push32(money);

		D1_INFO("Setze Gruppenvermögen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);

		return 0;
	}
	case 0x5331: {
		unsigned int money = CPU_Pop32();
		CPU_Push32(money);

		D1_INFO("Ändere Gruppenvermögen = %dD %dS %dH\n",
			money / 100, (money % 100) / 10, money % 10);
		return 0;
	}
	case 0x5349: {
		RealPt hero = CPU_Pop32();
		int ap = CPU_Pop32();
		CPU_Push32(ap);
		CPU_Push32(hero);

		D1_INFO("%s erhält %d AP\n",
					schick_getCharname(hero), ap);
		add_hero_ap(MemBase+Real2Phys(hero), ap);

		return 1;
	}
	case 0x535f: {
		/* APs verteilen */
		int group_ap = CPU_Pop32();
		CPU_Push32(group_ap);

		D1_INFO("Gruppe erhält %d AP\n", group_ap);

		return 0;
	}
	case 0x53e8: {
		signed short ap = CPU_Pop16();
		CPU_Push16(ap);

		D1_INFO("add_hero_ap_all(%+d)\n", ap);
		add_hero_ap_all(ap);

		return 1;
	}
	case 0x5452: {
		signed short ap = CPU_Pop16();
		CPU_Push16(ap);

		D1_INFO("sub_hero_ap_all(%+d)\n", ap);
		sub_hero_ap_all(ap);

		return 1;
	}
	case 0x54e9: {
		/* unsigned short get_hero_index(hero_ptr *hero); */
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		reg_ax = get_hero_index(MemBase+Real2Phys(hero));
		D1_LOG("get_hero_index(%s) = (%d)\n",
				schick_getCharname(hero), reg_ax);
		return 1;
	}
	case 0x5520: {
		/* int get_item_pos(hero_ptr *hero, unsigned short item)*/
		RealPt hero = CPU_Pop32();
		unsigned short item = CPU_Pop16();
		CPU_Push16(item);
		CPU_Push32(hero);

		reg_ax = get_item_pos(MemBase + Real2Phys(hero), item);

		D1_LOG("get_item_pos(%s, 0x%04x) = %d\n",
					schick_getCharname(hero),
					item, (short)reg_ax);
		return 1;
	}
	case 0x554c: {
		unsigned short item = CPU_Pop16();
		CPU_Push16(item);

		reg_ax = get_first_hero_with_item(item);
		D1_INFO("get_first_hero_with_item(%d) = %d\n",
			item, reg_ax);

		return 1;
	}
	case 0x55b1:	/* Leaf Function - near only */
	case 0x5615:	/* Krakenangriff */
	case 0x5667:
	case 0x56d6:	/* Tür einschlagen */
	case 0x573e:	/* Alle Tot */
			return 0;
	case 0x5799: {
		reg_ax = count_heroes_available_in_group();
		D1_LOG("count_heroes_available_in_group() = %d\n",
			reg_ax);
		return 1;
	}
	case 0x5816: {
		unsigned short argc = CPU_Pop16();
		CPU_Push16(argc);
		D1_TRAC("main(argc=0x%04x, ...)\n", argc);
		return 0;
	}
	case 0x5a68: {
		unsigned short bytes = CPU_Pop16();
		CPU_Push16(bytes);

		D1_LOG("alloc_byte(%d)\n", bytes);
		return 0;
	}
	default:
		D1_TRAC("Segment 0x51e:0x%04x\n", offs);
		return 0;
	}
}

static int seg098(unsigned short offs) {
	switch (offs) {

	case 0x20: {
		RealPt hero = CPU_Pop32();
		CPU_Push32(hero);

		D1_LOG("Menu: Magie Anwenden %s\n", schick_getCharname(hero));

		return 0;
	}
	case 0x25: {
		/* Zauberprobe */
		RealPt hero = CPU_Pop32();
		unsigned short spell = CPU_Pop16();
		signed short bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(spell);
		CPU_Push32(hero);

		reg_ax = test_spell(MemBase + Real2Phys(hero), spell, bonus);
		D1_INFO("Zauberprobe : %s %+d ", names_spell[spell], (char)bonus);
		return 1;
	}
	case 0x2a: {
		/* Zauber auswählen */
		RealPt hero = CPU_Pop32();
		unsigned short a1 = CPU_Pop16();
		unsigned short a2 = CPU_Pop16();
		CPU_Push16(a2);
		CPU_Push16(a1);
		CPU_Push32(hero);

		D1_LOG("Menu: Zauber auswählen(%s, %d, %d)\n",
			schick_getCharname(hero), a1, a2);

		return 0;
	}
	case 0x2f: {
		D1_INFO("Seg098:0x%04x()\n", offs);
		return 0;
	}
	case 0x39: {
		D1_INFO("Seg098:0x%04x()\n", offs);
		return 0;
	}
	case 0x3e: {
		/* Untested */
		D1_INFO("seg098_3e()\n");
		/*reg_ax = seg098_3e(); */
		return 0;
	}
	case 0x48: {
		unsigned short spell = CPU_Pop16();
		unsigned short half_cost = CPU_Pop16();
		CPU_Push16(half_cost);
		CPU_Push16(spell);

		reg_ax = get_spell_cost(spell, half_cost);

		D1_LOG("get_spell_cost(%s, %d) = %d\n",
			names_spell[spell], half_cost, (short)reg_ax);

		return 1;
	}
	case 0x52: {
		unsigned short spell = CPU_Pop16();
		signed short bonus = CPU_Pop16();
		CPU_Push16(bonus);
		CPU_Push16(spell);

		D1_LOG("Zauberprobe für alle\n");
		reg_ax = test_spell_group(spell, bonus);

		return 1;
	}
	case 0x57: {
		D1_LOG("Menu: Zauberer auswählen\n");
		return 0;
	}
	default:
		D1_ERR("Uncatched call to Segment seg098:0x%04x\n", offs);
		exit(1);
	}
	return 0;
}

// Intercept far CALLs (both 32 and 16 bit)
int schick_farcall_v302de(unsigned segm, unsigned offs, unsigned ss)
{
	if (segm == 0x4ac)
		return 0;
	if (segm == 0x51e)
		return seg002(offs);

	if (segm == 0xb2a)	{
		//this is for mouse handling and spams the log
		//D1_LOG("Segment 0xb2a:0x%04x\n", offs);
		return 0;
	}

	if (segm == 0xf18) {
		if (offs == 0x8) {
			unsigned short val = CPU_Pop16();
			CPU_Push16(val);

			reg_ax = swap_u16(val);
			D1_GFX("swap_u16(val=0x%04x); = 0x%04x\n", val, reg_ax);

			return 1;
		}
		if (offs == 0x14) {
			unsigned short mode = CPU_Pop16();
			CPU_Push16(mode);

			D1_GFX("set_video_mode(mode=0x%x);\n", mode);
			set_video_mode(mode);

			return 1;
		}
		if (offs == 0x2a) {
			unsigned short page = CPU_Pop16();
			CPU_Push16(page);

			D1_GFX("set_display_page(page=0x%x);\n", page);
			set_video_page(page);

			return 1;
		}
		if (offs == 0x40) {
			RealPt addr = CPU_Pop32();
			CPU_Push32(addr);

			D1_GFX("SaveDisplayStat(dstat=0x%x:0x%x);\n",
				RealSeg(addr), RealOff(addr));

			return 0;
		}
		if (offs == 0xea) {
			RealPt ptr = CPU_Pop32();
			unsigned short color = CPU_Pop16();
			CPU_Push16(color);
			CPU_Push32(ptr);

			D1_GFX("set_color(rgb=0x%x:0x%x, color=0x%x);\n",
				RealSeg(ptr), RealOff(ptr), color);

			set_color(MemBase + Real2Phys(ptr), color);

			D1_GFX("RGB=(0x%x, 0x%x, 0x%x);\n",
				real_readb(RealSeg(ptr), RealOff(ptr)),
				real_readb(RealSeg(ptr), RealOff(ptr) + 1),
				real_readb(RealSeg(ptr), RealOff(ptr) + 2));

			return 1;
		}
		if (offs == 0x119) {
			RealPt ptr = CPU_Pop32();
			unsigned short first_color = CPU_Pop16();
			unsigned short colors = CPU_Pop16();
			CPU_Push16(colors);
			CPU_Push16(first_color);
			CPU_Push32(ptr);

			unsigned short i;

			D1_GFX("set_palette(rgb=0x%x:0x%x, first_color=0x%x, colors=0x%x);\n",
				RealSeg(ptr), RealOff(ptr), first_color, colors);

			set_palette(MemBase + Real2Phys(ptr), first_color, colors);

			if (RealSeg(ptr) == datseg)
				D1_LOG("Palette at DS:0x%x\n", RealSeg(ptr));
			for (i=0; i<colors; i++)
				D1_GFX("\"\\%02d\\%02d\\%02d\"..\n",
					host_readb(MemBase + Real2Phys(ptr)+i*3),
					host_readb(MemBase + Real2Phys(ptr)+i*3+1),
					host_readb(MemBase + Real2Phys(ptr)+i*3+2));
			return 1;
		}
		if (offs == 0x14d) {
			RealPt rptr = CPU_Pop32();
			unsigned short cnt = CPU_Pop16();
			unsigned short color = CPU_Pop16();
			CPU_Push16(color);
			CPU_Push16(cnt);
			CPU_Push32(rptr);

			draw_h_line(Real2Phys(rptr), cnt, color);

			if (RealSeg(rptr) == 0xa000)
				D1_GFX("HLine(X=%03d,Y=%03d,len=%u,color=0x%02x);\n",
					RealOff(rptr) % 320,
					RealOff(rptr) / 320, cnt, color);
			else
				D1_GFX("HLine(0x%04x:0x%04x,len=%u,color=0x%02x);\n",
					RealSeg(rptr), RealOff(rptr), cnt, color);
			return 1;
		}
		if (offs == 0x1af) {
			RealPt rptr = CPU_Pop32();
			unsigned short cnt = CPU_Pop16();
			unsigned short color = CPU_Pop16();
			unsigned short space = CPU_Pop16();
			CPU_Push16(space);
			CPU_Push16(color);
			CPU_Push16(cnt);
			CPU_Push32(rptr);

			draw_h_spaced_dots(Real2Phys(rptr), cnt, color, space);
			if (RealSeg(rptr) == 0xa000)
				D1_GFX("HSpacedDots(X=%03d,Y=%03u,%03u,0x%02x,%u);\n",
					RealOff(rptr) % 320,
					RealOff(rptr) / 320, cnt, color, space);
			else
				D1_GFX("HSpacedDots(0x%04x:0x%04x,0x%04x,0x%02x,0x%04x);\n",
					RealSeg(rptr), RealOff(rptr), cnt, color, space);
			return 1;
		}
		if (offs == 0x219) {
			unsigned short off_dest=real_readw(ss, reg_sp);
			unsigned short seg_dest=real_readw(ss, reg_sp+2);
			unsigned short x1=real_readw(ss, reg_sp+4);
			unsigned short y1=real_readw(ss, reg_sp+6);
			unsigned short x2=real_readw(ss, reg_sp+8);
			unsigned short y2=real_readw(ss, reg_sp+10);

			unsigned short val12=real_readw(ss, reg_sp+12);
			unsigned short val14=real_readw(ss, reg_sp+14);
			unsigned short val16=real_readw(ss, reg_sp+16);
			unsigned short val18=real_readw(ss, reg_sp+18);

			unsigned short width=real_readw(ss, reg_sp+20);
			unsigned short height=real_readw(ss, reg_sp+22);
			unsigned short off_src=real_readw(ss, reg_sp+24);
			unsigned short seg_src=real_readw(ss, reg_sp+26);

			if (seg_dest == 0xa000)
				D1_GFX("PicCopy(X=%03u,Y=%03u,x1=%03u,y1=%03u,x2=%u,y2=%u,val12=%u,val14=%u,val16=%u,val18=%u,width=%03u,height=%03u,src=0x%04x:0x%04x)\n",
					off_dest%320, off_dest/320,
					x1, y1,	x2, y2,
					val12, val14, val16, val18,
					width, height, seg_src, off_src);
			else
				D1_GFX("PicCopy(dest=0x%04x:0x%04x,x1=%03u,y1=%03u,x2=%u,y2=%u,val12=%u,val14=%u,val16=%u,val18=%u,width=%03u,height=%03u,src=0x%04x:0x%04x)\n",
					seg_dest, off_dest,
					x1, y1,	x2, y2,
					val12, val14, val16, val18,
					width, height, seg_src, off_src);
			return 0;
		}
		if (offs == 0x655) {
			RealPt rptr = CPU_Pop32();
			RealPt dst = CPU_Pop32();
			unsigned short width = CPU_Pop16();
			unsigned short height = CPU_Pop16();
			CPU_Push16(height);
			CPU_Push16(width);
			CPU_Push32(dst);
			CPU_Push32(rptr);

			/* Seg and Off are swapped */
			rptr = (rptr >> 16) | (rptr << 16);

			save_rect(Real2Phys(rptr), Real2Phys(dst), width, height);
			if (RealSeg(rptr) == 0xa000)
			D1_GFX("save_rect(X=%u,Y=%u,dst=0x%04x:0x%04x,width=%u, height=%u)\n",
					RealOff(rptr) % 320,
					RealOff(rptr) / 320,
					RealSeg(dst), RealOff(dst),
					width, height);
			else
			D1_GFX("save_rect(src=0x%04x:0x%04x,dst=0x%04x:0x%04x,width=%u, height=%u)\n",
					RealSeg(rptr), RealOff(rptr),
					RealSeg(dst), RealOff(dst),
					width, height);
			return 1;
		}
		if (offs == 0x68c) {
			RealPt rptr = CPU_Pop32();
			unsigned short color = CPU_Pop16();
			unsigned short width = CPU_Pop16();
			unsigned short height = CPU_Pop16();
			CPU_Push16(height);
			CPU_Push16(width);
			CPU_Push16(color);
			CPU_Push32(rptr);

			/* Seg and Off are swapped */
			rptr = (rptr >> 16) | (rptr << 16);

			fill_rect(Real2Phys(rptr), color, width, height);

			if (RealSeg(rptr) == 0xa000)

				D1_GFX("fill_rect(X=%u,Y=%u,color=%u,width=%u,height=%u)\n",
					RealOff(rptr) % 320,
					RealOff(rptr) / 320,
					color, width, height);
			else
				D1_GFX("fill_rect(dest=0x%04x:0x%04x,color=%u,cnt=%u,%u)\n",
					RealSeg(rptr), RealOff(rptr),
					color, width, height);
			return 1;
		}
		if (offs == 0x6c5) {
			RealPt dst = CPU_Pop32();
			RealPt src = CPU_Pop32();
			unsigned short width_to_copy = CPU_Pop16();
			unsigned short height = CPU_Pop16();
			unsigned short dst_width = CPU_Pop16();
			unsigned short src_width = CPU_Pop16();
			unsigned short solid = CPU_Pop16();
			CPU_Push16(solid);
			CPU_Push16(src_width);
			CPU_Push16(dst_width);
			CPU_Push16(height);
			CPU_Push16(width_to_copy);
			CPU_Push32(src);
			CPU_Push32(dst);

			D1_GFX("copy_solid_permuted(dst=0x%04x:0x%04x,src=0x%04x:0x%04x,width_to_copy=%u,height=%u,dst_width=%u,src_width=%u,solid=%u)\n",
					RealSeg(dst), RealOff(dst),
					RealSeg(src), RealOff(src),
					width_to_copy, height, dst_width,
					src_width, solid);

			copy_solid_permuted(MemBase + Real2Phys(dst),
				MemBase + Real2Phys(src), width_to_copy,
				height,	dst_width, src_width, solid);

			return 1;
		}
		/* used often in cities and dungeons */
		if (offs == 0x816) {
			RealPt dst = CPU_Pop32();
			RealPt src = CPU_Pop32();
			unsigned short width_to_copy = CPU_Pop16();
			unsigned short height = CPU_Pop16();
			unsigned short dst_width = CPU_Pop16();
			unsigned short src_width = CPU_Pop16();
			unsigned short solid = CPU_Pop16();
			CPU_Push16(solid);
			CPU_Push16(src_width);
			CPU_Push16(dst_width);
			CPU_Push16(height);
			CPU_Push16(width_to_copy);
			CPU_Push32(src);
			CPU_Push32(dst);

			D1_GFX("copy_solid(dst=0x%04x:0x%04x,src=0x%04x:0x%04x,width_to_copy=%u,height=%u,dst_width=%u,src_width=%u,solid=%u)\n",
					RealSeg(dst), RealOff(dst),
					RealSeg(src), RealOff(src),
					width_to_copy, height, dst_width,
					src_width, solid);

			copy_solid(MemBase + Real2Phys(dst),
				MemBase + Real2Phys(src), width_to_copy,
				height,	dst_width, src_width, solid);

			return 1;
		}
		if (offs == 0x967){
			unsigned short width = CPU_Pop16();
			unsigned short height = CPU_Pop16();
			RealPt dst = CPU_Pop32();
			RealPt src = CPU_Pop32();
			RealPt tmp_buffer = CPU_Pop32();
			unsigned short mode = CPU_Pop16();
			CPU_Push16(mode);
			CPU_Push32(tmp_buffer);
			CPU_Push32(src);
			CPU_Push32(dst);
			CPU_Push16(height);
			CPU_Push16(width);

			D1_GFX("decomp_rle(width=%d, height=%d, dst=0x%x:0x%x, src=0x%x:0x%x, tmp_buffer=0x%x:0x%x, mode=%d)\n",
				width, height,	RealSeg(dst), RealOff(dst),
				RealSeg(src), RealOff(src), RealSeg(tmp_buffer),
				RealOff(tmp_buffer), mode);

			decomp_rle(width, height, MemBase + Real2Phys(dst),
				MemBase + Real2Phys(src),
				MemBase + Real2Phys(tmp_buffer), mode);

			return 1;
		}
		D1_GFX("Rasterlib:0x%x\n", offs);
		return 0;
	}


	if (segm == 0x0ae7) {
		if (offs == 0x000c) {
			unsigned short mod = CPU_Pop16();
			CPU_Push16(mod);

			D1_LOG("update_direction(%d)\n", mod);

			reg_ax = update_direction(mod);
			return 1;
		}
		return 0;
	}
	if (segm == 0x0c85) return 0;
	if (segm == 0x0e41) return 0;
	if (segm == 0x0ef8) {

		if (offs == 0x000b) {
			signed lo = CPU_Pop16();
			signed hi = CPU_Pop16();
			CPU_Push16(hi);
			CPU_Push16(lo);

			reg_ax = random_interval(lo, hi);

			D1_INFO("randomInterval %d - %d : %d\n",
				lo, hi, reg_ax);

			return 1;
		}
		if (offs == 0x002b) {
			unsigned p1 = CPU_Pop16();
			CPU_Push16(p1);

			reg_ax = random_schick(p1);

			D1_INFO("random(%d) = %d\n", p1, reg_ax);

			return 1;
		}
		if (offs == 0x007a) {
			unsigned n = CPU_Pop16();
			unsigned m = CPU_Pop16();
			unsigned x = CPU_Pop16();
			CPU_Push16(x);
			CPU_Push16(m);
			CPU_Push16(n);

			reg_ax = dice_roll(n, m, x);

			D1_INFO("wuerfel %dW%d%+d = %d\n",
				n, m, x, (short)reg_ax);

			return 1;
		}
		if (offs == 0x00c4) {
			unsigned val = CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			CPU_Push16(val);

			reg_ax = is_in_word_array(val, MemBase + Real2Phys(p));

			D1_LOG("is_in_word_array(0x%x, 0x%04x:0x%04x) = %d\n",
				val, RealSeg(p), RealOff(p), reg_ax);

			return 1;
		}
		if (offs == 0x00ef) {
			unsigned val = CPU_Pop16();
			RealPt p = CPU_Pop32();
			CPU_Push32(p);
			CPU_Push16(val);

			reg_ax = is_in_byte_array((char)val, MemBase+Real2Phys(p));
			D1_LOG("is_in_byte_array(0x%x, 0x%04x:0x%04x) = %d\n",
				(char)val, RealSeg(p), RealOff(p), reg_ax);

			return 1;
		}
		if (offs == 0x00a0) {
			signed n = CPU_Pop16();
			unsigned m = CPU_Pop16();
			signed x = CPU_Pop16();
			RealPt pmin = CPU_Pop32();
			RealPt pmax = CPU_Pop32();
			CPU_Push32(pmax);
			CPU_Push32(pmin);
			CPU_Push16(x);
			CPU_Push16(m);
			CPU_Push16(n);

			calc_damage_range(n, m, x, MemBase+Real2Phys(pmin),
				MemBase+Real2Phys(pmax));

			D1_LOG("calc_damage_range(%d, %d, %d)\n", n, m, x);

			return 1;
		}
		if (offs == 0x0119) {
		        unsigned val = CPU_Pop16();
		        CPU_Push16(val);

			unsigned short m = ((val & 0x0f00) >> 8) - 1;
			if (m > 3)
				m = 3;

			m = dice_tab[m];

			reg_ax = dice_template(val);

		        D1_INFO("Wuerfel %dW%d%+d = %d\n",
				(val & 0xf000) >> 12, m,
				(char)(val & 0xff), (short)reg_ax);

			return 1;
		}
		if (offs == 0x0186) {
			unsigned short val = CPU_Pop16();
			RealPt min = CPU_Pop32();
			RealPt max = CPU_Pop32();
			CPU_Push32(max);
			CPU_Push32(min);
			CPU_Push16(val);

			damage_range_template(val, MemBase + Real2Phys(min),
				MemBase + Real2Phys(max));

			D1_LOG("damage_range_template() Untested\n");

			return 1;
		}
		D1_ERR("Uncatched call to Segment seg007:0x%04x\n", offs);
		exit(1);
		return 0;
	}

	if (segm == 0x0ff1) return 0;
	if (segm == 0x1030) return 0;
	/* No overlay */
	if (segm == 0x1042) return 0;
	if (segm == 0x1112) return 0;

	if (segm == 0x12db) return 0;
	if (segm == 0x12de) return 0;

	/* stub026 */
	if (segm == 0x12e5) {
		if (offs == 0x0020) {
			D1_LOG("ip=0x%04X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0025) {
			D1_LOG("ip=0x%04X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x002a) {
			D1_LOG("ip=0x%04X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x002f) {
			D1_LOG("short ChooseLoadSavegame(void)\n");
			return 0;
		}
		if (offs == 0x0034) {
			D1_LOG("ip=0x%04X ChooseSaveSavegame()\n", offs);
			return 0;
		}
		if (offs == 0x0039) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x003e) {
			D1_LOG("ip=0x%4X ReleaseHero()\n", offs);
			return 0;
		}
		if (offs == 0x0043) {
			D1_LOG("ip=0x%4X ChooseFreeHero()\n", offs);
			return 0;
		}
		if (offs == 0x0048) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x004d) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0052) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0057) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x005c) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		if (offs == 0x0066) {
			D1_LOG("ip=0x%4X unknown()\n", offs);
			return 0;
		}
		return 0;
	}
	if (segm == 0x12ec) {
		if (offs = 0x0025) {
			short ani = CPU_Pop16();
			CPU_Push16(ani);

			D1_LOG("ANI %02d angefordert\n", ani);
			return 0;
		}
		return 0;
	}
	if (segm == 0x12f1) return 0;
	if (segm == 0x12f9) return 0;
	/* Spielstand und Zeit */
	if (segm == 0x12ff) return 0;
	if (segm == 0x1303) return 0;
	if (segm == 0x1309) {
		if (offs == 0x0020) {
			unsigned short v1 = CPU_Pop16();
			CPU_Push16(v1);

			D1_LOG("Kampf 0x%02x\n", v1);
			return 0;
		}
		return 0;
	}
	if (segm == 0x130f) return 0;
	/* Kampf */
	if (segm == 0x1312) return 0;
	/* Kampf Loot + AP*/
	if (segm == 0x1316) return 0;
	/* Kampf */
	if (segm == 0x131a) return 0;
	/* Kampf */
	if (segm == 0x131f) return 0;
	if (segm == 0x1324) return 0;
	/* Kampf */
	if (segm == 0x1328) return 0;
	/* Kampf */
	if (segm == 0x132d) return 0;
	/* Werte Prüfen */
	if (segm == 0x1330) return 0;
	if (segm == 0x1335) return 0;
	/* Kampf */
	if (segm == 0x1338) return 0;
	/* Kampf */
	if (segm == 0x133b) return 0;
	/* Kampf */
	if (segm == 0x133f) return 0;
	/* Zustand */
	if (segm == 0x1344) return 0;
	/* Wächter aussuchen */
	if (segm == 0x1348) return 0;
	if (segm == 0x1350) return 0;
	if (segm == 0x1353) return 0;
	/* Steigern */
	if (segm == 0x1358) return 0;
	/*Vorräte auffüllen */
	if (segm == 0x135c) return 0;
	if (segm == 0x135f) return 0;
	/* Heiler stub053 */
	if (segm == 0x1362) {
		if (offs == 0x20) {
			unsigned short typi = real_readb(datseg, 0x4224);
			char v1 = real_readb(datseg, typi * 2 + 0x66ea);
			char v2 = real_readb(datseg, typi * 2 + 0x66ea + 1);
			D1_INFO("Heiler: 0x%02x Rabatt: %d%% Qualität: %d\n",
								typi, v1, v2);
			return 0;
		}
		return 0;
	}

	if (segm == 0x1365) return 0;
	if (segm == 0x1369) return 0;
	/* Waren kaufen */
	if (segm == 0x136d) return 0;
	/* Waren verkaufen */
	if (segm == 0x1370) return 0;
	/* Dialog mit SCHMIED */
	if (segm == 0x1373) return 0;
	if (segm == 0x1377) return 0;
	if (segm == 0x137b) return 0;
	/* Held Löschen*/
	if (segm == 0x137e) return 0;
	/* Wunder erbitten */
	if (segm == 0x1383) return 0;
	if (segm == 0x1386) return 0;
	/* Hafen */
	if (segm == 0x138a) return 0;
	if (segm == 0x1392) return 0;
	/* Zufallsnachrichten */
	if (segm == 0x139a) return 0;
	/* Magierakademie + Stoerrebrandt */
	if (segm == 0x13a1) return 0;
	if (segm == 0x13a8) return 0;
	if (segm == 0x13b4) return 0;
	/* */
	if (segm == 0x13b9)  {

		if (offs == 0x20) {
			char city=real_readb(datseg, 0x2d67);
			unsigned char ww=real_readb(datseg, 0x7c9d+city);
			D1_LOG("Merkwürdige Funktion\n");
			D1_LOG("Stadt: 0x%02x\t WW: 0x%02x\n", city, ww);
			return 0;
		}
		if (offs == 0x25) return 0;
		if (offs == 0x2a) return 0;
		if (offs == 0x2f) return 0;

		return 0;
	}
	/* Automap */
	if (segm == 0x13bd) return 0;
	/* Dungeon betreten */
	if (segm == 0x13c3) return 0;
	if (segm == 0x13cb) return 0;
	/* Kampf "Verfallene Herberge" */
	if (segm == 0x13d7) return 0;
	if (segm == 0x13e4) return 0;
	if (segm == 0x13e9) return 0;
	if (segm == 0x1401) return 0;
	if (segm == 0x1408) return 0;
	/* Kampf */
	if (segm == 0x140b) return 0;
	if (segm == 0x1417) return 0;
	if (segm == 0x1420) return 0;
	if (segm == 0x1429) return 0;
	if (segm == 0x142c) return 0;
	if (segm == 0x1432) return 0;
	if (segm == 0x1438) {
		if (offs == 0x0052) {
			unsigned int  ptr=real_readd(ss, reg_sp);
			D1_LOG("drawString(%s)\n", getString(ptr));
			return 0;
		}
		if (offs == 0x007f) {
			unsigned int  ptr=real_readd(ss, reg_sp);
			D1_LOG("drawLocName(%s)\n", getString(ptr));
			return 0;
		}
		return 0;
	}
	/* stub097 */
	if (segm == 0x1442) {
		if (offs == 0x0039) {
			unsigned int  ptr=real_readd(ss, reg_sp);
			D1_LOG("dialog_schmal(%s)\n", getString(ptr));
			return 0;
		}
		return 0;
	}
	/* stub098 */
	if (segm == 0x1449)
		return seg098(offs);
	if (segm == 0x144f) return 0;
	if (segm == 0x145e) return 0;
	if (segm == 0x1467) return 0;
	/* Kampf Gegner zaubert */
	if (segm == 0x1472) return 0;
	/* Talent anwenden*/
	if (segm == 0x147b) {
		if (offs == 0x0020) { // Talentprobe
			unsigned p0 = CPU_Pop32();
			unsigned p1 = CPU_Pop16();
			unsigned p2 = CPU_Pop16();
			CPU_Push16(p2);
			CPU_Push16(p1);
			CPU_Push32(p0);

			signed char p2_r = p2 & 0xFF;
			D1_INFO("Talentprobe %s: %s %+d ",
						schick_getCharname(p0),
						names_skill[p1], p2_r);
			return 0;
		}
		if (offs == 0x0025) return 0;
		D1_INFO("\t\tTalent:0x%x\n", offs);
		return 0;
	}

	if (segm == 0x1480) return 0;
	if (segm == 0x1485) return 0;
	if (segm == 0x148c) return 0;
	if (segm == 0x1491) return 0;
	/* Essen */
	if (segm == 0x1498) return 0;
	/* Kampf Orvil-Rovik Wölfe */
	if (segm == 0x149b) {
		if (offs == 0x0020) {
			unsigned short kampf = CPU_Pop16();
			unsigned short v2 = CPU_Pop16();
			CPU_Push16(v2);
			CPU_Push16(kampf);

			D1_INFO("Event? : Kampf = 0x%02x 0x%02x\n", kampf, v2);
			return 0;
		}
		return 0;
	}

	/* Reise Skjal-Orvil */
	if (segm == 0x14b4) return 0;
	/* Reise Skjal-Orvil */
	if (segm == 0x14c2) return 0;
	/* Reise Orvil-Rovik "Wölfe" */
	if (segm == 0x14cb) return 0;
	if (segm == 0x14d1) return 0;
	if (segm == 0x14d8) return 0;
	if (segm == 0x14e0) return 0;
	if (segm == 0x14e7) return 0;
	if (segm == 0x14ed) return 0;
	if (segm == 0x14f0) return 0;
	if (segm == 0x14f6) return 0;
	if (segm == 0x14f9) return 0;

	/* Borland C++ runtime */
	if (segm == 0x0) {
		/* nullsub */
		if (offs == 0x2c9) return 0;
		if (offs == 0x2f7) {
			/* int chdir(const char* __path) */
			RealPt path = CPU_Pop32();
			CPU_Push32(path);

			D1_LOG("chdir(%s)\n", (char*)MemBase+Real2Phys(path));

			return 0;
		}
		/* close() */
		if (offs == 0x31b) return 0;
		/* open() */
		if (offs == 0x61e) return 0;
		/* read() */
		if (offs == 0x654) return 0;
		if (offs == 0x678) return 0;

		if (offs == 0x6d0) {
			D1_LOG("C-Lib exit(%d)\n", real_readw(ss, reg_sp));
			return 0;
		}
		if (offs == 0x6df) {
			/* Not Called from far */
			D1_LOG("_exit(%d)\n", real_readw(ss, reg_sp));
			return 0;
		}
		if (offs == 0x70b){
			unsigned long retval;
			retval = (reg_dx << 16 | reg_ax) * (reg_cx << 16 | reg_bx);
			D1_LOG("Mul unsigned long %u * %u = %lu\n",
				reg_dx << 16 | reg_ax,
				reg_cx << 16 | reg_bx,
				retval);
			reg_ax = retval & 0xffff;
			reg_dx = (retval >> 16) & 0xffff;

			return 1;
		}
		/* struct copy*/
		if (offs == 0x722) {
			RealPt src = CPU_Pop32();
			RealPt dst = CPU_Pop32();
			CPU_Push32(dst);
			CPU_Push32(src);

			D1_LOG("F_SCOPY()");
			memcpy(MemBase + Real2Phys(dst),
				MemBase + Real2Phys(src), reg_cx);
			RET(8);
			return 1;
		}
		/* getcurdir() */
		if (offs == 0x73e) return 0;
		/* getdisk() */
		if (offs == 0x781) return 0;
		/* setdisk() */
		if (offs == 0x79b) return 0;
		/* dos_getdiskfree() */
		if (offs == 0x7ed) return 0;
		if (offs == 0x816) {
			D1_TRAC("_dos_getvect(int=0x%x)\n", real_readw(ss,reg_sp));
			return 0;
		}
		if (offs == 0x0825) {
			unsigned short interruptno = CPU_Pop16();
			RealPt isr = CPU_Pop32();
			CPU_Push32(isr);
			CPU_Push16(interruptno);

			D1_TRAC("_dos_setvect(int=0x%x, *isr=0x%x:0x%x)\n",
				interruptno, RealSeg(isr) - relocation,
				RealOff(isr));
			return 0;
		}
		if (offs == 0x839) return 0;
		if (offs == 0x840) {
			D1_LOG("Div unsigned long\n");
			return 0;
		}
		if (offs == 0x848) return 0;
		if (offs == 0x850) { //not called
			D1_LOG("Mod unsigned long\n");
			return 0;
		}
		if (offs == 0x8e7) return 0;
		/* rshift() */
		if (offs == 0x908) return 0;
		if (offs == 0x928) return 0;
		if (offs == 0x9b0) return 0;
		if (offs == 0xa10) return 0;
		if (offs == 0xb33) {
			/*seek()*/
			D1_LOG("__seek(Handle=0x%x, pos=%u, Mode=%d)\n",
			real_readw(ss, reg_sp), real_readw(ss, reg_sp+4)<<16+
			real_readw(ss, reg_sp+2), real_readw(ss, reg_sp+6));
			return 0;
		}
		/* mkdir() */
		if (offs == 0xb5c) return 0;
		if (offs == 0xbac) {
			unsigned short val=real_readw(ss, reg_sp);
			D1_TRAC("C-Lib srand(%d)\n", val);
			return 0;
		}
		if (offs == 0xbbd) {
			D1_LOG("rand()\n");
			return 0;
		}
		if (offs == 0x0be3) {
			/*read()*/
			D1_LOG("_read(fd=0x%x, buffer=0x%x:0x%x, len=%d)\n",
			real_readw(ss, reg_sp), real_readw(ss, reg_sp+4),
			real_readw(ss, reg_sp+2), real_readw(ss, reg_sp+6));
			return 0;
		}
		if (offs == 0x1123) {
			/* time(), user for randomize */
			unsigned short hi=real_readw(ss, reg_sp);
			unsigned short lo=real_readw(ss, reg_sp+2);
			D1_TRAC("C-Lib time(0x%04x)\n", hi<<16+lo);
			return 0;
		}
		if (offs == 0x117b) return 0;
		/* delete() */
		if (offs == 0x11a7) return 0;
		if (offs == 0x176d) return 0;
		if (offs == 0x1792) return 0;
		if (offs == 0x1e55) {
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			D1_LOG("free(0x%04x:0x%04x)\n", seg, off);
			return 0;		}
		if (offs == 0x1f69) {
			unsigned short lo=real_readw(ss, reg_sp);
			unsigned short hi=real_readw(ss, reg_sp+2);
			D1_LOG("farmalloc(%d)\n", hi<<16+lo);
			return 0;		}
		if (offs == 0x2287) {
			unsigned short nl=real_readw(ss, reg_sp);
			unsigned short nh=real_readw(ss, reg_sp+2);
			unsigned short lo=real_readw(ss, reg_sp+4);
			unsigned short hi=real_readw(ss, reg_sp+6);

			D1_LOG("calloc(%d, 0x%x)\n",
					(nh<<16)+nl, (hi<<16)+lo);
			return 0;
		}
		if (offs == 0x2315) return 0;
		if (offs == 0x2411) return 0;
		if (offs == 0x2596) return 0;
		if (offs == 0x2d82) {
			unsigned short i=4;
			unsigned short off=real_readw(ss, reg_sp+i);
			unsigned short seg=real_readw(ss, reg_sp+i+2);

			D1_LOG("C-Lib exec?(\"%s\", ",
					MemBase+(seg<<4)+off);
			/*
			do {
				i+=4;
				off=real_readw(ss, reg_sp+i);
				seg=real_readw(ss, reg_sp+i+2);
				if ((seg<<4)+off > 0)
						D1_LOG("\"%s\", ",
						MemBase+(seg<<4)+off);
					else
						D1_LOG("\"%s\")\n",
						MemBase+(seg<<4)+off);
			} while ((seg<<4)+off > 0);
			*/
			return 0;
		}
		if (offs == 0x2dff) {
			/* long atol(const char* s) */
			int val;
			RealPt s = CPU_Pop32();
			CPU_Push32(s);

			D1_LOG("atol(\"%s\") = ", getString(s));

			val = atoi((char*)getString(s));
			D1_LOG("%d\n", val);

			reg_ax = val & 0xffff;
			reg_dx = (val>>16) & 0xffff;

			return 1;
		}
		if (offs == 0x2eb2) {
			D1_LOG("C-Lib close(%d)\n", real_readw(ss, reg_sp));
			return 0;
		}
		if (offs == 0x2eda) return 0;
		if (offs == 0x2f25) return 0;
		if (offs == 0x3040) return 0;
		if (offs == 0x3073) return 0;
		if (offs == 0x30a0) return 0;
		if (offs == 0x3350) {
			/* char* itoa(int __value, char* string, int radix);
			radix is everytime 10 in this game*/

			short value = CPU_Pop16();
			RealPt string = CPU_Pop32();
			short radix = CPU_Pop16();
			CPU_Push16(radix);
			CPU_Push32(string);
			CPU_Push16(value);

			D1_LOG("itoa(%d, 0x%04x:0x%04x, %d)\n",
					value, RealSeg(string),
					RealOff(string), radix);
			sprintf((char*)MemBase+Real2Phys(string), "%d", value);

			reg_ax = RealOff(string);
			reg_dx = RealSeg(string);
			return 1;
		}
		if (offs == 0x33c0) {
			/*void *memcpy(void *dest, const void *src, size_t n)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("memcpy(0x%04x:0x%04x, 0x%04x:0x%04x, %u)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src), n);

			memcpy(MemBase + Real2Phys(dest), MemBase + Real2Phys(src), n);

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		if (offs == 0x3408) {
			/*void* memset(void *s, int c, size_t n)*/
			RealPt s = CPU_Pop32();
			short c = CPU_Pop16();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push16(c);
			CPU_Push32(s);

			D1_LOG("memset(0x%04x:0x%04x, 0x%02x, %u)\n",
					RealSeg(s), RealOff(s), c, n);
			// No bypass on graphic memory
			if (RealSeg(s) == 0xa000)
				return 0;

			memset(MemBase+Real2Phys(s), c, n);

			reg_ax = RealOff(s);
			reg_dx = RealSeg(s);
			return 1;
		}
		if (offs == 0x3479) {
			/* write(handle) */
			unsigned short handle=real_readw(ss, reg_sp);
			D1_LOG("write_0(%d)\n", handle);
			return 0;
		}
		if (offs == 0x34c7) {
			/*open()*/
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			unsigned short mode=real_readw(ss, reg_sp+4);

			D1_LOG("open(\"%s\",\"%04x\")\n",
					MemBase+(seg<<4)+off, mode);
			return 0;
		}
		if (offs == 0x3636) {
			/* sortof open() */
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);
			unsigned short mode=real_readw(ss, reg_sp+4);

			D1_LOG("C-Lib Unkn(\"%s\", 0x%04x)\n",
					MemBase+(seg<<4)+off, mode);
			return 0;
		}
		if (offs == 0x36dd) {
			/*printf()*/
			unsigned short off=real_readw(ss, reg_sp);
			unsigned short seg=real_readw(ss, reg_sp+2);

			D1_LOG("printf(\"%s\")\n", MemBase+(seg<<4)+off);
			return 0; }
		if (offs == 0x3d74) return 0;
			/* ret 0x000a */
		if (offs == 0x41d2) {
			unsigned short	o1=real_readw(ss, reg_sp);
			unsigned short	s1=real_readw(ss, reg_sp+2);
			unsigned short	o2=real_readw(ss, reg_sp+4);
			unsigned short	s2=real_readw(ss, reg_sp+6);
			unsigned short	o3=real_readw(ss, reg_sp+8);
			unsigned short	s3=real_readw(ss, reg_sp+10);
			unsigned short	o4=real_readw(ss, reg_sp+12);
			unsigned short	s4=real_readw(ss, reg_sp+14);
			D1_LOG("C-Lib sprintf(0x%04x:0x%04x, \"%s\", 0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					s1, o1, MemBase+(s2<<4)+o2,
					s3, o3, s4, o4);

			return 0;
		}
		if (offs == 0x4215) {
			/*char *strcat(char* dest, const char* src)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strcat(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src));

			strcat((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src));

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		if (offs == 0x4254) {
			/*int strcmp(const char* s1, const char* s2)*/
			RealPt s1 = CPU_Pop32();
			RealPt s2 = CPU_Pop32();
			CPU_Push32(s2);
			CPU_Push32(s1);

			D1_LOG("strcmp(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(s1), RealOff(s1),
					RealSeg(s2), RealSeg(s2));

			reg_ax = strcmp((char*)MemBase + Real2Phys(s1),
					(char*)MemBase + Real2Phys(s2));

			return 1;
		}
		if (offs == 0x4284) {
			/*char *strcpy(char* dest, const char* src)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strcpy(0x%04x:0x%04x, 0x%04x:0x%04x)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src));

			strcpy((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src));

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		if (offs == 0x42ad) {
			/*strlen()*/
			RealPt str = CPU_Pop32();
			CPU_Push32(str);

			D1_LOG("strlen(\"%s\")\n", getString(str));
			reg_ax = strlen((char*)getString(str));

			return 1;
		}
		if (offs == 0x42cc) {
			/*char *strncpy(char* dest, const char* src, size_t n)*/
			RealPt dest = CPU_Pop32();
			RealPt src = CPU_Pop32();
			unsigned short n = CPU_Pop16();
			CPU_Push16(n);
			CPU_Push32(src);
			CPU_Push32(dest);

			D1_LOG("strncpy(0x%04x:0x%04x, 0x%04x:0x%04x, %u)\n",
					RealSeg(dest), RealOff(dest),
					RealSeg(src), RealSeg(src), n);

			strncpy((char*)MemBase + Real2Phys(dest),
				(char*)MemBase + Real2Phys(src), n);

			reg_ax = RealOff(dest);
			reg_dx = RealSeg(dest);

			return 1;
		}
		if (offs == 0x462b) return 0;
		if (offs == 0x4a85) {
			/*write()*/
			unsigned short handle=real_readw(ss, reg_sp);
			unsigned short off=real_readw(ss, reg_sp+2);
			unsigned short seg=real_readw(ss, reg_sp+4);
			unsigned short val=real_readw(ss, reg_sp+6);
			D1_LOG("C-Lib __write(Handle=0x%x, Buffer=0x%x:0x%x, Len=%d)\n", handle, seg ,off, val);
			return 0;
		}
		if (offs == 0x4a88) return 0;

		D1_TRAC("\t\tC-Lib:0x%x\n", offs);
		return 0;
	}
	D1_TRAC("Unfetched Segment: 0x%04x\n", segm);
	return 0;
}
/* Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "msm_fb.h"
#include "mipi_dsi.h"
#include "mipi_ili9486.h"
#include <mach/gpio.h>//Flea
static struct msm_panel_info pinfo;

static struct mipi_dsi_phy_ctrl dsi_cmd_mode_phy_db = {
#ifdef CONFIG_FB_MSM_MDP303
     /* DSI Bit Clock at 350 MHz, 2 lane, RGB888 */
	/* regulator */
//tracy add Qualcomm patch for fix LCM sharking++
	{0x03, 0x0B, 0x05, 0x50},
//tracy add Qualcomm patch for fix LCM sharking--
	/* timing   */
	{0xA8, 0x89, 0x15, 0x00, 0x90, 0x95, 0x04, 0x23,
	0x10, 0x03, 0x04},
	/* phy ctrl */
	{0x7f, 0x00, 0x00, 0x00},
	/* strength */
	{0xbb, 0x02, 0x06, 0x00},
	/* pll control */
//tracy add Qualcomm patch for fix LCM sharking++
	{0x01, 0x59, 0x31, 0xd2, 0x00, 0x24, 0x37, 0x62,
	0x01, 0x0f, 0x0f,
	0x05, 0x14, 0x03, 0x0, 0x0, 0x0, 0x20, 0x0, 0x02, 0x0},
//tracy add Qualcomm patch for fix LCM sharking--
#else
/* DSI Bit Clock at 500 MHz, 2 lane, RGB888 */
	/* regulator */
	{0x03, 0x01, 0x01, 0x00},
	/* timing   */
	{0xb9, 0x8e, 0x1f, 0x00, 0x98, 0x9c, 0x22, 0x90,
	0x18, 0x03, 0x04},
	/* phy ctrl */
	{0x7f, 0x00, 0x00, 0x00},
	/* strength */
	{0xbb, 0x02, 0x06, 0x00},
	/* pll control */
	{0x01, 0xec, 0x31, 0xd2, 0x00, 0x40, 0x37, 0x62,
	0x01, 0x0f, 0x0f,//Arima modification
	0x05, 0x14, 0x03, 0x0, 0x0, 0x0, 0x20, 0x0, 0x02, 0x0},
#endif
};

static int __init mipi_cmd_ili9486_wvga_pt_init(void)
{
	int ret;
       pr_emerg("Flea-mipi_cmd_ili9486_wvga_pt_init \n");



      pinfo.xres =320;
	pinfo.yres =480;
	pinfo.type = MIPI_CMD_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
#ifdef CONFIG_FB_MSM_MDP303
	pinfo.lcdc.h_back_porch = 100;
	pinfo.lcdc.h_front_porch = 100;
	pinfo.lcdc.h_pulse_width = 8;
	pinfo.lcdc.v_back_porch = 20;
	pinfo.lcdc.v_front_porch = 20;
	pinfo.lcdc.v_pulse_width = 1;
#else
	pinfo.lcdc.h_back_porch = 50;
	pinfo.lcdc.h_pulse_width = 20;
	pinfo.lcdc.h_pulse_width = 8;
	pinfo.lcdc.v_back_porch = 10;
	pinfo.lcdc.v_front_porch = 10;
	pinfo.lcdc.v_pulse_width = 5;
#endif

/* CONFIG_FB_MSM_MDP303 */
	pinfo.lcdc.border_clr = 0;	/* blk */
	pinfo.lcdc.underflow_clr = 0xff;	/* blue */
	pinfo.lcdc.hsync_skew = 0;
	pinfo.bl_max = DRIVER_MAX_BACKLIGHT_LEVEL;;
	pinfo.bl_min = 1;
	pinfo.fb_num = 2;

#ifdef CONFIG_FB_MSM_MDP303
	pinfo.clk_rate = 350000000;
#else
	pinfo.clk_rate = 152000000;
#endif
	pinfo.lcd.refx100 = 6790; /* adjust refx100 to prevent tearing */
	pinfo.mipi.mode = DSI_CMD_MODE;
	pinfo.mipi.dst_format = DSI_CMD_DST_FORMAT_RGB888;//Flea++
	pinfo.mipi.vc = 0;
	pinfo.mipi.rgb_swap = DSI_RGB_SWAP_RGB;
	pinfo.mipi.data_lane0 = TRUE;
#ifdef CONFIG_FB_MSM_MDP303
	pinfo.lcd.vsync_enable = TRUE;
	pinfo.lcd.v_back_porch = 2;
	pinfo.lcd.v_front_porch = 2;	
	pinfo.lcd.v_pulse_width = 25;
	pinfo.lcd.vsync_notifier_period = 0;
	pinfo.lcd.hw_vsync_mode = TRUE;
	pinfo.mipi.data_lane1 = FALSE;
	pinfo.mipi.t_clk_post = 0x20;
	pinfo.mipi.t_clk_pre = 0x2F;
	pinfo.mipi.stream = 0; /* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.te_sel = 1; /* TE from vsync gpio */
	pinfo.mipi.interleave_max = 1;
	pinfo.mipi.insert_dcs_cmd = TRUE;
	pinfo.mipi.wr_mem_continue = 0x3c;
	pinfo.mipi.wr_mem_start = 0x2c;
	pinfo.mipi.dsi_phy_db = &dsi_cmd_mode_phy_db;
	pinfo.mipi.tx_eot_append = 0x01;
	pinfo.mipi.rx_eot_ignore = 0;
	pinfo.mipi.dlane_swap = 0x01;
#else
	 pinfo.mipi.data_lane1 = FALSE;
	pinfo.mipi.t_clk_post = 0x18;
	pinfo.mipi.t_clk_pre = 0x14;
	pinfo.mipi.stream = 0;	/* dma_p */
	pinfo.mipi.mdp_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.dma_trigger = DSI_CMD_TRIGGER_SW;
	pinfo.mipi.te_sel = 1; /* TE from vsycn gpio */
	pinfo.mipi.interleave_max = 1;
	pinfo.mipi.insert_dcs_cmd = TRUE;
	pinfo.mipi.wr_mem_continue = 0x3c;
	pinfo.mipi.wr_mem_start = 0x2c;
	pinfo.mipi.dsi_phy_db = &dsi_cmd_mode_phy_db;
#endif /* CONFIG_FB_MSM_MDP303 */
 
       pr_emerg("Flea-mipi_cmd_ili9486_wvga_pt_init_set_bl \n");
	ret = mipi_ili9486_device_register(&pinfo, MIPI_DSI_PRIM,
						MIPI_DSI_PANEL_WVGA);
	if (ret)
		pr_err("%s: failed to register device!\n", __func__);

	return ret;
}

module_init(mipi_cmd_ili9486_wvga_pt_init);


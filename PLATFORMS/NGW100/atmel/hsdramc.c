/*
 * Copyright (C) 2005-2006 Atmel Corporation
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
//bh#include <common.h>
//#define CFG_HSDRAMC
//#ifdef CFG_HSDRAMC
#include "atngw100.h"
#include <avr32/io.h>
#include "sdram.h"

#include "clk.h"
#include "memory-map.h"
//#include <asm/io.h>
#include <stdlib.h>

#include "hsdramc1.h"
static void sdram_delay(int us);
#define __raw_readl(a)          (*(volatile unsigned int   *)(a))
#define readl(a)              __raw_readl(a)
#define __raw_writel(v,a)       (*(volatile unsigned int   *)(a) = (v))
#define writel(v,a)          __raw_writel(v,a)
#define P2SEG              0xa0000000
#define P2SEGADDR(a) ((__typeof__(a))(((unsigned long)(a) & 0x1fffffff) | P2SEG))

#define uncached(addr) ((void *)P2SEGADDR(addr))

unsigned long sdram_init(const struct sdram_info *info)
{
	unsigned long *sdram = (unsigned long *)uncached(info->phys_addr);
	unsigned long sdram_size;
	unsigned long tmp;
	unsigned long bus_hz;
	unsigned int i;

	tmp = (HSDRAMC1_BF(NC, info->col_bits - 8)
	       | HSDRAMC1_BF(NR, info->row_bits - 11)
	       | HSDRAMC1_BF(NB, info->bank_bits - 1)
	       | HSDRAMC1_BF(CAS, info->cas)
	       | HSDRAMC1_BF(TWR, info->twr)
	       | HSDRAMC1_BF(TRC, info->trc)
	       | HSDRAMC1_BF(TRP, info->trp)
	       | HSDRAMC1_BF(TRCD, info->trcd)
	       | HSDRAMC1_BF(TRAS, info->tras)
	       | HSDRAMC1_BF(TXSR, info->txsr));

#ifdef CFG_SDRAM_16BIT
	tmp |= HSDRAMC1_BIT(DBW);
	sdram_size = 1 << (info->row_bits + info->col_bits
			   + info->bank_bits + 1);
#else
	sdram_size = 1 << (info->row_bits + info->col_bits
			   + info->bank_bits + 2);
#endif

	hsdramc1_writel(CR, tmp);

	/*
	 * Initialization sequence for SDRAM, from the data sheet:
	 *
	 * 1. A minimum pause of 200 us is provided to precede any
	 *    signal toggle.
	 */
//	udelay(200);
sdram_delay(200);
	/*
	 * 2. A Precharge All command is issued to the SDRAM
	 */
	hsdramc1_writel(MR, HSDRAMC1_MODE_BANKS_PRECHARGE);
	hsdramc1_readl(MR);
	writel(0, sdram);

	/*
	 * 3. Eight auto-refresh (CBR) cycles are provided
	 */
	hsdramc1_writel(MR, HSDRAMC1_MODE_AUTO_REFRESH);
	hsdramc1_readl(MR);
	for (i = 0; i < 8; i++)
		writel(0, sdram);

	/*
	 * 4. A mode register set (MRS) cycle is issued to program
	 *    SDRAM parameters, in particular CAS latency and burst
	 *    length.
	 *
	 * CAS from info struct, burst length 1, serial burst type
	 */
	hsdramc1_writel(MR, HSDRAMC1_MODE_LOAD_MODE);
	hsdramc1_readl(MR);
	writel(0, sdram + (info->cas << 4));

	/*
	 * 5. A Normal Mode command is provided, 3 clocks after tMRD
	 *    is met.
	 *
	 * From the timing diagram, it looks like tMRD is 3
	 * cycles...try a dummy read from the peripheral bus.
	 */
	hsdramc1_readl(MR);
	hsdramc1_writel(MR, HSDRAMC1_MODE_NORMAL);
	hsdramc1_readl(MR);
	writel(0, sdram);

	/*
	 * 6. Write refresh rate into SDRAMC refresh timer count
	 *    register (refresh rate = timing between refresh cycles).
	 *
	 * 15.6 us is a typical value for a burst of length one
	 */
	bus_hz = get_sdram_clk_rate();
	hsdramc1_writel(TR, (156 * (bus_hz / 1000)) / 10000);
/*
	printf("SDRAM: %u MB at address 0x%08lx\n",
	       sdram_size >> 20, info->phys_addr);

	printf("Testing SDRAM...");
	for (i = 0; i < sdram_size / 4; i++)
		sdram[i] = i;

	for (i = 0; i < sdram_size / 4; i++) {
		tmp = sdram[i];
		if (tmp != i) {
			printf("FAILED at address 0x%08lx\n",
			       info->phys_addr + i * 4);
			printf("SDRAM: read 0x%lx, expected 0x%lx\n", tmp, i);
			return 0;
		}
	}

	puts("OK\n");
*/
	return sdram_size;
}

//#endif /* CFG_HSDRAMC */

static void sdram_delay(int us)
{
  int i, loop_limit;

  loop_limit = us * 20;

  for (i=0; i<loop_limit;i++);

}

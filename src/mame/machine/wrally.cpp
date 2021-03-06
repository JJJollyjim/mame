// license:BSD-3-Clause
// copyright-holders:Manuel Abadia, Mike Coates, Nicola Salmoria, Miguel Angel Horna
/***************************************************************************

    World Rally

    Functions to emulate general aspects of the machine (RAM, ROM, interrupts,
    I/O ports)

***************************************************************************/

#include "emu.h"
#include "cpu/mcs51/mcs51.h"
#include "includes/gaelcrpt.h"
#include "includes/wrally.h"


void wrally_state::machine_start()
{
	m_okibank->configure_entries(0, 16, memregion("oki")->base(), 0x10000);
}

/***************************************************************************

    World Rally memory handlers

***************************************************************************/

void wrally_state::shareram_w(offs_t offset, uint8_t data)
{
	// why isn't there address map functionality for this?
	reinterpret_cast<u8 *>(m_shareram.target())[BYTE_XOR_BE(offset)] = data;
}

uint8_t wrally_state::shareram_r(offs_t offset)
{
	// why isn't there address map functionality for this?
	return reinterpret_cast<u8 const *>(m_shareram.target())[BYTE_XOR_BE(offset)];
}

void wrally_state::vram_w(address_space &space, offs_t offset, uint16_t data, uint16_t mem_mask)
{
	data = gaelco_decrypt(space, offset, data, 0x1f, 0x522a);
	COMBINE_DATA(&m_videoram[offset]);

	m_tilemap[(offset & 0x1fff) >> 12]->mark_tile_dirty(((offset << 1) & 0x1fff) >> 2);
}

WRITE_LINE_MEMBER(wrally_state::flipscreen_w)
{
	flip_screen_set(state);
}

void wrally_state::okim6295_bankswitch_w(uint8_t data)
{
	m_okibank->set_entry(data & 0x0f);
}

WRITE_LINE_MEMBER(wrally_state::coin1_counter_w)
{
	machine().bookkeeping().coin_counter_w(0, state);
}

WRITE_LINE_MEMBER(wrally_state::coin2_counter_w)
{
	machine().bookkeeping().coin_counter_w(1, state);
}

WRITE_LINE_MEMBER(wrally_state::coin1_lockout_w)
{
	machine().bookkeeping().coin_lockout_w(0, !state);
}

WRITE_LINE_MEMBER(wrally_state::coin2_lockout_w)
{
	machine().bookkeeping().coin_lockout_w(1, !state);
}

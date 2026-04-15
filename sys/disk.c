/*
 * Copyright (c) 2026 LJC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <sys/asm.h>
#include <sys/disk.h>

#include <stdint.h>

static void
ata_wait_ready(void)
{
	while ((inb(ATA_COMMAND_PORT) & ATA_BSY ||
		!(inb(ATA_COMMAND_PORT) & ATA_DRDY)))
		;
}

static void
ata_wait_drq(void)
{
	while (!(inb(ATA_COMMAND_PORT) & ATA_DRQ))
		;
}

void
read_sectors(uint32_t lba, uint32_t count, uint16_t *buf)
{
	ata_wait_ready();

	outb(ATA_LBA, (0xE0 | ((lba >> 24) & 0x0F)));

	outb(ATA_SECTOR_COUNT, (uint8_t)count);

	outb(ATA_LBA_LOW, (uint8_t)lba);
	outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
	outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));

	outb(ATA_COMMAND_PORT, ATA_COMMAND_READ_SECTORS);

	for (uint32_t s = 0; s < count; s++)
	{
		ata_wait_ready();
		for (int i = 0; i < SECTOR_SIZE_WORDS; i++)
		{
			*buf++ = inw(ATA_DATA_PORT);
		}
	}
}

void
write_sectors(uint32_t lba, uint32_t count, uint16_t *buf)
{
	ata_wait_ready();

	outb(ATA_LBA, (0xE0 | ((lba >> 24) & 0x0F)));

	for (int i = 0; i < 4; i++)
		inb(ATA_COMMAND_PORT);

	outb(ATA_SECTOR_COUNT, (uint8_t)count);

	outb(ATA_LBA_LOW, (uint8_t)lba);
	outb(ATA_LBA_MID, (uint8_t)(lba >> 8));
	outb(ATA_LBA_HIGH, (uint8_t)(lba >> 16));

	outb(ATA_COMMAND_PORT, ATA_COMMAND_WRITE_SECTORS);

	for (uint32_t s = 0; s < count; s++)
	{
		ata_wait_drq();

		for (int i = 0; i < SECTOR_SIZE_WORDS; i++)
			outw(ATA_DATA_PORT, *buf++);
	}

	outb(ATA_COMMAND_PORT, ATA_COMMAND_FLUSH_CACHE);
	ata_wait_ready();
}

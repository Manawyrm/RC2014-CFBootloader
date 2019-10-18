#include "main.h"
#include "cf/cf.h"
#include "fat/pff.h"
#include "uart/uart.h"

void die (		/* Stop with dying message */
	FRESULT rc	/* FatFs return value */
)
{
	//printf("Failed with rc=%u.\n", rc);
	for (;;) ;
}

void main()
{
	putstring_uart("hallo welt!\n");

	FRESULT rc;
	FATFS fatfs;
	DIR dir;
	FILINFO fno;
	UINT bw, br, i;
	BYTE buff[64];

	putstring_uart("\nMount a volume.\n");
	rc = pf_mount(&fatfs);
	if (rc) die(rc);

	putstring_uart("\nOpen a test file (TEST.TXT).\n");
	rc = pf_open("TEST.TXT");
	if (rc) die(rc);

	//printf("\nType the file content.\n");
	for (;;) {
		rc = pf_read(buff, sizeof(buff), &br);	/* Read a chunk of file */
		if (rc || !br) break;			/* Error or end of file */
		//for (i = 0; i < br; i++)		/* Type the data */
			//putchar(buff[i]);
	}
	if (rc) die(rc);

	//printf("\nOpen root directory.\n");
	rc = pf_opendir(&dir, "");
	if (rc) die(rc);

	//printf("\nDirectory listing...\n");
	for (;;) {
		rc = pf_readdir(&dir, &fno);	/* Read a directory item */
		if (rc || !fno.fname[0]) break;	/* Error or end of dir */
		if (!(fno.fattrib & AM_DIR))
			
		putstring_uart(fno.fname);
		putchar_uart('\n');
	}
	if (rc) die(rc);

	while(1)
	{

	}
}